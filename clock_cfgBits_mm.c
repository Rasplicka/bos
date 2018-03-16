#include <xc.h>
#include <stdio.h>
#include "globals.h"

#ifdef PIC32MM

// PIC32MM
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
#pragma config RETVR = ON               // Retention Voltage Regulator Enable bit (Retention regulator is disabled)
#pragma config LPBOREN = ON             // Low Power Brown-out Enable bit (Low power BOR is enabled, when main BOR is disabled)

// FWDT
#pragma config SWDTPS = PS1024          // Sleep Mode Watchdog Timer Postscale Selection bits (1:1024)
#pragma config FWDTWINSZ = PS25_0       // Watchdog Timer Window Size bits (Watchdog timer window size is 25%)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Watchdog timer is in non-window mode)
#pragma config RWDTPS = PS1024          // Run Mode Watchdog Timer Postscale Selection bits (1:1024)
#pragma config RCLKSEL = LPRC           // Run Mode Watchdog Timer Clock Source Selection bits (Clock source is LPRC (same as for sleep mode))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT is disabled)

//FOSCSEL
//FNOSC=FRCDIV (8MHz FRC), nebo PLL (24MHz = FRCx3)
#ifdef CFG_POSC_XTAL
#pragma config FNOSC =   PLL            // Oscillator Selection bits 
#pragma config PLLSRC =  PRI            // System PLL Input Clock Selection bit 
#pragma config POSCMOD = HS             // Primary Oscillator Selection bit
#endif

#ifdef CFG_FRC
#pragma config FNOSC =   PLL            // Oscillator Selection bits
#pragma config PLLSRC =  FRC            // System PLL Input Clock Selection bit 
#pragma config POSCMOD = OFF            // Primary Oscillator Selection bit
#endif

#ifdef CFG_SOSC_ENABLE
#pragma config SOSCEN =  ON             // Secondary Oscillator Enable bit
#pragma config SOSCSEL = OFF            // Secondary Oscillator External Clock Enable bit (ON if external signal is used)
#else
#pragma config SOSCEN =  OFF             // Secondary Oscillator Enable bit
#pragma config SOSCSEL = OFF             // Secondary Oscillator External Clock Enable bit
#endif

#pragma config IESO = OFF               // Two Speed Startup Enable bit (Two speed startup is disabled)
#pragma config OSCIOFNC = OFF           // System Clock on CLKO Pin Enable bit (OSCO pin operates as a normal I/O)
//CSE musi zustat povoleno, aby se mohl prepinat LPRC pri sleep modu (ve sleep modu musi byt system napajen z LPRC)
#pragma config FCKSM = CSECME           // Clock Switching and Fail-Safe Clock Monitor Enable bits (Clock switching is enabled; Fail-safe clock monitor is enabled)

// FSEC
#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

void setClock(char mode)
{
    //0=CLK_NORMAL, 1=CLK_ALT1, 2=CLK_ALT2, 3=CLK_ALT3
    //nastavuje jeden ze tri preddefinovanych modu CLK
    
    SYSTEM_STATUS.CLOCK_CFG=mode;
    
    asm("di");
    //SYSKEY unlock
    SYSKEY = 0xAA996655; 
    SYSKEY = 0x556699AA;
    SYSKEY = 0x00000000; 
    
    if(mode==CLOCK_CFG.CLK_NORMAL)
    {
        //default mode 100%, 24MHz
        SPLLCONbits.PLLODIV=0x2;        // / 4  (96/4=24 MHz) CPU
    }
    else if (mode==CLOCK_CFG.CLK_ALT1)
    {
        //alt mode1 50%, 12MHz
        SPLLCONbits.PLLODIV=0x3;        // / 8  (96/8=12 MHz) CPU
    }
    else if (mode==CLOCK_CFG.CLK_ALT2)
    {
        //alt mode2 25%, 6MHz
        SPLLCONbits.PLLODIV=0x4;        // / 16  (96/16=6 MHz) CPU
    }
    else
    {
        //alt mode3 6%, 1.5MHz
        SPLLCONbits.PLLODIV=0x6;        // / 16  (96/64=1.5 MHz) CPU        
    }
    
    //SYSKEY force lock
    SYSKEY = 0x00000000;
    asm("ei");
}

void initClock()
{
    //predpoklada XTAL 8MHz, nebo FRC 8MHz, nastavi 24MHz

    //PIC32MM0256, nsatveni pro pouziti FRC(interni) 8MHz, pres system PLL (PLLODIV=96MHz)
    //1.Nastavi PLLMULT (f pro USB musi byt 96/2 MHz)
    //2.Nastavi REFCLK  (f pro SPI, UART, apod... 48MHz - zdroj clk se nastavuje v modulu SPI, UART...) 
    //3.Nastavi SYSCLK a PBCLK (SYSCLK pro CPU, PBCLK pro preriph, 24MHz) (PLLODIV = /4)
    
    //nastaveni config bitu: 
    //FNOSC = PLL (SYSCLK a PBCLK from PLL)
    //PLLSRC = FRC (src PLL from FRC)
    
    asm("di");
    //SYSKEY force lock
    SYSKEY = 0x00000000;
    //SYSKEY unlock
    SYSKEY = 0xAA996655; 
    SYSKEY = 0x556699AA;
    
    //zvysovani F, nejprve PLLODIV
    SPLLCONbits.PLLODIV=0x2;            // / 4  (96/4=24 MHz) CPU
    SPLLCONbits.PLLMULT=0x6;            // x 12 (8x12=96 MHz) USB  
    
    //SYSKEY force lock
    SYSKEY = 0x00000000;         
    asm("ei");
    
    //enable REFCLK
    //REFCLK, pouziva SPI, UART, nastavi 48MHz
    REFO1CONbits.ON=0;                  //OFF
    while(REFO1CONbits.ACTIVE==1) { }   //ceka na dokonceni
    
    REFO1CONbits.ROSEL=7;               //input SPLL (96MHz)
    REFO1CONbits.RODIV=0;               //RODIV / 1, pouzije freq. 48MHz

    //trim     
    REFO1TRIMbits.ROTRIM=0;
    REFO1CONbits.ON=1;
}
#endif


