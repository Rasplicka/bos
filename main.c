#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS core (system startup)
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */

// <editor-fold defaultstate="collapsed" desc="extern, add user app start function here">
//define extern functions
//user app start function add here, 
//and update main fn bellow, step 6. run apps

//system
//touch module
#ifdef TOUCHPAD_XPT2046_INIT 
    extern void touchXpt2046_start(); 
#endif

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="OS vars">
    
/*
 * OBSAZENI RAM od nejvyssi adresy
 * a).os
 * b).os_stack
 * c)zbytek RAM 0x8000 0000 ... &stack_area-4
 * 
 * sekce .os je ma nejvyssich adresach v RAM
 * obsahuje 
 * 3.ballast
 * 2.ostatni os vars        //nad proc_t
 * 1.proc_t                 //zacatek oblasti
 * 
 * sekce .os_stack 
 * volna oblast
 * stack applikaci
 * stack os modulu (protoze os moduly se spusti prvni)                  //nas interrupt stack
 * interrupt stack (jeden, nebo sedm, velikost kazdeho SRS_STACK_SIZE)  //zacatek oblasti
 */
    
//#define     TEST_DRIVER_INIT
//#define     TEST_DRIVER_VERSION     2

#define     REG_EVENT_T_SIZE            (REG_EVENT_TABLE_ISIZE * REG_EVENT_TABLE_CAPA)    
#define     EVENT_C_SIZE                (EVENT_CACHE_ISIZE * EVENT_CACHE_CAPA)    
#define     PROC_T_SIZE                 (PROC_T_ISIZE * PROC_T_CAPA)

#ifdef NETCOM_UART
    #define     NETCOM_DATAOUT_SIZE     (NETCOM_DATAOUT_CAPA * 4)               //velikost dataOut [bytes]
    #define     NETCOM_DATASET_SIZE     (NETCOM_DATASET_CAPA * 4)
    #define     NETCOM_DATAGET_SIZE     (NETCOM_DATAGET_CAPA * 4)
    #define     NETCOM_DATA_SIZE        NETCOM_DATAOUT_SIZE + NETCOM_DATASET_SIZE + NETCOM_DATAGET_SIZE
#else
    #define     NETCOM_DATA_SIZE        0
#endif    
                                //proccee_table + regEventTable + eventCache + vars (vars + ballast = 64B)
#define     OS_DATA_SIZE        (PROC_T_SIZE +  REG_EVENT_T_SIZE + EVENT_C_SIZE + NETCOM_DATA_SIZE + 64)     //velikost .os
#define     OS_DATA_BASE        ((RAM_BASE + RAM_SIZE) - OS_DATA_SIZE)                                       //adresa .os

//#ifdef PIC32MZ
//#pragma region name=".sdata" origin=0x80000000 size=0x2000    
//#pragma region name="os"   origin=0x8007E000 size=0x2000
//int ext_array[256] __attribute__((region("os"))) ;    
//#endif    
    
//#ifdef PIC32MM
//#pragma region name="os" origin=0x80004000 size=0x2000
//int ext_array[256] __attribute__((region("os"))) ;    
//#endif 
    
//process table (prvni polozka v sekci .os, proto definuje jeji adresu)   
uint proc_t[(PROC_T_SIZE / 4)]                          __section(".os") __at(OS_DATA_BASE);      
char regEventTable[REG_EVENT_T_SIZE]                    __section(".os");
char eventCache[EVENT_C_SIZE]                           __section(".os");

#ifdef NETCOM_UART
    NETCOM_DATAOUT* netcomDataOut[NETCOM_DATAOUT_CAPA]  __section(".os");
    NETCOM_DATAIN*  netcomDataSet[NETCOM_DATASET_CAPA]  __section(".os");
    NETCOM_DATAIN*  netcomDataGet[NETCOM_DATAGET_CAPA]  __section(".os");
#endif  

//.os vars 
//__region(".os")
uint* proc_t_pos            __section(".os") = 0;
uint* proc_t_after          __section(".os") = 0;
char proc_t_count           __section(".os") = 0;
char errEventCacheEvID      __section(".os") = 0;                               //eventID posdledni udalosti, ktera se nevesla do eventCache (byla zahozena)
char errEventCachePrID      __section(".os") = 0;                               //procID  posdledni udalosti, ktera se nevesla do eventCache (byla zahozena)
char errorProcID            __section(".os") = 0;

uint time_ms                __section(".os") = 0;                                   //timer1

