#include <xc.h>
#include <stdio.h>
#include "globals.h"

#ifdef PIC32MZ

//red LED
#define     _LED_INV_REG    LATHINV
#define     _LED_INV_VAL    0b001                               
int delay=10;
#endif

#ifdef PIC32MM0064

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b100000
int delay=1;
#endif  

#ifdef PIC32MM0256

#define     _LED_INV_REG    LATBINV
#define     _LED_INV_VAL    0b100000          //RB5                
int delay=1;
#endif

#ifdef USE_GRAPHICS
//#include "_display/image/img_woman_col565.h"
#include "_display/image/pokus.h"
//#include "_display/image/pokus_1306.h"
#endif

int testVar=100;
static int aktX=-1;
static int aktY=-1;
static char str[12];

static void longTime();
static void testSystemTimer(int i);
static void testRTC(char hour, char min, uint date);
static void testButton(int event, int value, int index);
static void call(int a, int b);
static void call1(int a, int b);
static void call2(int a, int b);
static void call3(int a, int b);
static void cn(uint base, uint stat);
static void onTouch(int p0, int p1, int p2);

static void dispText1306();
static void drawImage1306();

static void writeNetComStats();
static void dispText1();
static void dispText2();
static void exPrint();
#ifdef USE_GRAPHICS
static void setRedFont(IMAGE_SRC* font);
#endif
static void drawColorText04();
static void drawColorText01();
static void fontExample();
static void drawLines();
static void drawPoints();
static void drawBox();
static void drawImg();


NETCOM_DATAOUT dataStruct;
static void netInitPipe();
static void changeGetData();
static void netGetData(char oid, char opipe);
static void netSetData(char oid, char opipe);
static void netWaitForData(char pipe);


static int restart=0;
static int write_stats=0;

void m1_start()
{

    
    netInitPipe();
    
#ifdef TEST_BOARD_BOS1   
    #ifdef USE_GRAPHICS 
        initGraphics();
        //sysDisplay.setDefaultFont(&spfont_fixed_16);
        graphics.setDefaultFont(&font_twcen_22);
    #endif
#endif    
        
    systemTimerRegInterval(&testSystemTimer, 1000);
    
    while(1)
    {
        
        if(netcomDataSet[0]->Status==NETCOM_IN_STATUS.Full)
        {
            netcomDataSet[0]->Status=NETCOM_IN_STATUS.WaitToRx;
        }
        if(netcomDataSet[1]->Status==NETCOM_IN_STATUS.Full)
        {
            netcomDataSet[1]->Status=NETCOM_IN_STATUS.WaitToRx;
        } 
        
        
#ifdef TEST_BOARD_BOS1
        //ID 1
        netSetData(4, 0);
        netGetData(4, 1);
        netSetData(2, 0);
        netGetData(2, 0);
        //netSetData(1, 0);
#else
        //ID != 1
        //netSetData(4, 0);
        //netGetData(4, 1);
#endif   
        
        int a;
        for(a=0; a<100; a++)
        {
            doEvents();
        }
        
#ifdef USE_GRAPHICS         
        //doEvents();
        if(write_stats>0)
        {
            writeNetComStats();
            write_stats=0;
        }
#endif        
        
    }
        
    // <editor-fold defaultstate="collapsed" desc="vyrazeno">
    
#ifdef USE_GRAPHICS 
    initGraphics();
    //sysDisplay.setDefaultFont(&spfont_fixed_16);
    graphics.setDefaultFont(&font_twcen_22);
#endif
    //RGB16(5, 5, 5);
    /*
    cnStartPortA();
    cnEnable(PORTA_BASE, BIT3 | BIT8, CN_STYLE.LOW_TO_HIGH);             //povoli PORTA.0, PORTA.1
    cnRegEvent(&cn, PORTA_BASE);
    
    cnStartPortC();                                                     //zaktivuje fci CN (SIDL=0, pracuje v IDLE/SLEEP)
    cnEnable(PORTC_BASE, BIT1 | BIT2, CN_STYLE.LOW_TO_HIGH);             //povoli PORTA.0, PORTA.1
    cnRegEvent(&cn, PORTC_BASE);
    */
    
#ifdef USE_TOUCHPAD_XPT2046    
    touchXpt2046_regEvent(&onTouch);
#endif    
    
    //dispText1();
    //fontExample();
    //drawLines();
    //drawPoints();
    //drawBox();
    //drawImg();
    
    //dispText1306();
    //drawImage1306();
    while(1)
    {
        if(aktX != -1 || aktY != -1)
        {
#ifdef USE_GRAPHICS            
            intToChar(aktX, str, 4);
            graphics.drawString(str, NULL, 0, 0);
            intToChar(aktY, str, 4);
            graphics.drawString(str, NULL, 0, 30);
            
            graphics.fillBox(aktX-2, aktY-2, aktX+2, aktY+2, COLOR.Yellow);
#endif            
            aktX=-1; aktY=-1;
        }
        doEvents();
    }




    /*
    //setCanIdle(1);
    //setCanSleep(1);
    
    //if(restart==0)
    //{
    
    //}
    rtcRegTimeAlarm(&testRTC, 0, 1);
    //setClock(CLOCK_CFG.CLK_ALT3);
    //setLowPowerOsc();
    //setHighPowerOsc();
    
    //testSystemTimer();
    systemTimerRegInterval(&testSystemTimer, 10000);

    
    //int st=CNSTATB;
    rtcRegTimeAlarm(&testRTC, 0, 1);
    rtcRegTimeAlarm(&testRTC, 0, 2);
    //test sleep mode
    setCanSleep(0);
    
    int c=0;
    while(1)
    {
        if(c % 100 == 0)
        {
            doEvents();
            //call(1,1);
        }
        c++;
        //xxx=PORTB;
    }
     */


    //ubtnRegEvent(&testButton, 1, 10);
    //trap();

    int x = 0;
    while (x < 10000) {
        //do LATxINV zapise 1 na prislusnou pozici
        //_LED_INV_REG = _LED_INV_VAL;
        invPin(&LED1);

        int a, b = 0;
        for (a = 0; a < (100000 * delay); a++) {
            b++;
            if (a > 0 && a % 1000 == 0) {
                doEvents();
            }
        }
        //stack overflow
        //m1_start()
        x++;
    }

    //raiseEventID(155, 0, 1, 2);
    //uint day=getDayMs();
    //uint second=(day/1000) % 60;
    //uint minute=(day/1000/60) % 60;
    //uint hour=(day/1000/60/60);

    //int* ad=0;
    //int dat=*ad;

    while (1) {
        //do LATxINV zapise 1 na prislusnou pozici
        //_LED_INV_REG = _LED_INV_VAL;
        invPin(&LED1);

        int a, b = 0;
        for (a = 0; a < 250000; a++) {
            b++;
            if (a % 100 == 0) {
                doEvents();
            }
        }
        x++;
    } 
    
    // </editor-fold>

}

