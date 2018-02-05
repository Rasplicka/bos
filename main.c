#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "main.h"
#include "asm.h"

//#include "fnc.h"
//#include "graphics.h"                 //display modul
//#include "disp1306a.h"
//#include "spi.h"
//#include "i2c.h"
//#include "timer.h"
//#include "pwm.h"
//#include "usb_device_mm.h"
//#include "test_driver_a.h"
//#include "disp9341.h"


// <editor-fold defaultstate="collapsed" desc="extern, add user app start function here">
//define extern functions
//user app start function add here, 
//and update main fn bellow, step 6. run apps

//user apps
//app1
extern void m1_start();
//app2
extern void m2_start();
//app3
extern void m3_start();

//system
//touch module
#ifdef TOUCHPAD_XPT2046_INIT 
    extern void touchXpt2046_start(); 
#endif

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="OS vars">

//RAM sekce pro system (pouziva sekci .jcr, kterou linker vklada na zacatek RAM)
uint* proc_t_pos __section(".jcr") = 0;
uint* proc_t_max __section(".jcr") = 0;
char proc_t_count __section(".jcr") = 0;
uint proc_t[(PROC_T_ISIZE / 4) * PROC_T_CAPA] __section(".jcr"); //8 x 88B
//uint gp_value __section(".jcr") = 0;
//char* sp_srs1_top __section(".jcr") = 0;

//stack_list pro pouziti pri allocStack, stack pro kazdy proces je alokovan ve volne RAM
int stack_list[PROC_T_CAPA] __section(".jcr");                  //kazda polozka (32-bit) definuje velikost jednoho stacku

//stack pro interrupt SRS1 je mimo ostatni stacky
//#define     _SRS1_STACK_SIZE        512
//char stack_interrupt_srs1[_SRS1_STACK_SIZE] __section(".jcr");

//stack je posledni oblast RAM, stack ma definovanou velikost STACK_SIZE
char stack_area[STACK_SIZE] __at(RAM_BASE + RAM_SIZE - STACK_SIZE) __section(".os_stack");

// </editor-fold>


//global fn
void main() {
    
    // <editor-fold defaultstate="collapsed" desc="comment">
    //GND test commit, test doma, test v praci
    //+
    //CLK       17, RP12
    //DATA      18, RP13
    //RST       19, RP14 B9
    //DC        21, RP18 C9

    //MZ test LED
    //TRISH=0b1111111111111000;
    //PORTH=0x0;
    //LATHSET=0b000;

    //char* string="text";
    //disp1306a_drawText(0, string, 0, 0, 1); 
    //</editor-fold>
    
    //startup
    //1. set basic (clock...) --------------------------------------------------
    setClock();

    //2. set safe mode ---------------------------------------------------------
#ifdef SAFE_PROCESS    
    cpuTimer_init();
#endif
    
    //3. init system -----------------------------------------------------------
    system_init();                                  //init interrupt (ale zustane DI), nastav SRS1, ...
    timer1_init();                                  //timer1 1/100s, je-li definovano RTC, nastavi RTC modul na datum 1/1/2000
    periph_init();                                  //provede vychozi nastaveni periferii, prideluje IO piny

    
    //4. init system drivers ----------------------------------------------------------
#if (defined SPI1_INIT || defined SPI2_INIT || defined SPI3_INIT)    
    spi_init();
#endif

#if (defined I2C1_INIT || defined I2C2_INIT || defined I2C3_INIT)    
    i2c_init();
#endif    
    
#ifdef ADC_SCAN_INIT    
    adcScan_init();
#endif
   
#ifdef PWM_INIT    
    pwm_init();
#endif  
    
#ifdef USB_DEVICE_INIT
    usbDevice_init();
#endif    
    
    //testDriver_init();
    enableInterrupt();                              //provede EI
    globalsBeforeProcess();                         //inicializuje displej, touchpad, ...
    
    //5. run system modules -----------------------------------------------------------
#ifdef TOUCHPAD_XPT2046_INIT    
    //modul touchpad XPT2046
    reg_process((int*)touchXpt2046_start, 512);
#endif    
    
    
    //6. run user apps --------------------------------------------------------------
    //To run user app, modify this section. Use reg_process(int* app_start_fn_address, int stack_size)
    //The start fn of app, must be declared in extern section above
    //Check the maximum number of threads, please
 
    reg_process((int*)&m1_start, 1024); 
    reg_process((int*)&m2_start, 1024);
    reg_process((int*)&m3_start, 1024);
    
    //reg_process((int*)&disp9341a_start, 1024);
    //reg_process((int*)&disp1306a_start, 1024);
    //reg_process((int*)&ubtn_start, 512);          //dve tlacitka A2, A3
    //reg_process((int*)&m2, 1024);
    //reg_process((int*)&m3, 1024);
    
    
    void globalsAfterProcess();
    
    //7. start multitasking
    SYSTEM_STATUS.Threading=1;
    startEvents();
    
    while(1)
    {
        
    }
}