uint pauseCTCount           __section(".os") = 0;                               //pauza CPU timer
uint pauseCTCompare         __section(".os") = 0;                               //pauze CPU timer

char sleepStatus            __section(".os") = 0;
char idleStatus             __section(".os") = 0;
ushort netcomStratup_ms     __section(".os") = 0;
ushort netcomTx_ms          __section(".os") = 0;
ushort netcomRx_ms          __section(".os") = 0;
  



//ballast zajistuje, aby sekce .os byla plna, jinak kompilator vlozi za .os jeste sekci .data
//velikost ballast = 64 - vars.size (zarovnano na word)
char ballast[32]        __section(".os");


//.os_stack je oblast RAM tesne pod .os, stack ma definovanou velikost STACK_SIZE
#define     STACK_DATA_BASE     (OS_DATA_BASE - STACK_SIZE)     
char stack_area[STACK_SIZE] __at(STACK_DATA_BASE) __section(".os_stack");
//char x[200000];

// <editor-fold defaultstate="collapsed" desc="Stack Size Compiler warning">
#ifdef  PIC32MM
//PIC32MM jeden zasobnik pro vsechny interrupt level
//#define     STACK_MINIMUM   ((PROC_T_CAPA * 1024) + (1 * SRS_STACK_SIZE))      
#endif

#ifdef  PIC32MZ
//PIC32MZ kazdy interrupt level ma svuj zasobnik
//#define     STACK_MINIMUM   ((PROC_T_CAPA * 1024) + (7 * SRS_STACK_SIZE))      
#endif

//#if (STACK_SIZE < STACK_MINIMUM)
//#warning Definition of STACK_SIZE is too small! Please, check STACK_SIZE in def.h
//#endif
// </editor-fold>

// </editor-fold>

//core_fn.S extern fce pro tento modul (BOS startup)
extern void initCPU();
extern void startupStack();
extern int allocStack(int, int*);
extern void clearSystemData(uint, uint);
extern void setSrsValue();
extern uint getGP();
extern void systemProcess();
extern void startEvents();

//global fn
void main() 
{
    //startup
    //1. set basic (clock...) --------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="clock">
    
#ifdef WATCHDOG_TIMER
    clearWDT();                             //clear counter
    startWDT();                             //WDT on
#endif    
    
    initCPU();
    startupStack();                         //nastavi sp pro startup OS
    initClock();
    setClock(CLOCK_CFG.CLK_NORMAL);  
  
    //init vars
    //checkStackSpaceValue=0x7FFFFFFF;
    // </editor-fold>
    
    //2. set safe mode ---------------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="safe mode">
#ifdef SAFE_PROCESS    
    cpuTimerInit();
#endif
    // </editor-fold>

    //3. init system -----------------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="system init">
    clearSystemData(OS_DATA_BASE, OS_DATA_SIZE);    //nuluje celou oblast pro OS data
    netcomStratup_ms=100;                           //pokud 100ms neprobuha com. bude zahajena (pri NETCOM_DEVID=1)
    systemInit();                                   //init interrupt (ale zustane DI), nastav SRS[1-7] sp, gp, ...
    pinSetting();                                   //provede vychozi nastaveni periferii, prideluje IO piny
    timer1Init();                                   //timer1 1/100s, je-li definovano RTC, nastavi RTC modul na datum 1/1/2000
    
#ifdef RTC    
    //povoli RTC modul, nastavi 1/1/2001, nastav day_ms, podle aktualniho casu
    rtcInit();
#endif 

    //default param pro start aplikace
    defaultAppStartParam.TimeLimitExceedBehavior=ON_ERROR.RESET_PROCESS;
    defaultAppStartParam.GeneralExceptionBehavior=ON_ERROR.RESET_PROCESS;
    defaultAppStartParam.TrapBehavior=ON_ERROR.RESET_PROCESS;
    defaultAppStartParam.TimeLimitValue=SAFE_MODE_TIME_LIMIT_VALUE;
    // </editor-fold>
    
    //4. init system drivers ---------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="drivers">
    //drivers
    asm("EI");              //povoli interrupt
    
#if (defined SPI1_USE || defined SPI2_USE || defined SPI3_USE || defined SPI4_USE || defined SPI5_USE || defined SPI6_USE)
    spiInit();
#endif    

#ifdef USE_UARTNETCOM    
    netcomInit();
#endif
    
    // </editor-fold>

    //5. run system modules ----------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="run module, apps">

    //6. run user apps ---------------------------------------------------------
    //jako prvni spustit system process
    regProcess(&systemProcess, SYSPROC_STACK_SIZE, &defaultAppStartParam, 0xFE);
    
#ifdef UBTN
    regProcess(&ubtnStart, 512, &defaultAppStartParam, 0xF0);
#endif    
    
#ifdef USE_TOUCHPAD_XPT2046
    regProcess(&touchXpt2046_start, 128, &defaultAppStartParam, 0xF1);
#endif    
          
    
#ifdef WATCHDOG_TIMER
    clearWDT();
#endif
    
    //user apps
    userAppsStart();
     
    // </editor-fold>

    //7. start multitasking ----------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="start os">
   
    //globalsAfterProcess();
    SYSTEM_STATUS.SleepMode = 0;
    SYSTEM_STATUS.IdleMode =  0;
    SYSTEM_STATUS.Threading = 1;
    SYSTEM_STATUS.CLOCK_CFG = CLOCK_CFG.CLK_NORMAL;

    
    startEvents(); 
    // </editor-fold>
    
    while(1)
    {
        
    }
}

