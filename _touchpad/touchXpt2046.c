#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "../globals.h"

#ifdef USE_TOUCHPAD_XPT2046

#define     IFACE_INDEX     1               //(SPI2)

//definuje hodnoty krajnich bodu (plati pro 8-bit, Vcc=3.3V)
#define     MINX            10
#define     MAXX            110
#define     MINY            5
#define     MAXY            120
#define     MODE8BIT                        //8-bit, jinak 12-bit

//IRQ B7, CS B6
static PIN_INFO irqPin={PORTB_BASE, BIT7};
static PIN_INFO csPin={PORTB_BASE, BIT6};

//b7=1 start, b6,5,4 = 001Y, 101X, b3 1=8bit, 0=12bit, b2=0 interni ref, b1,0 = 00
#ifdef MODE8BIT
static char mx_out=0b11011000; 
static char my_out=0b10011000;
#else
static char mx_out=0b11010000; 
static char my_out=0b10010000;
#endif

static void onEvent();
static void setResult(short x, short y);
static void iface_getPort();
static void iface_freePort();

static short x_data, y_data, prew_x_data=-1, prew_y_data=-1;
static short screenX=0, screenY=0;                                              //result X, Y
static DISPLAY* display=NULL;                                                   //must set application (_setDisplay)
static char buffer[2];

void touchXpt2046_start()
{
    //pauseEvents(2000);               //ceka na init displeje
    
    while(display==NULL || display->getInitialized()==0)
    {
        //wait while display not initialized
        doEvents();
    }
    
    int speed;
    //char x1=1;
    while(1)
    {
        //x1=PORTAbits.RA4;
        //vstup PENIRQ je aktivni v 0
        if(getPin(&irqPin)==0) 
        {
            //nastal dotek (penirq=0)
            iface_getPort();
            speed=spiSetSpeed(IFACE_INDEX, 0x8);                                //backup and change speed
        
            //write command: read x
            buffer[0]=mx_out;
            spiExchange(IFACE_INDEX, buffer, NULL, 1);
            //pInfo_touchSys.writeBuffer(&pInfo_touchSys, buffer, 1);
    
            pauseEvents(10);
        
            //read data (x)
            buffer[0]=0; buffer[1]=0;
            spiExchange(IFACE_INDEX, buffer, buffer, 2);
            //pInfo_touchSys.exchangeBuffer(&pInfo_touchSys, buffer, buffer, 2);
#ifdef MODE8BIT   
            x_data=buffer[0];
            
#else
            x_data = ((short)buffer[0]) << 4;
            x_data |= (((short)buffer[1]) >> 4);
#endif            
    
            pauseEvents(10);
        
            //write command: read y
            buffer[0]=my_out;
            spiExchange(IFACE_INDEX, buffer, NULL, 1);
            //pInfo_touchSys.writeBuffer(&pInfo_touchSys, buffer, 1);
        
            pauseEvents(10);
            
            //read data (y)
            buffer[0]=0; buffer[1]=0;
            spiExchange(IFACE_INDEX, buffer, buffer, 2);
            //pInfo_touchSys.exchangeBuffer(&pInfo_touchSys, buffer, buffer, 2);
#ifdef MODE8BIT   
            y_data=buffer[0];
            
#else
            y_data = ((short)buffer[0]) << 4;
            y_data |= (((short)buffer[1]) >> 4);
#endif               
       
            spiSetSpeed(IFACE_INDEX, speed);                    //restore speed
            iface_freePort();
                                            
       
            //if(x_data<MINX){x_data=MINX;}
            //if(x_data>MAXX){x_data=MAXX;}
            //if(y_data<MINY){y_data=MINY;}
            //if(y_data>MAXY){y_data=MAXY;}
            //screenX=x_data;
            //screenY=y_data;
            //onEvent();
            
            if(x_data>=MINX && x_data<=MAXX && y_data>=MINY && y_data<=MAXY)
            {
            //screenX=x_data;
            //screenY=y_data;
            setResult(x_data, y_data);
            onEvent();
            }
            
            /*
            if( (x_data >= prew_x_data-5) && (x_data <= prew_x_data+5) )
            {
                if( (y_data >= prew_y_data-5) && (y_data <= prew_y_data+5) )
                {
                    setResult(x_data, y_data);
                    onEvent();
                }
            }
            prew_x_data=x_data;
            prew_y_data=y_data;
            */
        }
        else
        {
            prew_x_data=-1;
            prew_y_data=-1;
        }
        
        doEvents();
    }
    
}
void touchXpt2046_setDisplay(DISPLAY* d)
{
    display=d;
}
int touchXpt2046_regEvent(void* fn)
{
    regListener(fn, TOUCHPAD_EVENT_ID);
}

