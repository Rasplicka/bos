#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka 
 * Copyright (c) 2017, All rights reserved.      
 * spi driver, used by BOS
 * This file is shared for the PIC32MM, PIC32MZ
 * Last update 19/3/2018
 * Ver. 1.0 
*/

#if (defined SPI1_USE || defined SPI2_USE || defined SPI3_USE || defined SPI4_USE || defined SPI5_USE || defined SPI6_USE)

//spi driver (stary popis)
//SPI - Master mode
//SPI muze vyuzivat vice modulu, je-li na jednom SPI kanalu vice zarizeni, musi pouzivat CS signal
//fce spiExchange provede zapis dat SPI (nebo zapis i cteni, je-li zadany Rx buffer)
//data jsou v tx_bufferu, delka len
//volajici musi pockat na used=0
//napr:
//while(spiGetUsed(0)==1){ doEvents(); }
//spi_setUsed(0, 1)
//setCSpin(active)
//setDCPin(DATA)
//spiExchange(0, ... , &sendFinish)
//return

//void sendFinish() {
//setCSPin(deactive)
//spi_setUsed(0, 0) }


#define     SPI_INTERRUPT_PRIORITY      3
#define     SPI_INTERRUPT_SUBPRIORITY	0
#define     SPI_FIFO_SIZE               16     //fifo size [bytes]

//puvodni hodnota: 0x18130
//REFCLKO 48MHz, Enhanced buffer, ON, SIDL=0 (run in IDLE), SDO enable, 8-bit, Master mode, SDI enable, interrupt buffer empty (Tx), buffer full (Rx)
#define     SPI1_CONFIG                 0x818120;
#define     SPI2_CONFIG                 0x818120;
#define     SPI3_CONFIG                 0x818120;
#define     SPI4_CONFIG                 0x818120;
#define     SPI5_CONFIG                 0x818120;
#define     SPI6_CONFIG                 0x818120;

#ifdef PIC32MM0064
//SPI1-2
#define     SPI1_DISABLE_TxINTERRUPT    IEC0CLR=(1<<21) //0x00200000
#define     SPI1_ENABLE_TxINTERRUPT     IEC0SET=(1<<21) //0x00200000
#define     SPI2_DISABLE_TxINTERRUPT    IEC1CLR=(1<<6)  //0x00000040
#define     SPI2_ENABLE_TxINTERRUPT     IEC1SET=(1<<6)  //0x00000040
#endif

#ifdef PIC32MM0256
//SPI1-3
#define     SPI1_DISABLE_TxINTERRUPT    IEC1CLR=(1<<10) //0x00000400
#define     SPI1_ENABLE_TxINTERRUPT     IEC1SET=(1<<10) //0x00000400
#define     SPI2_DISABLE_TxINTERRUPT    IEC1CLR=(1<<13) //0x00002000
#define     SPI2_ENABLE_TxINTERRUPT     IEC1SET=(1<<13) //0x00002000
#define     SPI3_DISABLE_TxINTERRUPT    IEC1CLR=(1<<16) //0x00010000
#define     SPI3_ENABLE_TxINTERRUPT     IEC1SET=(1<<16) //0x00010000
#endif

#ifdef PIC32MZ
//SPI1-6
#define     SPI1_DISABLE_TxINTERRUPT    IEC3CLR=(1<<15) //0x00000400
#define     SPI1_ENABLE_TxINTERRUPT     IEC3SET=(1<<15) //0x00000400
#define     SPI2_DISABLE_TxINTERRUPT    IEC4CLR=(1<<16) //0x00002000
#define     SPI2_ENABLE_TxINTERRUPT     IEC4SET=(1<<16) //0x00002000
#define     SPI3_DISABLE_TxINTERRUPT    IEC4CLR=(1<<28) //0x00010000
#define     SPI3_ENABLE_TxINTERRUPT     IEC4SET=(1<<28) //0x00010000
#define     SPI4_DISABLE_TxINTERRUPT    IEC5CLR=(1<<5)  //0x00010000
#define     SPI4_ENABLE_TxINTERRUPT     IEC5SET=(1<<5)  //0x00010000
#define     SPI5_DISABLE_TxINTERRUPT    IEC5CLR=(1<<18) //0x00010000
#define     SPI5_ENABLE_TxINTERRUPT     IEC5SET=(1<<18) //0x00010000
#define     SPI6_DISABLE_TxINTERRUPT    IEC5CLR=(1<<27) //0x00010000
#define     SPI6_ENABLE_TxINTERRUPT     IEC5SET=(1<<27) //0x00010000
#endif

//local var
void (*_finish)(int);

