#include <xc.h>
#include <stdio.h>
#include "globals.h"

#ifdef PIC32MZ

#define     _LED_INV_REG    LATHINV
#define     _LED_INV_VAL    0b010
int delay=10;
#endif

#ifdef PIC32MM0064

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b0                 //nic
int delay=1;
#endif

#ifdef PIC32MM0256

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b10000000          //RB7                
int delay=1;
#endif

//static int testVar2=50;
static int cnt=0;
static void testSystemTimer(int a);
static void testButton(int event, int value, int index);
static void listener(int p0, int p1, int p2);
static void cn(uint base, uint stat);

void m2_start()
{
    
    //setCanIdle(1);

    //ubtnRegEvent(&testButton, 11, 20);
    //systemTimerRegInterval(&testSystemTimer, 2000);
    setCanSleep(1);
 
    /*
    cnRegEvent(&cn, PORTB_BASE);
    while(1)
    {
        doEvents();
    }
    */

    //ubtnRegEvent(&testButton, 0, 0xFF);
    //regListener(&listener, 155);
    
    while(1)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        //_LED_INV_REG = _LED_INV_VAL;
        invTestLed(2);
        
        int a, b=0;
        for(a=0; a<(150000*delay); a++)
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
    setCanSleep(1);
    unregEvent(&testSystemTimer);
    
    //int* p=0;
    //int x=*p;
    //invTestLed(2);
    //_LED_INV_REG = _LED_INV_VAL;
    
    /*
    cnt++;
    
    if(cnt==5)
    {
        setCanIdle(0);
    }
    
    if(cnt==15)
    {
        setCanIdle(1);
    }
    */
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

static void testButton(int event, int value, int index)
{
    //if(value==1)
    //{
    //setCanIdle(1);
    //exitProcess();
    /*
        if(event==UBTN_DOWN)
        {
            _LED_INV_REG = _LED_INV_VAL;
        }
    
        if(event==UBTN_UP)
        {
            _LED_INV_REG = _LED_INV_VAL;
        }
    */
    //}
}

static void listener(int p0, int p1, int p2)
{
    int a=p0+p1+p2;
    
}

static void cn(uint base, uint stat)
{
    if((stat & BIT13) != 0)
    {
        invTestLed(2);
        setCanSleep(0);
        systemTimerRegInterval(&testSystemTimer, 20000);
    }
    //setTestLed(1);
    //uint s=stat;
    //setCanSleep(0);
    
}