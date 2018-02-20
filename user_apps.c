#include <xc.h>
#include <stdio.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS user apps interface
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
    
#ifdef __32MM0256GPM048__     
    
#endif    
    
#ifdef __32MM0256GPM064__     
    
#endif  
    
#ifdef PIC32MZ

#endif    
    
}

void userAppsStart()
{
    //Calls OS on startup
    
    APP_START_PARAM p1;
    p1.TimeLimitExceedBehavior = ON_ERROR.RESET_PROCESS;
    p1.GeneralExceptionBehavior = ON_ERROR.RESET_PROCESS;
    p1.TrapBehavior = ON_ERROR.RESET_SYSTEM;
    p1.defaultID = 1;
    p1.TimeLimitValue = 0xFFFF;
    
    
    if(regProcess(&m1_start, 1024, &p1) < 0)
    {
        //error, cannot run app
    }
    
    if(regProcess(&m2_start, 1024, &defaultAppStartParam) < 0)
    {
        //error, cannot run app
    }
    
    if(regProcess(&m3_start, 1024, &defaultAppStartParam) < 0) 
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