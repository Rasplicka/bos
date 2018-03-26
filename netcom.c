#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

#ifdef USE_UARTNETCOM

// <editor-fold defaultstate="collapsed" desc="DATA IN buffers">
#if NETCOM_DATAIN_CAPA >= 1
NETCOM_DATAIN dataIn1;
char bufferPipe1[NETCOM_BUFFER_PIPE_LEN];
#endif

#if NETCOM_DATAIN_CAPA >= 2
NETCOM_DATAIN dataIn2;
char bufferPipe2[NETCOM_BUFFER_PIPE_LEN];
#endif

#if NETCOM_DATAIN_CAPA >= 3
NETCOM_DATAIN dataIn3;
char bufferPipe3[NETCOM_BUFFER_PIPE_LEN];
#endif

#if NETCOM_DATAIN_CAPA >= 4
NETCOM_DATAIN dataIn4;
char bufferPipe4[NETCOM_BUFFER_PIPE_LEN];
#endif
// </editor-fold>

//global fn
void netcomInit();
void netcomSetReceiveFn(char pipe, void* addr);
void netcomSendBuffer(NETCOM_DATAOUT* data);

//local fn
static char getSizeBits(char size);
static int dataOutAdd(NETCOM_DATAOUT* data);
void onReceiveHead(char abyte, volatile int* buffer);
void onReceiveData(volatile int* buffer);
static void dataReceived();
static void sendReply(char r);

//local var
char receivingPipe=0;                                           //aktualni pipe prijimanych dat
char receivingErrorStatus=0;                                    //error status, odesila jako odpoved
ushort receivingChecksum=0;                                     //zde prubezne scita checksum
ushort receivingChecksumOriginal=0;                             //prijaty checksum v head
char receivingDataPosition=0;                                   //aktualni pozice v pipe bufferu
char receivingDataCounter=0;                                    //cita velikost prijatych dat

char nextID=0;
NETCOM_DATAOUT* sendingItem;
NETCOM_DATAOUT endItem;

//char endPaket[]={0, 2, 2};

void (*_finish)();

void netcomInit()
{
    
#if NETCOM_DATAIN_CAPA >= 1    
    netcomDataIn[0]=&dataIn1; 
    dataIn1.DataBuffer=bufferPipe1;
#endif
#if NETCOM_DATAIN_CAPA >= 2     
    netcomDataIn[1]=&dataIn2;
    dataIn2.DataBuffer=bufferPipe2;
#endif    
#if NETCOM_DATAIN_CAPA >= 3     
    netcomDataIn[2]=&dataIn3;
    dataIn3.DataBuffer=bufferPipe3;
#endif 
#if NETCOM_DATAIN_CAPA >= 4     
    netcomDataIn[3]=&dataIn4;
    dataIn4.DataBuffer=bufferPipe4;
#endif     
    
    NETCOM_DATAIN* i;
    int a;
    for(a=0; a < NETCOM_DATAIN_CAPA; a++)
    {
        netcomDataIn[a]->Error=0;
        netcomDataIn[a]->OppID=0;
        netcomDataIn[a]->DataBufferLen=NETCOM_BUFFER_PIPE_LEN;
        netcomDataIn[a]->Status=NETCOM_IN_STATUS.Ready;
    }
    

    nextID=NETCOM_DEVID + 1;
    
}

//static void initEndItem()
//{
    //nextID=NETCOM_DEVID + 1;
    //endItem.DataBuffer[0]=0;                                                    //end paket
    //endItem.DataBuffer[1]=nextID;                                               //next DevID
    //endItem.DataBuffer[2]=endItem.DataBuffer[0] + endItem.DataBuffer[1];        //checksum
//}

void netcomSetReceiveFn(char pipe, void* addr)
{
    if(pipe <= NETCOM_DATAIN_CAPA)
    {
        netcomDataIn[pipe-1]->ReceiveFn=addr;
    }
}

