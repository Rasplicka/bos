#include <xc.h>
#include <stdio.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka 
 * Copyright (c) 2017, All rights reserved.
 * Version 1.0       
 * BOS user apps interface
 * This file is shared for the PIC32MM, PIC32MZ
 * 
 */

//user apps
//app1
extern void m1_start();
//app2
extern void m2_start();
//app3
extern void m3_start();


void pinSetting()
{
    //Call OS on startup

    //vstupni RP piny, do registru fce vlozi cislo RP pinu
    //INT4_IO = RP1_IN; //ICM1_IO = RP5_IN; //SDI2_IO = RP8_IN;
    
    //vystupni RP piny, do registru RP pinu vlozi cislo fce
    //RP2_OUT = C1OUT_IO; //RP3_OUT = SDO2_IO; //RP4_OUT = SCK2OUT_IO;
    
    
#ifdef __32MM0256GPM036__ 

     //RB5, RB7
    setPortDigOut(PORTB_BASE, BIT5 | BIT7);
    //RC3
    setPortDigOut(PORTC_BASE, BIT3);
    
    LED1_BASE=PORTB_BASE;
    LED1_BIT=BIT5;
    
    LED2_BASE=PORTB_BASE;
    LED2_BIT=BIT7;
    
    LED3_BASE=PORTC_BASE;
    LED3_BIT=BIT3;    
    
#endif
    
#ifdef __32MM0256GPM048__     
    
#endif    
    
#ifdef __32MM0256GPM064__     
    
#endif  
    
#ifdef PIC32MZ

    //test led PORTH.0-2
    setPortDigOut(PORTH_BASE, BIT0 | BIT1 | BIT2);
    LATHCLR=(BIT0 | BIT1 | BIT2);
    
    LED1_BASE=(void*)PORTH_BASE;
    LED1_BIT=BIT0;
    LED2_BASE=(void*)PORTH_BASE;
    LED2_BIT=BIT1;
    LED3_BASE=(void*)PORTH_BASE;
    LED3_BIT=BIT2;    
    
    //test buttons PORTB.12-14
    setPortDigIn(PORTB_BASE, BIT12 | BIT13 | BIT14);                            //ubtn provede nastaveni take
    setPortPullUp(PORTB_BASE, BIT12 | BIT13 | BIT14, 1);    
    TRISB=0xFFFF;
    PORTB=0xFFFF;
#endif    
    
#ifdef TEST_BOARD_BOS0
    
    //RC3/ LED1
    setPortDigOut(PORTC_BASE, BIT3);                                    //14
    LED1.portBase=PORTC_BASE;
    LED1.pin=BIT3;
    
    //RP11/RB5/ LED2
    setPortDigOut(PORTB_BASE, BIT5);                                    //15
    LED2.portBase=PORTB_BASE;
    LED2.pin=BIT5;    
    
    //RP12/RB7/ LED2
    setPortDigOut(PORTB_BASE, BIT7);                                    //17
    LED3.portBase=PORTB_BASE;
    LED3.pin=BIT7;     
    
    //UART ---------------------------------------------------------------------
    //Tx bus pin1
    setPortDigOut(PORTB_BASE, BIT2);
    setPortPin(PORTB_BASE, BIT2);
    setPortPullUp(PORTB_BASE, BIT2, 1);
    RP8_OUT=U2TX_IO;                //tx RP8/B2/pin1
    
    //Rx bus pin2
    //setPortDigIn(PORTC_BASE, BIT2);
    setPortDigIn(PORTB_BASE, BIT3);
    setPortPullUp(PORTB_BASE, BIT3, 1);
    U2RX_IO=RP9_IN;                 //rx PR9/B3/pin2
    
#endif    
   
#ifdef NETCOM_BOARD_0256
    
    //LED (nebo display SSD1306) -----------------------------------------------
    //RA2/pin9 LED1
    setPortDigOut(PORTA_BASE, BIT2);                                    //9
    LED1.portBase=PORTA_BASE;
    LED1.pin=BIT2;
    
    //RA3/pin10 LED2
    setPortDigOut(PORTA_BASE, BIT3);                                    //10
    LED2.portBase=PORTA_BASE;
    LED2.pin=BIT3;    
    
    //RB4/pin11 LED3
    setPortDigOut(PORTB_BASE, BIT4);                                    //11
    LED3.portBase=PORTB_BASE;
    LED3.pin=BIT4;

    //RA4/pin12 LED4
    setPortDigOut(PORTA_BASE, BIT4);                                    //12
    LED4.portBase=PORTA_BASE;
    LED4.pin=BIT4;    
    
    //Button -------------------------------------------------------------------
    //RB3/pin7 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT3);                                     //7
    setPortPullUp(PORTB_BASE, BIT3, 1);
    
    //Digital input ------------------------------------------------------------
    //RA0/pin2 
    setPortDigIn(PORTA_BASE, BIT0);                                     //2
    setPortPullDown(PORTA_BASE, BIT0, 1);    
    
    //Switches
    //SW0, RB5/pin14 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT5);                                     //14
    setPortPullUp(PORTB_BASE, BIT5, 1);        
    //SW1, RB9/pin18 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT9);                                     //18
    setPortPullUp(PORTB_BASE, BIT9, 1);        
    //SW2, RB8/pin17 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT8);                                     //17
    setPortPullUp(PORTB_BASE, BIT8, 1);        
    //SW3, RB7/pin16 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT7);                                     //16
    setPortPullUp(PORTB_BASE, BIT7, 1);        
        
    //Digital out --------------------------------------------------------------
    //RC9/pin19 (enable +12V output)
    setPortDigOut(PORTC_BASE, BIT9);                                     //19
    setPortPullDown(PORTC_BASE, BIT9, 1);         
    clearPortPin(PORTC_BASE, BIT9);
    
    //UART ---------------------------------------------------------------------
    //RB14/pin25 (U_Tx)
    setPortDigOut(PORTB_BASE, BIT14);                                   //25
    setPortPin(PORTB_BASE, BIT14);    
    
    //RB15/pin26 (U_Rx)
    setPortDigIn(PORTB_BASE, BIT15);                                    //26
        
    //RB13/pin24 (U_RTS, pin H = read)
    setPortDigOut(PORTB_BASE, BIT13);                                   //24
    setPortPullDown(PORTB_BASE, BIT13, 1);         
    setPortPin(PORTB_BASE, BIT13);
    
#endif    
    
#ifdef NETCOM_BOARD_0064
    
    //LED (nebo display SSD1306) -----------------------------------------------
    //RA2/pin9 LED1
    setPortDigOut(PORTA_BASE, BIT2);                                    //9
    LED1.portBase=PORTA_BASE;
    LED1.pin=BIT2;
    
    //RA3/pin10 LED2
    setPortDigOut(PORTA_BASE, BIT3);                                    //10
    LED2.portBase=PORTA_BASE;
    LED2.pin=BIT3;    
    
    //RB4/pin11 LED3
    setPortDigOut(PORTB_BASE, BIT4);                                    //11
    LED3.portBase=PORTB_BASE;
    LED3.pin=BIT4;

    //RA4/pin12 LED4
    setPortDigOut(PORTA_BASE, BIT4);                                    //12
    LED4.portBase=PORTA_BASE;
    LED4.pin=BIT4;    
    
    //Button -------------------------------------------------------------------
    //RB3/pin7 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT3);                                     //7
    setPortPullUp(PORTB_BASE, BIT3, 1);
    
    //Digital input ------------------------------------------------------------
    //RA0/pin2 
    setPortDigIn(PORTA_BASE, BIT0);                                     //2
    setPortPullDown(PORTA_BASE, BIT0, 1);    
    
    //Switches
    //SW0, RB5/pin14 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT5);                                     //14
    setPortPullUp(PORTB_BASE, BIT5, 1);        
    //SW1, RB9/pin18 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT9);                                     //18
    setPortPullUp(PORTB_BASE, BIT9, 1);        
    //SW2, RB8/pin17 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT8);                                     //17
    setPortPullUp(PORTB_BASE, BIT8, 1);        
    //SW3, RB7/pin16 (pin-gnd)
    setPortDigIn(PORTB_BASE, BIT7);                                     //16
    setPortPullUp(PORTB_BASE, BIT7, 1);        
        
    //Digital out --------------------------------------------------------------
    //RC9/pin19 (enable +12V output)
    setPortDigOut(PORTC_BASE, BIT9);                                     //19
    setPortPullDown(PORTC_BASE, BIT9, 1);         
    clearPortPin(PORTC_BASE, BIT9);
    
    //UART 1--------------------------------------------------------------------
    //RB14/pin25 (U_Tx)
    setPortDigOut(PORTB_BASE, BIT14);                                   //25
    setPortPin(PORTB_BASE, BIT14);    
    
    //RB15/pin26 (U_Rx)
    setPortDigIn(PORTB_BASE, BIT15);                                    //26
        
    //RB13/pin24 (U_RTS, pin H = read)
    setPortDigOut(PORTB_BASE, BIT13);                                   //24
    setPortPullDown(PORTB_BASE, BIT13, 1);         
    setPortPin(PORTB_BASE, BIT13);
    
#endif    
        
#ifdef TEST_BOARD_BOS1    
    
    
    //DISPLEJE -----------------------------------------------------------------
    
    //RB7/pin47 - TOUCHPAD IRQ
    setPortDigIn(PORTB_BASE, BIT7);                                     //47
    setPortPullDown(PORTB_BASE, BIT7, 1);
    
    //RP23/pin2 - SPI2-IN (TOUCHPAD SDO)
    SDI2_IO=RP23_IN;                                                    //2
    
    //PR16/pin15 - SPI2-OUT (DISP SDI)
    RP16_OUT=SDO2_IO;                                                   //15
    
    //RP21/pin14 - SPI2-SCK
    RP21_OUT=SCK2OUT_IO;                                                //14
    
    //RB6/pin46 - TOUCHPAD CS
    setPortDigOut(PORTB_BASE, BIT6);                                    //46
    
    //RP22/pin13 CS DISP2
    setPortDigOut(PORTA_BASE, BIT10);                                   //13
    
    //RB13/pin12 DC DISP
    setPortDigOut(PORTB_BASE, BIT13);                                   //12
    
    //RA15/pin8 RESET DISP
    setPortDigOut(PORTA_BASE, BIT15);                                   //8
    
    //RC8/pin4 CS DISP1
    setPortDigOut(PORTC_BASE, BIT8);                                    //4
    
    //RC9/pin5 DISP LED ON
    setPortDigOut(PORTC_BASE, BIT9);                                    //5
    setPortOpenDrain(PORTC_BASE, BIT9, 1);
    
    //LED ----------------------------------------------------------------------
    
    //RA9/pin37 LED1
    setPortDigOut(PORTA_BASE, BIT9);                                    //37
    LED1.portBase=PORTA_BASE;
    LED1.pin=BIT9;
    
    //RD0/pin38 LED2
    setPortDigOut(PORTD_BASE, BIT0);                                    //38
    LED2.portBase=PORTD_BASE;
    LED2.pin=BIT0;    
    
    //RC3/pin39 LED3
    setPortDigOut(PORTC_BASE, BIT3);                                    //39
    LED3.portBase=PORTC_BASE;
    LED3.pin=BIT3;    
    
    //RC4/pin40 LED4
    setPortDigOut(PORTC_BASE, BIT4);                                    //40
    LED4.portBase=PORTC_BASE;
    LED4.pin=BIT4;    
    
    //TLACITKA -----------------------------------------------------------------
    
    //RC1/pin28 Button1
    setPortDigIn(PORTC_BASE, BIT1);                                     //28
    setPortPullDown(PORTC_BASE, BIT1, 1);
    
    //RC2/pin29 Button2
    setPortDigIn(PORTC_BASE, BIT2);                                     //29
    setPortPullDown(PORTC_BASE, BIT2, 1);
    
    //RA3/pin33 Button3
    setPortDigIn(PORTA_BASE, BIT3);                                     //33
    setPortPullDown(PORTA_BASE, BIT3, 1);
    
    //RA8/pin34 Button4
    setPortDigIn(PORTA_BASE, BIT8);                                     //34
    setPortPullDown(PORTA_BASE, BIT8, 1);
    
    //PIEZO --------------------------------------------------------------------
    //RC12/pin44
    setPortDigOut(PORTC_BASE, BIT12);                                   //44
    
    //ANALOG INPUT -------------------------------------------------------------
    //RB2/pin25
    //setPortAnalogIn(PORTB_BASE, BIT2);                                  //25
    
    //RB3/pin26
    //setPortAnalogIn(PORTB_BASE, BIT3);                                  //26
    
    //RC0/pin27
    //setPortAnalogIn(PORTC_BASE, BIT0);                                  //27

    //UART ---------------------------------------------------------------------
    //setPortDigOut(PORTB_BASE, BIT2);
    //RP8_OUT=U2TX_IO;                //tx RP8/B2/pin25, UARTcon 4 (1=GND)
    
    //setPortDigIn(PORTB_BASE, BIT3);
    //U2RX_IO=RP9_IN;                 //rx PR9/B3/pin26, UARTcon 3
    
    //RP14/B9, pin1, TX, bus pin 6
    setPortDigOut(PORTB_BASE, BIT9); 
    setPortPin(PORTB_BASE, BIT9);
    setPortPullUp(PORTB_BASE, BIT9, 1);
    RP14_OUT=U2TX_IO;
    
    //RP13/B8, pin48, RX, bus pin 7
    setPortDigIn(PORTB_BASE, BIT8); 
    setPortPullUp(PORTB_BASE, BIT8, 1);
    U2RX_IO=RP13_IN;
#endif    

}