static void longTime()
{
        int a, b=0;
        for(a=0; a<190000; a++)
        {
            b++;
            if(a % 1000 == 0)
            {
                doEvents();
            }
        }    
}

static void testSystemTimer(int i)
{
    write_stats++;
    
#ifdef TEST_BOARD_BOS1    
    clearPin(&LED4);
#else
    clearPin(&LED1);
#endif    
    //clearPin(&LED2);
    //clearPin(&LED3);
    //int d=a+b;
    //_LED_INV_REG = _LED_INV_VAL;
    //doEvents();
    //setLowPowerOsc();
    //setCanSleep(1);
    //invTestLed(1);
    //unregEvent(&testSystemTimer);
    
    /*
    char x;
    
    while(1)
    {
        x=0;
        _LED_INV_REG = _LED_INV_VAL;
        if(systemTimerRegDelay(&x, 1000) == 0)
        {
            //chyba
        }
        
        while(x==0) { doEvents(); }
    
        x=0;
        _LED_INV_REG = _LED_INV_VAL;
        if(systemTimerRegDelay(&x, 500) == 0)
        {
            //chyba
        }
        while(x==0) { doEvents(); }
    }
    */
    
}

static void testRTC(char hour, char min, uint date)
{
    //invTestLed(1);
    //_LED_INV_REG = _LED_INV_VAL;
    setCanSleep(0);
    //setCanIdle(0);
    //restart=1;
    //restartProcess();
}

static void testButton(int event, int value, int index)
{
    trap();
    //exitProcess();
    //if(value==0)
    //{
        /*
        if(event==UBTN_DOWN)
        {
            _LED_INV_REG = _LED_INV_VAL;
        }
    
        if(event==UBTN_UP)
        {
            _LED_INV_REG = _LED_INV_VAL;
        }
        */
    //}
}

static void call(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    call1(1,1);
}

static void call1(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    call2(1,1);
}