#ifdef PIC32MM0064
SPIControl s1={ NULL, NULL, NULL, NULL, &SPI1BUF, 0, 0, 0, 0, 0, 0, 0 };
SPIControl s2={ NULL, NULL, NULL, NULL, &SPI2BUF, 0, 0, 0, 0, 0, 0, 0 };   
SPIControl* controlStruct[]={&s1, &s2};
#endif

#ifdef PIC32MM0256
SPIControl s1={ NULL, NULL, NULL, NULL, &SPI1BUF, 0, 0, 0, 0, 0, 0, 0 };
SPIControl s2={ NULL, NULL, NULL, NULL, &SPI2BUF, 0, 0, 0, 0, 0, 0, 0 };   
SPIControl s3={ NULL, NULL, NULL, NULL, &SPI3BUF, 0, 0, 0, 0, 0, 0, 0 }; 
SPIControl* controlStruct[]={&s1, &s2, &s3};
#endif

#ifdef PIC32MZ
SPIControl s1={ NULL, NULL, NULL, NULL, &SPI1BUF, 0, 0, 0, 0, 0, 0, 0 };
SPIControl s2={ NULL, NULL, NULL, NULL, &SPI2BUF, 0, 0, 0, 0, 0, 0, 0 };   
SPIControl s3={ NULL, NULL, NULL, NULL, &SPI3BUF, 0, 0, 0, 0, 0, 0, 0 };
SPIControl s4={ NULL, NULL, NULL, NULL, &SPI4BUF, 0, 0, 0, 0, 0, 0, 0 };
SPIControl s5={ NULL, NULL, NULL, NULL, &SPI5BUF, 0, 0, 0, 0, 0, 0, 0 };
SPIControl s6={ NULL, NULL, NULL, NULL, &SPI6BUF, 0, 0, 0, 0, 0, 0, 0 };
SPIControl* controlStruct[]={&s1, &s2, &s3, &s4, &s5, &s6};
#endif

//global void
void spiInit();
char spiGetUsed(int index);
void spiUse(char index, void* finish, void* event);
void spiFree(int index);
char spiGetActivity(char index);
void spiWaitToFinish(char index);

void spiExchangeDE(int index, char* txbuff, char* rxbuff, int l);
void spiExchange(int index, char* txbuff, char* rxbuff, int l);
void spiExchangeModeDE(int index, char* txbuff, char* rxbuff, int l, char mode);
void spiExchangeMode(int index, char* txbuff, char* rxbuff, int l, char mode);
volatile int* spiGetHwBuffer(char index);
void spiSetBusMode(char index, char mode);
void spiSetSpeed(int index, int speed);
void spiTxInterrupt(char index);

//local void
static void clearRxFifo(int index);
static void disableInterrupt(char index);
static void enableInterrupt(char index);