void netcomSendBuffer(NETCOM_DATAOUT* data)
{
    //doplni polozky level1 do struktury data
    //vyplni tato fce (level 1)
    //Head
    //Error
    //Status
    
    //vyplni app
    //appID
    //oppID
    //pipe
    //dataBuffer 
    //dataBufferLen
    
    
    //head
    data->Head[0]=data->OppID;
    
    char size=getSizeBits(data->DataBufferLen);
    char pipe=(data->Pipe << 4);
    data->Head[3]=(size | pipe);
    data->Head[4]=NETCOM_DEVID;
    
    //checksum
    unsigned short chsum=checksum16(data->DataBuffer, data->DataBufferLen);
    chsum+=data->Head[0];
    chsum+=data->Head[3];
    chsum+=data->Head[4];
    data->Head[1]=(char)chsum;
    data->Head[2]=(char)(chsum >> 8);
       
    //error
    data->Error=0;
    
    //status
    data->Status=NETCOM_OUT_STATUS.Ready;
    
    //vlozi polozku do netcomDataOut, pokud je plne, ceka na uvolneni
    while(dataOutAdd(data)==0)
    {
        doEvents();
    }
    
}

static char getSizeBits(char size)
{
    if (size <= 4) {return 1;}
    else if (size <= 8) {return 2;}
    else if (size <= 12) {return 3;}    
    else if (size <= 16) {return 4;}    
    else if (size <= 32) {return 5;}    
    else if (size <= 64) {return 6;}    
    else if (size <= 128) {return 7;}    
    else {return 0;}
}
static char getSizeValue(char bits)
{
    if (bits == 1) {return 4;}
    else if (bits == 2) {return 8;}
    else if (bits == 3) {return 12;}    
    else if (bits == 4) {return 16;}    
    else if (bits == 5) {return 32;}    
    else if (bits == 6) {return 64;}    
    else if (bits == 7) {return 128;}    
    else {return 0;}
}

static int dataOutAdd(NETCOM_DATAOUT* data)
{
    //doplni polozku do netcomDataOut, kde ceka na odeslani
    int a;
    for(a=0; a<NETCOM_DATAOUT_CAPA; a++)
    {
        if(netcomDataOut[a] == NULL)
        {
            netcomDataOut[a] = data;
            return 1;
        }
    }
    
    return 0;
    
}



void onReceiveHead(char abyte, volatile int* buffer)
{
    //abyte je prvni (address) byte (b8=1)
    //buffer je FIFO buffer UART modulu
    
    char byte1=(char) *buffer;
    char byte2=(char) *buffer;
    char byte3=(char) *buffer;
    char byte4=(char) *buffer;
    
    receivingChecksumOriginal=( ((ushort)byte1) | (((ushort)byte2)<<8) );
    
    char pipe=(byte3>>4);
    receivingChecksum=(ushort)abyte + (ushort)byte3 + (ushort)byte4;
    receivingDataPosition=0;
    receivingDataCounter=getSizeValue(byte3 & 0b00001111);
    
    if(pipe <= NETCOM_DATAIN_CAPA)
    {
        receivingPipe=pipe;
        
        if(netcomDataIn[pipe]->Status == NETCOM_IN_STATUS.Full)
        {
            //pipe buffer neni prazdny
            //(nebude data zapisovat do bufferu, ale bude provadet checksum)
            receivingErrorStatus=NETCOM_OUT_STATUS.ReplyBusy;
        }
        else
        {    
            //ok, buffer ready
            netcomDataIn[pipe]->OppID=byte4 & 0b00011111;
            netcomDataIn[pipe]->dataLen=receivingDataCounter;
        
            if(netcomDataIn[pipe]->dataLen > netcomDataIn[pipe]->DataBufferLen)
            {
                //data jsou prilis velka 
                //(nebude data zapisovat do bufferu, ale bude provadet checksum)
                receivingErrorStatus=NETCOM_OUT_STATUS.ReplyErrorSize;
            }
            else
            {
               receivingErrorStatus=NETCOM_OUT_STATUS.ReplyOk;
            }
        }
    }
    else
    {
        //chyba, nelze prijimat, neexistuje pipe
        //(nebude data zapisovat do bufferu, ale bude provadet checksum)
        receivingPipe=0;
        receivingErrorStatus=NETCOM_OUT_STATUS.ReplyErrorPipe;
    }
    
    if(receivingDataCounter==0)
    {
        //zadna dalsi data nebudou (size=0)
        dataReceived();
    }
}

