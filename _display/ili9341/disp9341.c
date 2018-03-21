#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../globals.h"
//#include "disp9341.h"
//#include "disp16_asm.h"

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS driver for display ILI9341
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */

//https://gist.github.com/postmodern/ed6e670999f456ad9f13

#ifdef USE_DISP9341
IMAGE_SRC* defaultFont;

//IFACE
#define     IFACE_INDEX     1                       //SPI2, index portu (0=SPI1, 1=SPI2, 2=SPI3, ...)
//definuje IO piny
PIN_INFO csPin = {PORTC_BASE, BIT8};                //CS signal
PIN_INFO resetPin = {PORTA_BASE, BIT15};            //RESET signal
PIN_INFO dcPin = {PORTB_BASE, BIT13};               //DC signal
PIN_INFO blPin = {PORTC_BASE, BIT9};                //backLight

#define     SW_RESET                                //je definovano, pokud je RESET signal pripojen k IO pinu. Neni-li definovano, ma HW RESET (RC obvod)
#define     BUFFER_SIZE      1024                   //min. velikost by mela byt: Width x 2 + 20 (viz. fillBox, clear)  

//globalni promene modulu disp9341 (tzn. pri pouziti vice displeju ili9341 jsou spolecne pro vsechny struct DISPLAY)
static char isInitialized=0;
static char pixAB=0;
static char pixelsEven[BUFFER_SIZE] __attribute__((aligned(4)));
static char pixelsOdd [BUFFER_SIZE] __attribute__((aligned(4)));
static LINE_SRC lineSrc;
static POINT point;

//privatni struct pro dany displej, pri pouziti vice displeju se nastavi pri kazdem volani setGraphics
static DISPLAY* displayInfo=NULL;

static short Width=240;                         //default na vysku, fce dinit nastavi displej na sirku a upravi W a H              
static short Height=320;
static char Orientation=0;
static char directMode=1;                       //0=zapis do bufferu, 1=zapis do portu


//local void
static void selectDriver(void* d);
static void setDefaultFont(IMAGE_SRC* font);
static void drawString(char* text, IMAGE_SRC* font, short x, short y);
static void fillBox(short x1, short y1, short x2, short y2, short color);
static void drawLine(short x1, short y1, short x2, short y2, short w, short color);
static void drawImage(IMAGE_SRC* da, short x, short y);
static void drawPoint(short x, short y, short color);
static short textWidth(char* text, IMAGE_SRC* font);
static void clear(short color);
static void initDisplay();
static void setOrientation(char x);
static char getOrientation();
static void setBrightness(char val);
static void controlDisplay(char on, char sleep, char bl, char inv);
static char getInitialized();
static void print(char* t);
static short getWidth();
static short getHeight();
static short getFontHeight(IMAGE_SRC* font);
static void dinit();
static void writeChar(IMAGE_SRC* font, char code, short x, short y);
static void eventDC(char x);
static void setColorDef();
static char* getBuffer();
static void resetDisplay();
static void setDCPin(char value);
static void setBacklight(char value);

//fce iface
//bude-li dispay pripojen pres jiny iface, staci zmenit tyto fce
static void iface_getPort(); 
static void iface_freePort();
static void iface_writeBufferMode(char* buffer, short len, char mode);
static volatile int* iface_getHWBuffer();
static void iface_setBusMode(char mode);
static void iface_Process();


//global
void disp9341_driver(DISPLAY* d)
{
    d->selectDriver=&selectDriver;
    d->setDefaultFont=&setDefaultFont;
    d->drawString=&drawString;
    d->fillBox=&fillBox;
    d->drawLine=&drawLine;
    d->drawImage=&drawImage;
    d->drawPoint=&drawPoint;
    d->textWidth=&textWidth;
    d->print=&print;
    d->clear=&clear;
    d->initDisplay=&initDisplay;
    d->setOrientation=&setOrientation;
    d->setBrightness=&setBrightness;
    d->controlDisplay=&controlDisplay;
    d->getInitialized=&getInitialized;
    d->getOrientation=&getOrientation;
    d->getWidth=&getWidth;
    d->getHeight=&getHeight;
    d->getFontHeight=&getFontHeight;
}


//local
static void selectDriver(void* d)
{
    displayInfo=(DISPLAY*)d;
}

static void setDefaultFont(IMAGE_SRC* font)
{
    defaultFont=font;    
}
static void drawString(char* text, IMAGE_SRC* font, short x, short y)
{
    int a;
    int l=strLen(text);
    
    if(font==NULL) { font=defaultFont; }
    
    iface_getPort();
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif    
    
    for(a=0; a<l; a++)
    {
        writeChar(font, text[a], x, y);
        x+=font->width;
    }
    
#ifdef WATCHDOG_TIMER     
    startWDT();
#endif    
    
    iface_freePort();
}

