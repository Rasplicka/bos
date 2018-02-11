#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS core/user apps interface
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */

//user apps
//app1
extern void m1_start();
//app2
extern void m2_start();
//app3
extern void m3_start();

void pinSetting()
{
    //Call OS on startup
#ifdef __32MM0256GPM036__ 

     //RB5, RB7
    setPortDigOut(PORTB_BASE, BIT5 | BIT7);
    //RC3
    setPortDigOut(PORTC_BASE, BIT3);
    
    LED1_BASE=PORTB_BASE;
    LED1_BIT=BIT5;
    
    LED2_BASE=PORTB_BASE;
    LED2_BIT=BIT7;
    
    LED3_BASE=PORTC_BASE;
    LED3_BIT=BIT3;    
    
#endif
    
}

void userAppsStart()
{
    //Call OS on startup
    
    APP_START_PARAM p1;
    p1.TimeLimitExceedBehavior = ON_ERROR.RESET_PROCESS;
    p1.GeneralExceptionBehavior = ON_ERROR.RESET_PROCESS;
    p1.TrapBehavior = ON_ERROR.RESET_SYSTEM;
    p1.TimeLimitValue = 0xFFFF;
    
    
    if(reg_process((int*) &m1_start, 1024, &p1) < 0)
    {
        //error
    }
    
    if(reg_process((int*) &m2_start, 1024, &defaultAppStartParam) < 0)
    {
        //error
    }
    
    if(reg_process((int*) &m3_start, 1024, &defaultAppStartParam) < 0) 
    {
        //error
    }
}

void userAppError(char id, char code)
{
    //id=procID, code=kod chyby
    //Call OS on error, before process restart, or system restart
    int a=0;
    int b=0;
    int c=a+b;
}