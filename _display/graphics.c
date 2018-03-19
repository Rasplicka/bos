//Graphics obsahuje zakladni graficke fce, vola primo fce driveru
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "../globals.h"
//#include "graphics.h"


//

#ifdef USE_GRAPHICS

// <editor-fold defaultstate="collapsed" desc="include FONT files">

#ifdef FONT_ARIAL_18
#include "font/font_arial_18.h"
IMAGE_SRC font_arial_18;
#endif

#ifdef FONT_DLG_18
#include "font/font_dlg_18.h"
IMAGE_SRC font_dlg_18;
#endif

#ifdef FONT_CONSOLAS_16
#include "font/font_consolas_16.h"
IMAGE_SRC font_consolas_16;
#endif

#ifdef FONT_CONSOLAS_20
#include "font/font_consolas_20.h"
IMAGE_SRC font_consolas_20;
#endif

#ifdef FONT_CONSOLAS_28
#include "font/font_consolas_28.h"
IMAGE_SRC font_consolas_28;
#endif

#ifdef FONT_CONSOLAS_36
#include "font/font_consolas_36.h"
IMAGE_SRC font_consolas_36;
#endif

#ifdef FONT_YGM_16
#include "font/font_ygm_16.h"
IMAGE_SRC font_ygm_16;
#endif

#ifdef FONT_YGM_20
#include "font/font_ygm_20.h"
IMAGE_SRC font_ygm_20;
#endif    

#ifdef FONT_YGM_28
#include "font/font_ygm_28.h"
IMAGE_SRC font_ygm_28;
#endif

#ifdef FONT_YGM_36
#include "font/font_ygm_36.h"
IMAGE_SRC font_ygm_36;
#endif    

#ifdef FONT_YGM_46
#include "font/font_ygm_46.h"
IMAGE_SRC font_ygm_46;
#endif     

#ifdef FONT_YGM_78
#include "font/font_ygm_78.h"
IMAGE_SRC font_ygm_78;
#endif 

#ifdef FONT_TWCEN_18
#include "font/font_twcen_18.h"
IMAGE_SRC font_twcen_18;
#endif  

#ifdef FONT_TWCEN_22
#include "font/font_twcen_22.h"
IMAGE_SRC font_twcen_22;
#endif 

#ifdef FONT_TWCEN_28
#include "font/font_twcen_28.h"
IMAGE_SRC font_twcen_28;
#endif 

#ifdef FONT_TWCEN_36
#include "font/font_twcen_36.h"
IMAGE_SRC font_twcen_36;
#endif 

#ifdef FONT_TWCEN_46
#include "font/font_twcen_46.h"
IMAGE_SRC font_twcen_46;
#endif 

#ifdef FONT_TWCEN_80
#include "font/font_twcen_80.h"
IMAGE_SRC font_twcen_80;
#endif 

// </editor-fold>

//gobal vars
GRAPHICS graphics;                              //universalni graficke fce
DISPLAY sysDisplay;                             //display driver

//local vars
static DISPLAY* actualDisplay;

void initGraphics();
void initFont(const FONT_HEAD* fontSrc, IMAGE_SRC* font);
void setGraphics(GRAPHICS* g,  DISPLAY* d);

//local fn
static void drawCircle(short x, short y, short r, short color);
static void drawBox(short x1, short y1, short x2, short y2, short w, short color);


