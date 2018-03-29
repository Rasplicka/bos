#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

#ifdef USE_UARTNETCOM

extern ushort netcomStratup_ms;
extern ushort netcomTx_ms;
extern ushort netcomRx_ms;

#define     TX_FIFO                     U2TXREG
#define     RX_FIFO                     U2RXREG
#define     TX_FIFO_SIZE                8

//#define     _ENABLE_TX_INTERRUPT
//#define     _DISABLE_TX_INTERRUPT

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
void netcomSendBuffer(NETCOM_DATAOUT* data);
void netcomNotRespond();
void netcomRxTimeout();
void netcomInitBus();

//local fn
static char initModule();
static void clearRxFifo();
static void enableTx();
static void enableRx();
static char getSizeBits(char size);
static char getSizeValue(char bits);
static int dataOutAdd(NETCOM_DATAOUT* data);
static void dataOutRemove();

static void startMaster();
static void nextMaster1();
static void nextMaster2();
static void master8();
static void startTx(char ms, char exc);
static void txWriteFifo();
static void notRespondAct();

static void onChecksum();
static void sendAccept();
static void sendPipe(char pipe);
static void sendReply(char repdata);

//local var
char nra=0;
char txf=0;
char rxExcept=0;
char nextID=0;

char headSize=0;
char headCommand=0;
char headPipe=0;
char headOpp=0;
ushort headChecksum=0;
ushort calcChecksum=0;

char* getBuffer=NULL;
char ra=0;
char rxStatus=0;
char rxIndex=0;
char rxAnyData=0;

NETCOM_DATAOUT* sendingItem;                            //aktualni polozka, kterou Master odesila
NETCOM_DATAOUT master1Item;
NETCOM_DATAOUT master2Item;
NETCOM_DATAOUT master8Item;
NETCOM_DATAOUT replyItem;

void (*_finish)();

// global fn -------------------------------------------------------------------
void netcomInit()
{
    
    /*
#if NETCOM_DATAIN_CAPA >= 1    
    netcomDataSet[0]=&dataIn1; 
    dataIn1.DataBuffer=bufferPipe1;
#endif
#if NETCOM_DATAIN_CAPA >= 2     
    netcomDataSet[1]=&dataIn2;
    dataIn2.DataBuffer=bufferPipe2;
#endif    
#if NETCOM_DATAIN_CAPA >= 3     
    netcomDataSet[2]=&dataIn3;
    dataIn3.DataBuffer=bufferPipe3;
#endif 
#if NETCOM_DATAIN_CAPA >= 4     
    netcomDataSet[3]=&dataIn4;
    dataIn4.DataBuffer=bufferPipe4;
#endif     
    
    NETCOM_DATAIN* i;
    int a;
    for(a=0; a < NETCOM_DATASET_CAPA; a++)
    {
        netcomDataSet[a].->Error=0;
        netcomDataSet[a]->OppID=0;
        netcomDataSet[a]->DataBufferLen=NETCOM_BUFFER_PIPE_LEN;
        netcomDataSet[a]->Status=NETCOM_IN_STATUS.Ready;
    }
    
    */
    netcomStratup_ms=0;
    netcomTx_ms=0;
    netcomRx_ms=0;
    
    
    //destation ID=1
    master8Item.Head[0]=1; 
    //head data
    ushort d=0 | (8<<3) | (NETCOM_DEVID << 7) | (6 << 13);
    master8Item.Head[3]=(char)d;
    master8Item.Head[4]=(char)(d>>8);
    //checksum
    d=master8Item.Head[3] + master8Item.Head[4];
    master8Item.Head[1]=(char)d;
    master8Item.Head[2]=(char)(d>>8);
    master8Item.DataLen=0;
    master8Item.DataIndex=0;

    
    master1Item.DataLen=0;              //nebude vysilat data
    master1Item.DataIndex=0;
    
    master2Item.DataLen=0;              //nebude vysilat data
    master2Item.DataIndex=0;    
    
    //nextID
    //nextID=NETCOM_DEVID + 1;
    
    initModule();
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
        //set, po odelsani bude nastaveno data.Status
        size=getSizeBits(data->DataLen);
        
        x=size |  (data->Pipe << 3) | (NETCOM_DEVID << 7) | (0 << 13);
        data->Head[3]=(char)x;
        data->Head[4]=(char)(x >> 8);
    }
    else
    {
        //get (vraci data v data.Data, size v data.DataLen)
        //data.Data musi byt nastaveno na buffer, kam budou get data zapsana
        //data.DataLen musi obsahovat velikost bufferu
        //vysledek podle data.Status (pri ReplyOK data.DataLen obsahuje velikost dat)
        //jinak data.Status obsahuje kod chyby
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
    
    data->HeadIndex=0;                                  //bude vysilat head
    data->DataIndex=0;                                  //bude vysilat data (pokud DataLen>0, nastavuje app)
    
    //vlozi polozku do netcomDataOut, pokud je plne, ceka na uvolneni
    while(dataOutAdd(data)==0)
    {
        doEvents();
    }
}
void netcomRxTimeout()
{
    //zachytil svoji adresu, ale prijem dat nebyl dokoncen ve stanovene lhute
    
    U2STAbits.URXEN=0;              //disable Rx
    U2STAbits.UTXEN=0;              //disable Tx
    clearRxFifo();
    ra=0;
    netcomRx_ms=0;
    
    if(netcomTx_ms>0)
    {
        //Master: ceka na data (ktera neprijdou, chyba cteni)
        netcomNotRespond();
    }
}
void netcomNotRespond()
{
#if defined NETCOM_DEVID==1
    //devID=1, neposila master8
    netcomStratup_ms=0;
    notRespondAct();
#else
    //neni devID=1, posila master8
    txf=NETCOM_TXFINISH_FN.NotRespondAct;
    master8();
#endif
    
}
#if defined NETCOM_DEVID==1
void netcomInitBus()
{
    //initBus, start comunication (after 100ms pause, DevID=1 only)
    
    U2STAbits.URXEN=0;      //Rx disable
    U2STAbits.UTXEN=0;      //Rx disable
    clearRxFifo();
    U2STAbits.URXEN=1;      //Rx enable
    
    startMaster();
}
#endif

