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
#pragma config FSOSCEN =    OFF             //
#pragma config IESO =       OFF             //OFF
#pragma config POSCMOD =    EC              //x
#pragma config OSCIOFNC =   OFF             //
#pragma config FCKSM =      CSECME          //x CSDCMD
#pragma config WDTPS =      PS1024          //1048576 
#pragma config WDTSPGM =    STOP            //
#pragma config FWDTEN =     OFF              //OFF
#pragma config WINDIS =     NORMAL          //
#pragma config FWDTWINSZ =  WINSZ_25        //
#pragma config DMTCNT =     DMT31           //x
#pragma config FDMTEN =     OFF             //

/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_3           //primary osc 24MHz / 3 = 8MHz
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50          //8 x 50 = 400MHz
#pragma config FPLLODIV =   DIV_2           //    /2 = 200MHz (SPLL = 200MHz)
#pragma config UPLLFSEL =   FREQ_24MHZ
#pragma config UPLLEN =     ON

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
    //verze MM, zde predelat!
    //0=CLK_NORMAL, 1=CLK_ALT1, 2=CLK_ALT2
    //nastavuje jeden ze tri preddefinovanych modu CLK
    
    //PIC32MM0256, nsatveni pro pouziti FRC(interni) 8MHz, pres system PLL (PLLODIV=96MHz)
    //1.Nastavi PLLMULT (f pro USB musi byt 96/2 MHz)
    //2.Nastavi REFCLK  (f pro SPI, UART, apod... 48MHz - zdroj clk se nastavuje v modulu SPI, UART...) 
    //3.Nastavi SYSCLK a PBCLK (SYSCLK pro CPU, PBCLK pro preriph, 24MHz) (PLLODIV = /4)
    
    //nastaveni config bitu: 
    //FNOSC = PLL (SYSCLK a PBCLK from PLL)
    //PLLSRC = FRC (src PLL from FRC)

    if(mode==0)
    {
        //default mode
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
    else if (mode==1)
    {
        //alt mode1
    }
    else
    {
        //alt mode2
    }
}

#endif


