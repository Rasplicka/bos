#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

#ifdef NETCOM_UART

extern ushort netcomStratup_ms;
extern ushort netcomTx_ms;
extern ushort netcomRx_ms;
extern char netcom_devId;
extern char netcom_maxId;

//#define     RS485

#if (defined NETCOM_BOARD_0256 || defined NETCOM_BOARD_0064)
    
    PIN_INFO tx_pin={PORTB_BASE, BIT14};
    
    //switch 4x pin, pro nastaveni device ID
    #define     ID_SW0     PORTB_BASE, BIT5
    #define     ID_SW1     PORTB_BASE, BIT9
    #define     ID_SW2     PORTB_BASE, BIT8
    #define     ID_SW3     PORTB_BASE, BIT7
    //#define     ID_SW4
    //#define     ID_SW5
    //#define     ID_SW6
    //#define     ID_SW7

#elif (defined TEST_BOARD_BOS0)
    PIN_INFO tx_pin={PORTB_BASE, BIT2};
    
#elif (defined TEST_BOARD_BOS1)
    PIN_INFO tx_pin={PORTB_BASE, BIT9};
    
#endif

#define     NC_PRIORITY                 5
#define     NC_SUBPRIORITY              2
#define     FIFO_SIZE                   8    
    
#if (NETCOM_UART == 1 && defined PIC32MM0064) 

#define     TX_FIFO                     U1TXREG
#define     RX_FIFO                     U1RXREG

#define     U_STA                       U1STA
#define     U_STABITS                   U1STAbits
#define     U_MODE                      U1MODE
#define     U_MODEBITS                  U1MODEbits
#define     U_BRG                       U1BRG
    
#define     RX_PRIORITY                 IPC5bits.U1RXIP=NC_PRIORITY; IPC5bits.U1RXIS=NC_SUBPRIORITY
#define     TX_PRIORITY                 IPC6bits.U1TXIP=NC_PRIORITY; IPC6bits.U1TXIS=NC_SUBPRIORITY
#define     ER_PRIORITY                 IPC6bits.U1EIP=NC_PRIORITY; IPC6bits.U1EIS=NC_SUBPRIORITY

#define     IFRX_CLEAR                  IFS0bits.U1RXIF=0
#define     IFTX_CLEAR                  IFS0bits.U1TXIF=0
#define     IFER_CLEAR                  IFS0bits.U1EIF=0

#define     IERX_SET                    IEC0bits.U1RXIE=1
#define     IETX_SET                    IEC0bits.U1TXIE=1
#define     IEER_SET                    IEC0bits.U1EIE=1

#endif    
#if (NETCOM_UART == 2 && defined PIC32MM0064) 

#define     TX_FIFO                     U2TXREG
#define     RX_FIFO                     U2RXREG

#define     U_STA                       U2STA
#define     U_STABITS                   U2STAbits
#define     U_MODE                      U2MODE
#define     U_MODEBITS                  U2MODEbits
#define     U_BRG                       U2BRG
    
#define     RX_PRIORITY                 IPC10bits.U2RXIP=NC_PRIORITY; IPC10bits.U2RXIS=NC_SUBPRIORITY
#define     TX_PRIORITY                 IPC10bits.U2TXIP=NC_PRIORITY; IPC10bits.U2TXIS=NC_SUBPRIORITY
#define     ER_PRIORITY                 IPC10bits.U2EIP=NC_PRIORITY; IPC10bits.U2EIS=NC_SUBPRIORITY

#define     IFRX_CLEAR                  IFS1bits.U2RXIF=0
#define     IFTX_CLEAR                  IFS1bits.U2TXIF=0
#define     IFER_CLEAR                  IFS1bits.U2EIF=0

#define     IERX_SET                    IEC1bits.U2RXIE=1
#define     IETX_SET                    IEC1bits.U2TXIE=1
#define     IEER_SET                    IEC1bits.U2EIE=1

#endif        
#if (NETCOM_UART == 1 && defined PIC32MM0256) 

#define     TX_FIFO                     U1TXREG
#define     RX_FIFO                     U1RXREG

#define     U_STA                       U1STA
#define     U_STABITS                   U1STAbits
#define     U_MODE                      U1MODE
#define     U_MODEBITS                  U1MODEbits
#define     U_BRG                       U1BRG
    
#define     RX_PRIORITY                 IPC13bits.U1RXIP=NC_PRIORITY; IPC13bits.U1RXIS=NC_SUBPRIORITY
#define     TX_PRIORITY                 IPC13bits.U1TXIP=NC_PRIORITY; IPC13bits.U1TXIS=NC_SUBPRIORITY
#define     ER_PRIORITY                 IPC13bits.U1EIP=NC_PRIORITY; IPC13bits.U1EIS=NC_SUBPRIORITY

#define     IFRX_CLEAR                  IFS1bits.U1RXIF=0
#define     IFTX_CLEAR                  IFS1bits.U1TXIF=0
#define     IFER_CLEAR                  IFS1bits.U1EIF=0

#define     IERX_SET                    IEC1bits.U1RXIE=1
#define     IETX_SET                    IEC1bits.U1TXIE=1
#define     IEER_SET                    IEC1bits.U1EIE=1

#endif
#if (NETCOM_UART == 2 && defined PIC32MM0256) 

#define     TX_FIFO                     U2TXREG
#define     RX_FIFO                     U2RXREG

#define     U_STA                       U2STA
#define     U_STABITS                   U2STAbits
#define     U_MODE                      U2MODE
#define     U_MODEBITS                  U2MODEbits
#define     U_BRG                       U2BRG
    
