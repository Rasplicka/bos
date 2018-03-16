#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS core
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */


#ifdef USE_GRAPHICS
//global vars
GRAPHICS graphics;
DISPLAY dispSys;
PORT_INFO pinfo_dispSys;

//local fce
//static void initDispSys();

void initGraphics()
{
    //Inicializuje systemovy display
    
    //definuje jinou colorMap (system obsahuje std colorMap (B/W), ktera se automaticky nastavi v IMAGE_SRC fci setFontSrc/setImageSrc)
    //loadColorMap(cmap);

    //nastaveni konkretniho displeje (ili9341/SPI)
    
#ifdef USE_DISP9341    
    //<editor-fold defaultstate="collapsed" desc="PortWriter, pin set, driver">
    //pin configuration C2=CS, C1=RESET, C0=DC
    pinfo_dispSys.cs_portBase = PORTC_BASE;             //CS
    pinfo_dispSys.cs_pin = BIT8;
    pinfo_dispSys.reset_portBase = PORTA_BASE;          //RESET
    pinfo_dispSys.reset_pin = BIT15;
    pinfo_dispSys.dc_portBase = PORTB_BASE;             //DC
    pinfo_dispSys.dc_pin = BIT13;
    pinfo_dispSys.busMode = BUS_MODE._8bit;             //8-bit mode
    pinfo_dispSys.directMode = 1;                       //direct mode

    //v pinfo_dispSys nastavi fce pro vysilani dat na pozadovany port SPI, PMP, ...
    portWriter_init(&pinfo_dispSys, PERIPH_TYPE.spi, 1); //SPI[1]=SPI2
    
    //v dispSys nastavi fce driveru disp9341 
    disp9341_driver(&dispSys);                    
    // </editor-fold>
#endif
    
#ifdef SYSDISPLAY_1306SPI
    //<editor-fold defaultstate="collapsed" desc="PortWriter, pin set, driver">
    //pin configuration C2=CS, C1=RESET, C0=DC
    pinfo_dispSys.cs_portBase = PORTC_BASE;             //CS
    pinfo_dispSys.cs_pin = BIT2;
    pinfo_dispSys.reset_portBase = PORTC_BASE;          //RESET
    pinfo_dispSys.reset_pin = BIT1;
    pinfo_dispSys.dc_portBase = PORTC_BASE;             //DC
    pinfo_dispSys.dc_pin = BIT0;
    pinfo_dispSys.busMode = BUS_MODE._8bit;             //8-bit mode
    pinfo_dispSys.directMode = 1;                       //direct mode

    //v pinfo_dispSys nastavi fce pro vysilani dat na pozadovany port SPI, PMP, ...
    portWriter_init(&pinfo_dispSys, PERIPH_TYPE.spi, 1); //SPI[1]=SPI2
    
    //v dispSys nastavi fce driveru disp9341 
    disp1306_driver(&dispSys);                    
    // </editor-fold>
#endif    
    
    //pro vsechny typy displeju
    
    // <editor-fold defaultstate="collapsed" desc="init displeje, aktivuje Graphics, init font">
    //inicializuje display
    dispSys.initDisplay(&pinfo_dispSys);            
    
    //nastavi fce Graphics na strukturu dispSys, vystup Graphics jde na dispSys
    setGraphics(&graphics, &dispSys, &pinfo_dispSys); 
    
    //init system font (pouziva font_sys v souboru font_sys.h)
#ifdef USE_SYSTEMFONT_ARIAL     
    setFontSrc(&font_arial18, &fontSys);
    fontSys.foreColor=RGB16(0, 63, 0);
#endif    
    
#ifdef USE_SYSTEMFONT_FIXEDx
    setFontSrc(&font_fixed16x, &fontSys);
    fontSys.foreColor=RGB16(31, 63, 31);
#endif    
    // </editor-fold>

    //cls
    graphics.clear(COLOR.Black);    
}
#endif

void trap()
{
    //vyvola general exception (trap)
    sleepStatus=0;
    asm("teq    $0, $0");
}

void softReset()
{
    //software reset
    sleepStatus=0;
    SYSKEY = 0x00000000; 
    SYSKEY = 0xAA996655;        //write key1 to SYSKEY
    SYSKEY = 0x556699AA;        //write key2 to SYSKEY
    RSWRSTSET = 1;
    volatile int* p = &RSWRST;
    *p;
    while(1){}    
}

void startSleepMode()
{
    //PIC32MM musi pri prechodu do sleep modu pouzit LPRC ocsilator, jinak ho nelze probudit
    asm("di");
        
    SYSKEY = 0x00000000;                        //SYSKEY force lock
    SYSKEY = 0xAA996655;                        //SYSKEY unlock
    SYSKEY = 0x556699AA;

#ifdef PIC32MM    
    OSCCONbits.NOSC=5;                          //use LPRC
    OSCCONSET = 1;                              //execute oscilator change
    PWRCONbits.RETEN=1;                         //retention voltage regulator
    PWRCONbits.VREGS=0;                         //main voltage regulator off
#endif    
       
    OSCCONbits.SLPEN=1;                         //wait > to sleep
    SYSKEY = 0x00000000;

    asm("ei");   
    
    SYSTEM_STATUS.SleepMode = 1;
}

void endSleepMode()
{
    //PIC32MM musi ve sleep modu pouzit LPRC ocsilator, jinak ho nelze probudit
    //nyni se vrati PLL, ale pritom musi byt PLLMULT=0, jinak nenabehne
    //az potom nastavi PLLMULT a PLLODIV
    
    asm("di");

    SYSKEY = 0x00000000;            //SYSKEY force lock
    SYSKEY = 0xAA996655;            //SYSKEY unlock
    SYSKEY = 0x556699AA;
    
#if PIC32MM
    SPLLCONbits.PLLODIV=0x0;        //8MHz
    SPLLCONbits.PLLMULT=0x0;        //x2 16MHz  
    
    OSCCONbits.NOSC=1;              //1=PLL
    OSCCONSET = 1;                  //execute
#endif    
    
    OSCCONbits.SLPEN=0;             //nuluj sleep bit
    SYSKEY = 0x00000000;
    
    asm("ei");
    
    initClock();
    setClock(SYSTEM_STATUS.CLOCK_CFG);
    SYSTEM_STATUS.SleepMode = 0;
}