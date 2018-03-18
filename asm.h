#include "def.h"
/*
 * Author Jiri Rasplicka, 2017
 */

//int __far checkStackSpaceValue;
//extern void _general_exception_handler();
//extern void _simple_tlb_refill_exception_handler();

//core_fn.S
//extern void initCPU();
//extern void startupStack();
//extern int allocStack(int, int*);
//extern void clearSystemData(uint, uint);
//extern void setSrsValue();
//extern void startEvents();
extern char sleepStatus;

extern void doEvents();
extern void doEventsL(void* label);
extern void doEventsError();
//extern uint getGP();
//extern void restartApp();
//extern void restartAppId(char id);
extern uint compareTimeMs(uint);
extern void pauseEvents(uint);                       //time ms
extern uint getTimeMs();
//extern uint getDayMs();                           //DayMs zruseno

//extern void general_exception();
extern void checkStackSpace();

extern char getProcID();
extern void* getProcTableItem(char id);
extern void exitProcess();
extern void exitProcessID(char id);
extern void restartProcess();
extern void restartProcessID(char id);

extern uint regListener(void* fn, char eventID);
extern uint raiseEventID(char eventID, int p0, int p1, int p2);

extern void clearWDT();
extern void startWDT();
extern void pauseWDT();
extern void pauseCT();
extern void startCT();

extern void setCanSleep(char);
extern void setCanIdle(char);
extern void unregEvent(void*);
//extern void systemProcess();

//periph_fn.S
extern void setPortDigOut(uint, uint);
extern void setPortDigIn(uint, uint);
extern void setPortAnalogIn(uint, uint);
extern void setPortOpenDrain(uint, uint, uint);
extern void setPortPullUp(uint, uint, uint);
extern void setPortPullDown(uint, uint, uint);

//timer1.S
void timer1Init();
int systemTimerRegInterval(void*, uint);


extern void iVector_timer1();
extern void iVector_usb();
extern void iVector_rtc();
extern void iVector_adc();
//extern void iVector_spi1Tx();
extern void iVector_spi2Tx();

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

#ifdef USE_GRAPHICS
//ILI9341
extern void setFontSrc(const void*, void*);

//Return short (16-bit) representation of RGB color
//@param red 5-bit value, 0-31
//@param green 6-bit value, 0-63
//@param blue 5-bit value, 0-31
extern short RGB16(int red, int green, int blue);

extern int imageToBuffer(void*, char*, int, char);
extern int imageToPort(void*, volatile int*, int, char);
extern void drawPointQuick(void*, volatile int*, void*);
extern void fillRectDirect(short, int, volatile int*);
extern void setImageColorMap(void*, short*);
extern void drawLineQuick(void*, void*, void*);

extern int fontCharParam(void*, char);                      ///TODO: help is here
#endif