//local fn
static char reg_process(int* start_addr, int stack_size)
{
    //prvede registraci procesu v proc_t
    //vlozi do proc_t adresu start fce a vychozi hodnotu pro stack(top adresa)
    //vraci ID procesu, nebo -1 pri chybe
    
    //test, zda je volne misto v proc_t
    if(proc_t_count >= PROC_T_CAPA) { return -1; } 
    
    char id=getFreeProcessID();
    int* tab=proc_t + ((proc_t_count) * (PROC_T_ISIZE/4));     //adresa polozky proc_t
    proc_t_count++;
    
    allocStack2(stack_size, tab);                //nastavuje SP a START_SP
    tab[TH_T_ID]=id;
    tab[TH_T_RA]=(int)start_addr;
    tab[TH_T_GP]=getGP();
    
    tab[TH_T_START_ADDR]=(int)start_addr;       //START_RA pro pripad restartu app
    
    proc_t_max=tab;
    
    return id;
}

static char getFreeProcessID()
{
    //v proc_t musi byt alespon jedno volne misto
    //prvni nulova hodnota ID znamena konec platnych polozek (volne polozky pouze na konci tabulky)
    char* proc_t_bytes=(char*)proc_t;
    
    char id=1;
    while(1)
    {
        char exist_id=proc_t_bytes[0];
        if(id==exist_id)
        {
            //nasel stejne ID, cele znova
            id++;                                   //nove id
            proc_t_bytes=(char*)proc_t;             //nastav zacatek proc_table
        }
        else if(exist_id==0)
        {
            //nasel konec tabulky, pouzije ID
            break;
        }
        else
        {
            //jdi na dalsi polozku
            proc_t_bytes += PROC_T_ISIZE;
        }
    }
    
    return id;
}

static void system_init()
{
    //nastavuje SRS[GP+SP], Multivector, ...
    //zatim nepovoli interrupt (EI)

#ifdef PIC32MM
    
    //char* sp_srs1 = stack_interrupt_srs1 + _SRS1_STACK_SIZE - 4;
    
    //nastavi vychozi hodnoty GP a SP pro SRS[1], SRS[1-7]
    //nastav GP SRS[1-7] na stejnou hodnotu, jako SRS[0]
    //nastav SP SRS[1-7] (zasobnik pro interrupt, dolni cast stack_area) 
    setSrsValue2(); //sp_srs1);

    //Multivector, spacing 8 bytes, IPL 1-7 pouziva SRS[1]
    //Neobsahuje EI, STATUS.EI zustava 0 (interrupt disable)
    setInterrupt();
    
#endif    
    
#ifdef PIC32MZ

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

static void setClock()
{
    //PIC32MM0256, nsatveni pro pouziti FRC(interni) 8MHz, pres system PLL (PLLODIV=96MHz)
    //1.Nastavi PLLMULT (f pro USB musi byt 96/2 MHz)
    //2.Nastavi REFCLK (f pro SPI, UART, apod... 48MHz) 
    //3.Nastavi SYSCLK a PBCLK (SYSCLK pro CPU, PBCLK pro preriph, 24MHz) (PLLODIV = /4)
    
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

#ifdef SAFE_PROCESS 

static inline void cpuTimer_init()
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