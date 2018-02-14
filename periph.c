#include <xc.h>
#include <stdio.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS core (pins setting)
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */

void portSet(uint base, uint bits)
{
    //base=bazova adresa portu (PORTA_BASE, PORTB_BASE, ...)
    //bits=ovlinvene bity (BIT0, BIT0 | BIT1 | BIT5, ...)
    //priklad: (nastaveni B.3 a B.7)
    //portSet(PORTB_BASE, BIT3 | BIT7)
    
    volatile int* p;
    p=(int*)(base + LAT_OFFSET + SET_OFFSET);
    *p=bits;
}

void portClear(uint base, uint bits)
{
    //base=bazova adresa portu (PORTA_BASE, PORTB_BASE, ...)
    //bits=ovlinvene bity (BIT0, BIT0 | BIT1 | BIT5, ...)
    //priklad: (nulovani B.3 a B.7)
    //portClear(PORTB_BASE, BIT3 | BIT7)
    
    volatile int* p;
    p=(int*)(base + LAT_OFFSET + CLR_OFFSET);
    *p=bits;
}

void portInv(uint base, uint bits)
{
    //base=bazova adresa portu (PORTA_BASE, PORTB_BASE, ...)
    //bits=ovlinvene bity (BIT0, BIT0 | BIT1 | BIT5, ...)
    //priklad: (inverze B.3 a B.7)
    //portInv(PORTB_BASE, BIT3 | BIT7)
    
    volatile int* p;
    p=(int*)(base + LAT_OFFSET + INV_OFFSET);
    *p=bits;
}

void testLedSet(int num)             //test led
{
    //num test led, nastavi pin=H
    
    volatile int* p;
    if(num==1)
    {
        p=(int*)(LED1_BASE + LAT_OFFSET + SET_OFFSET);
        *p=LED1_BIT;
    }
    else if(num==2)
    {
        p=(int*)(LED2_BASE + LAT_OFFSET + SET_OFFSET);
        *p=LED2_BIT;
    }
    else if(num==3)
    {
        p=(int*)(LED3_BASE + LAT_OFFSET + SET_OFFSET);
        *p=LED3_BIT;
    }
    else if(num==4)
    {
        p=(int*)(LED4_BASE + LAT_OFFSET + SET_OFFSET);
        *p=LED4_BIT;
    }    
}

void testLedClear(int num)             //test led
{
    //num test led, nastavi pin=L
    
    volatile int* p;
    if(num==1)
    {
        p=(int*)(LED1_BASE + LAT_OFFSET + CLR_OFFSET);
        *p=LED1_BIT;
    }
    else if(num==2)
    {
        p=(int*)(LED2_BASE + LAT_OFFSET + CLR_OFFSET);
        *p=LED2_BIT;
    }
    else if(num==3)
    {
        p=(int*)(LED3_BASE + LAT_OFFSET + CLR_OFFSET);
        *p=LED3_BIT;
    }
    else if(num==4)
    {
        p=(int*)(LED4_BASE + LAT_OFFSET + CLR_OFFSET);
        *p=LED4_BIT;
    }    
}

void testLedInv(int num)             //test led
{
    //num test led, invertuje pin
    
    volatile int* p;
    if(num==1)
    {
        p=(int*)(LED1_BASE + LAT_OFFSET + INV_OFFSET);
        *p=LED1_BIT;
    }
    else if(num==2)
    {
        p=(int*)(LED2_BASE + LAT_OFFSET + INV_OFFSET);
        *p=LED2_BIT;
    }
    else if(num==3)
    {
        p=(int*)(LED3_BASE + LAT_OFFSET + INV_OFFSET);
        *p=LED3_BIT;
    }
    else if(num==4)
    {
        p=(int*)(LED4_BASE + LAT_OFFSET + INV_OFFSET);
        *p=LED4_BIT;
    }    
}

void signalizeError(int num, int code)
{
    //num je test LED 1-4
    //code je kod, ktery se zobrazuje (definuje pocet bliknuti)
    
    while(1)
    {
        int x=code;
        
        while(x > 0)
        {
            testLedSet(num);
            pauseEvents(60);
        
            testLedClear(num);
            pauseEvents(400);
            
            x--;
        }
        
        pauseEvents(2000);
    }
}