#define     RX_PRIORITY                 IPC14bits.U2RXIP=NC_PRIORITY; IPC14bits.U2RXIS=NC_SUBPRIORITY
#define     TX_PRIORITY                 IPC14bits.U2TXIP=NC_PRIORITY; IPC14bits.U2TXIS=NC_SUBPRIORITY
#define     ER_PRIORITY                 IPC14bits.U2EIP=NC_PRIORITY; IPC14bits.U2EIS=NC_SUBPRIORITY

#define     IFRX_CLEAR                  IFS1bits.U2RXIF=0
#define     IFTX_CLEAR                  IFS1bits.U2TXIF=0
#define     IFER_CLEAR                  IFS1bits.U2EIF=0

#define     IERX_SET                    IEC1bits.U2RXIE=1
#define     IETX_SET                    IEC1bits.U2TXIE=1
#define     IEER_SET                    IEC1bits.U2EIE=1

#endif
#if (NETCOM_UART == 3 && defined PIC32MM0256) 

#define     TX_FIFO                     U3TXREG
#define     RX_FIFO                     U3RXREG

#define     U_STA                       U3STA
#define     U_STABITS                   U3STAbits
#define     U_MODE                      U3MODE
#define     U_MODEBITS                  U3MODEbits
#define     U_BRG                       U3BRG
    
#define     RX_PRIORITY                 IPC14bits.U3RXIP=NC_PRIORITY; IPC14bits.U3RXIS=NC_SUBPRIORITY
#define     TX_PRIORITY                 IPC15bits.U3TXIP=NC_PRIORITY; IPC15bits.U3TXIS=NC_SUBPRIORITY
#define     ER_PRIORITY                 IPC15bits.U3EIP=NC_PRIORITY; IPC15bits.U3EIS=NC_SUBPRIORITY

#define     IFRX_CLEAR                  IFS1bits.U3RXIF=0
#define     IFTX_CLEAR                  IFS1bits.U3TXIF=0
#define     IFER_CLEAR                  IFS1bits.U3EIF=0

#define     IERX_SET                    IEC1bits.U3RXIE=1
#define     IETX_SET                    IEC1bits.U3TXIE=1
#define     IEER_SET                    IEC1bits.U3EIE=1

#endif    

// <editor-fold defaultstate="collapsed" desc="SW0-7">
#ifdef ID_SW0
static const PIN_INFO id_sw0 = {ID_SW0};
#endif 
#ifdef ID_SW1
static const PIN_INFO id_sw1 = {ID_SW1};
#endif 
#ifdef ID_SW2
static const PIN_INFO id_sw2 = {ID_SW2};
#endif 
#ifdef ID_SW3
static const PIN_INFO id_sw3 = {ID_SW3};
#endif     
#ifdef ID_SW4
static const PIN_INFO id_sw4 = {ID_SW4};
#endif 
#ifdef ID_SW5
static const PIN_INFO id_sw5 = {ID_SW5};
#endif 
#ifdef ID_SW6
static const PIN_INFO id_sw6 = {ID_SW6};
#endif 
#ifdef ID_SW7
static const PIN_INFO id_sw7 = {ID_SW7};
#endif  
// </editor-fold>
    
//global fn
void netcomInit();
void netcomSetData(NETCOM_DATAOUT* data);
void netcomNotRespond();
void netcomRxTimeout();
void netcomInitBus();

void UART2Rx_interrupt();
void UART2Tx_interrupt();
void UART2Er_interrupt();

static void loadDeviceIDFromPin();
static void fillHeadSetData(NETCOM_DATAOUT* data);
static void fillHeadGetData(NETCOM_DATAOUT* data);
static void fillHeadReturnData(NETCOM_DATAOUT* item);
static void fillHeadControl(NETCOM_DATAOUT* item, char comm);

//local fn
static char initModule();
static void clearRxFifo();
static void enableTx();
static void enableRx();
static char getSizeBits(ushort size);
static ushort getSizeValue(char bits);
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
//static void sendAccept();
static int isReply(char command);
static void sendPipe(char pipe);
static void sendControl(char comm, char exception);
static inline void clearStartupTimer(void* fn1, char x, void* fn2);

//local var
static char nra=0;
static char txf=0;
static char rxExcept=0;
static char nextID=0;
static char haveNextID=0;

static ushort headSize=0;
static char headCommand=0;
static char headType=0;
static char headPipe=0;
static char headOpp=0;
static ushort headChecksum=0;
static ushort calcChecksum=0;

static char* getBuffer=NULL;
static char ra=0;
static char rxStatus=0;

static char IDPlusOne=0;
static char oneMaster=0;
static char canBeMaster=0;   
static char nextIDCounter=0;

NETCOM_DATAOUT* sendingItem;                            //aktualni polozka, kterou Master odesila
NETCOM_DATAOUT master1Item;
NETCOM_DATAOUT master2Item;
NETCOM_DATAOUT master8Item;
NETCOM_DATAOUT replyItem;

void (*_finish)();

// global fn -------------------------------------------------------------------
void netcomInit()
{
    
    
#ifdef NETCOM_DEVID
    netcom_devId=NETCOM_DEVID;
#else     
    loadDeviceIDFromPin();
#endif

    if(netcom_devId<1){ return; }                                 //neni nastaveni device ID
    
#ifdef NETCOM_MAXID
    netcom_maxId=NETCOM_MAXID;
#else
    int x=netcom_devId+16;
    if(x>0xFF) { x=0xFF; }
    netcom_maxId=(char)x;
#endif    
    
    if(netcom_devId==1) { netcomStratup_ms=_STARTUP_MS; }         //pouze device ID 1
    else { netcomStratup_ms=0; }
    netcomTx_ms=0;
    netcomRx_ms=0;

    if(netcom_maxId<netcom_devId){netcom_maxId=netcom_devId;}
    if(netcom_maxId<2){netcom_maxId=2;}
    
    if(netcom_maxId==netcom_devId){IDPlusOne=1;}
    else{IDPlusOne=netcom_devId+1;}
    
#ifdef NETCOM_ONE_MASTER
    oneMaster=1;
#endif   
    
#ifdef NETCOM_CAN_BE_MASTER
    canBeMaster=1;
#endif    
    
    //destation ID=1
    master8Item.OppID=1;
    fillHeadControl(&master8Item, NETCOM_OUT_STATUS.Master8);
    master8Item.DataLen=0;
    master8Item.DataIndex=0;
    
    master1Item.DataLen=0;              //nebude vysilat data
    master1Item.DataIndex=0;
    
    master2Item.DataLen=0;              //nebude vysilat data
    master2Item.DataIndex=0;    
    
    initModule();
}

