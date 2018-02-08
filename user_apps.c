#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

//user apps
//app1
extern void m1_start();
//app2
extern void m2_start();
//app3
extern void m3_start();

void userAppsStart()
{
    //Call OS on startup
    
    APP_START_PARAM p1;
    p1.LongLastingValue=0xFFFFF;
    p1.GeneralExceptionBehavior = ERROR_RESET_PROCESS;
    p1.LongLastingBehavior = ERROR_RESET_SYSTEM;
    
    if(reg_process((int*) &m1_start, 1024, &defaultAppStartParam) < 0)
    {
        //error
    }
    
    if(reg_process((int*) &m2_start, 1024, &p1) < 0)
    {
        //error
    }
    
    if(reg_process((int*) &m3_start, 1024, &p1) < 0) 
    {
        //error
    }
}

void userAppError(char id, char code)
{
    //Call OS on error, before process restart, or system restart
    int a=0;
    int b=0;
    int c=a+b;
}