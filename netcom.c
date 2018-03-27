#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

#ifdef USE_UARTNETCOM

#define     TX_FIFO             U2TXREG

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
void netcomNotRespond();


//local fn
static char getSizeBits(char size);
static char getSizeValue(char bits);
static int dataOutAdd(NETCOM_DATAOUT* data);
static void dataOutRemove();

void onReceiveHead(char abyte, volatile int* buffer);
void onReceiveData(volatile int* buffer);
static void dataReceived();
static void sendReply(char r);

static void startMaster();
static void nextMaster1();
static void nextMaster2();
static void startTx(char ms, char exc);
static void clearRxFifo();
static void txWriteFifo();


//local var
char receivingPipe=0;                                           //aktualni pipe prijimanych dat
char receivingErrorStatus=0;                                    //error status, odesila jako odpoved
ushort receivingChecksum=0;                                     //zde prubezne scita checksum
ushort receivingChecksumOriginal=0;                             //prijaty checksum v head
char receivingDataPosition=0;                                   //aktualni pozice v pipe bufferu
char receivingDataCounter=0;                                    //cita velikost prijatych dat

char nra=0;//NOT_RESPONSE_ACTION.Data;

char rxExcept=0;//NETCOM_EXCEPT.None;
char nextID=0;
NETCOM_DATAOUT* sendingItem;
NETCOM_DATAOUT nextMaster1Item;
NETCOM_DATAOUT nextMaster2Item;

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
    //Head, Error, Status, HeadIndex=0, DataIndex=0
    
    //vyplni app
    //appID, oppID, Pipe, Data (buffer), DataLen
    
    //head
    data->Head[0]=data->OppID;
    
    ushort x;
    char size;
    if(data->Direction==0)
    {
        //set
        size=getSizeBits(data->DataLen);
        
        x=size |  (data->Pipe << 3) | (NETCOM_DEVID << 7) | (0 << 13);
        data->Head[3]=(char)x;
        data->Head[4]=(char)(x >> 8);
    }
    else
    {
        //get
        size=0;
        x=size |  (data->Pipe << 3) | (NETCOM_DEVID << 7) | (1 << 13);
        data->Head[3]=(char)x;
        data->Head[4]=(char)(x >> 8);        
    }
    
    //checksum
    x=checksum16(data->Data, data->DataLen);
    x+=data->Head[3];
    x+=data->Head[4];
    data->Head[1]=(char)x;
    data->Head[2]=(char)(x >> 8);
    
    //error
    data->Error=0;
    
    //status
    data->Status=NETCOM_OUT_STATUS.WaitToTx;
    
    data->HeadIndex=0;
    data->DataIndex=0;
    
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
static void dataOutRemove()
{
    
}

// Rx --------------------------------------------------------------------------

void onReceiveHead(char abyte, volatile int* buffer)
{
    //abyte je prvni (address) byte (b8=1)
    //buffer je FIFO buffer UART modulu
    
    /*
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
    */
}
void onReceiveData(volatile int* buffer)
{
    /*
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
    
    */
}
static void dataReceived()
{
    /*
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
    
    */
}
static void sendReply(char r)
{
    
}


// Tx --------------------------------------------------------------------------

