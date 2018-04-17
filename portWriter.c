#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "asm.h"
//#include "graphics.h"

static void spiWriteBuffer(void* info, char* buffer, short len);
static void spiWriteBufferMode(void* info, char* buffer, short len, char mode);
static void spiGetPort(void* info);
static void spiFreePort(void* info);
//static void spiSetBusMode(void* info, char mode);

static void i2cWriteBuffer(void* info, char* buffer, short len);
static void i2cGetPort(void* info);
static void i2cFreePort(void* info);

void portWriter_init(PORT_INFO* info, char periph_type, char index)
{
    info->periphType=periph_type;
    info->portIndex=index;
    if(periph_type==PERIPH_TYPE.spi)
    {
        
#if (defined SPI1_USE || defined SPI2_USE || defined SPI3_USE)        
        //info->writeBuffer=&writeBufferSpi;      
        info->writeBuffer=&spiWriteBuffer;
        info->writeBufferMode=&spiWriteBufferMode;
        
        info->getPort=&spiGetPort;
        info->freePort=&spiFreePort;
        //info->setBusMode=&spiSetBusMode;

        info->directModeHwBuffer=(int*)spiGetHwBuffer(index);
#endif
        
    }
    else if(periph_type==PERIPH_TYPE.i2c)
    {
        info->writeBuffer=&i2cWriteBuffer;
        info->writeBufferMode=NULL;
        
        info->getPort=&i2cGetPort;
        info->freePort=&i2cFreePort;
    }
}



//<editor-fold defaultstate="collapsed" desc="SPI">
#if (defined SPI1_USE || defined SPI2_USE || defined SPI3_USE)

static void spiWriteBuffer(void* info, char* buffer, short len) 
{
    PORT_INFO* i = (PORT_INFO*) info;
    spiExchangeDE(i->portIndex, buffer, NULL, len);
}

static void spiWriteBufferMode(void* info, char* buffer, short len, char mode) 
{
    PORT_INFO* i = (PORT_INFO*) info;
    spiExchangeModeDE(i->portIndex, buffer, NULL, len, mode);
}

static void spiGetPort(void* info) 
{
    PORT_INFO* i = (PORT_INFO*) info;
    spiUse(i->portIndex, i->finishFn, i->eventFn);
}

static void spiFreePort(void* info) 
{
    PORT_INFO* i = (PORT_INFO*) info;
    spiFree(i->portIndex);
}

/*
static void spiSetBusMode(void* info, char mode) 
{
    PORT_INFO* i = (PORT_INFO*) info;
    spiSetBusMode(i->portIndex, mode);
}
*/

#endif

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="I2C">

static void i2cWriteBuffer(void* info, char* buffer, short len) {
    PORT_INFO* i = (PORT_INFO*) info;
    //i2c_WriteDE(i->portIndex, buffer, len, i->i2cAddress);
}

static void i2cGetPort(void* info) {
    PORT_INFO* i = (PORT_INFO*) info;
    //i2c_Use(i->portIndex, 1, i->finishFn);
}

static void i2cFreePort(void* info) {
    PORT_INFO* i = (PORT_INFO*) info;
    //i2c_Free(i->portIndex);
}

// </editor-fold>