//Initialize module
void spiInit()
{
    
#ifdef SPI1_USE
    
    SPI1CON = 0;                    // Stops and resets the SPI1.
    clearRxFifo(0);
    
    #if defined PIC32MM0064  
        // disable all interrupts IEC0.20 - SPI1Error, 21 - SPI1 TRN, 22 - SPI1 REC 
        IEC0CLR=0x00700000;
        // clear any existing event
        IFS0CLR=0x00700000;
    
        //SPI1 Tx, Rx, Err, Prior:1, SubPrior:0
        IPC5bits.SPI1TXIP = SPI_INTERRUPT_PRIORITY;                  //Tx prior
        IPC5bits.SPI1TXIS = SPI_INTERRUPT_SUBPRIORITY;                  //Tx subprior
        IPC5bits.SPI1EIP = SPI_INTERRUPT_PRIORITY;                   //Err prior
        IPC5bits.SPI1EIS = SPI_INTERRUPT_SUBPRIORITY;                   //Err subprior
        IPC5bits.SPI1RXIP = SPI_INTERRUPT_PRIORITY;                  //Rx prior
        IPC5bits.SPI1RXIS = SPI_INTERRUPT_SUBPRIORITY;                  //Rx subprior
   
    #elif defined PIC32MM0256   
        IEC1bits.SPI1EIE=0;
        IEC1bits.SPI1RXIE=0;
        IEC1bits.SPI1TXIE=0;
    
        IFS1bits.SPI1EIF=0;
        IFS1bits.SPI1RXIF=0;
        IFS1bits.SPI1TXIF=0;
    
        IPC10bits.SPI1TXIP = SPI_INTERRUPT_PRIORITY;
        IPC10bits.SPI1TXIS = SPI_INTERRUPT_SUBPRIORITY;
        IPC10bits.SPI1EIP = SPI_INTERRUPT_PRIORITY;
        IPC10bits.SPI1EIS = SPI_INTERRUPT_SUBPRIORITY;
        IPC10bits.SPI1RXIP = SPI_INTERRUPT_PRIORITY;
        IPC10bits.SPI1RXIS = SPI_INTERRUPT_SUBPRIORITY;
        
    #elif defined PIC32MZ
        IEC3bits.SPI1EIE=0;
        IEC3bits.SPI1RXIE=0;
        IEC3bits.SPI1TXIE=0;
        
        IFS3bits.SPI1EIF=0;
        IFS3bits.SPI1RXIF=0;
        IFS3bits.SPI1TXIF=0;
        
        OFF111=&iVector_spi1Tx - EBASE;
        IPC27bits.SPI1EIP=SPI_INTERRUPT_PRIORITY;
        IPC27bits.SPI1EIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC27bits.SPI1RXIP=SPI_INTERRUPT_PRIORITY;
        IPC27bits.SPI1RXIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC27bits.SPI1TXIP=SPI_INTERRUPT_PRIORITY;
        IPC27bits.SPI1TXIS=SPI_INTERRUPT_SUBPRIORITY;   
        
    #endif
    
    SPI1STAT = 0x0;
    SPI1BRG = 0x0;          //48MHz
    SPI1CON2 = 0x0;
    SPI1CON = SPI1_CONFIG;  
    controlStruct[0]->state=MODULE_STATE.On;    
    
#endif
    
#ifdef SPI2_USE

    SPI2CON = 0;                    // Stops and resets the SPI2.
    clearRxFifo(1);
    
    #if defined PIC32MM0064  
    
        // disable all interrupts IEC0.20 - SPI1Error, 21 - SPI1 TRN, 22 - SPI1 REC 
        IEC1CLR=0x000000E0;
        // clear any existing event
        IFS1CLR=0x000000E0;
    
        //SPI1 Tx, Rx, Err, Prior:1, SubPrior:0
        IPC9bits.SPI2TXIP = SPI_INTERRUPT_PRIORITY;                  //Tx prior
        IPC9bits.SPI2TXIS = SPI_INTERRUPT_SUBPRIORITY;                  //Tx subprior
        IPC9bits.SPI2EIP = SPI_INTERRUPT_PRIORITY;                   //Err prior
        IPC9bits.SPI2EIS = SPI_INTERRUPT_SUBPRIORITY;                   //Err subprior
        IPC9bits.SPI2RXIP = SPI_INTERRUPT_PRIORITY;                  //Rx prior
        IPC9bits.SPI2RXIS = SPI_INTERRUPT_SUBPRIORITY;                  //Rx subprior
   
    #elif  defined PIC32MM0256
        IEC1bits.SPI2EIE=0;
        IEC1bits.SPI2RXIE=0;
        IEC1bits.SPI2TXIE=0;
    
        IFS1bits.SPI2EIF=0;
        IFS1bits.SPI2RXIF=0;
        IFS1bits.SPI2TXIF=0;
    
        IPC11bits.SPI2TXIP = SPI_INTERRUPT_PRIORITY;
        IPC11bits.SPI2TXIS = SPI_INTERRUPT_SUBPRIORITY;
        IPC11bits.SPI2EIP = SPI_INTERRUPT_PRIORITY;
        IPC11bits.SPI2EIS = SPI_INTERRUPT_SUBPRIORITY;
        IPC11bits.SPI2RXIP = SPI_INTERRUPT_PRIORITY;
        IPC11bits.SPI2RXIS = SPI_INTERRUPT_SUBPRIORITY;
    
    #elif defined PIC32MZ
        IEC4bits.SPI2EIE=0;
        IEC4bits.SPI2RXIE=0;
        IEC4bits.SPI2TXIE=0;
        
        IFS4bits.SPI2EIF=0;
        IFS4bits.SPI2RXIF=0;
        IFS4bits.SPI2TXIF=0;
        
        OFF144=&iVector_spi2Tx - EBASE;
        IPC35bits.SPI2EIP=SPI_INTERRUPT_PRIORITY;
        IPC35bits.SPI2EIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC35bits.SPI2RXIP=SPI_INTERRUPT_PRIORITY;
        IPC35bits.SPI2RXIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC36bits.SPI2TXIP=SPI_INTERRUPT_PRIORITY;
        IPC36bits.SPI2TXIS=SPI_INTERRUPT_SUBPRIORITY;   
        
    #endif  
    
    SPI2STAT = 0x0;
    SPI2BRG = 0x0;          //48MHz
    SPI2CON2 = 0x0;
    SPI2CON = SPI2_CONFIG;
    controlStruct[1]->state=MODULE_STATE.On;
    
#endif    
    
#ifdef SPI3_USE

    SPI3CON = 0;                    // Stops and resets the SPI2.
    clearRxFifo(2);
    
    #if defined PIC32MM0256  
        IEC1bits.SPI3EIE=0;
        IEC1bits.SPI3RXIE=0;
        IEC1bits.SPI3TXIE=0;
    
        IFS1bits.SPI3EIF=0;
        IFS1bits.SPI3RXIF=0;
        IFS1bits.SPI3TXIF=0;
    
        IPC12bits.SPI3TXIP = SPI_INTERRUPT_PRIORITY;
        IPC12bits.SPI3TXIS = SPI_INTERRUPT_SUBPRIORITY;
        IPC11bits.SPI3EIP = SPI_INTERRUPT_PRIORITY;
        IPC11bits.SPI3EIS = SPI_INTERRUPT_SUBPRIORITY;
        IPC12bits.SPI3RXIP = SPI_INTERRUPT_PRIORITY;
        IPC12bits.SPI3RXIS = SPI_INTERRUPT_SUBPRIORITY;
        
    #elif defined PIC32MZ
        IEC4bits.SPI3EIE=0;
        IEC4bits.SPI3RXIE=0;
        IEC4bits.SPI3TXIE=0;
        
        IFS4bits.SPI3EIF=0;
        IFS4bits.SPI3RXIF=0;
        IFS4bits.SPI3TXIF=0;
        
        OFF156=&iVector_spi3Tx - EBASE;
        IPC38bits.SPI3EIP=SPI_INTERRUPT_PRIORITY;
        IPC38bits.SPI3EIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC38bits.SPI3RXIP=SPI_INTERRUPT_PRIORITY;
        IPC38bits.SPI3RXIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC39bits.SPI3TXIP=SPI_INTERRUPT_PRIORITY;
        IPC39bits.SPI3TXIS=SPI_INTERRUPT_SUBPRIORITY;   

    #endif        
    
    SPI3STAT = 0x0;
    SPI3BRG = 0x0;          //48MHz
    SPI3CON2 = 0x0;
    SPI3CON = SPI3_CONFIG;
    controlStruct[2]->state=MODULE_STATE.On;    
    
#endif        
  
#ifdef SPI4_USE

    SPI4CON = 0;                    // Stops and resets the SPI2.
    clearRxFifo(3);
    
    #if defined PIC32MZ
        IEC5bits.SPI4EIE=0;
        IEC5bits.SPI4RXIE=0;
        IEC5bits.SPI4TXIE=0;
        
        IFS5bits.SPI4EIF=0;
        IFS5bits.SPI4RXIF=0;
        IFS5bits.SPI4TXIF=0;
        
        OFF165=&iVector_spi4Tx - EBASE;
        IPC40bits.SPI4EIP=SPI_INTERRUPT_PRIORITY;
        IPC40bits.SPI4EIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC41bits.SPI4RXIP=SPI_INTERRUPT_PRIORITY;
        IPC41bits.SPI4RXIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC41bits.SPI4TXIP=SPI_INTERRUPT_PRIORITY;
        IPC41bits.SPI4TXIS=SPI_INTERRUPT_SUBPRIORITY;   

    #endif        
    
    SPI4STAT = 0x0;
    SPI4BRG = 0x0;          //48MHz
    SPI4CON2 = 0x0;
    SPI4CON = SPI4_CONFIG;
    controlStruct[3]->state=MODULE_STATE.On;    
    
#endif 
   
#ifdef SPI5_USE

    SPI5CON = 0;                    // Stops and resets the SPI2.
    clearRxFifo(4);
    
    #if defined PIC32MZ
        IEC5bits.SPI5EIE=0;
        IEC5bits.SPI5RXIE=0;
        IEC5bits.SPI5TXIE=0;
        
        IFS5bits.SPI5EIF=0;
        IFS5bits.SPI5RXIF=0;
        IFS5bits.SPI5TXIF=0;
        
        OFF178=&iVector_spi5Tx - EBASE;
        IPC44bits.SPI5EIP=SPI_INTERRUPT_PRIORITY;
        IPC44bits.SPI5EIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC44bits.SPI5RXIP=SPI_INTERRUPT_PRIORITY;
        IPC44bits.SPI5RXIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC44bits.SPI5TXIP=SPI_INTERRUPT_PRIORITY;
        IPC44bits.SPI5TXIS=SPI_INTERRUPT_SUBPRIORITY;   

    #endif        
    
    SPI5STAT = 0x0;
    SPI5BRG = 0x0;          //48MHz
    SPI5CON2 = 0x0;
    SPI5CON = SPI5_CONFIG;
    controlStruct[4]->state=MODULE_STATE.On;    
    
#endif 
   
#ifdef SPI6_USE

    SPI6CON = 0;                    // Stops and resets the SPI2.
    clearRxFifo(5);
    
    #if defined PIC32MZ
        IEC5bits.SPI6IE=0;
        IEC5bits.SPI6RXIE=0;
        IEC5bits.SPI6TXIE=0;
        
        IFS5bits.SPI6IF=0;
        IFS5bits.SPI6RXIF=0;
        IFS5bits.SPI6TX=0;
        
        OFF187=&iVector_spi6Tx - EBASE;
        IPC46bits.SPI6EIP=SPI_INTERRUPT_PRIORITY;
        IPC46bits.SPI6EIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC46bits.SPI6RXIP=SPI_INTERRUPT_PRIORITY;
        IPC46bits.SPI6RXIS=SPI_INTERRUPT_SUBPRIORITY;
        IPC46bits.SPI6TXIP=SPI_INTERRUPT_PRIORITY;
        IPC46bits.SPI6TXIS=SPI_INTERRUPT_SUBPRIORITY;   

    #endif        
    
    SPI6STAT = 0x0;
    SPI6BRG = 0x0;          //48MHz
    SPI6CON2 = 0x0;
    SPI6CON = SPI6_CONFIG;
    controlStruct[5]->state=MODULE_STATE.On;    
    
#endif     
    
}

