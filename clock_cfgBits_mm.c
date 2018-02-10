#include <xc.h>
#include <stdio.h>
#include "globals.h"

// <editor-fold defaultstate="collapsed" desc="DEVCFG0 PIC32MM">

#ifdef PIC32MM

// PIC32MM0064GPL028 Configuration Bit Settings
// 'C' source line config statements
// FDEVOPT
#pragma config SOSCHP = OFF             // Secondary Oscillator High Power Enable bit (SOSC oprerates in normal power mode.)
#pragma config FUSBIDIO = OFF
#pragma config FVBUSIO = OFF
#pragma config USERID = 0xFFFF          // User ID bits (User ID bits)

// FICD
//POZOR, pri JTAGEN=ON nefunguje SPI1, pri nastaveni OFF debug ICD3 funguje normalne
#pragma config JTAGEN = OFF             // !!!JTAG disable 
#pragma config ICS = PGx1               // ICE/ICD Communication Channel Selection bits (Communicate on PGEC1/PGED1)

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config RETVR = OFF              // Retention Voltage Regulator Enable bit (Retention regulator is disabled)
#pragma config LPBOREN = ON             // Low Power Brown-out Enable bit (Low power BOR is enabled, when main BOR is disabled)

// FWDT
#pragma config SWDTPS = PS1048576       // Sleep Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config FWDTWINSZ = PS25_0       // Watchdog Timer Window Size bits (Watchdog timer window size is 25%)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Watchdog timer is in non-window mode)
#pragma config RWDTPS = PS1048576       // Run Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config RCLKSEL = LPRC           // Run Mode Watchdog Timer Clock Source Selection bits (Clock source is LPRC (same as for sleep mode))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT is disabled)

//FOSCSEL
//FNOSC=FRCDIV (8MHz FRC), nebo PLL (24MHz = FRCx3)
#pragma config FNOSC = PLL              // Oscillator Selection bits (Fast RC oscillator (FRC) with divide-by-N) //PRI
#pragma config PLLSRC = FRC             // System PLL Input Clock Selection bit (FRC oscillator is selected as PLL reference input on device reset) //PRI
#pragma config SOSCEN = OFF             // Secondary Oscillator Enable bit (Secondary oscillator (SOSC) is disabled)
#pragma config IESO = OFF               // Two Speed Startup Enable bit (Two speed startup is disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Selection bit (Primary oscillator is disabled) //HS
#pragma config OSCIOFNC = OFF           // System Clock on CLKO Pin Enable bit (OSCO pin operates as a normal I/O)
#pragma config SOSCSEL = ON             // Secondary Oscillator External Clock Enable bit (RA4 and RB4 are controlled by I/O PORT)
#pragma config FCKSM = CSDCMD//SCECME           // Clock Switching and Fail-Safe Clock Monitor Enable bits (Clock switching is enabled; Fail-safe clock monitor is enabled)

// FSEC
#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

void setClock(char mode)
{
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

// </editor-fold>