void netcomSetData(NETCOM_DATAOUT* data)
{
    //doplni polozky level1 do struktury data
    //vyplni tato fce (level 1)
    //Head, Error, Status, HeadIndex=0, DataIndex=0
    
    //vyplni app
    //appID, oppID, Pipe, Data (buffer), DataLen
    
    //Pro pripad ukonceni aplikace?
    //data->AppID=getProcID();
    
    //head
    //data->Head[0]=data->OppID;

    //set, po odelsani bude nastaveno data.Status
    fillHeadSetData(data);
    
    //Set, direction=0
    data->Direction=0;
    
    //error
    data->Error=0;
    
    //status
    data->Status=NETCOM_OUT_STATUS.WaitToTx;

    //vlozi polozku do netcomDataOut, pokud je plne, ceka na uvolneni
    while(dataOutAdd(data)==0)
    {
        doEvents();
    }
}
void netcomGetData(NETCOM_DATAOUT* data)
{
    //doplni polozky level1 do struktury data
    //vyplni tato fce (level 1)
    //Head, Error, Status, HeadIndex=0, DataIndex=0
    
    //vyplni app
    //appID, oppID, Pipe, Data (buffer), DataLen
    
    //get (vraci data v data.Data, size v data.DataLen)
    //data.Data musi byt nastaveno na buffer, kam budou get data zapsana
    //data.DataLen musi obsahovat velikost bufferu
    //vysledek podle data.Status (pri ReplyOK data.DataLen obsahuje velikost dat)
    //jinak data.Status obsahuje kod chyby
    
    //Pro pripad ukonceni aplikace?
    //data->AppID=getProcID();
    
    //head
    //data->Head[0]=data->OppID;
    
    fillHeadGetData(data);
    
    //Get, direction=1
    data->Direction=1;
    
    //error
    data->Error=0;
    
    //status
    data->Status=NETCOM_OUT_STATUS.WaitToTx;

    //vlozi polozku do netcomDataOut, pokud je plne, ceka na uvolneni
    while(dataOutAdd(data)==0)
    {
        doEvents();
    }
}

void netcomRxTimeout()
{
    //zachytil svoji adresu, ale prijem dat nebyl dokoncen ve stanovene lhute
    
    U_STABITS.URXEN=0;              //disable Rx
    U_STABITS.UTXEN=0;              //disable Tx
    clearRxFifo();
    ra=0;
    netcomRx_ms=0;
    rxExcept = NETCOM_EXCEPTION.None;
    
    if(netcomTx_ms>0)
    {
        //Master: ceka na data (ktera prisla chybna, chyba cteni v tomto modulu)
        netcomNotRespond();
        netcomTx_ms=0;
    }
}
void netcomNotRespond()
{
    //master odeslal data, ceka na odpoved, ta neprisla ve stanovene lhute
    
    clearStartupTimer(&notRespondAct, NETCOM_TXFINISH_FN.NotRespondAct, &master8);

    /*
    if(netcom_devId==1)    
    {
        //devID=1, neposila master8
        netcomStratup_ms=_STARTUP_MS;
        notRespondAct();
    }
    else
    {
        //neni devID=1, posila master8 (informuje DevID1, ze probiha komunikace)
        txf=NETCOM_TXFINISH_FN.NotRespondAct;
        master8();
    }
    */
}
void netcomInitBus()
{
    setPin(&LED3);
    
    //initBus, start comunication (after 100ms pause, DevID=1 only)
    netcomStratup_ms=_STARTUP_MS;
    
    U_STABITS.URXEN=0;      //Rx disable
    U_STABITS.UTXEN=0;      //Rx disable
    clearRxFifo();
    
    enableRx();
    startMaster();
}


static void loadDeviceIDFromPin()
{
    netcom_devId=0;
    short x;
    
#if (defined  ID_SW0)
    x=getPin(&id_sw0);
    if(x>0){netcom_devId |= 0b1; }
#endif
    
#if (defined  ID_SW1)
    x=getPin(&id_sw1);
    if(x>0){netcom_devId |= 0b10; }
#endif    
    
#if (defined  ID_SW2)
    x=getPin(&id_sw2);
    if(x>0){netcom_devId |= 0b100; }
#endif    
    
#if (defined  ID_SW3)
    x=getPin(&id_sw3);
    if(x>0){netcom_devId |= 0b1000; }
#endif    
    
#if (defined  ID_SW4)
    x=getPin(&id_sw4);
    if(x>0){netcom_devId |= 0b10000; }
#endif 
    
#if (defined  ID_SW5)
    x=getPin(&id_sw5);
    if(x>0){netcom_devId |= 0b100000; }
#endif    
    
#if (defined  ID_SW6)
    x=getPin(&id_sw6);
    if(x>0){netcom_devId |= 0b1000000; }
#endif     
    
#if (defined  ID_SW7)
    x=getPin(&id_sw7);
    if(x>0){netcom_devId |= 0b10000000; }
#endif     
   
}