//Returns if the module is used. Returned value is MODULE_USED.No/Yes
char spiGetUsed(int index)
{
    //used nastavuje volajici proces, muze nastavit jakoukoliv hodnotu > -1(SPI_EMPTY)
    //tato hodnota bude pouzita po skonceni prenosu pri volani _finish, volajici proces
    //podle ni muze zjistit, jaky prenos byl ukoncen
    return controlStruct[index]->used;
}

//If the module is used, wait for release. While waiting, calls doEvents()
//Set the module as used.
//@param index Module index
//@param finish Address of finish function
//@param event Address of event function
void spiUse(char index, void* finish, void* event)
{
    //je-li spi volne, nastavi MODULE_USED.Yes a vraci 1 (true)
    //neni-li volne 
    //a) pri wait==1 ceka (doEvents) na uvolneni
    //b) pri wait==0 vraci 0 (obsazeno jinym procesem)
    
    while(1)
    {
        if(controlStruct[index]->used == MODULE_USED.No)
        {
            controlStruct[index]->used=MODULE_USED.Yes;
            controlStruct[index]->finishFn=finish;
            controlStruct[index]->eventFn=event;
            return;
        }

        doEvents();
    }
}

//waiting for the last operation to finish, then release module
//@param index Module index
void spiFree(int index)
{
    //pokud jeste probiha vysilani dat
    //ceka na dokonceni, protoze po spiFree se vetsinou vola setCS>1
    //takze by data nebyla prijata 
    
    while(controlStruct[index]->activity == MODULE_ACTIVITY.Works)
    {
        //jeste probiha vysilani
        doEvents();
    }
    controlStruct[index]->used=MODULE_USED.No;
}

