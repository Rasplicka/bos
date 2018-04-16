#include "globals.h"

//globals
void main();
int regProcess(void* start_addr, int stack_size, const APP_START_PARAM* param, char default_procID);

//void processException(char procId, char code, void* addr);
//void trap();
//void softReset();

//locals
static char getFreeProcessID(char defaultId);
static int* getEmptyProcessTableItem();
static void systemInit();
static inline void cpuTimerInit();
static void test();

//void _general_exception_context();
//void _general_exception_handler (void);

//interrupt vektory musi byt zde
#ifdef PIC32MM0064

extern void __attribute__((interrupt(), vector(0)))  iVector0();                //CPU timer

#ifdef PORT_CNA
extern void __attribute__((interrupt(), vector(8))) iVector_cnPortA();          //change notif. port A
#endif
#ifdef PORT_CNB
extern void __attribute__((interrupt(), vector(9))) iVector_cnPortB();          //change notif. port B
#endif
#ifdef PORT_CNC
extern void __attribute__((interrupt(), vector(10))) iVector_cnPortC();          //change notif. port C
#endif

extern void __attribute__((interrupt(), vector(11))) iVector_timer1();          //Timer1

extern void __attribute__((interrupt(), vector(14))) iVector_rtc();             //RTC alarm

//extern void __attribute__((interrupt(), vector(15))) iVector_adc();             //adc complete

#ifdef SPI1_USE
//extern void __attribute__((interrupt(), vector(21))) iVector_spi1Tx();        //SPI1 Tx
#endif
#ifdef SPI2_USE
//extern void __attribute__((interrupt(), vector(38))) iVector_spi1Tx();        //SPI2 Tx
#endif

#ifdef UART1_USE
extern void __attribute__((interrupt(), vector(23))) iVector_UART1Rx();        //UART1 Rx
extern void __attribute__((interrupt(), vector(24))) iVector_UART1Tx();        //UART1 Tx
extern void __attribute__((interrupt(), vector(25))) iVector_UART1Er();        //UART1 Error
#endif
#ifdef UART2_USE
extern void __attribute__((interrupt(), vector(40))) iVector_UART2Rx();        //UART2 Rx
extern void __attribute__((interrupt(), vector(41))) iVector_UART2Tx();        //UART2 Tx
extern void __attribute__((interrupt(), vector(42))) iVector_UART2Er();        //UART2 Error
#endif


#endif

#ifdef PIC32MM0256

extern void __attribute__((interrupt(), vector(0)))  iVector0();                //CPU timer

#ifdef PORT_CNA
extern void __attribute__((interrupt(), vector(8))) iVector_cnPortA();          //change notif. port A
#endif
#ifdef PORT_CNB
extern void __attribute__((interrupt(), vector(9))) iVector_cnPortB();          //change notif. port B
#endif
#ifdef PORT_CNC
extern void __attribute__((interrupt(), vector(10))) iVector_cnPortC();          //change notif. port C
#endif
#ifdef PORT_CND
extern void __attribute__((interrupt(), vector(11))) iVector_cnPortD();          //change notif. port D
#endif

extern void __attribute__((interrupt(), vector(17))) iVector_timer1();          //Timer1
//extern void __attribute__((interrupt(), vector(29))) iVector_usb();           //USB

#ifdef RTC
extern void __attribute__((interrupt(), vector(32))) iVector_rtc();             //RTC alarm
#endif

//extern void __attribute__((interrupt(), vector(33))) iVector_adc();           //adc complete

#ifdef SPI1_USE
extern void __attribute__((interrupt(), vector(42))) iVector_spi1Tx();        //SPI1 Tx
#endif
#ifdef SPI2_USE
extern void __attribute__((interrupt(), vector(45))) iVector_spi2Tx();        //SPI2 Tx
#endif
#ifdef SPI3_USE
extern void __attribute__((interrupt(), vector(48))) iVector_spi3Tx();        //SPI3 Tx
#endif

#ifdef UART1_USE
extern void __attribute__((interrupt(), vector(53))) iVector_UART1Rx();        //UART1 Rx
extern void __attribute__((interrupt(), vector(54))) iVector_UART1Tx();        //UART1 Tx
extern void __attribute__((interrupt(), vector(55))) iVector_UART1Er();        //UART1 Error
#endif
#ifdef UART2_USE
extern void __attribute__((interrupt(), vector(56))) iVector_UART2Rx();        //UART2 Rx
extern void __attribute__((interrupt(), vector(57))) iVector_UART2Tx();        //UART2 Tx
extern void __attribute__((interrupt(), vector(58))) iVector_UART2Er();        //UART2 Error
#endif
#ifdef UART3_USE
extern void __attribute__((interrupt(), vector(59))) iVector_UART3Rx();        //UART3 Rx
extern void __attribute__((interrupt(), vector(60))) iVector_UART3Tx();        //UART3 Tx
extern void __attribute__((interrupt(), vector(61))) iVector_UART3Er();        //UART3 Error
#endif
//extern void __attribute__((interrupt(), vector(65))) iVector_i2c1Slave();     //I2C1 Master Mode
//extern void __attribute__((interrupt(), vector(66))) iVector_i2c1Master();    //I2C1 Master Mode
//extern void __attribute__((interrupt(), vector(67))) iVector_i2c1Bus();       //I2C1 Master Mode
//extern void __attribute__((interrupt(), vector(68))) iVector_i2c2Slave();     //I2C2 Master Mode
//extern void __attribute__((interrupt(), vector(69))) iVector_i2c2Master();    //I2C2 Master Mode
//extern void __attribute__((interrupt(), vector(70))) iVector_i2c2Bus();       //I2C2 Master Mode
//extern void __attribute__((interrupt(), vector(71))) iVector_i2c3Slave();     //I2C3 Master Mode
//extern void __attribute__((interrupt(), vector(72))) iVector_i2c3Master();    //I2C3 Master Mode
//extern void __attribute__((interrupt(), vector(73))) iVector_i2c3Bus();       //I2C3 Master Mode

#endif

#ifdef PIC32MZ
//extern void __attribute__((interrupt(), vector(0)))  iVector0();            //CPU timer
#endif