void userAppsStart()
{
    //Calls OS on startup
    
    APP_START_PARAM p1;
    p1.TimeLimitExceedBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.GeneralExceptionBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.TrapBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.TimeLimitValue = 0xFFFFFF;
    
    
    if(regProcess(&m1_start, 1024, &p1, 0x1) < 0)
    {
        //error, cannot run app
    }
    
#ifndef NETCOM_BOARD_0064    
    if(regProcess(&m2_start, 1024, &defaultAppStartParam, 0x2) < 0)
    {
        //error, cannot run app
    }
#endif    
    
    if(regProcess(&m3_start, 1024, &defaultAppStartParam, 0x3) < 0) 
    {
        //error, cannot run app
    }
}

void userAppError(char procId, char code, void* addr)
{
    //Call OS on exception, when CP0_STATUS.EXL=1
    //now, all interrupts are disabled, it is possible execute sw HALT CPU 
    //procId=process ID
    //code=error code, 
    //addr=exception address 
    
    //When this fn ends, OS performs the appropriate action (reset process, reset system, ...)
    //according to "ON_ERROR" app behavior setting
    
    int a=0;
    int b=0;
    int c=a+b;
    
    /*
     * this is HALT code 
     * for example, you can set HALT LED indicator
    while(1)
    {

    }
    */
}

void userAppEventError(char procId, char eventId)
{
    int a=procId;
    int b=eventId;
}

void userAppCheckStackSpace(int space, int is_event)
{
    //space - vyjadruje aktualni hodnotu volneho mista ve stacku
    //is_event=1 - fce je volana jako obsluha udalosti (bezi jako systemProcess)
    //is_event=0 - fce neni volana jako obsluha udalosti

    int min=checkStackSpaceValue;
    
    if(space < 64)
    {
        //breakpoint
        int a=1;
        //trap();
        //softReset();
    }
}

void beforeSleep()
{
    //display LED off
    LATCbits.LATC9=1;
}

void beforeIdle()
{
    int a=0;
    int b=a+5;
}