//Returns if the last operation is still in progress
////@param index Module index
char spiGetActivity(char index)
{
    return controlStruct[index]->activity; 
}

//Wait for the operation to finish. While waiting, calls doEvents()
//@param index Module index 
void spiWaitToFinish(char index)
{
    while(controlStruct[index]->activity == MODULE_ACTIVITY.Works)
    {
        //vysilani neni dokonceno
        doEvents();
    }
}

//Starts the data transmission. Before starting the operation is waiting for the previous one to finish (call doEvents)
//@param index Module index
//@param txbuffer Tx buffer
//@param rxbuffer Rx buffer or NULL
//@param len Tx buffer size
void spiExchangeDE(int index, char* txbuff, char* rxbuff, int len)
{
    //pokud probiha vysilani, vola doEvents, ceka na dokonceni
    while(controlStruct[index]->activity == MODULE_ACTIVITY.Works)
    {
        doEvents();
    }
    
    spiExchange(index, txbuff, rxbuff, len);    
}

//Starts the data transmission now
//@param index Module index
//@param txbuffer Tx buffer
//@param rxbuffer Rx buffer or NULL
//@param len Tx buffer size
void spiExchange(int index, char* txbuff, char* rxbuff, int len)
{
    //je-li rxbuff=NULL, probiha pouze vysilani, jinak vysilani i prijem
    //void f se vola po ukonceni trasnakce
    
    controlStruct[index]->tx_buffer=txbuff;
    controlStruct[index]->rx_buffer=rxbuff;
    controlStruct[index]->len=len;
    controlStruct[index]->tx_count=0;
    controlStruct[index]->activity=MODULE_ACTIVITY.Works;
    
    if(controlStruct[index]->rx_buffer != NULL)
    {
        //vyjme vsechny data z spi_rx_bufferu (a nuluje overflow flag)
        clearRxFifo(index);
    }
    
    spiTxInterrupt(index);
}

