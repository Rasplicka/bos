#include <xc.h>
#include <stdio.h>
#include "globals.h"


#ifdef PIC32MZ

#define     _LED_INV_REG    LATHINV
#define     _LED_INV_VAL    0b100
int delay=10;
#endif

#ifdef PIC32MM0064

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b0                 //nic
int delay=1;
#endif

#ifdef PIC32MM0256

#define     _LED_INV_REG    LATCINV
#define     _LED_INV_VAL    0b1000              //RC3                
int delay=1;
#endif
static void testSystemTimer();
static void testSystemTimer2();
static void testButton(int event, int value, int index);
static void listener(int p0, int p1, int p2);

void m3_start()
{
    setCanIdle(0);
    setCanSleep(0);
    
    //systemTimerRegInterval(&testSystemTimer, 4000);
    
    //ubtnRegEvent(&testButton, 21, 30);
    //while(1)
    //{
    //    doEvents();
    //}    
    
    
    //regListener(&listener, 155);
    
    while(1)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        //_LED_INV_REG = _LED_INV_VAL;
        invTestLed(3);
        
        int a, b=0;
        for(a=0; a<(200000 * delay); a++)
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
    
    _LED_INV_REG = _LED_INV_VAL;
    
    /*
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
    */
}

static void testSystemTimer2()
{
    signalizeError(3, 5);
    
    while(1)
    {
        //_LED_INV_REG = _LED_INV_VAL;
        clearTestLed(3);
        pauseEvents(500);
        
        
        //_LED_INV_REG = _LED_INV_VAL;
        setTestLed(3);
        pauseEvents(50);
        
    }
}

static void testButton(int event, int value, int index)
{
    //if(value==2)
    //{
        if(event==UBTN_DOWN)
        {
            _LED_INV_REG = _LED_INV_VAL;
        }
    
        if(event==UBTN_UP)
        {
            _LED_INV_REG = _LED_INV_VAL;
        }
    //}
}

static void listener(int p0, int p1, int p2)
{
    int a=p0+p1+p2;
    
}