void periphInitX()
{
    //jez nepouzito, zatim ponechano pro info
#ifdef PIC32MZ
    
    TRISH=0b1111111111111000;
    PORTH=0x0;
    LATHSET=0b000;
    
#endif    
    
#ifdef PIC32MM0064_28pin
    
    
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0xFFFF;
    LATB = 0xFFFF;
    LATC = 0xFFFF;
    
    ANSELA=0;
    ANSELB=0;
    ANSELC=0;
    //direction out (0) 9,8,6,5
    //TRISB = 0xFC9F; //BC9F; 
    //PIC32MM0064

    TRISBbits.TRISB5=0;     //LED   pin 14
    TRISBbits.TRISB6=0;     //D/C   pin 15
    TRISBbits.TRISB8=0;     //CLK   pin 17
    TRISBbits.TRISB9=0;     //DO    pin 18    
    TRISBbits.TRISB7=0;     //RESET pin 16
    TRISBbits.TRISB3=0;     //CS    pin 11
    //ANSELBbits.ANSB3=0;     //CS    pin 7

    //TRISA=0;
    //TRISB=0;
    //TRISC=0;
    
    
    setPortDigOut(PORTB_BASE, BIT3 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9);

    //SPI2
    //RPOR2bits.RP12R=3;
    //RPOR4bits.RP18R=4;
    
#endif
    
    
#ifdef __32MM0256GPM036__ 
    
    //nastaveni IO portu v zavislosti a typu IC a aplikaci
    //PIC32MM0256
    
    //ANSEL=1 analog input, ANSEL=0 - digital IO
    //TRIS=1 - dig. input, TRIS=0 - dig. output (plati pri ANSEL=0)
    //digital out - nastaveni pinu = zapis do LATx (cteni nastavene hodnoty = cteni LATx, cteni skutecne hodnoty = cteni PORTx)
    //digital in - cteni stavu pinu = cteni PORTx
    
    //set analog in
    ANSELA=0xFFFF;
    ANSELA=0xFFFF;
    ANSELA=0xFFFF;
    
    TRISA=0xFFFF;
    TRISB=0xFFFF;
    TRISC=0xFFFF;
    
    LATA = 0xFFFF;
    LATB = 0xFFFF;
    LATC = 0xFFFF;
    
    //RB5, RB7
    setPortDigOut(PORTB_BASE, BIT5 | BIT7);

    //RC3
    setPortDigOut(PORTC_BASE, BIT3);
    
    
    return;
    
    ANSELA=0;
    ANSELB=0;
    ANSELC=0;
    
    //B2,3, C0,1
    
    TRISCbits.TRISC3=0;     //LED1   pin 14
    TRISBbits.TRISB5=0;     //LED2   pin 15
    TRISBbits.TRISB7=0;     //LED3   pin 17 SPI2 CLK
    TRISBbits.TRISB8=0;     //       pin 18 SPI2 DATA
    
    TRISBbits.TRISB9=0;     //       pin 19 SPI2 RESET
    TRISCbits.TRISC9=0;     //       pin 21 SPI2 DC
    
    TRISBbits.TRISB2=0;     //RP8-pin1 (CLK SPI2),       (DATA I2C2)
    TRISBbits.TRISB3=0;     //RP9-pin2 (DATA OUT SPI2),  (CLK  I2C2)
    
    //pozor, je pripojeno na GND
    //TRISAbits.TRISA2=0;
    //LATAbits.LATA2=1;
    

    
    //Vadny spoj
    TRISAbits.TRISA9=0;
    LATAbits.LATA9=1;

    //RB4, RP10, pouzito pro pwm displeje
    //TRISBbits.TRISB4=0;
    //LATBbits.LATB4=0;
    //RPOR2bits.RP10R=11;

    // <editor-fold defaultstate="collapsed" desc="test">
    //test

    /*
    LATBbits.LATB7 = 1;
    */


    
    // </editor-fold>

    //A.2 je pripojeno na GND (ili9341) - nemenit
    
    // <editor-fold defaultstate="collapsed" desc="SPI2, MM 36pin, RP8, RP9, C0, C1, C2 ">
    
    //pin
    //1  RP12 - SPI2CLK
    //2  RP13 - SPI2DATA out
    //3  C0 - DC     display
    //4  C1 - RESET  display
    //5  C2 - CS     display
    //7  A2-pripojeno na GND
    //8  A3 - CS     touchpad
    //9  RP10 - SPI2DATA in
    //10 A4 - penirq touchpad
    
    //DISPLAY: RESET, DC, CS
    TRISCbits.TRISC0=0;         //pin 3 DC (ili9341), C0
    TRISCbits.TRISC1=0;         //pin 4 RESET (ili9341), C1
    TRISCbits.TRISC2=0;         //pin 5 CS (ili9341), C2
    
    //TOUCH: CS signal TouchPad
    TRISAbits.TRISA3=0;         //pin 8 CS (XPT2046), RA3
    //LATAbits.LATA3=1;
    
    //CLK, SDO
    RPOR1bits.RP8R=9;           //pin 1 SPI2CLK  RP12,
    RPOR2bits.RP9R=8;           //pin 2 SPI2DO   RP13,
    
    //SDI
    TRISBbits.TRISB4=1;
    LATBbits.LATB4=1;
    RPINR11bits.SDI2R=0b01010;  //pin 9 SPI2DI  RP10, (RB4)
    
    //RA4, TouchPad penirq
    TRISAbits.TRISA4=1;
    LATAbits.LATA4=1;
    
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="I2C2, MM 36pin">
    //pin
    //1  SDA        display (pin se nastavi automaticky, pri zapnuti I2C2 modulu)
    //2  CLK        display (pin se nastavi automaticky, pri zapnuti I2C2 modulu)
    //4  C1 - RESET display
    
    TRISCbits.TRISC1=0;         //pin 4 RESET (ili9341), C1
    
    // </editor-fold>

    
    //setPortDigOut(PORTC_BASE, BIT0);        //nefunguje???
    
#endif    
    
}