//Starts the data or stream transmission. Before starting the operation is waiting for the previous one to finish (call doEvents)
//@param index Module index
//@param txbuffer Tx buffer
//@param rxbuffer Rx buffer or NULL
//@param len Tx buffer size
//@param mode WRITE_MODE.Stream or WRITE_MODE.DataOnly
void spiExchangeModeDE(int index, char* txbuff, char* rxbuff, int len, char mode)
{
    //pokud probiha vysilani, vola doEvents, ceka na dokonceni
    while(controlStruct[index]->activity == MODULE_ACTIVITY.Works)
    {
        doEvents();
    }
    
    spiExchangeMode(index, txbuff, rxbuff, len, mode);
}

//Starts the data or stream transmission now
//@param index Module index
//@param txbuffer Tx buffer
//@param rxbuffer Rx buffer or NULL
//@param len Tx buffer size
//@param mode WRITE_MODE.Stream or WRITE_MODE.DataOnly
void spiExchangeMode(int index, char* txbuff, char* rxbuff, int len, char mode)
{
    //je-li rxbuff=NULL, probiha pouze vysilani, jinak vysilani i prijem
    controlStruct[index]->tx_buffer=txbuff;
    controlStruct[index]->rx_buffer=rxbuff;
    controlStruct[index]->len=len;
    controlStruct[index]->tx_count=0;
    controlStruct[index]->mode_count=0;
    controlStruct[index]->mode=mode;
    controlStruct[index]->activity=MODULE_ACTIVITY.Works;
    
    if(controlStruct[index]->rx_buffer != NULL)
    {
        //vyjme vsechny data z spi_rx_bufferu
        clearRxFifo(index);
    }
    
    spiTxInterrupt(index);
}

//Returns address of SPIxBUF register (access to FIFO)
//@param index Module index
volatile int* spiGetHwBuffer(char index)
{
    return controlStruct[index]->hw_buffer;
}

//BUS mode. Changes SPIxCONbits.MODE16 and SPIxCONbits.MODE32
//@param index Module index
//@param mode BUS_MODE._8bit, BUS_MODE._16bit, BUS_MODE._32bit
void spiSetBusMode(char index, char mode)
{
    if(index==0)
    {
#ifdef SPI1_USE       
        if (mode==BUS_MODE._16bit) { SPI1CONbits.MODE16=1; SPI1CONbits.MODE32=0;}
        else if (mode==BUS_MODE._32bit) { SPI1CONbits.MODE16=0; SPI1CONbits.MODE32=1;}
        else { SPI1CONbits.MODE16=0; SPI1CONbits.MODE32=0;}
#endif        
    }
    
    else if(index==1)
    {
#ifdef SPI2_USE         
        if (mode==BUS_MODE._16bit) { SPI2CONbits.MODE16=1; SPI2CONbits.MODE32=0;}
        else if (mode==BUS_MODE._32bit) { SPI2CONbits.MODE16=0; SPI2CONbits.MODE32=1;}
        else { SPI2CONbits.MODE16=0; SPI2CONbits.MODE32=0;}
#endif        
    }
    
    else if(index==2)
    {
#ifdef SPI3_USE         
        if (mode==BUS_MODE._16bit) { SPI3CONbits.MODE16=1; SPI3CONbits.MODE32=0;}
        else if (mode==BUS_MODE._32bit) { SPI3CONbits.MODE16=0; SPI3CONbits.MODE32=1;}
        else { SPI3CONbits.MODE16=0; SPI3CONbits.MODE32=0;}
#endif        
    } 

    else if(index==3)
    {
#ifdef SPI4_USE          
        if (mode==BUS_MODE._16bit) { SPI4CONbits.MODE16=1; SPI4CONbits.MODE32=0;}
        else if (mode==BUS_MODE._32bit) { SPI4CONbits.MODE16=0; SPI4CONbits.MODE32=1;}
        else { SPI4CONbits.MODE16=0; SPI4CONbits.MODE32=0;}
#endif         
    }     
    else if(index==4)
    {
#ifdef SPI5_USE          
        if (mode==BUS_MODE._16bit) { SPI5CONbits.MODE16=1; SPI5CONbits.MODE32=0;}
        else if (mode==BUS_MODE._32bit) { SPI5CONbits.MODE16=0; SPI5CONbits.MODE32=1;}
        else { SPI5CONbits.MODE16=0; SPI5CONbits.MODE32=0;}
#endif         
    }      
    else
    {
#ifdef SPI6_USE          
        if (mode==BUS_MODE._16bit) { SPI6CONbits.MODE16=1; SPI6CONbits.MODE32=0;}
        else if (mode==BUS_MODE._32bit) { SPI6CONbits.MODE16=0; SPI6CONbits.MODE32=1;}
        else { SPI6CONbits.MODE16=0; SPI6CONbits.MODE32=0;}
#endif         
    }      
    
}

