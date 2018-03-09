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
    
#ifdef TEST_BOARD_BOS1    
    
    //DISPLEJE -----------------------------------------------------------------
    
    //RB7/pin47 - TOUCHPAD IRQ
    setPortDigIn(PORTB_BASE, BIT7);                                     //47
    
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
    LED1_BASE=(void*)PORTA_BASE;
    LED1_BIT=BIT9;
    
    //RD0/pin38 LED2
    setPortDigOut(PORTD_BASE, BIT0);                                    //38
    LED2_BASE=(void*)PORTD_BASE;
    LED2_BIT=BIT0;
    
    //RC3/pin39 LED3
    setPortDigOut(PORTC_BASE, BIT3);                                    //39
    LED3_BASE=(void*)PORTC_BASE;
    LED3_BIT=BIT3;
    
    //RC4/pin40 LED4
    setPortDigOut(PORTC_BASE, BIT4);                                    //40
    LED4_BASE=(void*)PORTC_BASE;
    LED4_BIT=BIT4;
    
    //TLACITKA -----------------------------------------------------------------
    
    //RC1/pin28 B1
    setPortDigIn(PORTC_BASE, BIT1);                                     //28
    
    //RC2/pin29 B2
    setPortDigIn(PORTC_BASE, BIT2);                                     //29
    
    //RA3/pin33
    setPortDigIn(PORTA_BASE, BIT3);                                     //33
    
    //RA8/pin34
    setPortDigIn(PORTA_BASE, BIT8);                                     //34
    
    //PIEZO --------------------------------------------------------------------
    //RC12/pin44
    setPortDigOut(PORTC_BASE, BIT12);                                   //44
    
    //ANALOG INPUT -------------------------------------------------------------
    //RB2/pin25
    setPortAnalogIn(PORTB_BASE, BIT2);                                  //25
    
    //RB3/pin26
    setPortAnalogIn(PORTB_BASE, BIT3);                                  //26
    
    //RC0/pin27
    setPortAnalogIn(PORTC_BASE, BIT0);                                  //27
    
    
    
    
#endif    

}

void userAppsStart()
{
    //Calls OS on startup
    
    APP_START_PARAM p1;
    p1.TimeLimitExceedBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.GeneralExceptionBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.TrapBehavior = ON_ERROR.REMOVE_PROCESS;
    p1.TimeLimitValue = 0xFFFFF;
    
    
    if(regProcess(&m1_start, 1024, &p1, 0x1) < 0)
    {
        //error, cannot run app
    }
    
    if(regProcess(&m2_start, 1024, &defaultAppStartParam, 0x2) < 0)
    {
        //error, cannot run app
    }
    
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
    
}

void beforeIdle()
{
    int a=0;
    int b=a+5;
}