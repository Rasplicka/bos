#include <xc.h>
#include <stdio.h>
#include "globals.h"

//void appNet_start();

static void st1(int i);
static void st3(int i);
static void netInitPort();
static void searchModule();
static void comunicate();
static void writeStat();


#define     SETPORT_CAPA            32
#define     GETPORT_CAPA            32

//datove struktury set portu
static NETCOM_DATAIN setPort0;
static NETCOM_DATAIN setPort1;
static NETCOM_DATAIN setPort2;
static NETCOM_DATAIN setPort3;
//datove struktury get portu
static NETCOM_DATAIN getPort0;
static NETCOM_DATAIN getPort1;
static NETCOM_DATAIN getPort2;
static NETCOM_DATAIN getPort3;

//buffery set portu
static char setPort0data[SETPORT_CAPA]; 
static char setPort1data[SETPORT_CAPA];
static char setPort2data[SETPORT_CAPA]; 
static char setPort3data[SETPORT_CAPA];
//buffery get portu
static char getPort0data[GETPORT_CAPA]; 
static char getPort1data[GETPORT_CAPA];
static char getPort2data[GETPORT_CAPA]; 
static char getPort3data[GETPORT_CAPA];

//datova struktura dataOut
static NETCOM_DATAOUT dataStruct;

static char write_stats=0;
static char module_stats=1;
static int  modules[NETCOM_MAXID+1];
static int cnt=0;

//LED1 - blick
//LED2 - master
//LED3 - initBus 
//LED4 - bus error

void appNet_start()
{
    // <editor-fold defaultstate="collapsed" desc="graphics">
#ifdef USE_GRAPHICS 
    initGraphics();
#endif        

#ifdef USE_DISP9341
    graphics.setDefaultFont(&font_twcen_22);
#endif        
#ifdef USE_DISP1306            
    //sysDisplay.setDefaultFont(&spfont_fixed_16);
#endif   
    
    // </editor-fold>
     
#ifdef TEST_BOARD_BOS1    
    systemTimerRegInterval(&st1, 1000);
    systemTimerRegInterval(&st3, 3000);
#else
    systemTimerRegInterval(&st1, 1000);
#endif    
    
    netInitPort();

    int a;
    while(1)
    {
        //clear status Full
        for(a=0; a<4; a++)
        {
            if(netcomDataSet[a]->Status==NETCOM_IN_STATUS.Full)
            {
                netcomDataSet[a]->Status=NETCOM_IN_STATUS.WaitToRx;
            }
        }
        

#ifdef TEST_BOARD_BOS1
        //pouze BOS1 (ID:1)
        if(module_stats>0)
        {
            #ifdef USE_GRAPHICS
            graphics.clear(COLOR.Black);
            #endif
            searchModule();
            module_stats=0;
        }
    
        if(write_stats>0)
        {
            #ifdef USE_GRAPHICS
            writeStat();
            #endif
            write_stats=0;
        }        
        comunicate();        
#endif        
        
        doEvents();
        
        cnt++;
        if(cnt>10000) { invPin(&LED1); cnt=0; }
    } 
}

static void st1(int i)
{
    clearPin(LED3);
    clearPin(LED4);
    write_stats++;
}
static void st3(int i)
{
    module_stats++;
}