static void fillHeadSetData(NETCOM_DATAOUT* data)
{
    
    data->Head[0]=data->OppID;
    data->Head[4]=netcom_devId; //NETCOM_DEVID;
    
    short chksm=0;
    
    char size=getSizeBits(data->DataLen);
    data->Head[3]=NETCOM_HEADTYPE.SetData | (size<<3);                            //comm=0, size
    data->Head[5]=data->Pipe;
        
    //checksum, data + head[5-8]
   
    if(size==0)
    {
        //velikost buferu 0, pouze HeadData
        data->Head[7]=(char)data->HeadData;
        data->Head[8]=(char)(data->HeadData >> 8);
    }
    else
    {
        //velikost buferu > 0, obsahuje data
        chksm = checksum16(data->Data, data->DataLen);
        data->Head[7]=0;
        data->Head[8]=0;
    }
        
    chksm += data->Head[5];
    chksm += data->Head[6];
    chksm += data->Head[7];
    chksm += data->Head[8];

    short x = (short)(data->Head[4]<<8);
    x |= data->Head[3];
    
    x += chksm;
    data->Head[1]=(char)x;
    data->Head[2]=(char)(x >> 8);  
    data->HeadLen=8;
    data->HeadIndex=0;
}
static void fillHeadGetData(NETCOM_DATAOUT* data)
{
    data->Head[0] = data->OppID;
    data->Head[4] = netcom_devId; //NETCOM_DEVID;
    data->Head[3] = NETCOM_HEADTYPE.GetData | (data->Pipe<<3);                            //comm=0, pipe
    
    data->Head[1]=data->Head[3];
    data->Head[2]=data->Head[4];
    data->HeadLen=4;
    data->HeadIndex=0;
}
static void fillHeadReturnData(NETCOM_DATAOUT* item)
{
    //item = ReplyItem, kde je nastaveno Data, DataLen, atd...
    item->Head[0]=item->OppID;
    item->Head[4]=netcom_devId; //NETCOM_DEVID;
    
    char size=getSizeBits(item->DataLen);
    item->Head[3]=NETCOM_HEADTYPE.ReturnData | (size<<3);                            //comm=0, size
    

    short chksm = checksum16(item->Data, item->DataLen);
    short x = (short)(item->Head[4]<<8);
    x |= item->Head[3];
    
    x += chksm;
    item->Head[1]=(char)x;
    item->Head[2]=(char)(x >> 8);     
    
    item->HeadLen=4;
    item->HeadIndex=0;    
}
static void fillHeadControl(NETCOM_DATAOUT* item, char comm)
{
    //item = ReplyItem, kde je nastaveno Data, DataLen, atd...
    item->Head[0]=item->OppID;
    item->Head[4]=netcom_devId; //NETCOM_DEVID;
    item->Head[3]=NETCOM_HEADTYPE.Control | (comm<<3);                            //comm=Control, command (ReplyOK/ERR..., M1/2/8,Acc... )
    
    item->Head[1]=item->Head[3];
    item->Head[2]=item->Head[4];
    item->HeadLen=4;
    item->HeadIndex=0;
}

