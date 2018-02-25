#include <xc.h>
#include <stdio.h>
#include "globals.h"

#ifdef PIC32MZ
/*** DEVCFG0 ***/

#pragma config DEBUG =      OFF             //x
#pragma config JTAGEN =     OFF             //x
#pragma config ICESEL =     ICS_PGx2        //
#pragma config TRCEN =      OFF             //x
#pragma config BOOTISA =    MIPS32 //MICROMIPS       //MIPS32
#pragma config FECCCON =    OFF_UNLOCKED    //
#pragma config FSLEEP =     OFF             //
#pragma config DBGPER =     PG_ALL          //x
#pragma config EJTAGBEN =   NORMAL          //
#pragma config CP =         OFF             //

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL            //SPLL
#pragma config DMTINTV =    WIN_127_128     //

//Secondary Oscilator (32.768kHz)
#ifdef CFG_SOSC_ENABLE
#pragma config FSOSCEN =    ON              //ON (XTAL 32.768kHz)
#else
#pragma config FSOSCEN =    OFF             //OFF (na SOSCO je ext. signal 32.768kHz, nebo SOSC neni pouzit)
#endif

#pragma config IESO =       OFF             //OFF
#pragma config POSCMOD =    EC              //x
#pragma config OSCIOFNC =   OFF             //
#pragma config FCKSM =      CSECME          //x CSDCMD
#pragma config WDTPS =      PS1024           // doba cca 1s
#pragma config WDTSPGM =    STOP            //

#ifdef WATCH_DOG_TIMER
#pragma config FWDTEN =     ON              //ON
#else
#pragma config FWDTEN =     OFF              //OFF
#endif

#pragma config WINDIS =     NORMAL          //
#pragma config FWDTWINSZ =  WINSZ_50        //
#pragma config DMTCNT =     DMT31           //x
#pragma config FDMTEN =     OFF             //

/*** DEVCFG2 ***/

#if defined CFG_CLKSYS_POSC && CFG_POSC_XTAL == 24
//XTAL 24MHz
#pragma config FPLLIDIV =   DIV_3           //primary osc 24MHz / 3 = 8MHz
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50          //  8 x 50 = 400MHz
#pragma config FPLLODIV =   DIV_2           //  /2 = 200MHz (SPLL = 200MHz)

#elif defined CFG_CLKSYS_POSC && CFG_POSC_XTAL == 12
//XTAL 12MHz
#pragma config FPLLIDIV =   DIV_1           //primary osc 12MHz / 1 = 12MHz
#pragma config FPLLRNG =    RANGE_8_16_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_33          //  12 x 33 = 396MHz
#pragma config FPLLODIV =   DIV_2           //  /2 = 198MHz (SPLL = 198MHz)

#elif defined CFG_CLKSYS_FRC
//FRC
#pragma config FPLLIDIV =   DIV_1           //FRC 8MHz / 1 = 8MHz
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_FRC
#pragma config FPLLMULT =   MUL_50          //  8 x 50 = 400MHz
#pragma config FPLLODIV =   DIV_2           //  /2 = 200MHz (SPLL = 200MHz)

#endif

//USB CLK POSC, 12 or 24 MHz only
#if defined CFG_POSC_XTAL && CFG_POSC_XTAL == 24
#pragma config UPLLFSEL =   FREQ_24MHZ
#pragma config UPLLEN =     ON
#elif defined CFG_POSC_XTAL && CFG_POSC_XTAL == 12
#pragma config UPLLFSEL =   FREQ_12MHZ
#pragma config UPLLEN =     ON
#else
#pragma config UPLLEN =     OFF
#endif

/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     OFF
#pragma config PGL1WAY =    0 //OFF
#pragma config PMDL1WAY =   0 //OFF
#pragma config IOL1WAY =    0 //ON
#pragma config FUSBIDIO =   OFF

/*** BF1SEQ0 ***/
#pragma config TSEQ =       0xffff
#pragma config CSEQ =       0xffff


void setClock(char mode)
{
    //MZ only
    //mod 0-3, 0=100%, 1=75%, 2=50%, 3=25% CPU freq
    //SPLL=200MHz
 
    //0=CLK_NORMAL, 1=CLK_ALT1, 2=CLK_ALT2, 2=CLK_ALT3
    //nastavuje jeden ze 4 preddefinovanych modu CLK
    
    //SYSKEY unlock
    //SYSKEY = 0xAA996655; 
    //SYSKEY = 0x556699AA;
    SYSKEY = 0x0;
    SYSKEY = 0xAA996655; 
    SYSKEY = 0x556699AA;
    //ceka na RDY
    while(PB7DIVbits.PBDIVRDY==0){}
    
    if(mode==CLOCK_CFG.CLK_NORMAL)
    {
        //100%, 200MHz
        PB7DIVbits.PBDIV=0;                         //divide 1
    }
    else if (mode==CLOCK_CFG.CLK_ALT1)
    {
        //50%, 100MHz
        PB7DIVbits.PBDIV=1;                         //divide 2
    }
    else if (mode==CLOCK_CFG.CLK_ALT2)
    {
        //25%, 50MHz
        PB7DIVbits.PBDIV=3;                         //divide 4
    }    
    else
    {

        //10%, 20MHz
        PB7DIVbits.PBDIV=9;                         //divide 10        
    }
    
    //SYSKEY force lock
    SYSKEY = 0x00000000;

}

#endif  //PIC32MZ