static void startMaster()
{
    //vola se, pokud ma zacit odesilat data
    
    nextID=NETCOM_DEVID + 1;
    
    if(netcomDataOut[0]!=NULL && netcomDataOut[0]->Status==NETCOM_OUT_STATUS.WaitToTx)
    {
        //Odesila data
        sendingItem=netcomDataOut[0];
        nra=NOT_RESPONSE_ACTION.Data;
        if(netcomDataOut[0]->Direction==0)
        {
            //set
            startTx(1, NETCOM_EXCEPT.Reply);
        }
        else
        {
            //get
            startTx(1, NETCOM_EXCEPT.ReturnData);            
        }
    }
    else
    {
        //nema data, odesila endPaket
        nextMaster1();
    }
}
static void nextMaster1()
{
    nextMaster1Item.Head[0]=nextID;                                                    
    
    //head data
    ushort d=0 | (1<<3) | (NETCOM_DEVID << 7) | (6 << 13);
    nextMaster1Item.Head[3]=(char)d;
    nextMaster1Item.Head[4]=(char)(d>>8);
    
    //checksum
    d=nextMaster1Item.Head[3] + nextMaster1Item.Head[4];
    nextMaster1Item.Head[1]=(char)d;
    nextMaster1Item.Head[2]=(char)(d>>8);    
    
    nextMaster1Item.HeadLen=5;
    nextMaster1Item.HeadIndex=0;
    nextMaster1Item.DataLen=0;
    sendingItem=&nextMaster1Item;
    
    nextID++;
    if(nextID==NETCOM_MAXID)
    {
        nextID=1; 
    }
    
    nra=NOT_RESPONSE_ACTION.NextMaster;
    startTx(1, NETCOM_EXCEPT.AcceptMaster);
}
static void nextMaster2()
{
    nextMaster2Item.Head[0]=1; 
    
    //head data
    ushort d=0 | (8<<3) | (NETCOM_DEVID << 7) | (6 << 13);
    nextMaster2Item.Head[3]=(char)d;
    nextMaster2Item.Head[4]=(char)(d>>8);
    
    //checksum
    d=nextMaster2Item.Head[3] + nextMaster2Item.Head[4];
    nextMaster2Item.Head[1]=(char)d;
    nextMaster2Item.Head[2]=(char)(d>>8);     

    
    
    nextMaster2Item.Head[5]=nextID; 
    
    //head data
    d=0 | (2<<3) | (NETCOM_DEVID << 7) | (6 << 13);
    nextMaster2Item.Head[8]=(char)d;
    nextMaster2Item.Head[9]=(char)(d>>8);
    
    //checksum
    d=nextMaster2Item. Head[8] + nextMaster2Item.Head[9];
    nextMaster2Item.Head[6]=(char)d;
    nextMaster2Item.Head[7]=(char)(d>>8);  

    nextMaster2Item.HeadLen=10;
    nextMaster2Item.HeadIndex=0;
    nextMaster2Item.DataLen=0;
    nextMaster2Item.DataIndex=0;
    sendingItem=&nextMaster2Item;
    
    startTx(0, NETCOM_EXCEPT.None);    
    
}
static void startTx(char ms, char exc)
{
    netcom_ms=ms;
    rxExcept=exc;
    clearRxFifo();
    txWriteFifo();
}
static void clearRxFifo()
{
    
}
static void txWriteFifo()
{
    
    ushort x;
    if(sendingItem->HeadLen > sendingItem->HeadIndex + 1)
    {
        //head
        if(sendingItem->HeadIndex==0 || sendingItem->HeadIndex==5)
        {
            //adresa, b8=1
            x=(ushort)sendingItem->Head[sendingItem->HeadIndex];
            x |= 0x100;
            TX_FIFO=x;
            sendingItem->HeadIndex++;
        }
        else
        {
           //data b8=0
            x=(ushort)sendingItem->Head[sendingItem->HeadIndex];
            TX_FIFO=x;
            sendingItem->HeadIndex++;            
        }
    }
    else if(sendingItem->DataLen > sendingItem->DataIndex + 1)
    {
        //data b8=0
        x=(ushort)sendingItem->Data[sendingItem->DataIndex];
        TX_FIFO=x;        
        x=(ushort)sendingItem->Data[sendingItem->DataIndex+1];
        TX_FIFO=x;         
        x=(ushort)sendingItem->Data[sendingItem->DataIndex+2];
        TX_FIFO=x;         
        x=(ushort)sendingItem->Data[sendingItem->DataIndex+3];
        TX_FIFO=x;         
        
        sendingItem->DataIndex+=4;
    }
        
}
void netcomNotRespond()
{
    //volano z timer1, pri netcom_ms>50
    
    //zadna odpoved 
    netcomStratup_ms=0;
    
    if(nra==NOT_RESPONSE_ACTION.Data)
    {
        //cekalo se na REPLY
        if(sendingItem->Error>=3)
        {
            //Oznaci, nelze odeslat, vyradi z dataOut
            sendingItem->Status=NETCOM_OUT_STATUS.ReplyNotExist;
            dataOutRemove();
            startMaster();
        }
        else
        {
            sendingItem->Error++;
            sendingItem->HeadIndex=0;
            sendingItem->DataIndex=0;
            if(netcomDataOut[0]->Direction==0) { startTx(1, NETCOM_EXCEPT.Reply); }     //set
            else { startTx(1, NETCOM_EXCEPT.ReturnData); }                              //get
        }
    }
    else
    {
        //cekalo se Accept, bude volat dalsi modul
        nextMaster1();
    }
}


// interrupt -------------------------------------------------------------------
void tx_interrupt()
{
    txWriteFifo();
}

void rx_interrupt()
{
    netcom_ms=0;                                    //ok, nastala komunikace
    netcomStratup_ms=0;
    
}

#endif  //USE_UARTNETCOM