static void netInitPort()
{
    //setPort ------------------------------------------------------------------
    //pripravi port0 pro prijem dat
    setPort0.Data=setPort0data;
    setPort0.Status=NETCOM_IN_STATUS.WaitToRx;
    setPort0.DataCapacity=SETPORT_CAPA;
    netcomDataSet[0]=&setPort0;
    
    //pripravi port1 pro prijem dat
    setPort1.Data=setPort1data;
    setPort1.Status=NETCOM_IN_STATUS.WaitToRx;
    setPort1.DataCapacity=SETPORT_CAPA;
    netcomDataSet[1]=&setPort1;    
    
    //pripravi port2 pro prijem dat
    setPort2.Data=setPort2data;
    setPort2.Status=NETCOM_IN_STATUS.WaitToRx;
    setPort2.DataCapacity=SETPORT_CAPA;
    netcomDataSet[2]=&setPort2;    
    
    //pripravi port3 pro prijem dat
    setPort3.Data=setPort3data;
    setPort3.Status=NETCOM_IN_STATUS.WaitToRx;
    setPort3.DataCapacity=SETPORT_CAPA;
    netcomDataSet[3]=&setPort3;    
    
    //getPort ------------------------------------------------------------------
    //pripravi port0 na odesilani dat
    getPort0.Data=getPort0data;
    getPort0.DataLen=GETPORT_CAPA;
    getPort0.Status=NETCOM_IN_STATUS.Valid;
    getPort0.Locked=0;
    netcomDataGet[0]=&getPort0;
    
    //pripravi port1 na odesilani dat
    getPort1.Data=getPort1data;
    getPort1.DataLen=GETPORT_CAPA;
    getPort1.Status=NETCOM_IN_STATUS.Valid;
    getPort1.Locked=0;
    netcomDataGet[1]=&getPort1;    
    
    //pripravi port2 na odesilani dat
    getPort2.Data=getPort2data;
    getPort2.DataLen=GETPORT_CAPA;
    getPort2.Status=NETCOM_IN_STATUS.Valid;
    getPort2.Locked=0;
    netcomDataGet[2]=&getPort2;    
    
    //pripravi port3 na odesilani dat
    getPort3.Data=getPort3data;
    getPort3.DataLen=GETPORT_CAPA;
    getPort3.Status=NETCOM_IN_STATUS.Valid;
    getPort3.Locked=0;
    netcomDataGet[3]=&getPort3;    
}
static void searchModule()
{
    char ret_data[32];
    dataStruct.Data=ret_data;
    dataStruct.DataLen=32;
    dataStruct.Pipe=0;
    
    int a;
    for(a=1; a <= NETCOM_MAXID; a++)
    {
        if(a != netcom_devId)
        {
            dataStruct.OppID=a;
 
            netcomGetData(&dataStruct);
            while(dataStruct.Status==NETCOM_OUT_STATUS.WaitToTx)
            {
                doEvents();
            } 
            
            if(dataStruct.Status==NETCOM_OUT_STATUS.ReplyOk)
            {   
                //ok, modul odpovida
                if(modules[a]<=0){ modules[a]=1; }
            }
            else
            {
                //chyba, neexistuje
                modules[a]=-1;
            }
            
        }
    }
}
static void comunicate()
{
    char send_data[32];
    dataStruct.Pipe=0;
    dataStruct.Data=send_data;
    dataStruct.DataLen=32;
    
    int a;
    for(a=1; a <= NETCOM_MAXID; a++)
    {
        if(modules[a]>0 && a!=netcom_devId)
        {
            dataStruct.OppID=a;
 
            netcomSetData(&dataStruct);
            while(dataStruct.Status==NETCOM_OUT_STATUS.WaitToTx)
            {
                doEvents();
            } 
            
            if(dataStruct.Status==NETCOM_OUT_STATUS.ReplyOk)
            {        
                modules[a]+=32;
            }
        }
    }
}

#ifdef USE_GRAPHICS
static void writeStat()
{
    int a, y=5;
    char str[12];
    
    //this ID
    graphics.drawString("ID:", NULL, 0, y);
    intToChar(netcom_devId, str, 1);
    graphics.drawString(str, NULL, 30, y);
    y+=22;
    
    for(a=1; a <= NETCOM_MAXID; a++)
    {
        if(modules[a]>0 && a!=netcom_devId)
        {
            graphics.drawString("ID:", NULL, 0, y);
            
            intToChar(a, str, 1);
            graphics.drawString(str, NULL, 30, y);
            
            intToChar(modules[a], str, 1);
            graphics.drawString(str, NULL, 100, y);
            
            y+=22;
        }
    }    
    
    graphics.drawString("Bus Err", NULL, 0, y);
    intToChar(netcomBusError, str, 12);
    graphics.drawString(str, NULL, 100, y);
    y+=22;
    
    graphics.drawString("Send Err", NULL, 0, y);
    intToChar(netcomSendDataError, str, 12);
    graphics.drawString(str, NULL, 100, y);
    y+=22;    
    
    graphics.drawString("Check Err", NULL, 0, y);
    intToChar(netcomChkError, str, 12);
    graphics.drawString(str, NULL, 100, y);
    y+=22;   
}

#endif