static void call2(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    call3(1,1);
}

static void call3(int a, int b)
{
    int p[50];
    p[0]=0xff;
    
    checkStackSpace();
    
    //call1(1,1);
}

static void cn(uint base, uint stat)
{
    if(base==PORTA_BASE)
    {
        //drawImg();
        //dispText1();
        //dispText1306();
    }
    else
    {
        //graphics.clear(COLOR.Black);
        //drawImage1306();
        //dispText2();
        //drawColorText04();
        //drawColorText01();
    }
    
    //RCONbits.SLEEP==0;
    //setHighPowerOsc();
    //invTestLed(4);
    //setCanSleep(0);
    //LATCbits.LATC9=0;
    //softReset();
    //setCanIdle(0);
    //if((stat & BIT12) != 0)
    //{
        
        //setCanSleep(0);
        //systemTimerRegInterval(&testSystemTimer, 5000);
    //}
    //setTestLed(1);
    //uint s=stat;
    //setCanSleep(0);
    
}

static void onTouch(int p0, int p1, int p2)
{
    aktX=p0;
    aktY=p1;
    //int c=x+y;
    //return;
}

#ifdef USE_DISP1306

static void dispText1306()
{
    char txt[] = "\xA6lu\x9Cou\x9Fký k\xDE\xE5";
    graphics.clear(COLOR.Black);
    
    short y=0, x=0;
    graphics.drawString(txt, &spfont_fixed_16, x, y);
    
    y=16; x=0;    
    graphics.drawString("15:33", &spfont_rock_48, x, y);
}

static void drawImage1306()
{
    IMAGE_SRC img;
    setImageSrc(&pokus_1306, &img);
    graphics.drawImage(&img, 0, 0);    
}

#endif

#ifdef USE_DISP9341

static void writeNetComStats()
{
    //graphics.clear(COLOR.Black);
    int y=0, x1=0, x2=120;
    
    graphics.drawString("Tx data", NULL, x1, y);
    intToChar(netcomTxBytes, str, 12);
    graphics.drawString(str, NULL, x2, y);
    y+=25;
    
    graphics.drawString("Tx CTRL", NULL, x1, y);
    intToChar(netcomTxCTRL, str, 12);
    graphics.drawString(str, NULL, x2, y);
    y+=25;    
    
    graphics.drawString("Rx data", NULL, x1, y);
    intToChar(netcomRxBytes, str, 12);
    graphics.drawString(str, NULL, x2, y);
    y+=25;
    
    graphics.drawString("Rx CTRL", NULL, x1, y);
    intToChar(netcomRxCTRL, str, 12);
    graphics.drawString(str, NULL, x2, y);
    y+=25;       
    
    graphics.drawString("Bus Err", NULL, x1, y);
    intToChar(netcomBusError, str, 12);
    graphics.drawString(str, NULL, x2, y);
    y+=25;   
    
    graphics.drawString("Send Err", NULL, x1, y);
    intToChar(netcomSendDataError, str, 12);
    graphics.drawString(str, NULL, x2, y);
    y+=25;      
    
    graphics.drawString("Chk Err", NULL, x1, y);
    intToChar(netcomChkError, str, 12);
    graphics.drawString(str, NULL, x2, y);
    y+=25;      
    
}

