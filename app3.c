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

void m3_start()
{
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