void onReceiveData(volatile int* buffer)
{
    //prijal prvni, nebo dalsi 4 bytes dat
    char data1=(char)*buffer;
    char data2=(char)*buffer;
    char data3=(char)*buffer;
    char data4=(char)*buffer;
    receivingDataCounter-=4;
    receivingChecksum += (ushort)data1 + (ushort)data2 + (ushort)data3 + (ushort)data4;
    
    if(receivingPipe > 0)
    {
        //prijima data do bufferu
        netcomDataIn[receivingPipe]->DataBuffer[receivingDataPosition] = data1;
        netcomDataIn[receivingPipe]->DataBuffer[receivingDataPosition+1]=data2;
        netcomDataIn[receivingPipe]->DataBuffer[receivingDataPosition+2]=data3;
        netcomDataIn[receivingPipe]->DataBuffer[receivingDataPosition+3]=data4;        
        
        receivingDataPosition += 4;
    }
    
    if(receivingDataCounter==0)
    {
        dataReceived();
    }
}

static void dataReceived()
{
    //ok, prijal vsechna data
    if(receivingChecksum != receivingChecksumOriginal)
    {
        //chybny checksum, neodpovida
        receivingErrorStatus=NETCOM_OUT_STATUS.ReplyNone;
    }
    else
    {
        //checksum OK
        sendReply(receivingErrorStatus);
        netcomDataIn[receivingPipe]->Status==NETCOM_IN_STATUS.Full;
        _finish=netcomDataIn[receivingPipe]->ReceiveFn;
        _finish();
    }
    
    receivingPipe=0;
}

static void sendReply(char r)
{
    
}



void netcomSend()
{
    //vola se, pokud ma zacit odesilat data
    
    nextID=NETCOM_DEVID + 1;
    
    if(netcomDataOut[0]!=NULL && netcomDataOut[0]->Status==NETCOM_OUT_STATUS.Ready)
    {
        //Odesila data
        sendingItem=netcomDataOut[0];
        tx_interrupt();
    }
    else
    {
        //nema data, odesila endPaket
        netcomSendEndPaket();
    }
}

void netcomNextDeviceNotRespond()
{
    //DevID, ktere melo zacit com., nekomunikuje
    
    if(nextID==NETCOM_DEVID)
    {
        //odeslat data tohoto modulu
        netcomSend();
    }
    else
    {
        //odeslat endPaked dalsiho DevID
        netcomSendEndPaket();
    }
        
}

void netcomSendEndPaket()
{
    endItem.Head[0]=0;                                                    //end paket
    endItem.Head[1]=nextID;                                               //next DevID
    endItem.Head[2]=endItem.DataBuffer[0] + endItem.DataBuffer[1];        //checksum    
    endItem.HeadLen=3;
    endItem.HeadIndex=0;
    endItem.DataLen=0;
    sendingItem=&endItem;
    
    nextID++;
    if(nextID==NETCOM_MAXID)
    {
        nextID=1; 
    }
    
    tx_interrupt();
}

void sendFinish()
{
    //po odvisilani head+data, nebo endPaketu
    if(sendingItem == &endItem)
    {
        //byl odvysilan endPaket
        netcom_ms=1;                                //ceka 50ms, zda nastane komunikace
    }
    else
    {
        //byl odvysilan head+data, ceka na odpoved
        
    }
}

void tx_interrupt()
{
    netcomStratup_ms=0;
}

void rx_interrupt()
{
    netcom_ms=0;                                    //ok, nastala komunikace
    netcomStratup_ms=0;
    
}

#endif  //USE_UARTNETCOM