// local fn --------------------------------------------------------------------
static char initModule()
{
    U_MODE=0;
    U_STA=0;
    
    RX_PRIORITY;
    TX_PRIORITY;
    ER_PRIORITY;
    
    IFRX_CLEAR;
    IFTX_CLEAR;
    IFER_CLEAR;
    
    IERX_SET;
    IETX_SET;
    IEER_SET;
    
    //UART2
    U_MODEBITS.ACTIVE=0;
    U_MODEBITS.SLPEN=0;     
    U_MODEBITS.CLKSEL=3;    //REFO
    U_MODEBITS.OVFDIS=1;    //Rx pri overflow
    U_MODEBITS.SIDL=0;      //continue in IDLE
    U_MODEBITS.IREN=0;      //IR disable
#ifdef RS485
    //Rx nebo Tx ridi RTS (RS485 driver)
    U_MODEBITS.UEN=1;       //Rx, Tx a RTS
#else 
    //Tx a Rx jsou propojeny, (Tx pin pri prijimani dat prepne do Input modu)
    U_MODEBITS.UEN=0;       //pouze Rx a Tx
#endif    
    U_MODEBITS.WAKE=0;      //wake up SLEEP disable
    U_MODEBITS.LPBACK=0;    //Loop disable
    U_MODEBITS.ABAUD=0;     //autobaud disable
    U_MODEBITS.RXINV=0;     //
    U_MODEBITS.BRGH=0;      //std. speed (16x)
    U_MODEBITS.PDSEL=3;     //9-bit, no parity
    U_MODEBITS.STSEL=0;     //1x stop bit
    
    //povoli prijem dat, zakaze vysilani
    U_STABITS.MASK=0xFF;     //0xFF
    U_STABITS.ADDR=netcom_devId;  //NETCOM_DEVID; //DevID
    U_STABITS.UTXISEL=1;    //Tx interrupt, po odvysilani
    U_STABITS.UTXINV=0;     //
    U_STABITS.URXEN=1;      //Rx enable
    U_STABITS.UTXBRK=0;     //send break disable
    U_STABITS.UTXEN=0;      //Tx disable
    U_STABITS.URXISEL=1;    //Rx interrupt 4 bytes ve FIFO
    U_STABITS.ADDEN=1;      //addres detect enable
    
    U_BRG=2;               //29=100k baud BRG=Clk/(16xbaud) -1
    
    
    enableRx();
    
    //start
    U_MODEBITS.ON=1;
}
static void clearRxFifo()
{
    short x;
    while(U_STABITS.URXDA)
    {
        x=RX_FIFO;
    }
}
static void enableTx()
{
    //ceka na dokonceni predchoziho prijmu data (stop bit)
    int a=30;
    while(U_STABITS.RIDLE==0 || a>0) { a--; }

    //U2MODEbits.LPBACK=0;
    //nastavi modul na vysilani
    U_STABITS.URXEN=0;      //Rx disable
    
#ifndef RS485    
    //neni RS485, Tx a Rx jsou propojeny
    //Tx pin=Output, vysilani
    setPortDigOut(tx_pin.portBase, tx_pin.pin);
#endif         
    U_STABITS.UTXEN=1;      //Tx enable
    
}
static void enableRx()
{
    //ceka na dokonceni predchoziho vysilani data (stop bit)
    
    while(U_STABITS.TRMT==0) { }
    
    //nastavi modul na prijem Rx
    
 
#ifndef RS485    
    //neni RS485, Tx a Rx jsou propojeny
    //Tx pin=Input, nevysila
    setPortDigIn(tx_pin.portBase, tx_pin.pin);
#endif    
    U_STABITS.UTXEN=0;      //Tx disable
    U_STABITS.URXEN=1;      //Rx enable
}
static char getSizeBits(ushort size)
{
    if (size == 0) {return 0;}
    else if (size == 4) {return 1;}
    else if (size == 8) {return 2;}
    else if (size == 12) {return 3;}    
    else if (size == 16) {return 4;} 
    else if (size == 20) {return 5;} 
    else if (size == 24) {return 6;} 
    else if (size == 28) {return 7;} 
    else if (size == 32) {return 8;}    
    else if (size == 40) {return 9;}    
    else if (size == 64) {return 10;}
    else if (size == 80) {return 11;} 
    else if (size == 128) {return 12;} 
    else if (size == 256) {return 13;}    
    else if (size == 512) {return 14;}    
    else if (size == 1024) {return 15;}
    
    else {return 0;}
}
static ushort getSizeValue(char bits)
{
    if (bits == 0) {return 0;}
    else if (bits == 1) {return 4;}
    else if (bits == 2) {return 8;}
    else if (bits == 3) {return 12;}    
    else if (bits == 4) {return 16;}    
    else if (bits == 5) {return 20;}    
    else if (bits == 6) {return 24;}    
    else if (bits == 7) {return 28;}   
    else if (bits == 8) {return 32;}
    else if (bits == 9) {return 40;}    
    else if (bits == 10) {return 64;}    
    else if (bits == 11) {return 80;}    
    else if (bits == 12) {return 128;}    
    else if (bits == 13) {return 256;}  
    else if (bits == 14) {return 512;}
    else if (bits == 15) {return 1024;}    
     
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
    if(sendingItem->Status==0)
    {
        sendingItem->Status=1;
    }
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
#if (defined NETCOM_UART && NETCOM_UART == 1) 
void UART1Tx_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 2) 
void UART2Tx_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 3)
void UART3Tx_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 4)
void UART4Tx_interrupt()
#endif
{
    
    if(sendingItem != NULL)
    {
        txWriteFifo();
    }
    IFTX_CLEAR;
    //IFS1bits.U2TXIF=0;
}
static void startMaster()
{
   
    //vola se, pokud ma zacit odesilat data
    if(netcomDataOut[0] != NULL && netcomDataOut[0]->Status == NETCOM_OUT_STATUS.WaitToTx)
    {
        //netcomStratup_ms=_STARTUP_MS;
        
        //Odesila data
        sendingItem=netcomDataOut[0];
        
        //sendingItem->HeadIndex=0;                                   //bude vysilat head
        nra=NOT_RESPONSE_ACTION.Data;                               //pri notResponse bude opakovat vysilani (max. 3x)
        txf=NETCOM_TXFINISH_FN.None;                                //po skonceni vysilani zadna akce (ceka na odpoved)
        
        if(sendingItem->Direction==0)
        {
            //set
            sendingItem->DataIndex=0;                                //bude vysilat data (pokud DataLen>0, nastavuje app)
            startTx(_TX_LONG_MS, NETCOM_EXCEPTION.Reply);                        //ocekava REPLY (OK, nebo s kodem chyby)
        }
        else
        {
            //get
            sendingItem->DataIndex=sendingItem->DataLen;            //DataLen obsahuje velikost bufferu, DataIndex zabrani vysilani dat 
            startTx(_TX_LONG_MS, NETCOM_EXCEPTION.ReturnData);                   //ocekava data, nebo REPLY s kodem chyby
        }
    }
    else
    {
        //nema data, preda fci master
        nextMaster1();
    }
}
static void nextMaster1()
{
    //nextID

    if(oneMaster==1)
    {
        //Pouze toto je master, po 10ms znova startMaster
        netcomTx_ms=10;
        nra=NOT_RESPONSE_ACTION.StartMaster;
        return;
    }
    
    if(haveNextID==2)
    {
        //nove hledani nextID
        nextID=0;
    }
    
    if(nextID==0)
    {
        //nextID neni inicializovano, nove hledani nextID
        nextID=IDPlusOne;
        haveNextID=0;
    }
    else if(haveNextID==0)
    {
        nextID++;
        if(nextID > netcom_maxId){ nextID=1; }
        
        if(nextID == netcom_devId)
        {
            //prosel vsechny ID a nenasel jiny master, po 5ms startMaster
            netcomTx_ms=5;
            nra=NOT_RESPONSE_ACTION.StartMaster;
            return;            
        }
    }

    netcomTxCTRL++;
    //ma nextID, kteremu se pokusi predat master
    master1Item.OppID=nextID;
    fillHeadControl(&master1Item, NETCOM_OUT_STATUS.Master1);
    
    //master1Item.HeadIndex=0;
    sendingItem=&master1Item;
    
    nra=NOT_RESPONSE_ACTION.NextMaster;
    txf=NETCOM_TXFINISH_FN.None;
    startTx(_TX_SHORT_MS, NETCOM_EXCEPTION.AcceptMaster);
}
static void nextMaster2()
{
    netcomTxCTRL++;
    //addr
    master2Item.OppID=nextID;
    fillHeadControl(&master2Item, NETCOM_OUT_STATUS.Master2);
    
    //master2Item.HeadIndex=0;
    sendingItem=&master2Item;
    
    txf=NETCOM_TXFINISH_FN.None;
    startTx(0, NETCOM_EXCEPTION.None);   
    
    //odevzdal master
    clearPin(&LED2);
}
static void master8()
{
    //master8Item.HeadIndex=0;
    netcomTxCTRL++;
    sendingItem=&master8Item;
    //txf=NETCOM_TXFINISH_FN.NextMaster2;
    startTx(0, NETCOM_EXCEPTION.None);    
}
static void startTx(char ms, char exc)
{
    netcomTx_ms=ms;
    rxExcept=exc;

    ushort x;
    //adresa, b8=1
    x=(ushort)sendingItem->Head[0];
    x |= 0x100;
    TX_FIFO=x;
        
    //data b8=0
    x=(ushort)sendingItem->Head[1];
    TX_FIFO=x;
    x=(ushort)sendingItem->Head[2];
    TX_FIFO=x;
    x=(ushort)sendingItem->Head[3];
    TX_FIFO=x;
    x=(ushort)sendingItem->Head[4];
    TX_FIFO=x;

    enableTx();

    //head[5-8] muze primo zapsat do bufferu
    if(sendingItem->HeadLen==8)
    {
        x=(ushort)sendingItem->Head[5];
        TX_FIFO=x;
        x=(ushort)sendingItem->Head[6];
        TX_FIFO=x;
        x=(ushort)sendingItem->Head[7];
        TX_FIFO=x;
        x=(ushort)sendingItem->Head[8];
        TX_FIFO=x;   
    }
}
static void txWriteFifo()
{
    if(sendingItem->DataIndex < sendingItem->DataLen)
    {
        //odeslat dalsi data
        ushort x;
        char c=0;

        while((sendingItem->DataIndex < sendingItem->DataLen) && (c < FIFO_SIZE))
        {
            x=(ushort)sendingItem->Data[sendingItem->DataIndex];
            TX_FIFO=x;  
            sendingItem->DataIndex++;
            c++;
            netcomTxBytes++;
        }
    }
    else
    {
        //?
#ifndef RS485        
        setPortDigIn(tx_pin.portBase, tx_pin.pin);
#endif        
        
        //odeslano vsechno
        if (txf==NETCOM_TXFINISH_FN.NextMaster1)
        {
            nextMaster1();
            return;                                         //zustava Tx mode
        }
        else if (txf==NETCOM_TXFINISH_FN.NextMaster2)
        {
            nextMaster2();
            return;                                         //zustava Tx mode
        }
        else if (txf==NETCOM_TXFINISH_FN.NotRespondAct)
        {
            notRespondAct();
            return;                                         //zustava Tx mode
        }

        else if(txf==NETCOM_TXFINISH_FN.StartMaster)
        {
            startMaster();
            return;                                         //zustava Tx mode
        }
        else if (txf==NETCOM_TXFINISH_FN.ReturnData)
        {
            //SLAVE: po odeslani odpovedi (dat) na get
            netcomDataGet[headPipe]->Locked=0;              //unlock pipe
            sendingItem=NULL;
        }
        else
        {
            //txf==NETCOM_TXFINISH_FN.None
            //Neni master, nebo predal mastera, nebo ceka na data (get)
            //Zadna akce, pouze ceka v Rx mode            
        }
        
        enableRx(); 
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
            netcomSendDataError++;
            sendingItem->Error++;
            
            //sendingItem->HeadIndex=0;
            //nra zustava NOT_RESPONSE_ACTION.Data
            txf=NETCOM_TXFINISH_FN.None;                                        //po odvysilani zadna akce (ceka na odpoved)
            if(sendingItem->Direction==0) 
            { 
                //set
                sendingItem->DataIndex=0;
                startTx(_TX_LONG_MS, NETCOM_EXCEPTION.Reply); 
            }     
            else 
            { 
                //get
                sendingItem->DataIndex=sendingItem->DataLen;
                startTx(_TX_LONG_MS, NETCOM_EXCEPTION.ReturnData); 
            }                              
        }
    }
    else if(nra==NOT_RESPONSE_ACTION.StartMaster)
    {
        startMaster();
    }
    else
    {
        //cekalo se AcceptMaster, ale neprisla odpoved, bude hledat dalsi master
        haveNextID=0;
        nextMaster1();
    }
}


