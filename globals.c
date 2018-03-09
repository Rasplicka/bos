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