//global fn
void initGraphics()
{
    //Inicializuje systemovy display

    // <editor-fold defaultstate="collapsed" desc="init Fonts">
#ifdef FONT_ARIAL_18
    initFont(&font_arial18, &font_arial_18);
#endif     
   
#ifdef FONT_DLG_18
    initFont(&font_dlg18, &font_dlg_18);
#endif     
    
#ifdef FONT_CONSOLAS_16
    initFont(&font_consolas16, &font_consolas_16);
#endif     
    
#ifdef FONT_CONSOLAS_20
    initFont(&font_consolas20, &font_consolas_20);
#endif      
    
#ifdef FONT_CONSOLAS_28
    initFont(&font_consolas28, &font_consolas_28);
#endif      
    
#ifdef FONT_CONSOLAS_36
    initFont(&font_consolas36, &font_consolas_36);
#endif      
    
#ifdef FONT_YGM_16
    initFont(&font_ygm16, &font_ygm_16);
#endif 

#ifdef FONT_YGM_20
    initFont(&font_ygm20, &font_ygm_20);
#endif   

#ifdef FONT_YGM_28
    initFont(&font_ygm28, &font_ygm_28);
#endif  

#ifdef FONT_YGM_36
    initFont(&font_ygm36, &font_ygm_36);
#endif     

#ifdef FONT_YGM_46
    initFont(&font_ygm46, &font_ygm_46);
#endif     

#ifdef FONT_YGM_78
    initFont(&font_ygm78, &font_ygm_78);
#endif 
    
#ifdef FONT_TWCEN_18
    initFont(&font_twcen18, &font_twcen_18);
#endif 

#ifdef FONT_TWCEN_22
    initFont(&font_twcen22, &font_twcen_22);
#endif     
    
#ifdef FONT_TWCEN_28
    initFont(&font_twcen28, &font_twcen_28);
#endif 
    
#ifdef FONT_TWCEN_36
    initFont(&font_twcen36, &font_twcen_36);
#endif     
    
#ifdef FONT_TWCEN_46
    initFont(&font_twcen46, &font_twcen_46);
#endif     
    
#ifdef FONT_TWCEN_80
    initFont(&font_twcen80, &font_twcen_80);
#endif     
    
    // </editor-fold>
    
    
#ifdef USE_DISP9341    
    //ili9341/SPI, v display nastavi fce driveru disp9341 
    disp9341_driver(&sysDisplay);                    
#endif
    
#ifdef USE_DISP1306
    //SSD1306/SPIv v display nastavi fce driveru disp1306
    disp1306_driver(&sysDisplay);                    
#endif    
    
    //pro vsechny typy displeju
    sysDisplay.initDisplay();                   //driver inicializuje display         
    setGraphics(&graphics, &sysDisplay);        //nastavi fce graphics na display driver, vystup graphics jde na zvoleny display
    graphics.clear(COLOR.Black);                //cls   
}

void initFont(const FONT_HEAD* fontSrc, IMAGE_SRC* font)
{
    setFontSrc(fontSrc, font);
    if(font->format==0x4){ setImageColorMap(font, stdColorMap); }
    else if(font->format==0x1){ font->foreColor=RGB16(31, 63, 31); }
}



void setGraphics(GRAPHICS* g,  DISPLAY* d)
{
    actualDisplay=d;
    
    d->selectDriver(actualDisplay);
    
    //fce graphics
    g->drawCircle=&drawCircle;
    g->drawBox=&drawBox;
    //g->print=&print;
    
    //fce driveru
    g->drawString=d->drawString;
    g->fillBox=d->fillBox;
    g->drawLine=d->drawLine;
    g->drawImage=d->drawImage;
    g->drawPoint=d->drawPoint;
    g->print=d->print;
    
    g->clear=d->clear;
    g->textWidth=d->textWidth;
    g->getFontHeight=d->getFontHeight;
}


static void drawCircle(short x, short y, short r, short color)
{
    //disp->draw...
    //disp->fillBox(x, y, x+200, y+100, RGB16(31,63,31));
    //disp->drawLine(x, y, x+200, y+100, 1, RGB16(0,63,0));
    //disp->drawLine(x, y+100, x+200, y, 1, RGB16(0,63,0));
    
    
}

static void drawBox(short x1, short y1, short x2, short y2, short w, short color)
{
    actualDisplay->drawLine(x1, y1, x2, y1, w, color);
    actualDisplay->drawLine(x2, y1, x2, y2, w, color);
    actualDisplay->drawLine(x2, y2, x1, y2, w, color);
    actualDisplay->drawLine(x1, y2, x1, y1, w, color);    
    
}



#endif