// Rx --------------------------------------------------------------------------
#if (defined NETCOM_UART && NETCOM_UART == 1) 
void UART1Rx_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 2) 
void UART2Rx_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 3)
void UART3Rx_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 4)
void UART4Rx_interrupt()
#endif
{
    ushort h1, h2, h3, h4;
    h1=RX_FIFO;
    h2=RX_FIFO;
    h3=RX_FIFO;
    h4=RX_FIFO;
     
    if(ra==0)
    {
        //head
        netcomRx_ms=_RX_LONG_MS;                                      //spusti citani

        headType=h3 & 0b00000111;
        headCommand=h3>>3;
        headOpp=h4;
         
        //init checksum
        headChecksum=h1 | (((ushort)h2) << 8);
        calcChecksum=h3 | (((ushort)h4) << 8);
        
        if(headType==NETCOM_HEADTYPE.Control)
        {
            //master: Reply, Accept, Slave: setMaster(1,2,8)
            netcomRxCTRL++;
            onChecksum();
        }
        else if(headType==NETCOM_HEADTYPE.GetData)
        {
            //slave prijal getData (bode odesilat data)
            netcomRxCTRL++;
            headPipe=headCommand;
            onChecksum();
        }
                
        else if (headType==NETCOM_HEADTYPE.ReturnData)
        {
            //master prijima data (odpoved na get)
            headSize=getSizeValue(headCommand);
            if(sendingItem != NULL && sendingItem->Data != NULL)
            {
                if(sendingItem->DataLen >= headSize)
                {
                    //ok, muze prijimat data
                    sendingItem->DataLen=headSize;
                    //rxIndex=0;
                    getBuffer=sendingItem->Data;
                    rxStatus=NETCOM_OUT_STATUS.ReplyOk;
                }
                else
                {
                    //data jsou moc velka
                    getBuffer=NULL;
                    rxStatus=NETCOM_OUT_STATUS.GetBufferSize;
                }
            }
            else
            {
                //buffer neexistuje (musi ho nastavit volajici fce, NETCOM_DATAOUT.Data)
                getBuffer=NULL;
                rxStatus=NETCOM_OUT_STATUS.GetBufferNotExist;
            }
            
            //budou nasledovat data
            ra=2;
        }
        else if(headType==NETCOM_HEADTYPE.SetData)
        {
            //slave prijima data (set)
            //netcomRxCTRL++;
            headSize = getSizeValue(headCommand);
            //bude nasledovat druha cast head
            ra=1;   
        }
    }
    else if (ra==1)
    {
        //druha cast head setData (prijima slave)
        calcChecksum += h1 + h2 + h3 + h4;
        headPipe=h1;
        
        if(headSize==0)
        {
            //data v head (16-bit HeadData)
            headSize=2;
        }
        else
        {
            ra=2;                                   //budou nasledovat data            
        }

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
            else if(headSize > p->DataCapacity)
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

            else if (headSize==2)
            {
                //prijal 2 bytes v head (HeadData)
                p->Data[0]=h3;
                p->Data[1]=h4;
                netcomRxBytes+=2;
                rxStatus=NETCOM_OUT_STATUS.ReplyOk;
                onChecksum();
            }
            else
                
            {
                //ok, muze prijimat data
                rxStatus=NETCOM_OUT_STATUS.ReplyOk;
                //rxIndex=0;
                getBuffer=p->Data;
            }
        }
        
    }
    else 
    {
        //ra=2, prijem dat
        //checksum
        netcomRxBytes+=4;
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
    
    //IFS1bits.U2RXIF=0;
    IFRX_CLEAR;
}

