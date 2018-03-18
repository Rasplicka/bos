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

GRAPHICS graphics;                              //universalni graficke fce
DISPLAY display;                                //display driver

void initGraphics()
{
    //Inicializuje systemovy display
    
#ifdef USE_DISP9341    
    //ili9341/SPI, v display nastavi fce driveru disp9341 
    disp9341_driver(&display);                    
#endif
    
#ifdef USE_DISP1306
    //SSD1306/SPIv v display nastavi fce driveru disp1306
    disp1306_driver(&display);                    
#endif    
    
    //pro vsechny typy displeju
    display.initDisplay();                      //driver inicializuje display         
    setGraphics(&graphics, &display);           //nastavi fce graphics na display driver, vystup graphics jde na zvoleny display
    graphics.clear(COLOR.Black);                //cls   
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