/*
static void dispText1()
{
    //char txt2[] = {"áéíóúý ÁÉÍÓÚÝ"};
    //char txt[] = {"Kratky text"};
    char txt[] = {'J','i','\xFE','í',' ','R','a','\xE7','p','l','i','\x9F','k','a'}; //,'ý','á','í','é','?','?',' '};
    char txt2[] = "Ji\xFEí Ra\xE7pli\x9Fka";
    char txt3[] = "\xA6lu\x9Cou\x9Fký k\xDE\xE5, \xE6árka";
    
    graphics.clear(COLOR.Black);
    short y=0, x=0;
    graphics.drawString("PIC32MM0256", NULL, x, y);
    y+=graphics.getFontHeight(NULL);
    graphics.drawString("CPU 24MHz", NULL, x, y);
    y+=graphics.getFontHeight(NULL);
    graphics.drawString("SPI 48MHz", NULL, x, y);
    y+=graphics.getFontHeight(NULL);
    graphics.drawString("SSD1306 display", NULL, x, y);
    y+=graphics.getFontHeight(NULL);
    graphics.drawString(txt, NULL, x, y);
    y+=graphics.getFontHeight(NULL);
    graphics.drawString(txt3, &font_ygm_36, x, y);    
    
    //short h = sysDisplay.getHeight();       //240
    //short w = sysDisplay.getWidth();        //320
    //char o = sysDisplay.getOrientation();   //0=na vysku, 1=na sirku, 2=na vysku obracene, 3=na sirku obracene
}   

static void exPrint()
{
    graphics.clear(COLOR.Black);
    graphics.print("toto je \xFEádek 1");
    graphics.print("toto je \xFEádek 2");
    graphics.print("toto je \xFEádek 3");
    graphics.print("toto je \xFEádek 4");
}

static void dispText2()
{
    //char txt2[] = {"áéíóúý ÁÉÍÓÚÝ"};
    //char txt[] = {"Kratky text"};
    //char txt[] = {};
    graphics.clear(COLOR.Black);
    
    font_ygm_20.foreColor=COLOR.Blue;
    
    short width = graphics.textWidth("Muj text", &font_ygm_20);
    short height = graphics.getFontHeight(&font_ygm_20);
    
    //nebo
    IMAGE_SRC* fontSmall = &font_ygm_20;
    IMAGE_SRC* fontBig = &font_ygm_36;
    short wsmall = graphics.textWidth("Muj text", fontSmall);
    short wbig = graphics.textWidth("Muj text", fontBig);
    
    
    //graphics.clear(COLOR.Black);
    //graphics.clear(COLOR.Black);
    //graphics.clear(COLOR.Black);    
    
    //IMAGE_SRC* font16 = &font_ygm_16;
    //IMAGE_SRC* font20 = &font_ygm_20;
    //IMAGE_SRC* font28 = &font_ygm_28;
    //IMAGE_SRC* font36 = &font_twcen_80;
    
    //short y=0, x=0;
    //graphics.drawString("Nejaky text, radek 1",&font_ygm_28, x, y);
    //y+=graphics.getFontHeight(&font_ygm_28);
    //graphics.drawString("jiny text, radek 2", &font_ygm_28, x, y);
    //y+=graphics.getFontHeight(&font_ygm_28);
      
    //graphics.drawString("Tzn. ze neni deklarovany", font28, x, y);
    //y+=graphics.getFontHeight(font28);
    //graphics.drawString("12:55:32", font36, 10, y+10);
    //y+=graphics.getFontHeight(font36);
    
    //graphics.drawString("primo v kodu v uvozovkach", NULL, x, y);
} 

static void setRedFont(IMAGE_SRC* font)
{
    char x[]="?lu?ou?ký k?n";
    
    if(font->format==0x1)
    {
        //1-bit format barvy
        font->foreColor=RGB16(31, 0, 0);
    }    
    else if (font->format==0x4)
    {
        //4-bit format barvy
        int a;
        short cmap_red[16];                 //musi byt deklarovano mimo fci, aby nebyla na zasobniku
        for(a=0; a<16; a++)
        {
            cmap_red[a]=RGB16(a*2, 0, 0);
        }
        setColorMap(font, cmap_red);        
    }
}

static void drawColorText04()
{
    short x, y, a;
    //cls
    graphics.clear(COLOR.Black);
    IMAGE_SRC* font28 = &font_ygm_28;
    
    if(font28->format==0x1)
    {
        //1-bit format barvy
        font28->foreColor=RGB16(31, 0, 0);
        font28->bgColor=RGB16(0, 0, 31);
    }
        
    //zelena barva textu, cerna barva pozadi
    short cmap_green[16];
    for(a=0; a<16; a++)
    {
        cmap_green[a]=RGB16(0, a*4, 0);
    }
    setColorMap(font28, cmap_green);
    y=0; x=0;
    graphics.drawString("Zeleny text", font28, x, y);

    
    //carvena barva textu, modra barva pozadi
    short cmap_red[16];
    for(a=0; a<16; a++)
    {
        cmap_red[a]=RGB16(a*2, 0, 30-2*a);
    }
    setColorMap(font28, cmap_red);
    y=50; x=0;
    graphics.drawString("Cerveny text", font28, x, y);
    

    //puvodni barevna mapa, bily text, cerne pozadi
    setColorMap(font28, stdColorMap);
    y=100; x=0;
    graphics.drawString("Bily text", font28, x, y); 
    
}
    
static void drawColorText01()
{
    short x, y;
    
    graphics.clear(COLOR.Black);                            //cls   
    IMAGE_SRC* fontA = &font_arial_18;
    IMAGE_SRC* fontC = &font_consolas_28;
    
    fontA->foreColor=RGB16(31, 0, 0);
    fontA->bgColor=RGB16(0, 0, 31);
    y=0; x=0;
    graphics.drawString("Cerveny text", fontA, x, y);    
    
    fontA->foreColor=COLOR.Yellow;
    fontA->bgColor=COLOR.Black;
    y=50; x=0;
    graphics.drawString("Zluty text", fontA, x, y);  
    
    fontC->foreColor=COLOR.Pink;
    fontC->bgColor=COLOR.DarkGrey;
    y=100; x=0;
    graphics.drawString("Ruzovy text", fontC, x, y);  
    
    fontA->foreColor=COLOR.White;
    fontA->bgColor=COLOR.Black;
}

static void fontExample()
{
    graphics.drawString("YGM36 4-bit font", &font_ygm_36, 10, 30);    
    //graphics.drawString("CONSOLAS 1-bit font", &font_consolas_36, 10, 130);
}

static void drawLines()
{
    //Zluty ramecek kolem displeje
    short w=graphics.getDisplayWidth();
    short h=graphics.getDisplayHeight();
    graphics.drawLine(0, 0, w-1, 0, 1, COLOR.Yellow);
    graphics.drawLine(w-1, 0, w-1, h-1, 1, COLOR.Yellow);    
    graphics.drawLine(w-1, h-1, 0, h-1, 1, COLOR.Yellow);    
    graphics.drawLine(0, h-1, 0, 0, 1, COLOR.Yellow);    
    
    graphics.drawLine(0, 0, w-1, h-1, 1, COLOR.Grey);
    graphics.drawLine(0, h-1, w-1, 0, 1, COLOR.Grey);
    
    graphics.drawLine(10, 20, 300, 20, 5, COLOR.GreenYellow);
    
    graphics.drawLine(10, 20, 300, 220, 10, COLOR.Green);
    
}

static void drawPoints()
{
    int a;
    for(a=10; a<320; a+=5)
    {
        graphics.drawPoint(a, 20, COLOR.White);
    }
    
}

static void drawBox()
{
    graphics.drawBox(10, 10, 300, 220, 5, COLOR.Orange);
    
    int a;
    for(a=40; a<280; a+=20) 
    {
        graphics.fillBox(a, 30, a+15, 130, COLOR.Blue);
        graphics.drawBox(a, 30, a+15, 130, 2, COLOR.Yellow);
    }
    
}

*/
static void drawImg()
{
    //#include "_display/image/img_woman_col4.h"
    IMAGE_SRC img;
    setImageSrc(&pokus565, &img);
    graphics.drawImage(&img, 0, 0);
}