int regProcess(void* start_addr, int stack_size, const APP_START_PARAM* param, char default_procID)
{
    //prvede registraci procesu v proc_t
    //vlozi do proc_t adresu start fce a vychozi hodnotu pro stack(top adresa)
    //vraci ID procesu, nebo:
    //-1 = prilis mnoho procesu
    //-2 = nelze alokovat stack
    
    //test, zda je volne misto v proc_t
    if(proc_t_count >= PROC_T_CAPA) { return -1; } 

    //najdi volne procID
    char id = getFreeProcessID(default_procID);
    if(id < 1) { return -1; }
    
    //najdi volnou polozku v proc_t
    int* tab = getEmptyProcessTableItem();
    if(tab == NULL) { return -1; }
    
    //alokuj stack
    int ret=allocStack(stack_size, tab);                            //nastavuje SP, START_SP a BASE_SP
    if(ret==0)
    {
        //ok, stack allocated, set process item at proc_t
        proc_t_count++;
        
        //proc_t[0]= byte[0]=ID, byte[1]=b0-b1 je TimeLimitExceedBehavior, b2-3 je GeneralExceptionBehavior, b4-5 TrapBehavior
        int x1 = ((param->TimeLimitExceedBehavior) << 8);
        int x2 = ((param->GeneralExceptionBehavior) << 10);
        int x3 = ((param->TrapBehavior) << 12);
        tab[TH_T_ID]=((int)id | x1 | x2 | x3);                      //b.0-7 = procID, b.8-9, b.10-11, b.12-13 ErrorBehavior
        
        tab[TH_T_RA]=(int)start_addr;
        tab[TH_T_GP]=getGP();
        tab[TH_T_START_ADDR]=(int)start_addr;                       //START_ADDR pro pripad restartu app
        tab[TH_T_LIMIT]=param->TimeLimitValue;
        tab[TH_T_EXIT]=(int)&exitProcess;
        //ok
        return id;
    }
    else
    {
        //error
        return -2;
    }
}

//local fn
static char getFreeProcessID(char defaultId)
{
    //vraci ID procesu
    //pokud muze pouzit defaultId, pouzije ho
    //jinak najde volne id, ktere vraci (id>0)
    //pri chybe vraci 0
        
    char a, exist=0;
    char* procTabB = (char*)proc_t;
    //try to use defaultId
    if(defaultId > 0 && defaultId < 0xFF)
    {
        for(a=0; a<PROC_T_CAPA; a++)
        {
            //prochazi vsechny polozky proc_t    
            if(procTabB[0]==defaultId) { exist=1; break; }
            procTabB += PROC_T_ISIZE;
        }
        
        if(exist == 0) 
        { 
            //stejne id neexistuje, pouzije ho
            return defaultId; 
        }
    }
    
    
    defaultId=1;
    //use automatic ID
    while(defaultId < 255)
    {
        exist=0;
        procTabB = (char*)proc_t;
        for(a=0; a<PROC_T_CAPA; a++)
        {
            //prochazi vsechny polozky proc_t    
            if(procTabB[0]==defaultId) { exist=1; break; }
            procTabB += PROC_T_ISIZE;
        }
        
        if(exist == 0) 
        { 
            //stejne id neexistuje, pouzije ho
            return defaultId; 
        }
        
        defaultId++;
    }
    
    //nenasel volne id
    return 0;
}