static void fillBox(short x1, short y1, short x2, short y2, short color)
{
    char* buffer=getBuffer();  
    
    //x1 musi byt mensi, nez x2, to same y1 a y2
    if(x1 > x2) { short x=x1; x1=x2, x2=x; }
    if(y1 > y2) { short y=y1; y1=y2, y2=y; }  

    
    if(x1 >= Width) { return; }                             //mimo, vlevo
    if(x2 < 0) { return; }                                  //mimo, vpravo
    if(y1 >= Height) { return; }                            //mimo nahore
    if(y2 < 0) { return; }                                  //mimo dole
        
    if (x1 < 0) { x1 = 0; }
    if (x2 > Width - 1) { x2 = Width - 1; }

    if (y1 < 0) { y1 = 0; }
    if (y2 > Height - 1) { y2 = Height - 1; }

    short w=x2-x1 + 1;
    short h=y2-y1 + 1;
    
    iface_getPort();
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif  
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
    buffer[0]=0b00000001;                                   //control byte
    buffer[1]=0x2A;                                         //command (ColSet)
    
    buffer[2]=0b01000100;                                   //control byte
    buffer[3]=(char)(x1>>8);                                //data, start col H
    buffer[4]=(char)x1;                                     //data, start col L
    buffer[5]=(char)(x2>>8);                                //data, end col H
    buffer[6]=(char)x2;                                     //data, end col L
    
    buffer[7]=0b00000001;                                   //control byte
    buffer[8]=0x2B;                                         //command (RowSet)
    
    buffer[9]=0b01000100;                                   //control byte
    buffer[10]=(char)(y1>>8);                               //data, start row H
    buffer[11]=(char)y1;                                    //data, start row L
    buffer[12]=(char)(y2>>8);                               //data, end row H
    buffer[13]=(char)y2;                                    //data, end row L

    buffer[14]=0b00000001;                                  //control byte
    buffer[15]=0x2C;                                        //write data    
    buffer[16]=0b11000000;                                  //control byte nastav DC=1

    iface_writeBufferMode(buffer, 17, WRITE_MODE.Stream);
    
    if(directMode==1)
    {
        //directMode    
        iface_Process();                                    //ceka na dokonceni
    
        iface_setBusMode(BUS_MODE._16bit);
        fillRectDirect(color, (w*h), iface_getHWBuffer());
        iface_setBusMode(BUS_MODE._8bit);
    }
    else
    {
        int a;
        
        buffer=getBuffer();                                 //druhy buffer
        short* pix16=(short*)buffer;
    
        //napln buffer linkou clr barvy
        for(a=0; a<w; a++)
        {
            pix16[a]=color;
        }

        //odeslat buffer Height krat
        for(a=0; a<h; a++)
        {
            iface_writeBufferMode(buffer, w*2, WRITE_MODE.DataOnly);
        }
    }
    
#ifdef WATCHDOG_TIMER    
    startWDT();
#endif
    
#ifdef SAFE_PROCESS
    startCT();
#endif     
    
    iface_freePort();
}

