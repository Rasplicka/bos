#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka 
 * Copyright (c) 2017, All rights reserved.      
 * BOS core
 * This file is shared for the PIC32MM, PIC32MZ
 * Last update 19/3/2018
 * Ver. 1.0 
*/

void trap()
{
    //vyvola general exception (trap)
    asm("teq    $0, $0");
}

void softReset()
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