// local fn --------------------------------------------------------------------
static char initModule()
{
#define     U2_PRIORITY     1
#define     U2_SUBPRIORITY  0

    IPC14bits.U2RXIP=U2_PRIORITY;
    IPC14bits.U2RXIS=U2_SUBPRIORITY;
    IPC14bits.U2TXIP=U2_PRIORITY;
    IPC14bits.U2TXIS=U2_SUBPRIORITY;
    IPC14bits.U2EIP=U2_PRIORITY;
    IPC14bits.U2EIS=U2_SUBPRIORITY;
    
    IFS1bits.U2RXIF=0;
    IFS1bits.U2TXIF=0;
    IFS1bits.U2EIF=0;
    
    IEC1bits.U2RXIE=1;
    IFS1bits.U2TXIF=1;
    IFS1bits.U2EIF=1;
    
    //UART2
    U2MODEbits.SLPEN=0;     
    U2MODEbits.CLKSEL=3;    //REFO
    
    U2MODEbits.SIDL=0;      //continue in IDLE
    U2MODEbits.IREN=0;      //IR disable
    U2MODEbits.UEN=0;       //pouze Rx a Tx
    U2MODEbits.WAKE=0;      //wake up SLEEP disable
    U2MODEbits.LPBACK=0;    //Loop disable
    U2MODEbits.ABAUD=0;     //autobaud disable
    U2MODEbits.RXINV=0;     //
    U2MODEbits.BRGH=0;      //std. speed (16x)
    U2MODEbits.PDSEL=3;     //9-bit, no parity
    U2MODEbits.STSEL=0;     //1x stop bit
    
    //povoli prijem dat, zakaze vysilani
    
    U2STAbits.MASK=0xFF;    //
    U2STAbits.ADDR=NETCOM_DEVID; //DevID
    U2STAbits.UTXSEL=1;     //Tx interrupt, po odvysilani
    U2STAbits.UTXINV=0;     //
    U2STAbits.URXEN=1;      //Rx enable
    U2STAbits.UTXBRK=0;     //send break disable
    U2STAbits.UTXEN=0;      //Tx disable
    U2STAbits.URXISEL=1;    //Rx interrupt 4 bytes ve FIFO
    U2STAbits.ADDEN=1;      //addres detect enable
    
    U2BRG=29;               //100k baud BRG=Clk/(16xbaud) -1
    
    //start
    U2MODEbits.ON=1;
}
static void clearRxFifo()
{
    char x;
    while(U2STAbits.URXDA)
    {
        x=U2RXREG;
    }
}
static void enableTx()
{
    //nastavi modul na vysilani
    U2STAbits.URXEN=0;      //Rx disable
    U2STAbits.UTXEN=1;      //Tx enable
}
static void enableRx()
{
    //nastavi modul na prijem
    U2STAbits.UTXEN=0;      //Tx enable
    U2STAbits.URXEN=1;      //Rx disable
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
    //remove
    int a;
    for(a=1; a < NETCOM_DATAOUT_CAPA; a++)
    {
        netcomDataOut[a-1]=netcomDataOut[a];
        if(netcomDataOut[a] == NULL) { break; }
    }
    //vymazat posledni polozku, pokud dataOut bylo plne
    netcomDataOut[NETCOM_DATAOUT_CAPA-1]=NULL;
    
    sendingItem=NULL;
}


