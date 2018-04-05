#include "def.h"
/*
 * Author Jiri Rasplicka, 2017
 */

//core_fn.S --------------------------------------------------------------------
extern void doEvents();
extern void doEventsL(void* label);
extern void doEventsError();
extern uint compareTimeMs(uint);
extern void pauseEvents(uint);                       //time ms
extern short checksum16(char*, uint len);
extern uint getTimeMs();

extern void checkStackSpace();

extern char getProcID();
extern void* getProcTableItem(char id);
extern void exitProcess();
extern void exitProcessID(char id);
extern void restartProcess();
extern void restartProcessID(char id);

extern uint regListener(void* fn, char eventID);
extern uint regListenerWithVar(void* fn, char eventID, uint var0, uint var1);
extern uint raiseEventID(char eventID, int p0, int p1, int p2);
extern uint* getRegEvent(uint* last_item, char event_id);
extern void raiseEvent(uint* item, int param0, int param1, int param2);
//extern void regListener(void* fn, char event_id);

extern void clearWDT();
extern void startWDT();
extern void pauseWDT();
extern void pauseCT();
extern void startCT();

extern void setCanSleep(char);
extern void setCanIdle(char);
extern void unregEvent(void*);

//When the byte is unlocked, lock it and returns 1.
//When the byte is locked, returns 0.
//@param addr Address of the byte
extern char lockSafe(char* addr);

extern void intToChar(int munber, char* ret, char minLen);

//periph_fn.S ------------------------------------------------------------------
extern void setPortDigOut(uint, uint);
extern void setPortDigIn(uint, uint);
extern void setPortAnalogIn(uint, uint);
extern void setPortOpenDrain(uint, uint, uint);
extern void setPortPullUp(uint, uint, uint);
extern void setPortPullDown(uint, uint, uint);

//timer1.S ---------------------------------------------------------------------
void timer1Init();
int systemTimerRegInterval(void*, uint);


//extern void iVector_timer1();
//extern void iVector_usb();
//extern void iVector_rtc();
//extern void iVector_adc();
//extern void iVector_spi1Tx();
//extern void iVector_spi2Tx();

#ifdef PIC32MM0256
    extern void iVector_i2c1Slave();
    extern void iVector_i2c1Master();
    extern void iVector_i2c1Bus();
    extern void iVector_i2c2Slave();
    extern void iVector_i2c2Master();
    extern void iVector_i2c2Bus();
    extern void iVector_i2c3Slave();
    extern void iVector_i2c3Master();
    extern void iVector_i2c3Bus();
#endif


#ifdef UBTN
    //ubtn.S
    extern void ubtnStart();
    extern int  ubtnRegEvent(void*, char min_value, char max_value);
#endif
    
#ifdef USE_TOUCHPAD_XPT2046 
    extern void touchXpt2046_start();
    //extern void touchXpt2046_setDisplay(void* d);
#endif    

#ifdef RTC
    extern void rtcInit();
    extern void rtcSetTimeBCD(uint);
    extern void rtcSetDateBCD(uint);
    extern uint rtcGetTimeBCD();
    extern uint rtcGetDateBCD();

    extern char rtcGetHour();
    extern char rtcGetMinute();
    extern char rtcGetSecond();
    extern char rtcGetYear();
    extern char rtcGetMonth();
    extern char rtcGetDay();
    extern char rtcGetDayOfWeek();

    extern char rtcGetHalfSecond();
    extern int  rtcRegTimeAlarm(void*, char, char);
#endif

#if defined PORT_CN 
    #if defined PIC32MZ
        extern void cnStartPortA(char);
        extern void cnStopPortA();
        extern void cnStartPortB(char);
        extern void cnStopPortB();
        
        extern void cnEnable(uint, short);
        extern void cnDisble(uint, short);
    #elif defined PIC32MM
        extern void cnStartPortA();
        extern void cnStopPortA();
        extern void cnStartPortB();
        extern void cnStopPortB();
        extern void cnStartPortC();
        extern void cnStopPortC();
        extern void cnStartPortD();
        extern void cnStopPortD();

        extern void cnEnable(uint, short, char);
        extern void cnDisble(uint, short, char);        
        
        extern void iVector_cnPortA();
        extern void iVector_cnPortB();
        extern void iVector_cnPortC();
        extern void iVector_cnPortD();
    #endif

    extern void cnRegEvent(void*, uint);
#endif


extern int strLen(char*);

/*
#if defined USE_GRAPHICS
    //vsechny displeje

    //Sets information about the font
    //@param head FONT_HEAD*, address of font head
    //@param src IMAGE_SRC*, address of structure, that represents the font data
    extern void setFontSrc(const void* head, void* src);
    
    //Sets information about the image
    //@param head IMAGE_HEAD*, address of image head
    //@param src IMAGE_SRC*, address of structure, that represents the image data
    extern void setImageSrc(const void* head, void* src);
    
    
    #if defined USE_DISP9341
        //pouze barevne (ILI9341)

        //Return short (16-bit) representation of RGB color
        //@param red 5-bit value, 0-31
        //@param green 6-bit value, 0-63
        //@param blue 5-bit value, 0-31
        extern short RGB16(int red, int green, int blue);
    
        //Sets color map
        //@param src Address of IMAGE_SRC structure, that represents image/font data
        //@param map Color map
        extern void setColorMap(void* src, const short* map);
                      
    #endif  //USE_DISP9341

    
#endif  //USE_GRAPHICS
*/