static int* getEmptyProcessTableItem()
{
    //hleda volnou polozku v proc_t, jeji adresu vraci
    //nebo NULL
    
    char* proc_t_bytes = (char*)proc_t;
    int a;
    for(a=0; a<PROC_T_CAPA; a++)
    {
        if(proc_t_bytes[0]==0) 
        { 
            //ok, polozka je volna (procID=0)
            return (int*)proc_t_bytes; 
        }
        
        proc_t_bytes += PROC_T_ISIZE;
    }   
    
    return NULL;
}

static void systemInit()
{
    //nastavuje SRS[GP+SP], Multivector, ...
    //zatim nepovoli interrupt (EI) STATUS.EI zustava 0 (interrupt disable)
    
    //nastavi vychozi hodnoty GP a SP pro SRS[1], SRS[1-7]
    //nastav GP SRS[1-7] na stejnou hodnotu, jako SRS[0]
    //nastav SP SRS[1-7] (zasobnik pro interrupt, dolni cast stack_area) 
    setSrsValue(); 
    
    //init interrupt
    
#ifdef PIC32MM
    //Vector spacing=8, MVEC=1, prox. timer disable
    INTCON=0x00011000;
    
    //kazdy interrupt level pouziva SRS[1]
    PRISS=0x11111110;
    
    //_CP0_SRSMAP = $12,3
    //jako PRISS
    asm("li	    $2, 0x11111110");       //v0
    asm("mtc0   $2, $12, 3");           //v0, _CP0_SRSMAP
    asm("ehb");
    
#endif    
    
#ifdef PIC32MZ

    //MVEC=1, prox. timer disable
    INTCON=0x1000;
    
    //kazdy interrupt level pouziva jiny SRS
    PRISS=0x76543210;

    //_CP0_SRSMAP = $12,3
    //jako PRISS
    asm("li	    $2, 0x76543210");
    asm("mtc0   $2, $12, 3");
    asm("ehb");    
    
    //CAUSE.IV (CAUSE.23) = 1
    asm("li	    $3, 1");     
    asm("mfc0   $2, $13, 0");
    asm("ins	$2, $3, 23, 1");
    asm("mtc0   $2, $13, 0");
    asm("ehb");

    //nepouziva auto-prologue - epilogue, viz. INTCTL - pritom nefunguje prepinani SRS
    
#endif    
    
}

static void blick()
{
    //RB5, RB7, RC3
    
    LATA = 0xFFFF;
    LATB = 0xFFFF;
    LATC = 0xFFFF;
    
    ANSELA=0;
    ANSELB=0;
    ANSELC=0;

    TRISBbits.TRISB5=0;     //LED   pin 14
    TRISBbits.TRISB7=0;
    TRISCbits.TRISC3=0;
    
    LATBINV=0b10100000;
    LATCINV=0b1000;
    
    while(1)
    {
        int a, b;
        for(a=0; a<1500000; a++)
        {
            b=a+1;
        }
        
        LATBINV=0b10100000;
        LATCINV=0b1000;
    }
    
}


#ifdef SAFE_PROCESS 

static inline void cpuTimerInit()
{
    //Inicializuje interrupt Cpu Timer, nastavi Compare na max. hodnotu 0xFFFFFFFF 
    //je pouzit k preruseni procesu, pokud bezi dele nez je povoleno, je-li definovano SAFE_PROCESS 
    
    //$9=CP0_COUNT, $11=CP0_COMPARE
    asm("li	    $2, 0xFFFFFFFF");       //v0=0xFFFFFFFF
    //asm("li	    $2, 0xFFF");        //v0=0xFFFFFFFF
    asm("mtc0   $2, $11");              //CP0_COMPARE=v0
    asm("ehb");
    asm("mtc0   $0, $9");               //CP0_COUNT=0 (zero)
    asm("ehb");
    
 #ifdef PIC32MZ
    //OFF000 = offset interrupt vektoru 0 (CPU TIMER)
    extern void iVector0();
    uint off=(uint)&iVector0;
    off-=EBASE;
    OFF000=off;
#endif    
    
//#ifdef PIC32MM
    //Core Timer interrupt param
    IPC0bits.CTIP=1;        //Priority=1
    IPC0bits.CTIS=0;        //Subpriority=0
    IFS0bits.CTIF=0;        //Flag=0
    IEC0bits.CTIE=1;        //Enable=1
//#endif    
    
}

#endif  //SAFE_PROCESS 