static void onChecksum()
{
    netcomRx_ms=0;                      //ukonci citani
    ra=0;                               //nasleduje head
    int a=0;
    
    if(calcChecksum==headChecksum)
    {
        //for(a=0; a<100; a++){}
        
        //checksum ok
        if(rxExcept == NETCOM_EXCEPTION.AcceptMaster)
        {
            // <editor-fold defaultstate="collapsed" desc="AcceptMaster/NotAcceptMaster">
            //ocekava AcceptMaster / NotAcceptMaster
            if (headCommand == NETCOM_OUT_STATUS.AcceptMaster) 
            {
                //Master: prijal AcceptMaster, odeslat master2
                haveNextID = 1; //nasel platne nextID (prisla odpoved)

                if (nextID != IDPlusOne) 
                {
                    nextIDCounter++;
                    if (nextIDCounter > 10) 
                    {
                        //nove skenovani nextID
                        nextIDCounter = 0;
                        haveNextID=2;
                        //nextID = 0;
                    }
                }

                clearStartupTimer(&nextMaster2, NETCOM_TXFINISH_FN.NextMaster2, &master8);
            } 
            else if (headCommand == NETCOM_OUT_STATUS.NotAcceptMaster) 
            {
                //Master: prijal NotAcceptMaster, (nextMaster1, hleda dalsi master)
                haveNextID = 0;
                
                clearStartupTimer(nextMaster1, NETCOM_TXFINISH_FN.NextMaster1, master8);
                /*
                #if (defined NETCOM_DEVID && NETCOM_DEVID <= 1)
                    netcomStratup_ms = _STARTUP_MS;
                    nextMaster1();
                #else
                    txf = NETCOM_TXFINISH_FN.NextMaster1;
                    master8();
                #endif
                */    
            }
            
            // </editor-fold>
        }
        
        else if(rxExcept == NETCOM_EXCEPTION.Reply && isReply(headCommand))
        {
            // <editor-fold defaultstate="collapsed" desc="Reply">
            //Master: prijal Reply
            sendingItem->Status = headCommand; //Reply data
            //if(headCommand==0)
            //{
            //    headCommand=1;
            //}
            dataOutRemove();

            clearStartupTimer(startMaster, NETCOM_TXFINISH_FN.StartMaster, master8);

            // </editor-fold>
        }
        
        else if(rxExcept == NETCOM_EXCEPTION.ReturnData)
        {
            // <editor-fold defaultstate="collapsed" desc="ReturnData/Reply">
            if (headType == NETCOM_HEADTYPE.ReturnData) 
            {
                //Master: prijal data, jako odpoved na get
                sendingItem->Status = rxStatus; //ok, nebo chyba (pokud data obsahovala chybu)
            } 
            else 
            {
                //Master: misto dat prijal Reply, jako odpoved na get
                //Reply data (obsahuje chybu, proc nebyla odeslana data)
                sendingItem->Status = headCommand; 
            }
            dataOutRemove();             

            clearStartupTimer(startMaster, NETCOM_TXFINISH_FN.StartMaster, master8);

            // </editor-fold>
        }   
        
        else if(headType == NETCOM_HEADTYPE.Control)
        {
            // <editor-fold defaultstate="collapsed" desc="setMaster1/2/8">
            //Slave: prijal setMaster(1,2,8)
            if (headCommand == NETCOM_OUT_STATUS.Master1) 
            {
                //setMaster1
                if (canBeMaster == 1) 
                {
                    replyItem.OppID = headOpp;
                    sendControl(NETCOM_OUT_STATUS.AcceptMaster, NETCOM_EXCEPTION.SetMaster2);
                } 
                else 
                {
                    replyItem.OppID = headOpp;
                    sendControl(NETCOM_OUT_STATUS.NotAcceptMaster, NETCOM_EXCEPTION.None);
                }
            } 
            else if (headCommand == NETCOM_OUT_STATUS.Master2) 
            {
                //setMaster2
                if (rxExcept == NETCOM_EXCEPTION.SetMaster2) 
                {
                    //ocekaval setMaster2 (prevezme master)
                    setPin(&LED2);
                    startMaster();
                }
            } 
            else 
            {
                //master8
                //#if (defined NETCOM_DEVID && NETCOM_DEVID <= 1)
                if(netcom_devId==1)
                {
                    //pouze DevID=1
                    netcomStratup_ms = _STARTUP_MS;
                }
                //#endif          
            }
            
            // </editor-fold>
        }
        
        else
        {
            // <editor-fold defaultstate="collapsed" desc="set/get data">
            //slave prijal set, nebo get
            if (headType == NETCOM_HEADTYPE.SetData) 
            {
                if (rxStatus == NETCOM_OUT_STATUS.ReplyOk) 
                {
                    //data jsou OK (checksum)
                    netcomDataSet[headPipe]->DataLen = headSize;
                    netcomDataSet[headPipe]->DataIndex = 0;
                    netcomDataSet[headPipe]->Status = NETCOM_IN_STATUS.Full;
                }
                replyItem.OppID = headOpp;
                //if(rxStatus==0)
                //{
                //    rxStatus=1;
                //}
                sendControl(rxStatus, NETCOM_EXCEPTION.None);
            }
            else if (headType == NETCOM_HEADTYPE.GetData) 
            {
                sendPipe(headPipe);
            }
            
            // </editor-fold>
        }
    }
    else
    {
        //chybne checksum
        netcomChkError++;
        netcomRxTimeout();
    }
}
static int isReply(char command)
{
    //Reply kody 0-15 (OK, Error, ...)
    if(command <= 15) {return 1;}
    else {return 0;}
}
static void sendPipe(char pipe)
{
    if(pipe > NETCOM_DATAGET_CAPA)
    {
        replyItem.OppID=headOpp;
        sendControl(NETCOM_OUT_STATUS.ReplyErrorPipe, NETCOM_EXCEPTION.None);
        return;
    }
    
    NETCOM_DATAIN* p = netcomDataGet[pipe];
    if(p == NULL)
    {
        replyItem.OppID=headOpp;
        sendControl(NETCOM_OUT_STATUS.ReplyErrorPipe, NETCOM_EXCEPTION.None);        
        return;
    }
    else if (p->Locked == 1)
    {
        //pipe je locked (aplikace upravuje data)
        replyItem.OppID=headOpp;
        sendControl(NETCOM_OUT_STATUS.ReplyBusy, NETCOM_EXCEPTION.None);         
        return;
    }
    else if(p->Status == NETCOM_IN_STATUS.Invalid)
    {
        //pipe je invalid, aplikace nenastavila data
        replyItem.OppID=headOpp;
        sendControl(NETCOM_OUT_STATUS.ReplyBusy, NETCOM_EXCEPTION.None);  
        return;        
    }
    
    //ok, neni locked, Status=Valid
    p->Locked=1;
    
    replyItem.Data=p->Data;
    replyItem.DataLen=p->DataLen;
    replyItem.OppID=headOpp;

    //head (+ headLen a headIndex)
    fillHeadReturnData(&replyItem);
    
    replyItem.DataIndex=0;
    sendingItem=&replyItem;
    
    txf=NETCOM_TXFINISH_FN.ReturnData;
    startTx(0, NETCOM_EXCEPTION.None);     
}
static void sendControl(char comm, char exception)
{
    netcomTxCTRL++;
    //Reply, AcceptMaster,...
    //head (+ headLen a headIndex)
    fillHeadControl(&replyItem, comm);
    
    replyItem.DataLen=0;                            //zadna data
    replyItem.HeadIndex=0;
    sendingItem=&replyItem;
    
    txf=NETCOM_TXFINISH_FN.None;
    startTx(0, exception);    
}


