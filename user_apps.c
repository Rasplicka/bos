#include <xc.h>
#include <stdio.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka 
 * Copyright (c) 2017, All rights reserved.
 * Version 1.0       
 * BOS user apps interface
 * This file is shared for the PIC32MM, PIC32MZ
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
    
#ifdef __32MM0256GPM048__     
    
#endif    
    
#ifdef __32MM0256GPM064__     
    
#endif  
    
#ifdef PIC32MZ

    //test led PORTH.0-2
    setPortDigOut(PORTH_BASE, BIT0 | BIT1 | BIT2);
    LATHCLR=(BIT0 | BIT1 | BIT2);
    
    //test buttons PORTB.12-14
    setPortDigIn(PORTB_BASE, BIT12 | BIT13 | BIT14);                            //ubtn provede nastaveni take
    
#endif    
    
}

void userAppsStart()
{
    //Calls OS on startup
    
    APP_START_PARAM p1;
    p1.TimeLimitExceedBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.GeneralExceptionBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.TrapBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.TimeLimitValue = 0xFFFFF;
    
    
    if(regProcess(&m1_start, 1024, &p1, 0x1) < 0)
    {
        //error, cannot run app
    }
    
    if(regProcess(&m2_start, 1024, &defaultAppStartParam, 0x2) < 0)
    {
        //error, cannot run app
    }
    
    if(regProcess(&m3_start, 1024, &defaultAppStartParam, 0x3) < 0) 
    {
        //error, cannot run app
    }
}

void userAppError(char procId, char code, void* addr)
{
    //Call OS on exception, when CP0_STATUS.EXL=1
    //now, all interrupts are disabled, it is possible execute sw HALT CPU 
    //procId=process ID
    //code=error code, 
    //addr=exception address 
    
    //When this fn ends, OS performs the appropriate action (reset process, reset system, ...)
    //according to "ON_ERROR" app behavior setting
    
    int a=0;
    int b=0;
    int c=a+b;
    
    /*
     * this is HALT code 
     * for example, you can set HALT LED indicator
    while(1)
    {

    }
    */
}

void userAppEventError(char procId, char eventId)
{
    int a=procId;
    int b=eventId;
}

void beforeSleep()
{
    
}

void beforeIdle()
{
    int a=0;
    int b=a+5;
}