static void onEvent()
{
    uint* item=getRegEvent(NULL, TOUCHPAD_EVENT_ID);
    
    while(item != NULL)
    {
        raiseEvent(item, screenX, screenY, 0);
        
        item=getRegEvent(item, TOUCHPAD_EVENT_ID);
    } 
    
    /*
    int a;
    for(a=0; a<EVENT_CAPA; a++)
    {
        if(events[a]!=0x0)
        {
            _event = (void*)events[a];
            _event(screenX, screenY);
        }
        else
        {
            //konec
            return;
        }
    }    
    */
}

static void setResult(short x, short y)
{
    if(display==NULL){return;}
    float w=(float)display->getWidth();     //  320;
    float h=(float)display->getHeight();    // 240;
    char orient=display->getOrientation();
    
    float f;
    
    if(orient==0)
    {
        //na vysku, normalne
        f=((float)MAXY - (float) y) / ((float)MAXY - (float)MINY) * h;
        screenY=(short)f;
        if(screenY < 0){ screenY = 0; }
        if(screenY >= h){ screenY = h-1; }
        
        f=((float)MAXX - (float) x) / ((float)MAXX - (float)MINX) * w;
        screenX= w-(short)f;
        if(screenX < 0){ screenX = 0; }
        if(screenX >= w){ screenX = w-1; }          
    }
    else if(orient==1)
    {
        //na sirku, normalne
        f=((float)MAXY - (float) y) / ((float)MAXY - (float)MINY) * w;
        screenX=(short)f;
        if(screenX < 0){ screenX = 0; }
        if(screenX >= w){ screenX = w-1; }
        
        f=((float)MAXX - (float) x) / ((float)MAXX - (float)MINX) * h;
        screenY=(short)f;
        if(screenY < 0){ screenY = 0; }
        if(screenY >= h){ screenY = h-1; }        
    }
    else if(orient==2)
    {
        //na vysku, obracene
        f=((float)MAXY - (float) y) / ((float)MAXY - (float)MINY) * h;
        screenY=h-(short)f;
        if(screenY < 0){ screenY = 0; }
        if(screenY >= h){ screenY = h-1; }
        
        f=((float)MAXX - (float) x) / ((float)MAXX - (float)MINX) * w;
        screenX=(short)f;
        if(screenX < 0){ screenX = 0; }
        if(screenX >= w){ screenX = w-1; }          
    }
    else if(orient==3)
    {
        //na sirku, obracene
        f=((float)MAXY - (float) y) / ((float)MAXY - (float)MINY) * w;
        screenX=w-(short)f;
        if(screenX < 0){ screenX = 0; }
        if(screenX >= w){ screenX = w-1; }
        
        f=((float)MAXX - (float) x) / ((float)MAXX - (float)MINX) * h;
        screenY=h-(short)f;
        if(screenY < 0){ screenY = 0; }
        if(screenY >= h){ screenY = h-1; }        
    }    
    //screenX=x;
    //screenY=y;
    
    //intToChar(screenX, val, 4);
    //createString(res, 32, "x= ", val, NULL);
    //graphics.drawString(res, NULL, 100, 80);

    //intToChar(screenY, val, 4);
    //createString(res, 32, "y= ", val, NULL);
    //graphics.drawString(res, NULL, 100, 110);    
}

//Use the SPI module if it is not occupied, otherwise, wait to release
static void iface_getPort()
{
    //obsadi SPI port, pokud neni volny, ceka na uvolneni (doEvents)
    spiUse(IFACE_INDEX, NULL, NULL); //&eventDC);

    //aktivuje CS signal displeje
    clearPin(&csPin);
}

//Wait to complete the transmission, then release SPI module
static void iface_freePort()
{
    //uvolni SPI port, predtim ceka na dokonceni vysilani (doEvents)
    spiFree(IFACE_INDEX);
    
    //deaktivuje CS signal displeje
    setPin(&csPin);
}

#endif  //USE_TOUCHPAD_XPT2046