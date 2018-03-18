#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../globals.h"

//font nebo image se generuje programem lcd-image-converter
//pro ssd1306 plati toto nastaveni
//1. load image (nebo font)
//2. okno Options: Prepare: Monochrome, MainScanDirection=Left To Right, LineScanDirection=Forward, Bands=8px
//3. okno Reordering: prehodit b0>b7, b1>b6, b2>b5, b3>b4, ... 
//4. okno Options: Image: BlockSize=8bit, ByteOrder=Little Endian
//Kazdy bajt je sloupec 8 pixelu, b0=horni px, b7=dolni px. Nejprve se plni prvni radek (8 px na vysku), pak druhy radek atd...
//Tzn. ze napr. 10 bytes na jednom radku vytvori box 8(h) x 10(w) px
//https://gist.github.com/postmodern/ed6e670999f456ad9f13

#ifdef USE_DISP1306

//globalni promene modulu (tzn. pri pouziti vice displeju ssd1306 jsou spolecne pro vsechny struct DISPLAY)
#include "../font/font_fixed1306.h"
IMAGE_SRC fontDefault;

//IFACE
#define     IFACE_INDEX     1                       //SPI2, index portu (0=SPI1, 1=SPI2, 2=SPI3, ...)
//definuje IO piny
PIN_INFO csPin = {PORTA_BASE, BIT10};                //CS signal
PIN_INFO resetPin = {PORTA_BASE, BIT15};            //RESET signal
PIN_INFO dcPin = {PORTB_BASE, BIT13};               //DC signal

#define     SW_RESET                                //je definovano, pokud je RESET signal pripojen k IO pinu. Neni-li definovano, ma HW RESET (RC obvod)
#define     BUFFER_SIZE      256                    //min. velikost by mela byt: Width x 2 + 20 (viz. fillBox, clear)           

static char pixelsEven[BUFFER_SIZE] __attribute__((aligned(4)));
static char pixelsOdd [BUFFER_SIZE] __attribute__((aligned(4)));
static char pixAB=0;

//privatni struct pro dany displej, pri pouziti vice displeju se nastavi pri kazdem volani setGraphics
static DISPLAY* displayInfo=NULL;

//static char Orientation=0;
static short Width=128;                         //default na vysku, fce dinit nastavi displej na sirku a upravi W a H              
static short Height=64;
static char isInitialized=0;
static char directMode=1;                       //0=zapis do bufferu, 1=zapis do portu


//local void
static void selectDriver(void* d);
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
static char* getBuffer();
static void resetDisplay();

//fce iface
//bude-li dispay pripojen pres jiny iface, staci zmenit tyto fce
static void iface_getPort(); 
static void iface_freePort();
static void iface_writeBufferMode(char* buffer, short len, char mode);
static volatile int* iface_getHWBuffer();
static void iface_setBusMode(char mode);
static void iface_Process();

