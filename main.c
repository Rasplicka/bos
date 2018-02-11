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
    
#define     TIMER1_EVENT_T_SIZE (TIMER1_EVENT_CAPA * TIMER1_EVENT_ISIZE)   
                                //proccee_table                 timer1_events         other vars + ballast
#define     OS_DATA_SIZE        ((PROC_T_ISIZE * PROC_T_CAPA) + TIMER1_EVENT_T_SIZE + 64)     //velikost .os
#define     OS_DATA_BASE        ((RAM_BASE + RAM_SIZE) - OS_DATA_SIZE)                      //adresa .os

//process table (prvni polozka v sekci .os, proto definuje jeji adresu)   
uint proc_t[(PROC_T_ISIZE / 4) * PROC_T_CAPA] __section(".os") __at(OS_DATA_BASE);      
//tabulka, kde se registruji casovace
char timer1_events[TIMER1_EVENT_T_SIZE]__section(".os");        //timer1

//.os vars 
uint* proc_t_pos    __section(".os") = 0;
uint* proc_t_after  __section(".os") = 0;
char proc_t_count   __section(".os") = 0;

uint timer_ms __section(".os");                         //timer1
uint day_ms __section(".os");                           //timer1


//ballast zajistuje, aby sekce .os byla plna, jinak kompilator vlozi za .os jeste sekci .data
//velikost ballast = 64 - vars.size (zarovnano na word)
char ballast[44]    __section(".os");


//.os_stack je oblast RAM tesne pod .os, stack ma definovanou velikost STACK_SIZE
#define     STACK_DATA_BASE (OS_DATA_BASE - STACK_SIZE)     
char stack_area[STACK_SIZE] __at(STACK_DATA_BASE) __section(".os_stack");
//char x[200000];

// <editor-fold defaultstate="collapsed" desc="Stack Size Compiler warning">
#ifdef  PIC32MM
//PIC32MM jeden zasobnik pro vsechny interrupt level
#define     STACK_MINIMUM   ((PROC_T_CAPA * 1024) + (1 * SRS_STACK_SIZE))      
#endif

#ifdef  PIC32MZ
//PIC32MZ kazdy interrupt level ma svuj zasobnik
#define     STACK_MINIMUM   ((PROC_T_CAPA * 1024) + (7 * SRS_STACK_SIZE))      
#endif

#if (STACK_SIZE < STACK_MINIMUM)
#warning Definition of STACK_SIZE is too small! Please, check STACK_SIZE in def.h
#endif
// </editor-fold>

// </editor-fold>


//global fn
void main() 
{
    //startup
    //1. set basic (clock...) --------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="clock">
    
    startupStack();                         //nastavi sp pro startup OS
    setClock(CLOCK_CFG.CLK_NORMAL);                 
    // </editor-fold>
    
    //2. set safe mode ---------------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="safe mode">
#ifdef SAFE_PROCESS    
    cpuTimerInit();
#endif
    // </editor-fold>

    //3. init system -----------------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="system init">
    
    //nuluje celou oblast pro OS data
    clearSystemData(OS_DATA_BASE, OS_DATA_SIZE); 
    
    systemInit(); //init interrupt (ale zustane DI), nastav SRS[1-7] sp, gp, ...
    //periphInit(); //provede vychozi nastaveni periferii, prideluje IO piny
    pinSetting();
    timer1Init(); //timer1 1/100s, je-li definovano RTC, nastavi RTC modul na datum 1/1/2000

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
    
    // </editor-fold>

    //5. run system modules ----------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="run module, apps">


    //6. run user apps --------------------------------------------------------------
    userAppsStart();
     
    // </editor-fold>

    //6. start multitasking ----------------------------------------------------
    // <editor-fold defaultstate="collapsed" desc="start os">
   
    globalsAfterProcess();
    SYSTEM_STATUS.Threading = 1;
    startEvents(); 
    // </editor-fold>
    
    while(1)
    {
        
    }
}

void processException(char code, uint addr)
{
    //procID b.0-7
    char id=(char)proc_t_pos[TH_T_ID];
    userAppError(id, code, addr);
    
    int behavior;
    
    //zjisti pozadovanou akci
    if(code==ERR_CODE_TIME_LIMIT_EXCEED)
    {
        //LongLasting error - chovani podle b.8-9
        behavior=(proc_t_pos[TH_T_ID] & 0x00000300) >> 8;
    }
    else if(code==ERR_CODE_GENERAL_EXCEPTION)
    {
        //General Exception - chovani podle b.10-11
        behavior=(proc_t_pos[TH_T_ID] & 0x00000C00) >> 10;
    }
    else if(code==ERR_CODE_TRAP)
    {
        //TRAP instruction - chovani podle b.12-13
        behavior=(proc_t_pos[TH_T_ID] & 0x00003000) >> 12;        
    }
    else
    {
        //code=ERROR_STACK_OVERFLOW, stack overflow, reset
        behavior=ON_ERROR.RESET_SYSTEM;
    }
    
    //proved akci
    if(behavior==ON_ERROR.RESET_PROCESS)
    {
        removeEvents();
        restartApp();
    }
    else if(behavior==ON_ERROR.REMOVE_PROCESS)
    {
        removeProcess(id);
    } 
    else
    {
        softReset();
    }
    
}

void trap()
{
    //vyvola general exception (trap)
    asm("teq    $0, $0");
}