static void drawLine(short x1, short y1, short x2, short y2, short w, short color)
{
    
    //horiz. nebo vert. cara
    //kresli box, je-li sirsi nez 1px, y(horiz), x(vert) je stred cary
    
    if(y1==y2)
    {
        //horizontal line
        if(x1>x2) { short x=x1; x1=x2, x2=x; }

        //sirka cary > 1
        char ud=0;
        while(w>1)
        {
            if(ud==0){y2++; ud=1;}
            else{y1--; ud=0;}
            w--;
        }
        
        fillBox(x1, y1, x2, y2, color);
    }
    else if (x1==x2)
    {
        //vertical line
        if(y1>y2) { short y=y1; y1=y2, y2=y; }

        //sirka cary > 1
        char ud=0;
        while(w>1)
        {
            if(ud==0){x2++; ud=1;}
            else{x1--; ud=0;}
            w--;
        }
        
        fillBox(x1, y1, x2, y2, color);
    }
    else
    {
        //neni vodorovna, ani svisla
        iface_getPort();
        
#ifdef WATCHDOG_TIMER        
        pauseWDT();
#endif
        
#ifdef SAFE_PROCESS
    pauseCT();
#endif         
        //directMode
        if(directMode==1)
        {
            //directMode
            lineSrc.displayWidth=Width;
            lineSrc.displayHeight=Height;
            lineSrc.x1=x1;
            lineSrc.x2=x2;
            lineSrc.y1=y1;
            lineSrc.y2=y2;
            lineSrc.color=color;
    
            iface_setBusMode(BUS_MODE._16bit);
            drawLineQuick(&lineSrc, iface_getHWBuffer(), &setDCPin);
            iface_setBusMode(BUS_MODE._8bit);
        }   
        else
        {
            //buffer mode
            short dx=x2-x1, dy=y2-y1;
            if(dx<0) {dx*=-1;}
            if(dy<0) {dy*=-1;}

            if(dx>=dy)
            {    
                //x1 musi byt mensi, nez x2
                if(x1 > x2) 
                { 
                    short x=x1; x1=x2, x2=x; 
                    short y=y1; y1=y2, y2=y;
                }
    
                short rx=x2-x1;
                short ry=y2-y1;
                float r=(float)ry/(float)rx;
    
                short a; 
                short resy;

                for(a=x1; a<=x2; a++)
                {
                    resy=((float)a)*r + y1;
                    drawPoint(a, resy, color);
                }
            }
            else
            {
                //y1 musi byt mensi, nez y2
                if(y1 > y2) 
                { 
                    short y=y1; y1=y2, y2=y;
                    short x=x1; x1=x2, x2=x; 
                }        
        
                short rx=x2-x1;
                short ry=y2-y1;
                float r=(float)rx/(float)ry;
    
                short a; 
                short resx;        
        
                for(a=y1; a<=y2; a++)
                {
                    resx=((float)a)*r + x1;
                    drawPoint(resx, a, color);
                }        
            }            
        }
        
#ifdef WATCHDOG_TIMER        
        startWDT();
#endif    
        
#ifdef SAFE_PROCESS
    startCT();
#endif         
        
        iface_freePort();
    }
}

static void drawImage(IMAGE_SRC* da, short x, short y)
{
    
    if(x + da->width <= 0) { return; }                      //mimo, vlevo
    if(x >= Width) { return; }                              //mimo, vpravo
    if(y + da->height <= 0) { return; }                     //mimo nahore
    if(y >= Height) { return; }                             //mimo dole
    
    short dx1, dx2, dy1, dy2;
    char is_out=0;
    
    //startX, dx1
    if(x < 0) { da->start_x = x*(-1); dx1 = 0; is_out++; }
    else { da->start_x = 0; dx1 = x; }
    
    //endX, dx2
    dx2 = x + da->width - 1;
    if(dx2 >= Width) { dx2 = Width-1; da->end_x = Width - x - 1;  is_out++;}
    else { da->end_x = da->width-1; }
    
    //startY, dy1
    if(y < 0) { da->start_y = y*(-1); dy1 = 0; is_out++; }
    else { da->start_y = 0; dy1 = y; }
    
    //endY, dy2
    dy2 = y + da->height - 1;
    if(dy2 >= Height) { dy2 = Height-1; da->end_y = Height - y - 1;  is_out++; }
    else { da->end_y = da->height-1; }    
    
    //nastav startX=-1, pokud je cely obrazek na displeji (zadna cast neni mimo)
    if(is_out==0) { da->start_x = -1; }
    
    char* buffer=getBuffer(); 
    iface_getPort();
    
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif 
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
    buffer[0]=0b00000001;                                   //control byte
    buffer[1]=0x2A;                                         //command (ColSet)
    
    buffer[2]=0b01000100;                                   //control byte
    buffer[3]=(char)(dx1>>8);                               //data, start col H
    buffer[4]=(char)(dx1);                                  //data, start col L
    buffer[5]=(char)((dx2)>>8);                             //data, end col H
    buffer[6]=(char)(dx2);                                  //data, end col L
    
    buffer[7]=0b00000001;                                   //control byte
    buffer[8]=0x2B;                                         //command (RowSet)
    
    buffer[9]=0b01000100;                                   //control byte
    buffer[10]=(char)(dy1>>8);                              //data, start row H
    buffer[11]=(char)(dy1);                                 //data, start row L
    buffer[12]=(char)((dy2)>>8);                            //data, end row H
    buffer[13]=(char)(dy2);                                 //data, end row L

    buffer[14]=0b00000001;                                  //control byte
    buffer[15]=0x2C;                                        //write data
    buffer[16]=0b11000000;                                  //control byte (ukonci SPI mode 1, odesila zbytek bufferu)
    iface_writeBufferMode(buffer, 17, WRITE_MODE.Stream);
    
    if(directMode==1)
    {
        //direct write data >> SPIBUF
        iface_Process();                                    //ceka na dokonceni 
    
        iface_setBusMode(BUS_MODE._16bit);
        imageToPort(da, iface_getHWBuffer(), 0, 2);
        iface_setBusMode(BUS_MODE._8bit);
    }
    else
    {
        int len;
        do
        {
            buffer=getBuffer();
            len=imageToBuffer(da, buffer, BUFFER_SIZE, BUS_MODE._8bit); 
            iface_writeBufferMode(buffer, len, WRITE_MODE.DataOnly);
        } while(da->eof == 0);
    }
    
#ifdef WATCHDOG_TIMER    
    startWDT();
#endif
    
#ifdef SAFE_PROCESS
    startCT();
#endif     
    
    iface_freePort();
}