//global
void disp1306_driver(DISPLAY* d)
{
    d->selectDriver=&selectDriver;
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

static void drawString(char* text, IMAGE_SRC* font, short x, short y)
{
    int a;
    int l=strLen(text);
    
    if(font==NULL) { font=&fontDefault; }
    
    iface_getPort();
    
    for(a=0; a<l; a++)
    {
        writeChar(font, text[a], x, y);
        x+=font->width;
    }
    
    iface_freePort();
}

static void fillBox(short x1, short y1, short x2, short y2, short color)
{
    //plni box vzorem, dolni byte parametru color
    //y musi byt delitelne 8
    if((y1 % 8) != 0) { return; }
     
    //x1 musi byt mensi, nez x2, to same y1 a y2
    if(x1 > x2) { short x=x1; x1=x2, x2=x; }
    if(y1 > y2) { short y=y1; y1=y2, y2=y; }  
    
    if(x1 >= Width) { return; }             //mimo, vlevo
    if(x2 < 0) { return; }                  //mimo, vpravo
    if(y1 >= Height) { return; }            //mimo nahore
    if(y2 < 0) { return; }                  //mimo dole
    
    if(x1 < 0){ x1=0; }
    if(x2 > (Width-1)) { x2=Width-1; }
    if(y1 < 0) { y1=0; }
    if(y2 > (Height-1)) { y2=Height-1; }
    
    iface_getPort();
    
    int len=0;
    char* buffer=getBuffer();

#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif  
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
    //x
    buffer[0]=0b00000011;                       //control byte (DC=0)
    buffer[1]=0x21;
    buffer[2]=(char)x1;
    buffer[3]=(char)x2;
    //y
    buffer[4]=0b00000011;                       //control byte (DC=0)
    buffer[5]=0x22;
    buffer[6]=(char)(y1/8);
    buffer[7]=(char)(y2/8);
    buffer[8]=0b11000000;                      //control byte (ukonci SPI mode 1, nastavi DC=1)                      //control byte
        
    iface_writeBufferMode(buffer, 9, WRITE_MODE.Stream); 
    
    int count=(x2-x1 + 1) * ((y2-y1)/8 + 1);        //pocet bytes
    char patern=(char)color;                        //pouzije dolni byte parametru color
    
    if(directMode==1)
    {
        //direct write data >> SPIBUF
        iface_Process();                            //ceka na dokonceni      

        //odesila patern primo do hw_bufferu (SPIxBUF) (SPI 8-bit mode)
        volatile int* spi_buf=iface_getHWBuffer(); 
        while(count>0)
        {
            *spi_buf=patern;
            count--;
        }
    }
    else
    {
        int a;
        while(count>0)
        {
            len=0;
            buffer=getBuffer(); 
            
            for(a=0; a<BUFFER_SIZE; a++)
            {
                buffer[a]=patern;
                len++;
                count--;
                if(count==0) { break; }
            }
            iface_writeBufferMode(buffer, len, WRITE_MODE.BufferOnly);
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
    //neumoznuje zobrazit cary
}

static void drawImage(IMAGE_SRC* da, short x, short y)
{
    //predpoklada, ze obrazek ma vysku 8/16/24/32/40/48/64
    //y musi byt delitelne 8
    if((y % 8 != 0)) { return; }
    
    //cely obrazek musi byt na displeji, nepodporuje zobrazeni casti obrazku
    if(x < 0) { return; }
    if((x + da->width) > Width) { return; }
    if(y < 0) { return; }
    if((y + da->height) > Height) { return; }
   
    iface_getPort();
    
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif  
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
    int len=0;
    short start_x=x;
    short end_x=x + da->width - 1;
    short start_page=y/8;
    short end_page=y/8 + da->height/8 - 1;
    
    char* buffer=getBuffer();

    //x
    buffer[0]=0b00000011;                       //control byte (DC=0)
    buffer[1]=0x21;
    buffer[2]=(char)start_x;
    buffer[3]=(char)end_x;
     //y
    buffer[4]=0b00000011;                       //control byte (DC=0)
    buffer[5]=0x22;
    buffer[6]=(char)start_page;
    buffer[7]=(char)end_page;
    buffer[8]=0b11000000;                      //control byte (ukonci SPI mode 1, nastavi DC=1)                      //control byte
    iface_writeBufferMode(buffer, 9, WRITE_MODE.Stream); 
    
    if(directMode==1)
    {
        //direct write data >> SPIBUF
        iface_Process();         //ceka na dokonceni      
        
        //odesila data znaku primo do hw_bufferu (SPIxBUF) (SPI 8-bit mode)
        char* a;
        volatile int* spi_buf=iface_getHWBuffer();
        for(a=da->srcStartPosition; a < da->srcAfter; a++)
        {
            *spi_buf=*a;
        }
    }
    else
    {
        //vlozi data znaku do bufferu a ten odesila
        da->srcPosition=da->srcStartPosition;
        da->eof=0;
        
        do
        {
            len=0;
            buffer=getBuffer();
            
            while(len < BUFFER_SIZE)
            {
                buffer[len]=*da->srcPosition;
                da->srcPosition++;
                len++;
            
                if(da->srcPosition == da->srcAfter)
                {
                    da->eof=1; 
                    break;
                }
            }
            
            iface_writeBufferMode(buffer, len, WRITE_MODE.BufferOnly);
            
        } while (da->eof == 0);
    }

#ifdef WATCHDOG_TIMER    
    startWDT();
#endif  
    
#ifdef SAFE_PROCESS
    startCT();
#endif 
    
    iface_freePort();
}

static void drawPoint(short x, short y, short color)
{
    //neumoznuje zobrazit jednotlive body

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
    fillBox(0, 0, Width-1, Height-1, color);
}

static void initDisplay(PORT_INFO* pi)
{
    isInitialized=0;
    
#ifndef SIMULATOR    
    dinit();
#endif    
   
    isInitialized=1;
}

static void setOrientation(char x)
{
    //nelze menit orientaci
}

static char getOrientation()
{
    return 0;
}

static void setBrightness(char val)
{
    iface_getPort();
    char* buffer=getBuffer();  

    buffer[0]=0b00000001;                       //control byte, nastavi DC=0
    buffer[1]=0x81;                             //0x81 = Contrast
    buffer[2]=val;                              //param 0 ... 0xFF
    iface_writeBufferMode(buffer, 3, WRITE_MODE.Stream);

    iface_freePort();
}

static void controlDisplay(char on, char sleep, char bl, char inv)
{
    //hodnota 0xFF = bez zmeny, 0=OFF(vypni), 1=ON(zapni)
    //on (display) nastavuje sleep mode (on=1 - SLEEP OFF, on=0 - SLEEP ON)
    //sleep - nastavi/ukonci sleep mode (sleep=1 - SLEEP ON, sleep=0 - SLEEP OFF)
    //ON i SLEEP naji stejnou fci, ale pracuji opacne
    //bl (backLight) nema zadnou fci
    
    iface_getPort();
    char* buffer;
    
    if(on != 0xFF)
    {
        buffer=getBuffer();
        if(on==0)
        {
            //0 = display off (sleep ON)
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0xAE;                             //0xAE = sleep ON
        }
        else
        {
            //1 = display on (sleep OFF)
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0xAF;                             //0xAF = sleep OFF
        }   
        iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    }
    
    if(sleep != 0xFF)
    {
        buffer=getBuffer();
        if(on==0)
        {
            //0 = sleep off
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0xAF;                             //0xAF = sleep OFF
        }
        else
        {
            //1 = sleep on
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0xAE;                             //0xAE = sleep ON
        }
        iface_writeBufferMode(buffer, 2, WRITE_MODE.Stream);
    }
    
    if(bl != 0xFF)
    {
        //backlight nema
    }
    
    if(inv != 0xFF)
    {
        buffer=getBuffer();
        if(inv==0)
        {
            //0=normal, not inv
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0xA6;                             //0xA6 = inversion OFF (normal mode)
        }
        else if (inv==1)
        {
            //1=inv 
            buffer[0]=0b00000001;                       //control byte
            buffer[1]=0xA7;                             //0x21 = inversion ON (inv mode)
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
    if((displayInfo->print_y + fontDefault.height) > Height)
    {
        //plna obrazovka
        clear(COLOR.Black);
        displayInfo->print_y = 0;
    }
    
    drawString(t, &fontDefault, 0, displayInfo->print_y);
    displayInfo->print_y += fontDefault.height;
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
        return fontDefault.height;
    }
}

static void dinit()
{
    setFontSrc(&font_fixed16x, &fontDefault);
    
#ifdef WATCHDOG_TIMER    
    pauseWDT();
#endif  
    
#ifdef SAFE_PROCESS
    pauseCT();
#endif     
    
	resetDisplay();

    iface_getPort();
    char* buffer = getBuffer();

    buffer[0]=0xAE;
    buffer[1]=0xD5;
    buffer[2]=0x80;            
    buffer[3]=0xA8;            
    buffer[4]=0x3F;            
    buffer[5]=0xD3;      //offset
    buffer[6]=0x00;           
    buffer[7]=0x00;            
    buffer[8]=0x8D;      //charge pump
    buffer[9]=0x14;
    buffer[10]=0x20;            
    buffer[11]=0x00;            
    buffer[12]=0xA1;     //set remap, 
    buffer[13]=0xC8;
    buffer[14]=0xDA;     //pin hw config
    buffer[15]=0x12;            
    buffer[16]=0x81;     //contrast,
    buffer[17]=0x7F;     //CF
    buffer[18]=0xD9;     //pre-charge
    buffer[19]=0x02;     //F1      
    buffer[20]=0xDB;     //deselect level 
    buffer[21]=0x40;
    buffer[22]=0xA4;            
    buffer[23]=0xA6;            
    buffer[24]=0xAF; 
        
    clearPin(&dcPin);   //0=command
    iface_writeBufferMode(buffer, 25, WRITE_MODE.BufferOnly);

#ifdef WATCHDOG_TIMER    
    startWDT();
#endif  
    
#ifdef SAFE_PROCESS
    startCT();
#endif 
    
    iface_freePort();
}

static void writeChar(IMAGE_SRC* fi, char code, short x, short y)
{
    //fi=font-image struct IMAGE_SRC
    if(fontCharParam(fi, code)==0)
    {
        //chyba, neni nastaven font (fi->fileID=0)
        return;
    }
    
    //cely znak musi byt na displeji, jinak ho nezobrazuje (neumoznuje zobrazit pouze cast znaku)
    if(y % 8 != 0)
    {
        //y musi byt delitelne 8
        return;
    }
    if(x < 0) { return; }
    if((x + fi->width) > Width) { return; }
    if(y < 0) { return; }
    if((y + fi->height) > Height) { return; }

    //x
    short start_x=x;
    short end_x=x + fi->width - 1;
    //y
    short start_page=y/8;
    short end_page=y/8 + fi->height/8 - 1;
    
    char* buffer=getBuffer();

    //x
    buffer[0]=0b00000011;                       //control byte (DC=0)
    buffer[1]=0x21;
    buffer[2]=(char)start_x;
    buffer[3]=(char)end_x;
    
    //y
    buffer[4]=0b00000011;                       //control byte (DC=0)
    buffer[5]=0x22;
    buffer[6]=(char)start_page;
    buffer[7]=(char)end_page;
    buffer[8]=0b11000000;                      //control byte (ukonci SPI mode 1, nastavi DC=1)                      //control byte
    iface_writeBufferMode(buffer, 9, WRITE_MODE.Stream);         

    if(directMode==1)
    {
        iface_Process();                        //ceka na dokonceni          
        
        //odesila data znaku primo do hw_bufferu (SPIxBUF) (SPI 8-bit mode)
        char* a;
        volatile int* spi_buf=iface_getHWBuffer();
        for(a=fi->srcStartPosition; a < fi->srcAfter; a++)
        {
            *spi_buf=*a;
        }
    }
    else
    {
        //vlozi data znaku do bufferu a ten odesila
        fi->srcPosition=fi->srcStartPosition;
        fi->eof=0;
        int len;
        do
        {
            //int len=0;
            len=0;
            buffer=getBuffer();
            
            while(len < BUFFER_SIZE)
            {
                buffer[len]=*fi->srcPosition;
                fi->srcPosition++;
                len++;
            
                if(fi->srcPosition == fi->srcAfter)
                {
                    fi->eof=1; 
                    break;
                }
            }
            iface_writeBufferMode(buffer, len, WRITE_MODE.BufferOnly);
            
        } while (fi->eof == 0);
    }
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
    //plati pouze pro SPI
    //b6=0/1 obsahuje hodnotu, ktera bude nastavena na DC pin 
    //b7=0 - command v b0-b5 obsahuje pocet bytes dat, ktere nasleduji (nez bude dalsi command)
    //b7=1 - command ukonci MODE 1, port pokracuje v MODE 0 (vse dalsi jsou data)
    x=x & 0b01000000; // x>>6;
    if(x==0)
    {
        clearPin(&dcPin);
    }
    else
    {
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


//Use the SPI module if it is not occupied, otherwise, wait to release
static void iface_getPort()
{
    //obsadi SPI port, pokud neni volny, ceka na uvolneni (doEvents)
    spi_Use(IFACE_INDEX, WAIT.Enable, NULL, &eventDC);

    //aktivuje CS signal displeje
    clearPin(&csPin);
}

//Wait to complete the transmission, then release SPI module
static void iface_freePort()
{
    //uvolni SPI port, predtim ceka na dokonceni vysilani (doEvents)
    spi_Free(IFACE_INDEX);
    
    //deaktivuje CS signal displeje
    setPin(&csPin);
}

//start the transmission
//@param buffer array of bytes
//@param len    number of bytes in buffer
//@param mode   WRITE_MODE.BufferOnly (buffer contains data only), or WRITE_MODE.Stream (buffer contains control bytes and data)
static void iface_writeBufferMode(char* buffer, short len, char mode)
{
    //odesila data na SPI
    while(spi_Process(IFACE_INDEX) == MODULE_ACTIVITY.Works)
    {
        //ceka na odeslani dat
    }
    
    spi_ExchangeMode(IFACE_INDEX, buffer, NULL, len, mode);
    //spi_ExchangeModeDE(IFACE_INDEX, buffer, NULL, len, mode);
}

//return address of SPIBUF register
static volatile int* iface_getHWBuffer()
{
    //vraci adresu HW bufferu v SPI modulu (directWrite)
    (int*)spi_getHwBuffer(IFACE_INDEX);
}

//set 8/16/32 bit bus mode
//@param mode  BUS_MODE._8bit/_16bit/_32bit
static void iface_setBusMode(char mode) 
{
    //nastavi mod SPI (8-bit, 16-bit)
    spi_setBusMode(IFACE_INDEX, mode);
}

//Wait to complete the transmission
static void iface_Process()
{
    //ceka na dokonceni predchoziho vysilani
    while(spi_Process(IFACE_INDEX) == MODULE_ACTIVITY.Works)
    {
        //ceka na dokonceni   
    }             
}

#endif