/*
//iface, zajisti volani spravne fce
//vola modul, ktery ovlada displej podle indexu displeje

//definuje fce pro kazdy displayIndex [0-7]
//kazdy displej muze mit vlastni driver, tzn. ze mohou byt pouzity ruzne displeje soucasne 
void (*_drawText)(int, char*, int, int, int);
const void* func_drawText[]={     &disp1306a_drawText,      NULL, NULL, NULL, NULL, NULL, NULL, NULL };

void (*_drawString)(int, char*, int, int, int);
const void* func_drawString[]={   &disp1306a_drawString,    NULL, NULL, NULL, NULL, NULL, NULL, NULL };

void (*_print)(int, char*);
const void* func_print[]={        &disp1306a_print,         NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void (*_drawImage)(int, char*, int, int);
const void* func_drawImage[]={    &disp1306a_drawImage,     NULL, NULL, NULL, NULL, NULL, NULL, NULL };

void (*_fillRect)(int, int, int, int, int, int);
const void* func_fillRect[]={     &disp1306a_fillRect,     NULL, NULL, NULL, NULL, NULL, NULL, NULL };

void (*_clear)(int);
const void* func_clear[]={        &disp1306a_clear,         NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void (*_setContrast)(int, int);
const void* func_setContrast[]={  &disp1306a_setContrast,   NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void (*_sleep)(int);
const void* func_sleep[]={        &disp1306a_sleep,         NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void (*_resume)(int);
const void* func_resume[]={       &disp1306a_resume,        NULL, NULL, NULL, NULL, NULL, NULL, NULL};

int (*_getReady)(int);
const void* func_getReady[]={     &disp1306a_getReady,      NULL, NULL, NULL, NULL, NULL, NULL, NULL};

char* (*_getImageData)(int);
const void* func_getImageData[]={ &disp1306a_getImageData,  NULL, NULL, NULL, NULL, NULL, NULL, NULL};

int (*_getWidth)(int);
const void* func_getWidth[]={     &disp1306a_getWidth,      NULL, NULL, NULL, NULL, NULL, NULL, NULL};

int (*_getHeight)(int);
const void* func_getHeight[]={    &disp1306a_getHeight,      NULL, NULL, NULL, NULL, NULL, NULL, NULL};


void drawText(int index, char* string, int col, int row, int font)
{
    //vypis v txt rezimu (sloupec, radek)
    _drawText=func_drawText[index];
    _drawText(index, string, col, row, font);
}

void drawString(int index, char* string, int x, int y, int font)
{
    //vypis na zadanou pozici x, y
    _drawString=func_drawString[index];
    _drawString(index, string, x, y, font);    
}

void printString(int index, char* string)
{
    //vypis na dalsi radek (console)
    _print=func_print[index];
    _print(index, string);
}

void drawImage(int index, char* img, int x, int y)
{
    //kresli image na pozici x, y
    _drawImage=func_drawImage[index];
    _drawImage(index, img, x, y);    
}

void fillRect(int index, int x1, int y1, int x2, int y2, int color)
{
    //vyplni zadany obdelnik barvou color
    _fillRect=func_fillRect[index];
    _fillRect(index, x1, y1, x2, y2, color);
}

void displayClear(int index)
{
    //vymaze cely displej
    _clear=func_clear[index];
    _clear(index);
}

void displaySetContrast(int index, int val)
{
    _setContrast=func_setContrast[index];
    _setContrast(index, val);
}

void displaySleep(int index)
{
    _sleep=func_sleep[index];
    _sleep(index);
}

void displayResume(int index)
{
    _resume=func_resume[index];
    _resume(index);
}

int displayGetReady(int index)
{
    _getReady=func_getReady[index];
    _getReady(index);
}

char* displayGetImageData(int index, int id)
{
    _getImageData=func_getImageData[index];
    _getImageData(id);
}

int displayGetWidth(int index)
{
    _getWidth=func_getWidth[index];
    _getWidth(index);
}

int displayGetHeight(int index)
{
    _getHeight=func_getHeight[index];
    _getHeight(index);
}

*/