//Change SPIxBRG divider. Divides the base frequency 48MHz
//@param index Module index
//@param option 0=48MHz, 1=24MHz, 2=12MHz, ...
void spiSetSpeed(int index, int option)
{
    int x;
    if(index==0)
    {
#ifdef SPI1_USE        
        x=SPI1CON;                      //zaloha SPIxCON
        SPI1CON=0x0;                    //SPIxCON=0, SPI OFF
        SPI1BRG=option;                 //nastav BRG (divider)
        SPI1CON=x;                      //SPIxCON puvodni hodnota        
#endif        
    }
    else if(index==1)
    {
#ifdef SPI2_USE         
        x=SPI2CON;                      //zaloha SPIxCON
        SPI2CON=0x0;                    //SPIxCON=0, SPI OFF
        SPI2BRG=option;                 //nastav BRG (divider)
        SPI2CON=x;                      //SPIxCON puvodni hodnota
#endif        
    }
    else if (index==2)
    {
#ifdef SPI3_USE         
        x=SPI3CON;                      //zaloha SPIxCON
        SPI3CON=0x0;                    //SPIxCON=0, SPI OFF
        SPI3BRG=option;                 //nastav BRG (divider)
        SPI3CON=x;                      //SPIxCON puvodni hodnota        
#endif        
    }
    else if (index==3)
    {
#ifdef SPI4_USE         
        x=SPI4CON;                      //zaloha SPIxCON
        SPI4CON=0x0;                    //SPIxCON=0, SPI OFF
        SPI4BRG=option;                 //nastav BRG (divider)
        SPI4CON=x;                      //SPIxCON puvodni hodnota        
#endif      
    }
    else if (index==4)
    {
#ifdef SPI5_USE         
        x=SPI5CON;                      //zaloha SPIxCON
        SPI5CON=0x0;                    //SPIxCON=0, SPI OFF
        SPI5BRG=option;                 //nastav BRG (divider)
        SPI5CON=x;                      //SPIxCON puvodni hodnota        
#endif     
    }
    else
    {
#ifdef SPI6_USE         
        x=SPI6CON;                      //zaloha SPIxCON
        SPI6CON=0x0;                    //SPIxCON=0, SPI OFF
        SPI6BRG=option;                 //nastav BRG (divider)
        SPI6CON=x;                      //SPIxCON puvodni hodnota        
#endif            
    }
}


//shared SPI Tx interrupt handler
void spiTxInterrupt(char index)
{
    //vola se z spiExchange a z SPI interruptu
    //interrupt pri dokonceni vysilani (nikoliv tx_fifo empty)
    
    //ctl->mode=1
    //[0] 0b00 000001    //nasleduje 1 byte + vola EventFn(0b00000001)
    //[1] 0xXX           //data byte
    //[2] 0b01 000100    //nasleduji 4 byte + vola EventFn(0b01000100)
    //[3-6] 4xdata
    //len=7
    //pokud ctl->mode_count=0, jedna se o command byte 
    //command byte definuje pocet bytes dat, ktere nasleduji (ty se vysilaji). Zaroven se CB pouzije jako param pri volani Event fce
    //po odvysilani daneho poctu dat nasleduje dalsi CB, dokud (ctl->len > 0)
    
    SPIControl* ctl = controlStruct[index];
    volatile int* _SPIxBUF = ctl->hw_buffer;
    
    //load rx_fifo, pokud existuje
    while(ctl->tx_count > 0)
    {
        //tx_count je pocet bytes posledniho vysilani
        //predpoklada, ze v rx_fifo je stejny pocet bytes, jaky byl minule odvysilan
        *ctl->rx_buffer=*_SPIxBUF; //SPI2BUF;
        ctl->rx_buffer++;
        ctl->tx_count--;
    }
    
    if(ctl->len == 0)
    {
        //konec, vsechna data byla odeslana
        //SPI2_DISABLE_TxINTERRUPT;
        disableInterrupt(index);
        
        if(ctl->finishFn != NULL)
        {
            _finish=ctl->finishFn;
            _finish(ctl->used);
        }
        
        ctl->activity=MODULE_ACTIVITY.Finished;
    }
    else
    {
        if((ctl->mode==1) && (ctl->mode_count==0))
        {
            //control byte
            char x=*ctl->tx_buffer;
            ctl->mode_count=(x & 0x3F);                   //nuluje b7,b6 (b0-b5 = pocet bytes)
            _finish=ctl->eventFn;
            _finish(x);
            
            if((x>>6)==0b11)
            {
                //pokud command byte b6-b7 = 11, nastavi mode=0 (pouze odesila zbytek bufferu v modu 0)
                ctl->mode=0;
            }
                
            ctl->len--;
            ctl->tx_buffer++; 
        }
        
        int c=0;
        if(ctl->mode==0)
        {
            //odeslat dalsi data (viz. len), max SPI_HW_BUFFER_SIZE bytes
            while((ctl->len > 0) && (c<SPI_FIFO_SIZE))
            {
                c++;
                ctl->len--;                                                     //pocet --
                if(ctl->rx_buffer != NULL) { ctl->tx_count ++;}                 //priste bude probihat read fifo
                //tx fifo
                //SPI2BUF=*ctl->tx_buffer;
                *_SPIxBUF=*ctl->tx_buffer;
                ctl->tx_buffer++;                                               //dalsi znak
            }
        }
        else
        {
            //odeslat dalsi data (viz. mode_count), max SPI_HW_BUFFER_SIZE bytes
            while((ctl->mode_count > 0) && (ctl->len > 0) && (c < SPI_FIFO_SIZE))
            {
                c++;
                ctl->mode_count--;
                ctl->len--;                                                     //pocet --
                if(ctl->rx_buffer != NULL) { ctl->tx_count ++;}                 //priste bude probihat read fifo
                //tx fifo
                //SPI2BUF=*ctl->tx_buffer;
                *_SPIxBUF=*ctl->tx_buffer;
                ctl->tx_buffer++;                                               //dalsi znak
            }                
        }
        
        //povolit TX buffer empty interrupt
        enableInterrupt(index);
    }
        
}

