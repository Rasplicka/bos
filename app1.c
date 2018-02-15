#include <xc.h>
#include <stdio.h>
#include "globals.h"

#ifdef PIC32MZ

#define     _LED_INV_REG    LATHINV
#define     _LED_INV_VAL    0b001

#endif

#ifdef PIC32MM0064

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b100000

#endif  

#ifdef PIC32MM0256

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b100000          //RB5                

#endif

static void testSystemTimer();

void m1_start()
{
    //testSystemTimer();
    
    int x=0;
    while(x<1000)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        _LED_INV_REG = _LED_INV_VAL;
        
        int a, b=0;
        for(a=0; a<100000; a++)
        {
            b++;
            if(a>0 && a % 1000 == 0)
            {
                doEvents();

            }
        }
        //stack overflow
        //m1_start()
        x++;
    }    
    
    //int* ad=0;
    //int dat=*ad;
    
    while(1)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        _LED_INV_REG = _LED_INV_VAL;
        
        int a, b=0;
        for(a=0; a<190000; a++)
        {
            b++;
            if(a % 10000 == 0)
            {
                doEvents();
            }
        }
        x++;
    }    
}

static void testSystemTimer()
{
    char x;
    
    while(1)
    {
        x=0;
        _LED_INV_REG = _LED_INV_VAL;
        if(systemTimerRegDelay(&x, 1000) == 0)
        {
            //chyba
        }
        
        while(x==0) { doEvents(); }
    
        x=0;
        _LED_INV_REG = _LED_INV_VAL;
        if(systemTimerRegDelay(&x, 500) == 0)
        {
            //chyba
        }
        while(x==0) { doEvents(); }
    }
}