static void drawPointBuffer(short x, short y, short color)
{
    //tato fce je volana z jinych, ktere provedou pauseWDT()
    //buffer mode
    if(x < 0 || x >= Width)  { return; }
    if(y < 0 || y >= Height) { return; }
    
    char* buffer=getBuffer(); 
    
    buffer[0]=0b00000001;                                   //control byte
    buffer[1]=0x2A;                                         //command (ColSet)
    
    buffer[2]=0b01000100;                                   //control byte
    buffer[3]=(char)(x>>8);                                 //data, start col H
    buffer[4]=(char)x;                                      //data, start col L
    buffer[5]=(char)(x>>8);                                 //data, end col H
    buffer[6]=(char)x;                                      //data, end col L
    
    buffer[7]=0b00000001;                                   //control byte
    buffer[8]=0x2B;                                         //command (RowSet)
    
    buffer[9]=0b01000100;                                   //control byte
    buffer[10]=(char)(y>>8);                                //data, start row H
    buffer[11]=(char)y;                                     //data, start row L
    buffer[12]=(char)(y>>8);                                //data, end row H
    buffer[13]=(char)y;                                     //data, end row L

    buffer[14]=0b00000001;                                  //control byte
    buffer[15]=0x2C;                                        //write data    
    buffer[16]=0b11000000;                                  //control byte nastav DC=1
    
    buffer[17]=(char)(color>>8);
    buffer[18]=(char)color;

    iface_writeBufferMode(buffer, 19, WRITE_MODE.Stream);    
}

static void drawPoint(short x, short y, short color)
{
    iface_getPort();
    
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif 
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
    if(directMode==1)
    {
        //directMode
        point.x=x;
        point.y=y;
        point.color=color;
        
        iface_setBusMode(BUS_MODE._16bit);
        drawPointQuick(&point, iface_getHWBuffer(), &setDCPin);
        iface_setBusMode(BUS_MODE._8bit);
    }
    else
    {
        drawPointBuffer(x, y, color);
    }

#ifdef WATCHDOG_TIMER    
    startWDT();
#endif  
    
#ifdef SAFE_PROCESS
    startCT();
#endif     
    
    iface_freePort();
}

static short textWidth(char* text, IMAGE_SRC* font)
{
    int a, w=0;
    int l=strLen(text);
    
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif    
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif 
    
    for(a=0; a<l; a++)
    {
        fontCharParam(font, text[a]);
        w+=font->width;
    }     
    
#ifdef WATCHDOG_TIMER    
    startWDT();
#endif    

#ifdef SAFE_PROCESS
    startCT();
#endif 
    
    return w;
}

static void clear(short color)
{
    char* buffer=getBuffer();    
    iface_getPort();
    
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif 
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
    buffer[0]=0b00000001;                                   //control byte
    buffer[1]=0x2A;                                         //command (ColSet)
    
    buffer[2]=0b01000100;                                   //control byte
    buffer[3]=0;                                            //data, start col H
    buffer[4]=0;                                            //data, start col L
    buffer[5]=(char)((Width-1)>>8);                         //data, end col H
    buffer[6]=(char)(Width-1);                              //data, end col L
    
    buffer[7]=0b00000001;                                   //control byte
    buffer[8]=0x2B;                                         //command (RowSet)
    
    buffer[9]=0b01000100;                                   //control byte
    buffer[10]=0;                                           //data, start row H
    buffer[11]=0;                                           //data, start row L
    buffer[12]=(char)((Height-1)>>8);                       //data, end row H
    buffer[13]=(char)(Height-1);                            //data, end row L

    
    buffer[14]=0b00000001;                                  //control byte
    buffer[15]=0x2C;                                        //write data  
    buffer[16]=0b11000000;                                  //nastav DC=1

    iface_writeBufferMode(buffer, 17, WRITE_MODE.Stream);
    
    if(directMode==1) //portInfo->directMode)
    {
        //directMode
        iface_Process();                                    //ceka na dokonceni  
        
        iface_setBusMode(BUS_MODE._16bit);
        fillRectDirect(color, (Width*Height), iface_getHWBuffer());
        iface_setBusMode(BUS_MODE._8bit);
    }
    else
    {
        int a;
        buffer=getBuffer();                                 //druhy buffer
        short* pix16=(short*)buffer;
    
        //napln buffer linkou clr barvy
        for(a=0; a<Width; a++)
        {
            pix16[a]=color;
        }
    
        //odeslat buffer Height krat
        for(a=0; a<Height; a++)
        {
            iface_writeBufferMode(buffer, Width*2, WRITE_MODE.DataOnly);
        }
    }
    
#ifdef WATCHDOG_TIMER    
    startWDT();
#endif    
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif 
    
    iface_freePort();
}

