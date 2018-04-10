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

//Set IO pin to log. 1
//@param base Port registers base address (e.g. PORTA_BASE)
//@param bits Bits definition (e.g. BIT0, or BIT5 | BIT8 | BIT11)
void setPortPin(uint base, uint bits)
{
    //base=bazova adresa portu (PORTA_BASE, PORTB_BASE, ...)
    //bits=ovlinvene bity (BIT0, BIT0 | BIT1 | BIT5, ...)
    //priklad: (nastaveni B.3 a B.7)
    //portSet(PORTB_BASE, BIT3 | BIT7)
    
    volatile int* p;
    p=(int*)(base + LAT_OFFSET + SET_OFFSET);
    *p=bits;
}

//Clear IO pin to log. 0
//@param base Port registers base address (e.g. PORTA_BASE)
//@param bits Bits definition (e.g. BIT0, or BIT5 | BIT8 | BIT11)
void clearPortPin(uint base, uint bits)
{
    //base=bazova adresa portu (PORTA_BASE, PORTB_BASE, ...)
    //bits=ovlinvene bity (BIT0, BIT0 | BIT1 | BIT5, ...)
    //priklad: (nulovani B.3 a B.7)
    //portClear(PORTB_BASE, BIT3 | BIT7)
    
    volatile int* p;
    p=(int*)(base + LAT_OFFSET + CLR_OFFSET);
    *p=bits;
}

//Inverts IO pin state
//@param base Port registers base address (e.g. PORTA_BASE)
//@param bits Bits definition (e.g. BIT0, or BIT5 | BIT8 | BIT11)
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

//Set IO pin to log. 1
//@param pi PIN_INFO structure
void setPin(PIN_INFO* pi)
{
    volatile int* p=(int*)(pi->portBase + LAT_OFFSET + SET_OFFSET);
    *p=pi->pin;
}

//Clear IO pin to log. 0
//@param pi PIN_INFO structure
void clearPin(PIN_INFO* pi)
{
    volatile int* p=(int*)(pi->portBase + LAT_OFFSET + CLR_OFFSET);
    *p=pi->pin;
}

//Inverts IO pin state
//@param pi PIN_INFO structure
void invPin(PIN_INFO* pi)
{
    volatile int* p=(int*)(pi->portBase + LAT_OFFSET + INV_OFFSET);
    *p=pi->pin;
}

//Returns zero, when pin is low, or non zero, when pin is high
//@param pi PIN_INFO structure
short getPin(PIN_INFO* pi)
{
    //uint a=pi->portBase + 0x0;
    volatile short* p=(short*)(pi->portBase + PORT_OFFSET);
    short ret = *p;
    return ret & pi->pin;
}


//test LED
/*
void setTestLed(int num)             //test led
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
        p=(LED2_BASE + LAT_OFFSET + SET_OFFSET);
        *p=LED2_BIT;
    }
    else if(num==3)
    {
        p=(LED3_BASE + LAT_OFFSET + SET_OFFSET);
        *p=LED3_BIT;
    }
    else if(num==4)
    {
        p=(int*)(LED4_BASE + LAT_OFFSET + SET_OFFSET);
        *p=LED4_BIT;
    }    
}
*/

/*
void clearTestLed(int num)             //test led
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
*/

/*
void invTestLed(int num)             //test led
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
*/

void signalizeError(PIN_INFO* pi, int code)
{
    //num je test LED 1-4
    //code je kod, ktery se zobrazuje (definuje pocet bliknuti)
    
    while(1)
    {
        int x=code;
        
        while(x > 0)
        {
            setPin(pi);
            //setTestLed(num);
            pauseEvents(60);
        
            clearPin(pi);
            //clearTestLed(num);
            pauseEvents(400);
            
            x--;
        }
        
        pauseEvents(2000);
    }
}