/*
 * Author Jiri Rasplicka, 2017
 */

//core_fn.S
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
extern void ubtn_start();
extern int  ubtn_regEvent(void*);
extern void ubtn_unregEvent(void*);

#endif

#ifdef RTC
extern void rtcInit();
extern void rtcSetTime(uint);
extern void rtcSetDate(uint);
extern uint rtcGetTime();
extern uint rtcGetDate();
extern char rtcGetHalfSecond();
#endif