static void initDisplay()
{
    isInitialized=0;
    
#ifndef SIMULATOR    
    dinit();
#endif    
    
    isInitialized=1;
}

static void setOrientation(char x)
{
    //0=na vysku, 1=na sirku, 2=na vysku obracene, 3=na sirku obracene
    iface_getPort();
    char* buffer=getBuffer();  
    
    if(x==0)
    {
        //na vysku
        buffer[0]=0b00000001;                       //control byte
        buffer[1]=0x36;                             //0x36 = orientation
        buffer[2]=0b01000001;                       //control byte
        buffer[3]=0x48;                             //param
        iface_writeBufferMode(buffer, 4, WRITE_MODE.Stream);
        
        Width=240;
        Height=320;
    }
    else if(x==1)
    {
        //na sirku
        buffer[0]=0b00000001;                       //control byte
        buffer[1]=0x36;                             //0x36 = orientation
        buffer[2]=0b01000001;                       //control byte
        buffer[3]=0x28;                             //param
        iface_writeBufferMode(buffer, 4, WRITE_MODE.Stream);
        
        Width=320;
        Height=240;
    }
    else if(x==2)
    {
        //na vysku obracene
        buffer[0]=0b00000001;                       //control byte
        buffer[1]=0x36;                             //0x36 = orientation
        buffer[2]=0b01000001;                       //control byte
        buffer[3]=0x88;                             //param
        iface_writeBufferMode(buffer, 4, WRITE_MODE.Stream);
        
        Width=240;
        Height=320;        
    }
    else
    {
        //na sirku obracene
        buffer[0]=0b00000001;                       //control byte
        buffer[1]=0x36;                             //0x36 = orientation
        buffer[2]=0b01000001;                       //control byte
        buffer[3]=0xE8;                             //param
        iface_writeBufferMode(buffer, 4, WRITE_MODE.Stream);
        
        Width=320;
        Height=240;        
    }
    
    Orientation=x;
    iface_freePort();
}

static char getOrientation()
{
    return Orientation;
}

static void setBrightness(char val)
{
    iface_getPort();
    char* buffer=getBuffer();  

    buffer[0]=0b00000001;                       //control byte
    buffer[1]=0x51;                             //0x51 = Brightness
    buffer[2]=0b01000001;                       //control byte
    buffer[3]=val;                              //param 0 ... 0xFF
    
    iface_writeBufferMode(buffer, 4, WRITE_MODE.Stream);
    iface_freePort();
}

static void controlDisplay(char on, char sleep, char bl, char inv)
{
    //hodnota -1 = bez zmeny, 0=OFF, 1=ON
    
    iface_getPort();
    char* buffer;
    
    if(on != 0xFF)
    {
        //ON/OFF
        buffer=getBuffer();
        if(on==0)
        {
            //0=off
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x28;                             //0x28 = off
            setBacklight(0);
        }
        else
        {
            //1=on
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x29;                             //0x28 = off
            setBacklight(1);
        }

        iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    }
    
    if(sleep != 0xFF)
    {
        buffer=getBuffer();
        if(sleep==0)
        {
            //0= sleep off
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x11;                             //0x11 = sleep out
        }
        else
        {
            //1= sleep on
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x10;                             //0x10 = sleep in
        }

        iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    }
    
    if(bl != 0xFF)
    {
        buffer=getBuffer();
        if(bl==0)
        {
            //backlight off
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x53;                             //0x53
            buffer[2]=0b01000001;                       //control byte
            buffer[3]=0x28;                             //param BL = b2
        }
        else
        {
            //backlight on
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x53;                             //0x53
            buffer[2]=0b01000001;                       //control byte
            buffer[3]=0x2C;                             //param BL = b2
        }

        iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    }
    
    if(inv != 0xFF)
    {
        buffer=getBuffer();
        if(inv==0)
        {
            //0=normal, not inv
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x20;                             //0x20 = inversion OFF
        }
        else if (inv==1)
        {
            //1=inv 
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0x21;                             //0x21 = inversion ON
        }

        iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    }
    
    iface_freePort();
}

