#include <xc.h>
#include <stdio.h>
#include "globals.h"


#ifdef PIC32MZ

#define     _LED_INV_REG    LATHINV
#define     _LED_INV_VAL    0b100

#endif

#ifdef PIC32MM0064

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b0                 //nic

#endif

#ifdef PIC32MM0256

#define     _LED_INV_REG    LATCINV
#define     _LED_INV_VAL    0b1000              //RC3                

#endif
static void testSystemTimer();
static void testSystemTimer2();

void m3_start()
{
    //testSystemTimer2();
    //testSystemTimer();
    
    while(1)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        _LED_INV_REG = _LED_INV_VAL;
        
        int a, b=0;
        for(a=0; a<120000; a++)
        {
            b++;
            if(a % 1000 == 0)
            {
                doEvents();
            }
        }
    }   
        
   
}

static void testSystemTimer()
{
    char x=0;
    
    if(systemTimerRegInterval(&x, 300) == 0)
    {
        //chyba
    }
    
    while(1)
    {
        while(x==0) { doEvents(); }
        _LED_INV_REG = _LED_INV_VAL;
        x=0;
    }
}

static void testSystemTimer2()
{
    signalizeError(3, 5);
    
    while(1)
    {
        //_LED_INV_REG = _LED_INV_VAL;
        testLedClear(3);
        pauseEvents(500);
        
        
        //_LED_INV_REG = _LED_INV_VAL;
        testLedSet(3);
        pauseEvents(50);
        
    }
}