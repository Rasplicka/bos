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

extern GRAPHICS graphics;
int testVar=100;

static void longTime();
static void testSystemTimer(int i);
static void testRTC(char hour, char min, uint date);
static void testButton(int event, int value, int index);
static void call(int a, int b);
static void call1(int a, int b);
static void call2(int a, int b);
static void call3(int a, int b);
static void cn(uint base, uint stat);
static void dispText();

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
    
    /*
    setCanSleep(1);
    cnStartPortB(0);                                //zaktivuje fci CN (SIDL=0, pracuje v IDLE/SLEEP)
    cnEnable(PORTB_BASE, BIT12 | BIT13 | BIT14);                    //povoli PORTA.0, PORTA.1
    
    cnRegEvent(&cn, PORTB_BASE);
    
    while(1)
    {
        doEvents();
    }
    */
    dispText();
    
    //int st=CNSTATB;
    rtcRegTimeAlarm(&testRTC, 0, 1);
    rtcRegTimeAlarm(&testRTC, 0, 2);
    //test sleep mode
    setCanSleep(0);
    
    int c=0;
    while(1)
    {
        if(c % 100 == 0)
        {
            doEvents();
            //call(1,1);
        }
        c++;
        //xxx=PORTB;
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
    //_LED_INV_REG = _LED_INV_VAL;
    //doEvents();
    setCanSleep(1);
    unregEvent(&testSystemTimer);
    
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
    invTestLed(1);
    //_LED_INV_REG = _LED_INV_VAL;
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

static void call(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    call1(1,1);
}

static void call1(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    call2(1,1);
}

static void call2(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    call3(1,1);
}

static void call3(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    //call1(1,1);
}

static void cn(uint base, uint stat)
{
    if((stat & BIT12) != 0)
    {
        invTestLed(1);
        setCanSleep(0);
        systemTimerRegInterval(&testSystemTimer, 5000);
    }
    //setTestLed(1);
    //uint s=stat;
    //setCanSleep(0);
    
}

static void dispText()
{
    //char txt2[] = {"áéíóúý ÁÉÍÓÚÝ"};
    //char txt[] = {"Kratky text"};
    char txt[] = {'J','i','\xFE','í',' ','R','a','\xE7','p','l','i','\x9F','k','a','ý','á','í','é','?','?',' '};
    //char txt[] = {'\xA6','l','u','\x9C','o','u','\x9F','k','\xEC',' ','k','\xDE','\xE5',',',' ','\x9F','e','p','i','c','e'};

    
    short y=0, x=0;
    
    graphics.drawString(txt, NULL, x, y);
    y+=20;
    graphics.drawString(txt, NULL, x, y);
    
    /*
    int a;
    //FONT_SRC fo;
    //setFontSrc(fontConsolas12x20, &fo);
    //fo.foreColor=colWHITE;    //RGB16(31, 63, 31);
    //fo.bgColor=colBLACK;    //RGB16(0, 0, 0);
     
    FONT_SRC foa;
    setFontSrc(fontArial18, &foa);
    foa.foreColor=colWHITE; //RGB16(31, 63, 31);
    foa.bgColor=colBLACK;   //RGB16(0, 0, 0);    
    
    d->drawString(txt, &foa, 0, y);
    y+=foa.height;
    */
    
    
    //short color=colWHITE;
    //d->drawLine(0, 0, 319, 0, color);
    //d->drawLine(319, 0, 319, 239, color);
    //d->drawLine(319, 239, 0, 239, color);
    //d->drawLine(0, 239, 0, 0, color);
    
}