// Tx --------------------------------------------------------------------------
void UART2Tx_interrupt()
{
    txWriteFifo();
}
static void startMaster()
{
    //vola se, pokud ma zacit odesilat data
    if(netcomDataOut[0]!=NULL && netcomDataOut[0]->Status==NETCOM_OUT_STATUS.WaitToTx)
    {
        //Odesila data
        sendingItem=netcomDataOut[0];
        nra=NOT_RESPONSE_ACTION.Data;                               //pri notResponse bude opakovat vysilani (max. 3x)
        txf=NETCOM_TXFINISH_FN.None;                                //po skonceni vysilani zadna akce (ceka na odpoved)
        if(netcomDataOut[0]->Direction==0)
        {
            //set
            startTx(1, NETCOM_COMMAND.Reply);                        //ocekava REPLY (OK, nebo s kodem chyby)
        }
        else
        {
            //get
            startTx(1, NETCOM_COMMAND.ReturnData);                   //ocekava data, nebo REPLY s kodem chyby
        }
    }
    else
    {
        //nema data, odesila endPaket
        nextID=NETCOM_DEVID;                                        //nextMaster1 zvysi nextID++
        nextMaster1();
    }
}
static void nextMaster1()
{
    nextID++;
    if(nextID==NETCOM_MAXID) { nextID=1; }

    //addr
    master1Item.Head[0]=nextID;                                                    
    //head data
    ushort d=0 | (1<<3) | (NETCOM_DEVID << 7) | (6 << 13);
    master1Item.Head[3]=(char)d;
    master1Item.Head[4]=(char)(d>>8);
    //checksum
    d=master1Item.Head[3] + master1Item.Head[4];
    master1Item.Head[1]=(char)d;
    master1Item.Head[2]=(char)(d>>8);    
    
    master1Item.HeadIndex=0;
    sendingItem=&master1Item;
    
    nra=NOT_RESPONSE_ACTION.NextMaster;
    txf=NETCOM_TXFINISH_FN.None;
    startTx(1, NETCOM_COMMAND.AcceptMaster);
}
static void nextMaster2()
{
    //addr
    master2Item.Head[0]=nextID; 
    //head data
    ushort d=0 | (2<<3) | (NETCOM_DEVID << 7) | (6 << 13);
    master2Item.Head[3]=(char)d;
    master2Item.Head[4]=(char)(d>>8);
    //checksum
    d=master2Item. Head[3] + master2Item.Head[4];
    master2Item.Head[1]=(char)d;
    master2Item.Head[2]=(char)(d>>8);  

    master2Item.HeadIndex=0;
    sendingItem=&master2Item;
    
    txf=NETCOM_TXFINISH_FN.None;
    startTx(0, NETCOM_COMMAND.None);    
}
static void master8()
{
    master8Item.HeadIndex=0;
    sendingItem=&master8Item;
    startTx(0, NETCOM_COMMAND.None);    
}
static void startTx(char ms, char exc)
{
    netcomTx_ms=ms;
    rxExcept=exc;
    clearRxFifo();
    txWriteFifo();
}
static void txWriteFifo()
{
    ushort x;
    if(sendingItem->HeadIndex==0)
    {
        //head
        //_ENABLE_TX_INTERRUPT;
        enableTx();                                                 //ukonci Rx, nastavi Tx
        
        //adresa, b8=1
        x=(ushort)sendingItem->Head[0];
        x |= 0x100;
        TX_FIFO=x;
        sendingItem->HeadIndex++;
        
        //data b8=0
        x=(ushort)sendingItem->Head[1];
        TX_FIFO=x;
        x=(ushort)sendingItem->Head[2];
        TX_FIFO=x;
        x=(ushort)sendingItem->Head[3];
        TX_FIFO=x;
        x=(ushort)sendingItem->Head[4];
        TX_FIFO=x;        
        
        sendingItem->HeadIndex=5;            
    }
    else if(sendingItem->DataIndex < sendingItem->DataLen)
    {
        //data
        char c=0;
        while(sendingItem->DataIndex < sendingItem->DataLen && c < TX_FIFO_SIZE)
        {
            x=(ushort)sendingItem->Data[sendingItem->DataIndex];
            TX_FIFO=x;  
            sendingItem->DataIndex++;
            c++;
        }
    }
    else
    {
        enableRx();                                                 //ukonci Tx, nastavi Rx
        //odeslano vsechno
        if(txf==NETCOM_TXFINISH_FN.None)
        {
            //_DISABLE_TX_INTERRUPT;
        }
        else if (txf==NETCOM_TXFINISH_FN.NextMaster2)
        {
            nextMaster2();
        }
        else if (txf==NETCOM_TXFINISH_FN.NotRespondAct)
        {
            notRespondAct();
        }
        else if (txf==NETCOM_TXFINISH_FN.ReturnData)
        {
            //SLAVE: po odeslani odpovedi (dat) na get
            NETCOM_DATAIN* p = netcomDataGet[headPipe];
            p->Locked=0;
            sendingItem=NULL;
            // _DISABLE_TX_INTERRUPT;
        }
        else
        {
            startMaster();
        }
    }
}
static void notRespondAct()
{
    //zadna odpoved na vysilani
    if(nra==NOT_RESPONSE_ACTION.Data)
    {
        //cekalo se na REPLY
        if(sendingItem->Error >= 3)
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
            //nra zustava NOT_RESPONSE_ACTION.Data
            txf=NETCOM_TXFINISH_FN.None;                                                //po odvysilani zadna okce (ceka na odpoved)
            if(netcomDataOut[0]->Direction==0) { startTx(1, NETCOM_COMMAND.Reply); }     //set
            else { startTx(1, NETCOM_COMMAND.ReturnData); }                              //get
        }
    }
    else
    {
        //cekalo se Accept, bude volat dalsi modul
        //znovu nastavi nra a txf
        nextMaster1();
    }
}


