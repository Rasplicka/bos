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
    //reg_process((int*) &m1_start, 1024);
    //reg_process((int*) &m2_start, 1024);
    //reg_process((int*) &m3_start, 1024); 
}