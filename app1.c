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


void m1_start()
{
    int x=0;
    while(x<500)
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