#endif

#ifdef NETCOM_UART

NETCOM_DATAIN setPipe0;
NETCOM_DATAIN setPipe1;
NETCOM_DATAIN getPipe0;
NETCOM_DATAIN getPipe1;
char setPipe0data[1024]; //NETCOM_SETPIPE_SIZE];
char setPipe1data[32];

char getPipe0data[4]={0,1,2,3};
char getPipe1data[8]={0,1,2,3,10,11,12,13};
char data[]={1,2,3,4,5,6,7,8,         9,10,11,12,13,14,15,16,
             17,18,19,20,21,22,23,24, 25,26,27,28,29,30,31,32};

char data1024[1024];
    
static void netInitPipe()
{
    /*
    Netcom nc;
    nc.setData[0]=&setPipe0;
    nc.setData[0]->Data=setPipe0data;
    nc.setData[0]->Status=NETCOM_IN_STATUS.WaitToRx;
    nc.setData[0]->Data=&setPipe0;
    nc.setData[0]->DataCapacity=1024;
    */
    
    
    
    //pripravi buffer pro set, pipe0
    setPipe0.Data=setPipe0data;
    setPipe0.Status=NETCOM_IN_STATUS.WaitToRx;
    setPipe0.DataCapacity=1024;
    netcomDataSet[0]=&setPipe0;
    
    //pripravi buffer pro set, pipe1
    setPipe1.Data=setPipe1data;
    setPipe1.Status=NETCOM_IN_STATUS.WaitToRx;
    setPipe1.DataCapacity=32;
    netcomDataSet[1]=&setPipe1;
    
    getPipe0.Data=getPipe0data;
    getPipe0.DataLen=4;
    //getPipe0.ReceiveFn=&onGetData;
    getPipe0.Status=NETCOM_IN_STATUS.Valid;
    getPipe0.Locked=0;
    netcomDataGet[0]=&getPipe0;
    
    getPipe1.Data=getPipe1data;
    getPipe1.DataLen=8;
    //getPipe1.ReceiveFn=&onGetData;
    getPipe1.Status=NETCOM_IN_STATUS.Valid;
    getPipe1.Locked=0;
    netcomDataGet[1]=&getPipe1;
    
    int a; 
    char b=0;
    for(a=0; a<1024; a++)
    {
        data1024[a]=b;
        b++;
        if(b==0xFF){b=0;}
    }
}