//local fn
int reg_process(int* start_addr, int stack_size, const APP_START_PARAM* param)
{
    //prvede registraci procesu v proc_t
    //vlozi do proc_t adresu start fce a vychozi hodnotu pro stack(top adresa)
    //vraci ID procesu, nebo:
    //-1 = prilis mnoho procesu
    //-2 = nelze alokovat stack
    
    //test, zda je volne misto v proc_t
    if(proc_t_count >= PROC_T_CAPA) { return -1; } 

    //najdi volne procID
    char id = getFreeProcessID();
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
        
        //proc_t[0]= byte[0]=ID, byte[1]=b0-b3 je TimeLimitExceedBehavior, b4-7 je GeneralExceptionBehavior
        int x1 = ((param->TimeLimitExceedBehavior) << 8);
        int x2 = ((param->GeneralExceptionBehavior) << 10);
        int x3 = ((param->TrapBehavior) << 12);
        tab[TH_T_ID]=((int)id | x1 | x2 | x3);                      //b.0-7 = procID, b.8-9, b.10-11, b.12-13 ErrorBehavior
        
        tab[TH_T_RA]=(int)start_addr;
        tab[TH_T_GP]=getGP();
        tab[TH_T_START_ADDR]=(int)start_addr;                       //START_ADDR pro pripad restartu app
        tab[TH_T_LIMIT]=param->TimeLimitValue;

        //ok
        return id;
    }
    else
    {
        //error
        return -2;
    }
}

static char getFreeProcessID()
{
    //najde volne id, ktere vraci (id>0)
    //pri chybe vraci 0
        
    char id=1, a, exist;
    while(id < 255)
    {
        char* proc_t_bytes = (char*)proc_t;
        exist=0;
        
        for(a=0; a<PROC_T_CAPA; a++)
        {
            //prochazi vsechny polozky proc_t    
            if(proc_t_bytes[0]==id) { exist=1; break; }
            proc_t_bytes += PROC_T_ISIZE;
        }
        
        if(exist == 0) 
        { 
            //stejne id neexistuje, pouzije ho
            return id; 
        }
        
        id++;
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
    PRISS=0x11111100;
    
    //_CP0_SRSMAP = $12,3
    //jako PRISS
    asm("li	    $2, 0x11111100");
    asm("mtc0   $2, $12, 3");
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

/*
static void setClock(int cl)
{
    //PIC32MM0256, nsatveni pro pouziti FRC(interni) 8MHz, pres system PLL (PLLODIV=96MHz)
    //1.Nastavi PLLMULT (f pro USB musi byt 96/2 MHz)
    //2.Nastavi REFCLK (f pro SPI, UART, apod... 48MHz) 
    //3.Nastavi SYSCLK a PBCLK (SYSCLK pro CPU, PBCLK pro preriph, 24MHz) (PLLODIV = /4)
    

    //if(cl==CLOCK.INTERNAL_FRC_24MHz)
    {
        
    }
    
    //SYSKEY unlock
    SYSKEY = 0xAA996655; 
    SYSKEY = 0x556699AA;
    
    //nastaveni pro POSC Q=12MHz: SPLLCONbits.PLLMULT = 0x4 (externi krystal 12 MHz)
    //natsaveni pro FRC     8MHz: SPLLCONbits.PLLMULT = 0x6 (podle doc. by melo byt 0x5)
    
    SPLLCONbits.PLLMULT=0x6;        // x 12 (8x12=96 MHz) USB
    SPLLCONbits.PLLODIV=0x2;        // / 4  (96/4=24 MHz) CPU
    
    //SYSKEY force lock
    SYSKEY = 0x00000000;            
    
    //ceka na dokonceni
    while(REFO1CONbits.ACTIVE==1)
    { }
    
    //nastaveni REFCLK pro SPI, UART,...
    REFO1CONbits.ROSEL=7;                   //input SPLL (96MHz)
    //na vstupu REFCLK je / 2, f=48MHz
    REFO1CONbits.RODIV=0;                   //RODIV / 1, pouzije freq. 48MHz

    //trim     
    REFO1TRIMbits.ROTRIM=0;
    REFO1CONbits.ON=1;
}
*/

static void restartApp()
{
    //restartuje aktualni process
    //po chybe LongLasting, nebo GeneralException
    //nove spusteni nastana v nasledujicim cyklu
    
    int x=proc_t_pos[TH_T_START_SP];
    proc_t_pos[TH_T_SP]=x;
    
    x=proc_t_pos[TH_T_START_ADDR];
    proc_t_pos[TH_T_RA]=x;
    
    //vynecha save_regs
    doEventsError();
}

static void softReset()
{
    //software reset
    
    SYSKEY = 0x00000000; 
    SYSKEY = 0xAA996655;        //write key1 to SYSKEY
    SYSKEY = 0x556699AA;        //write key2 to SYSKEY
    RSWRSTSET = 1;
    volatile int* p = &RSWRST;
    *p;
    while(1){}    
}

#ifdef SAFE_PROCESS 

static inline void cpuTimerInit()
{
    //Inicializuje interrupt Cpu Timer, nastavi Compare na max. hodnotu 0xFFFFFFFF 
    //je pouzit k preruseni procesu, pokud bezi dele nez je povoleno, je-li definovano SAFE_PROCESS 
    
    //$9=CP0_COUNT, $11=CP0_COMPARE
    asm("li	    $2, 0xFFFFFFFF");       //v0=0xFFFFFFFF
    asm("mtc0   $2, $11");              //CP0_COMPARE=v0
    asm("ehb");
    asm("mtc0   $0, $9");               //CP0_COUNT=0 (zero)
    asm("ehb");
    
#ifdef PIC32MM
    //Core Timer interrupt param
    IPC0bits.CTIP=1;        //Priority=1
    IPC0bits.CTIS=0;        //Subpriority=0
    IFS0bits.CTIF=0;        //Flag=0
    IEC0bits.CTIE=1;        //Enable=1
#endif    
    
}

#endif

