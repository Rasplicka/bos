#include "def.h"
/*
 * Author Jiri Rasplicka, 2017
 */


//extern void _general_exception_handler();
//extern void _simple_tlb_refill_exception_handler();

//core_fn.S
extern void initCPU();
extern void startupStack();
extern int allocStack(int, int*);
extern void clearSystemData(uint, uint);
extern void setSrsValue();
extern void startEvents();
extern void doEvents();
extern void doEventsL(void* label);
extern void doEventsError();
extern uint getGP();
extern void restartApp();
extern void restartAppId(char id);
extern uint compareTimerMs(uint);
extern void pauseEvents(uint);                       //time ms
extern void general_exception();

extern char getProcID();
extern void* getProcTableItem(char id);
extern void exitProcess();
extern void clearWDT();
extern void unregEvent(void*);
extern void systemProcess();

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
extern void iVector_spi1Tx();
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