static inline void clearStartupTimer(void* fn1, char x, void* fn2)
{
    //fn1 - vola se pri thidID=1
    //fn2 - vila se pri netcom_devId>1, txf se nastavi na x
    if(netcom_devId==1)
    {
        netcomStratup_ms = _STARTUP_MS;
        _finish=fn1;
        _finish();
        //fn1;
    }
    else
    {
        txf = x;
        _finish=fn2;
        _finish();
    }
}

// Error -------------------------------------------------------------------
#if (defined NETCOM_UART && NETCOM_UART == 1) 
void UART1Er_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 2) 
void UART2Er_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 3)
void UART3Er_interrupt()
#elif (defined NETCOM_UART && NETCOM_UART == 4)
void UART4Er_interrupt()
#endif
{
    //error
    U_STABITS.URXEN=0;      //Rx disable
    U_STABITS.UTXEN=0;      //Tx enable    
    clearRxFifo();
    ra=0;
    netcomRx_ms=0;
    
    U_STABITS.OERR=0;
    U_STABITS.FERR=0;
    U_STABITS.PERR=0;
    
    //U2STAbits.URXEN=1;
    enableRx();
    
    netcomBusError++;
    
    IFRX_CLEAR;
    IFTX_CLEAR;
    IFER_CLEAR;
    
#ifndef TEST_BOARD_BOS0    
    setPin(&LED4);
#endif 
    
}

#endif  //NETCOM_UART