// Rx --------------------------------------------------------------------------
void UART2Rx_interrupt()
{
    char h1, h2, h3, h4;
    h1=RX_FIFO;
    h2=RX_FIFO;
    h3=RX_FIFO;
    h4=RX_FIFO;
    
    if(ra==0)
    {
        //head
        netcomRx_ms=1;                                      //spusti citani

        ushort x=h3 | (((ushort)h4) << 8);
        headSize = getSizeValue(h3 & 0b00000111);
        headPipe = (h3 & 0b01111000) >> 3;
        headOpp =  (x & 0b0001111110000000) >> 7;
        headCommand = (h4 & 0b11100000) >> 5;
        
        //init checksum
        headChecksum=h1 | (((ushort)h2) << 8);
        calcChecksum=h3+h4;
        
        if(headSize==0)
        {
            //master: Reply, Accept, Slave: Get, setMaster(1,2,8)
            onChecksum();
        }
        else if (headCommand==3)
        {
            //master prijima data (odpoved na get)
            if(sendingItem != NULL && sendingItem->Data != NULL)
            {
                if(sendingItem->DataLen >= headSize)
                {
                    //ok, muze prijimat data
                    sendingItem->DataLen=headSize;
                    rxStatus=NETCOM_OUT_STATUS.ReplyOk;
                    rxIndex=0;
                    getBuffer=sendingItem->Data;
                    //rxStatus=NETCOM_OUT_STATUS.ReplyOk;
                    ra=1;
                    return;
                }
                else
                {
                    //data jsou moc velka
                    getBuffer=NULL;
                    rxStatus=NETCOM_OUT_STATUS.ReplyErrorSize;
                    ra=1;
                    return;
                }
            }
            else
            {
                //buffer neexistuje (musi ho nastavit volajici fce, NETCOM_DATAOUT.Data)
                getBuffer=NULL;
                rxStatus=NETCOM_OUT_STATUS.ReplyErrorPipe;
                ra=1;
                return;                
            }
        }
        else if(headCommand==0)
        {
            //slave prijima data (set)
            if(headPipe > NETCOM_DATASET_CAPA)
            {
                //pipe neexistuje
                rxStatus=NETCOM_OUT_STATUS.ReplyErrorPipe;
                getBuffer=NULL;
            }
            else
            {
                NETCOM_DATAIN* p=netcomDataSet[headPipe];
                
                if(p==NULL)
                {
                     //pipe neexistuje
                    rxStatus=NETCOM_OUT_STATUS.ReplyErrorPipe;
                    getBuffer=NULL;
                }
                else if(headSize > NETCOM_SETPIPE_SIZE)
                {
                    //moc velke size
                    rxStatus=NETCOM_OUT_STATUS.ReplyErrorSize;
                    getBuffer=NULL;
                }
                else if(p->Status==NETCOM_IN_STATUS.Full)
                {
                    //pipe neni prazdna
                    rxStatus=NETCOM_OUT_STATUS.ReplyBusy;
                    getBuffer=NULL;
                }

                else
                {
                    //ok, muze prijimat data
                    rxStatus=NETCOM_OUT_STATUS.ReplyOk;
                    rxIndex=0;
                    getBuffer=p->Data;
                }
            }
            
            ra=1;
        }
    }
    else
    {
        //data
        //checksum
        calcChecksum += h1 + h2 + h3 + h4;
        
        if(getBuffer != NULL)
        {
            //buffer existuje
            getBuffer[0]=h1;
            getBuffer[1]=h2;
            getBuffer[2]=h3;
            getBuffer[3]=h4;
            getBuffer+=4;
        }
        
        headSize-=4;
        
        if(headSize==0)
        {
            //konec dat
            onChecksum();
        }
    }
}
static void onChecksum()
{
    netcomRx_ms=0;                      //ukonci citani
    ra=0;                               //nasleduje head
    
    if(calcChecksum==headChecksum)
    {
        //checksum ok
        if(rxExcept == NETCOM_COMMAND.AcceptMaster && headCommand == NETCOM_COMMAND.AcceptMaster)
        {
            //Master: prijal AcceptMaster
            //odeslat master2
            #if defined NETCOM_DEVID==1
                //devID=1, neposila master8
                netcomStratup_ms=0;
                nextMaster2();
            #else
                //neni devID=1, posila master8
                txf=NETCOM_TXFINISH_FN.NextMaster2;
                master8();
            #endif                
        }
        
        else if(rxExcept == NETCOM_COMMAND.Reply && headCommand == NETCOM_COMMAND.Reply)
        {
            //Master: prijal Reply
            sendingItem->Status=headPipe;                   //Reply data
            dataOutRemove();                                //sendingItem=NULL
            
            #if defined NETCOM_DEVID==1
                //devID=1, neposila master8
                netcomStratup_ms=0;
                startMaster();
            #else
                //neni devID=1, posila master8
                txf=NETCOM_TXFINISH_FN.StartMaster;
                master8();
            #endif             
        }
        else if(rxExcept == NETCOM_COMMAND.ReturnData)
        {
            if(headCommand == NETCOM_COMMAND.ReturnData)
            {
                //Master: prijal data, jako odpoved na get
                sendingItem->Status=rxStatus;               //ok, nebo chyba (pokud data obsahovala chybu)
            }
            else
            {
                //Master: misto dat prijal Reply, jako odpoved na get
                sendingItem->Status=headPipe;               //Reply data (obsahuje chybu, pro nebyla odeslana data)
            }
            dataOutRemove();                                //sendingItem=NULL             
            
            #if defined NETCOM_DEVID==1
                //devID=1, neposila master8
                netcomStratup_ms=0;
                startMaster();
            #else
                //neni devID=1, posila master8
                txf=NETCOM_TXFINISH_FN.StartMaster;
                master8();
            #endif             
        }   
        
        else if(headCommand == NETCOM_COMMAND.SetMaster)
        {
            //Slave: prijal setMaster(1,2,8)
            if(headPipe == 1)
            {
                //setMaster1
                sendAccept();
            }
            else if (headPipe == 2)
            {
                //setMaster2
                if(rxExcept == NETCOM_COMMAND.SetMaster)
                {
                    //ocekaval setMaster2 (prevezme master)
                    startMaster();
                }
            }
            else
            {
                //master8
                #if defined NETCOM_DEVID==1    
                    //pouze DevID=1
                    netcomStratup_ms=0;
                #endif          
            }
        }
        
        else
        {
            //slave prijal set, nebo get
            if(headCommand==NETCOM_COMMAND.SetData)
            {
                if(rxStatus==NETCOM_OUT_STATUS.ReplyOk)
                {
                    netcomDataSet[headPipe]->DataLen=headSize;
                    netcomDataSet[headPipe]->DataIndex=0;
                    netcomDataSet[headPipe]->Status=NETCOM_IN_STATUS.Full;
                }
                sendReply(rxStatus);
            }
            
            else if(headCommand==NETCOM_COMMAND.GetData)
            {
                sendPipe(headPipe);
            }
        }
    }
    else
    {
        //chybne checksum
        //resetRxModule();
        netcomRxTimeout();
    }
}
static void sendAccept()
{
    rxExcept = NETCOM_COMMAND.SetMaster;
    
    replyItem.Head[0]=headOpp;

    ushort x = 0 |  (0 << 3) | (NETCOM_DEVID << 7) | (7 << 13);
    replyItem.Head[3]=(char)x;
    replyItem.Head[4]=(char)(x >> 8); 
        
    //checksum
    x=replyItem.Head[3] + replyItem.Head[4];
    replyItem.Head[1]=(char)x;
    replyItem.Head[2]=(char)(x >> 8);        
    
    replyItem.HeadIndex=0;
    sendingItem=&replyItem;
    
    txf=NETCOM_TXFINISH_FN.None;
    startTx(0, NETCOM_COMMAND.None);  
}
static void sendPipe(char pipe)
{
    if(pipe > NETCOM_DATAGET_CAPA)
    {
        sendReply(NETCOM_OUT_STATUS.ReplyErrorPipe);
        return;
    }
    
    NETCOM_DATAIN* p = netcomDataGet[pipe];
    if(p == NULL)
    {
        sendReply(NETCOM_OUT_STATUS.ReplyErrorPipe);
        return;
    }
    else if (p->Locked == 1)
    {
        //pipe je locked (aplikace upravuje data)
        sendReply(NETCOM_OUT_STATUS.ReplyBusy);
        return;
    }
    else if(p->Status == NETCOM_IN_STATUS.Invalid)
    {
        //pipe je invalid, aplikace nenastavila data
        sendReply(NETCOM_OUT_STATUS.ReplyBusy);
        return;        
    }
            
    
    //ok, neni locked, Status=Valid
    p->Locked=1;
    
    replyItem.Head[0]=headOpp;

    char size=getSizeBits(p->DataLen);
    ushort x=size |  (0 << 3) | (NETCOM_DEVID << 7) | (3 << 13);                //return data
    replyItem.Head[3]=(char)x;
    replyItem.Head[4]=(char)(x >> 8); 
        
    //checksum
    x=checksum16(p->Data, p->DataLen);
    x+=replyItem.Head[3];
    x+= replyItem.Head[4];
    replyItem.Head[1]=(char)x;
    replyItem.Head[2]=(char)(x >> 8);        
    
    replyItem.HeadIndex=0;
    replyItem.DataIndex=0;
    sendingItem=&replyItem;
    
    txf=NETCOM_TXFINISH_FN.ReturnData;
    startTx(0, NETCOM_COMMAND.None);     
}
static void sendReply(char repdata)
{
    replyItem.Head[0]=headOpp;

    ushort x = 0 |  (repdata << 3) | (NETCOM_DEVID << 7) | (2 << 13);
    replyItem.Head[3]=(char)x;
    replyItem.Head[4]=(char)(x >> 8); 
        
    //checksum
    x=replyItem.Head[3] + replyItem.Head[4];
    replyItem.Head[1]=(char)x;
    replyItem.Head[2]=(char)(x >> 8);        
    
    replyItem.HeadIndex=0;
    sendingItem=&replyItem;
    
    txf=NETCOM_TXFINISH_FN.None;
    startTx(0, NETCOM_COMMAND.None);    
}


// Error -------------------------------------------------------------------
void UART2Er_interrupt()
{
    //error
}

#endif  //USE_UARTNETCOM