static char getInitialized()
{
    return isInitialized;
}

static void print(char* t)
{
    //provede vypis na aktualni radek a posune kurzor na dalsi
    //je-li plna obrazovka, vynaze ji
    if(defaultFont==NULL) { return; }
    
    if((displayInfo->print_y + defaultFont->height) > Height)
    {
        //plna obrazovka
        clear(COLOR.Black);
        displayInfo->print_y = 0;
    }
    
    drawString(t, defaultFont, 0, displayInfo->print_y);
    displayInfo->print_y += defaultFont->height;
}

static short getWidth()
{
    return Width;
}

static short getHeight()
{
    return Height;
}

static short getFontHeight(IMAGE_SRC* font)
{
    if(font != NULL)
    {
        return font->height;
    }
    else
    {
        return defaultFont->height;
    }
}


static void dinit()
{
    //initFont(&font_arial18, &fontDefault);
    //setFontSrc(&font_ygm20, &fontDefault);
    //setFontSrc(&font_arial18, &fontDefault);

    //if(fontDefault.format==0x4){ setImageColorMap(&fontDefault, (short*)stdColorMap); }
    //else if(fontDefault.format==0x1){ fontDefault.foreColor=RGB16(0, 63, 0); }
    
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif  
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
    setBacklight(1);                            //backlight ON
    
    char* buffer;
	resetDisplay();

    iface_getPort();
    buffer=getBuffer();
    
    //soft RESET
    buffer[0]=0b00000001;                       //control byte
    buffer[1]=0x01;                             //0x01 = soft reset
    iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    iface_freePort();

    pauseEvents(200);                            //po resetu pauza 200ms
    
    iface_getPort();
    buffer=getBuffer();
    
    //power control 1    
    buffer[0]=0b00000001;                       //control byte
    buffer[1]=0xC0;                             //0xC0 = power control 1
    buffer[2]=0b01000001;                       //control byte
    buffer[3]=0x21;                             //param

    //power control 2
    buffer[4]=0b00000001;                       //control byte
    buffer[5]=0xC1;                             //0xC0 = power control 2
    buffer[6]=0b01000001;                       //control byte
    buffer[7]=0x10;                             //param
    
    //normal mode
    buffer[8]=0b00000001;                       //control byte
    buffer[9]=0x13;                             //0x13 = normal mode
    
    //inversion OFF
    buffer[10]=0b00000001;                       //control byte
    buffer[11]=0x20;                             //0x20 = inversion OFF
    
    //orientation (=1)                          //0=na vysku, 1=na sirku, 2=na vysku obracene, 3=na sirku obracene
    buffer[12]=0b00000001;                       //control byte
    buffer[13]=0x36;                             //0x36 = orientation
    buffer[14]=0b01000001;                       //control byte
    buffer[15]=0x28;                             //param 48 - na vysku
    
    //nastavi displej na sirku (Orientation=1)
    Width=320;
    Height=240;
    Orientation=1;
    
    //gamma set
    buffer[16]=0b00000001;                       //control byte
    buffer[17]=0x26;                             //0x26 gamma set
    buffer[18]=0b01000001;                       //control byte
    buffer[19]=0x1;     


    /*
    //positive gamma correction
    pixels[0]=0b00000001;                       //control byte
    pixels[1]=0xE0;                             //command
    pixels[2]=0b11000000;                       //control byte
    pixels[3]=0x0F;
    pixels[4]=0x31;
    pixels[5]=0x2B;
    pixels[6]=0x0C;
    pixels[7]=0x0E;
    pixels[8]=0x08;
    pixels[9]=0x4E;
    pixels[10]=0xF1;
    pixels[11]=0x37;
    pixels[12]=0x07;
    pixels[13]=0x10;
    pixels[14]=0x03;
    pixels[15]=0x0E;
    pixels[16]=0x09;
    pixels[17]=0x00;
    sendSyncMode2(0, pixels, 18, 1);    
       
    //negative gamma correction
    pixels[0]=0b00000001;                       //control byte
    pixels[1]=0xE1;                             //command
    pixels[2]=0b11000000;                       //control byte
    pixels[3]=0x00;
    pixels[4]=0x0E;
    pixels[5]=0x14;
    pixels[6]=0x03;
    pixels[7]=0x11;
    pixels[8]=0x07;
    pixels[9]=0x31;
    pixels[10]=0xC1;
    pixels[11]=0x48;
    pixels[12]=0x08;
    pixels[13]=0x0F;
    pixels[14]=0x0C;
    pixels[15]=0x31;
    pixels[16]=0x36;
    pixels[17]=0x0F;
    sendSyncMode2(0, pixels, 18, 1);
    */
    
    //format 565/666
    buffer[20]=0b00000001;                       //control byte
    buffer[21]=0x3A;                             //0x3A = pixel format, 0x55=16-bit    
    buffer[22]=0b01000001;                       //control byte
    buffer[23]=0x55;                             //param pixel format 666-18bit / 565-16bit
  
    //idle off
    buffer[24]=0b00000001;                       //control byte
    buffer[25]=0x38;                             //0x38 Idle OFF
    
    //sleep out
    buffer[26]=0b00000001;                       //control byte
    buffer[27]=0x11;                             //0x11 = sleep out
    iface_writeBufferMode(buffer, 28, WRITE_MODE.Stream);
    iface_freePort();
    
    pauseEvents(120);                            //po sleep out pauza 120 ms
    iface_getPort();
    
    //on
    buffer=getBuffer();
    buffer[0]=0b00000001;                       //control byte
    buffer[1]=0x29;                             ///0x29 = display ON    
    iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    
    iface_freePort();
    pauseEvents(200);                            //po display ON pauza 200 ms
    
#ifdef WATCHDOG_TIMER    
    startWDT();
#endif  
    
#ifdef SAFE_PROCESS
    startCT();
#endif 
    
}

