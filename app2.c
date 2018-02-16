#include <xc.h>
#include <stdio.h>
#include "globals.h"

#ifdef PIC32MZ

#define     _LED_INV_REG    LATHINV
#define     _LED_INV_VAL    0b010

#endif

#ifdef PIC32MM0064

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b0                 //nic

#endif

#ifdef PIC32MM0256

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b10000000          //RB7                

#endif

static void testSystemTimer(int a);

void m2_start()
{
    systemTimerRegInterval(&testSystemTimer, 500);
    while(1)
    {
        doEvents();
    }
    
    while(1)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        _LED_INV_REG = _LED_INV_VAL;
        
        int a, b=0;
        for(a=0; a<150000; a++)
        {
            b++;
            if(a % 1000 == 0)
            {
                doEvents();
            }
        }
    }
}

static void testSystemTimer(int a)
{
    //int* p=0;
    //int x=*p;
    _LED_INV_REG = _LED_INV_VAL;
    
    /*
    char x;
    
    while(1)
    {
        x=0;
        _LED_INV_REG = _LED_INV_VAL;
        if(systemTimerRegDelay(&x, 900) == 0)
        {
            //chyba
        }
    
        while(x==0) { doEvents(); }
    
        x=0;
        _LED_INV_REG = _LED_INV_VAL;
        if(systemTimerRegDelay(&x, 1500) == 0)
        {
            //chyba
        }
        while(x==0) { doEvents(); }
    }
    */
}