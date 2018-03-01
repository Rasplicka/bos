#include <xc.h>
#include <stdio.h>
#include "globals.h"

#ifdef PIC32MZ

//red LED
#define     _LED_INV_REG    LATHINV
#define     _LED_INV_VAL    0b001                               
int delay=10;
#endif

#ifdef PIC32MM0064

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b100000
int delay=1;
#endif  

#ifdef PIC32MM0256

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b100000          //RB5                
int delay=1;
#endif
static void longTime();
static void testSystemTimer(int i);
static void testRTC(char hour, char min, uint date);
static void testButton(int event, int value, int index);

static int restart=0;

void m1_start()
{
    //if(restart==0)
    //{
    //    setCanIdle(1);
    //}
    //rtcRegTimeAlarm(&testRTC, 0, 1);
    //setClock(CLOCK_CFG.CLK_ALT3);
    
    //testSystemTimer();
    //systemTimerRegInterval(&testSystemTimer, 1000);
    
    rtcRegTimeAlarm(&testRTC, 0, 1);
    rtcRegTimeAlarm(&testRTC, 0, 2);
    //test sleep mode
    //setCanSleep(1);
    while(1)
    {
        doEvents();
    }

    
    
    //ubtnRegEvent(&testButton, 1, 10);
    //trap();
    
    int x=0;
    while(x<10000)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        _LED_INV_REG = _LED_INV_VAL;
        
        int a, b=0;
        for(a=0; a<(100000 * delay); a++)
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
    
    //raiseEventID(155, 0, 1, 2);
    //uint day=getDayMs();
    //uint second=(day/1000) % 60;
    //uint minute=(day/1000/60) % 60;
    //uint hour=(day/1000/60/60);
    
    //int* ad=0;
    //int dat=*ad;
    
    while(1)
    {
        //do LATxINV zapise 1 na prislusnou pozici
        _LED_INV_REG = _LED_INV_VAL;
        
        int a, b=0;
        for(a=0; a<250000; a++)
        {
            b++;
            if(a % 100 == 0)
            {
                doEvents();
            }
        }
        x++;
    } 
    
    
}

static void longTime()
{
        int a, b=0;
        for(a=0; a<190000; a++)
        {
            b++;
            if(a % 1000 == 0)
            {
                doEvents();
            }
        }    
}

static void testSystemTimer(int i)
{

    //int d=a+b;
    _LED_INV_REG = _LED_INV_VAL;
    //doEvents();
    
    /*
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
    */
    
}

static void testRTC(char hour, char min, uint date)
{
    _LED_INV_REG = _LED_INV_VAL;
    setCanSleep(0);
    //setCanIdle(0);
    //restart=1;
    //restartProcess();
}

static void testButton(int event, int value, int index)
{
    trap();
    //exitProcess();
    //if(value==0)
    //{
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