static void changeGetData()
{
    while(lockSafe(&getPipe0.Locked)==0)
    {
        //probiha vysilani obsahu, nelze menit
        doEvents();
    }

    //getPipe0.Locked je nyni nastaven, fce muze menit data
    getPipe0.Data[0]=100;
    getPipe0.Data[1]=101;
    getPipe0.Data[2]=102;
    getPipe0.Data[3]=103;
    
    //unlock
    getPipe0.Locked=0;
}

static void netGetData(char oid, char opipe)
{
    char ret_data[32];
    //dataStruct.AppID=getProcID();
    dataStruct.OppID=oid;
    dataStruct.Pipe=opipe;
    dataStruct.Data=ret_data;
    dataStruct.DataLen=32;
    //dataStruct.Direction=1;         //set    
    
    netcomGetData(&dataStruct);
    setPin(&LED1);
    
    while(dataStruct.Status==NETCOM_OUT_STATUS.WaitToTx)
    {
        doEvents();
    }
    
    if(dataStruct.Status==NETCOM_OUT_STATUS.ReplyOk)
    {
        //ok
        //dataStruct.Data (ret_data) obsahuje prijata data
        //dataStruct.DataLen obsahuje velikost dat
        
        if(opipe==0 && ret_data[0]==0 && ret_data[1]==1 && ret_data[2]==2 && ret_data[3]==3)
        {
            clearPin(&LED1);
        }
        if(opipe==1 && ret_data[0]==0 && ret_data[1]==1 && ret_data[2]==2 && ret_data[3]==3 && ret_data[4]==10 && ret_data[5]==11 && ret_data[6]==12 && ret_data[7]==13)
        {
            clearPin(&LED1);  
        }
    }    
    else
    {
        //dataStruct.Status obsahuje kod chyby
        setPin(&LED4);
    }
    
}

static void netSetData(char oid, char opipe)
{
    //dataStruct.AppID=getProcID();
    dataStruct.OppID=oid;
    dataStruct.Pipe=opipe;
    dataStruct.Data=data1024;
    dataStruct.DataLen=1024;
    //dataStruct.Direction=0;         //set
    
    netcomSetData(&dataStruct);
    setPin(&LED1);
    
    while(dataStruct.Status==NETCOM_OUT_STATUS.WaitToTx)
    {
        doEvents();
    }
    
    if(dataStruct.Status==NETCOM_OUT_STATUS.ReplyOk)
    {
        //ok
        clearPin(&LED1);
    }
    else
    {
        setPin(&LED4);
    }
    
    /*
    else if (dataStruct.Status==NETCOM_OUT_STATUS.ReplyBusy)
    {
        //busy
        //setPin(&LED4);
    }
    else if (dataStruct.Status==NETCOM_OUT_STATUS.ReplyErrorPipe)
    {
        //pipe in opp not exist
        //setPin(&LED4);
    }
    else if (dataStruct.Status==NETCOM_OUT_STATUS.ReplyErrorSize)
    {
        //data size too big
        //setPin(&LED4);
    }
    else if (dataStruct.Status==NETCOM_OUT_STATUS.ReplyNotExist)
    {
        //opp not exist
        //setPin(&LED4);
    }
    */
    
    
}

static void netWaitForData(char pipe)
{
    //velikost setPipe je NETCOM_SETPIPE_SIZE
    //setPipe.DataLen obsahuje velikost prijatych dat
    
    //ceka na prijem dat do pipe
    while(netcomDataSet[pipe]->Status == NETCOM_IN_STATUS.WaitToRx)
    {
        doEvents();
    }
    
    //netcomDataSet[pipe].Data obsahuje prijata data
    //netcomDataSet[pipe].DataLen obsahuje velikost dat
    
    //po zpracovani povoli novy prijem dat
    netcomDataSet[pipe]->Status = NETCOM_IN_STATUS.WaitToRx;
}

#endif