static void writeChar(IMAGE_SRC* fi, char code, short x, short y)
{
    //fi=font-image struct IMAGE_SRC
    if(fontCharParam(fi, code)==0)
    {
        //chyba, neni nastaven font (fi->fileID=0)
        return;
    }
    
    if(x + fi->width <= 0) { return; }          //mimo, vlevo
    if(x >= Width) { return; }                  //mimo, vpravo
    if(y + fi->height <= 0) { return; }          //mimo nahore
    if(y >= Height) { return; }                 //mimo dole
    
    short dx1, dx2, dy1, dy2;
    char is_out=0;
    
    //startX, dx1
    if(x < 0) { fi->start_x = x*(-1); dx1 = 0; is_out++; }
    else { fi->start_x = 0; dx1 = x; }
    
    //endX, dx2
    dx2 = x + fi->width - 1;
    if(dx2 >= Width) { dx2 = Width-1; fi->end_x = Width - x - 1;  is_out++;}
    else { fi->end_x = fi->width-1; }
    
    //startY, dy1
    if(y < 0) { fi->start_y = y*(-1); dy1 = 0; is_out++; }
    else { fi->start_y = 0; dy1 = y; }
    
    //endY, dy2
    dy2 = y + fi->height - 1;
    if(dy2 >= Height) { dy2 = Height-1; fi->end_y = Height - y - 1;  is_out++; }
    else { fi->end_y = fi->height-1; }    
    
    //nastav startX=-1, pokud je cely obrazek na displeji (zadna cast neni mimo)
    if(is_out==0) { fi->start_x = -1; }    
    
    
    char* buffer=getBuffer();
    
    buffer[0]=0b00000001;                                   //control byte
    buffer[1]=0x2A;                                         //command (ColSet)
    
    buffer[2]=0b01000100;                                   //control byte
    buffer[3]=(char)(dx1>>8);                               //data, start col H
    buffer[4]=(char)(dx1);                                  //data, start col L
    buffer[5]=(char)(dx2>>8);                               //data, end col H
    buffer[6]=(char)(dx2);                                  //data, end col L
    
    buffer[7]=0b00000001;                                   //control byte
    buffer[8]=0x2B;                                         //command (RowSet)
    
    buffer[9]=0b01000100;                                   //control byte
    buffer[10]=(char)(dy1>>8);                              //data, start row H
    buffer[11]=(char)(dy1);                                 //data, start row L
    buffer[12]=(char)(dy2>>8);                              //data, end row H
    buffer[13]=(char)(dy2);                                 //data, end row L

    buffer[14]=0b00000001;                                  //control byte
    buffer[15]=0x2C;                                        //write data
    buffer[16]=0b11000000;                                  //control byte (ukonci SPI mode 1, odesila zbytek bufferu)

    iface_writeBufferMode(buffer, 17, WRITE_MODE.Stream);
    
    if(directMode==1)
    {
        //direct write data >> SPIBUF
        iface_Process();                                    //ceka na dokonceni  
    
        iface_setBusMode(BUS_MODE._16bit);
        imageToPort(fi, iface_getHWBuffer(), 0, 2);
        iface_setBusMode(BUS_MODE._8bit);
    }
    else
    {
        int len;
        do
        {
            buffer=getBuffer();
            len=imageToBuffer(fi, buffer, BUFFER_SIZE, BUS_MODE._8bit);
            iface_writeBufferMode(buffer, len, WRITE_MODE.DataOnly);
        } while(fi->eof == 0);
    }
    
}