//Clears the Rx FIFO
//@param index Module index 
static void clearRxFifo(int index)
{
#ifdef SIMULATOR
    return;
#endif    
    
    //vyprazdni rx_fifo
    int x;
    if(index==0)
    {
#ifdef SPI1_USE        
        while(SPI1STATbits.SPIRBE==0)
        {
            x=SPI1BUF;
        }
        //nuluje overflow flag (pokud je nastaven, neprijima data)
        SPI1STATbits.SPIROV=0;
#endif         
    }
    else if (index==1) 
    {
#ifdef SPI2_USE          
        while(SPI2STATbits.SPIRBE==0)
        {
            x=SPI2BUF;
        }
        //nuluje overflow flag (pokud je nastaven, neprijima data)
        SPI2STATbits.SPIROV=0;
#endif          
    }
    else if (index==2) 
    {
#ifdef SPI3_USE          
        while(SPI3STATbits.SPIRBE==0)
        {
            x=SPI3BUF;
        }
        //nuluje overflow flag (pokud je nastaven, neprijima data)
        SPI3STATbits.SPIROV=0;
#endif          
    }    
    else if (index==3) 
    {
#ifdef SPI4_USE          
        while(SPI4STATbits.SPIRBE==0)
        {
            x=SPI4BUF;
        }
        //nuluje overflow flag (pokud je nastaven, neprijima data)
        SPI4STATbits.SPIROV=0;
#endif          
    }    
    else if (index==4) 
    {
#ifdef SPI5_USE          
        while(SPI5STATbits.SPIRBE==0)
        {
            x=SPI5BUF;
        }
        //nuluje overflow flag (pokud je nastaven, neprijima data)
        SPI5STATbits.SPIROV=0;
#endif          
    }    
    else if (index==5) 
    {
#ifdef SPI6_USE          
        while(SPI6STATbits.SPIRBE==0)
        {
            x=SPI6BUF;
        }
        //nuluje overflow flag (pokud je nastaven, neprijima data)
        SPI6STATbits.SPIROV=0;
#endif          
    }    
}

static void disableInterrupt(char index)
{
    if(index==0) { SPI1_DISABLE_TxINTERRUPT; }
    else if (index==1){ SPI2_DISABLE_TxINTERRUPT; }
    
#if defined PIC32MM0256 || defined PIC32MZ    
    else if (index==2){ SPI3_DISABLE_TxINTERRUPT; }
#endif

#if defined PIC32MZ
    else if (index==3){ SPI4_DISABLE_TxINTERRUPT; }
    else if (index==4){ SPI5_DISABLE_TxINTERRUPT; }
    else { SPI6_DISABLE_TxINTERRUPT; }
#endif    
}

static void enableInterrupt(char index)
{
    if(index==0) { SPI1_ENABLE_TxINTERRUPT; }
    else if (index==1){ SPI2_ENABLE_TxINTERRUPT; }
    
#if defined PIC32MM0256 || defined PIC32MZ    
    else if (index==2){ SPI3_ENABLE_TxINTERRUPT; }
#endif

#if defined PIC32MZ
    else if (index==3){ SPI4_ENABLE_TxINTERRUPT; }
    else if (index==4){ SPI5_ENABLE_TxINTERRUPT; }
    else { SPI6_ENABLE_TxINTERRUPT; }
#endif    
}

#endif //(ifdef SPI1_USE, SPI2_USE, SPI3_USE, ...)