static void setColorDef()
{
    char* buffer=getBuffer();  
    
    buffer[0]=0b00000001;                       //control byte
    buffer[1]=0xE0;                             //command
    buffer[2]=0b11000000;                       //control byte
    
    //odasila tabuklu konverze barev 16/18 bit
    int a;
    //RED 32 polozek
    for(a=0; a<32; a++){buffer[a+3]=a;}
    //GREEN 64 polozek
    for(a=0; a<64; a++){buffer[a+32+3]=a;}
    //BLUE
    for(a=0; a<32; a++){buffer[a+96+3]=a;}
    
    iface_writeBufferMode(buffer, 131, WRITE_MODE.Stream);
}

static char* getBuffer()
{
    if(pixAB==0)
    {
        pixAB=1;
        return pixelsEven;
    }
    else
    {
        pixAB=0;
        return pixelsOdd;
    }
}

static void eventDC(char x)
{
    //fci vola SPI interrupt, pokud pri odesilani streamu najde odpovidajici command
    //plati pouze pro SPI
    //b6=0/1 obsahuje hodnotu, ktera bude nastavena na DC pin 
    //b7=0 - command v b0-b5 obsahuje pocet bytes dat, ktere nasleduji (nez bude dalsi command)
    //b7=1 - command ukonci STREAM MODE, port pokracuje v BUFFER MODE  (vse dalsi jsou data)
    x=x & 0b01000000; // x>>6;    
    if(x==0)
    {
        //setDCPin(0);
        clearPin(&dcPin);
    }
    else
    {
        //setDCPin(1);
        setPin(&dcPin);
    }
}

static void resetDisplay()
{
    //provede UP, DOWN (reset), UP
    //je-li zapojeno vice displeju, maji spolecny RESET (pripadne i D/C signal)
    
#ifdef SW_RESET
    //SW reset
    //UP
    setPin(&resetPin);
    pauseEvents(50);                     //50ms

    //DOWN - probiha reset displeje
    clearPin(&resetPin);
    pauseEvents(100);                    //100ms

    //UP
    setPin(&resetPin);
    pauseEvents(50);                     //50ms
#else    
    //HW RESET
    //pauseEvent(50);                     //50ms (je-li treba cekat na dokonceni hw resetu) 
#endif   
    
}

static void setDCPin(char value)
{
    //nastav DC pin na value
    if(value==0)
    {
        clearPin(&dcPin);
    }
    else
    {
        setPin(&dcPin);
    }
}

static void setBacklight(char value)
{
    //value 1=ON, 0=OFF
    if(value==1)
    {
        //LATCbits.LATC9=0;
        clearPin(&blPin);
    }
    else
    {
        //LATCbits.LATC9=1;
        setPin(&blPin);
    }
}


//Use the SPI module if it is not occupied, otherwise, wait to release
static void iface_getPort()
{
    //obsadi SPI port, pokud neni volny, ceka na uvolneni (doEvents)
    spiUse(IFACE_INDEX, NULL, &eventDC);

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

//start the transmission
//@param buffer array of bytes
//@param len    number of bytes in buffer
//@param mode   WRITE_MODE.DataOnly (buffer contains data only), or WRITE_MODE.Stream (buffer contains control bytes and data)
static void iface_writeBufferMode(char* buffer, short len, char mode)
{
    //odesila data na SPI
    while(spiGetActivity(IFACE_INDEX) == MODULE_ACTIVITY.Works)
    {
        //ceka na odeslani dat
    }
    
    spiExchangeMode(IFACE_INDEX, buffer, NULL, len, mode);
}

//return address of SPIBUF register
static volatile int* iface_getHWBuffer()
{
    //vraci adresu HW bufferu v SPI modulu (directWrite)
    spiGetHwBuffer(IFACE_INDEX);
}

//set 8/16/32 bit bus mode
//@param mode  BUS_MODE._8bit/_16bit/_32bit
static void iface_setBusMode(char mode) 
{
    //nastavi mod SPI (8-bit, 16-bit)
    spiSetBusMode(IFACE_INDEX, mode);
}

//Wait to complete the transmission
static void iface_Process()
{
    //ceka na dokonceni predchoziho vysilani
    while(spiGetActivity(IFACE_INDEX) == MODULE_ACTIVITY.Works)
    {
        //ceka na dokonceni   
    }
}


#endif