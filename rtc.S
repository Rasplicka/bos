#include "xc.h"
#include "def.h"    
#include "asm_macro.S"    
   
/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0    
 * RTC, used by BOS core
 * This file is shared to the PIC32MM, PIC32MZ
     
//date a time se cte z RTC modulu
//presny cas (ms 32-bit) od zacatku dne je v day_ms, kterou incrementuje timer1
//day_ms se synchronizuje s RTC kazdou minutu (alarm interrupt)    
//(doba spusteni OS je v timer_ms)

//Poskytuje event - RTC ALARM, v zadanou hodinu a minutu
//do fce predava i aktualni datum v BCD formatu (obsah RTCDATE)     
     
//bcd format detail:
//   b31       b23       b15       b7      b0
//   des  jed  des  jed  des  jed
//0b RRRR.RRRR-MMMM.MMMM-DDDD.DDDD-xxxx xWWW		//RTCDATE, WWW = week day
//      0    5    1    0    2    4         1		//05/10/24 Sunday     
     
//0b HHHH.HHHH-MMMM.MMMM-SSSS.SSSS-xxxx xxxx		//RTCTIME
//      1    4    3    5    0    2			//14:35:02
 */ 
    
#ifdef RTC

.set micromips
.section .asm_rtc, code  
    
#ifdef PIC32MM 
    
    #define	    WRLOCK	    0x0800		//RTCCON1.b11  
    #define	    TIME_SYNC	    0x0004
    #define	    ALARM_SYNC	    0x0002
    #define	    RTCCON1_SET	    0xC3FF0000		//ALARM enable, ON, ... 
    #define	    RTCCON1_ON	    0x00008000	    
    #define	    RTCCON2_SET	    0x3E800001		//clock=Low Power RC oscilator (32.0kHz)  
    
#endif	    //MM        
    
//<editor-fold defaultstate="collapsed" desc="MACRO">
    
//<editor-fold defaultstate="collapsed" desc="_GET_HOUR_FROM_BCD">
.macro _GET_HOUR_FROM_BCD src, dest
    ext     \dest, \src, 28, 4		//desitky
    li      t9, 10	    
    mul     \dest, t9
    ext     t9, \src, 24, 4		//jednotky
    addu    \dest, t9        
.endm
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="_GET_MIN_FROM_BCD">
.macro _GET_MIN_FROM_BCD src, dest
    ext     \dest, \src, 20, 4		//desitky
    li      t9, 10	    
    mul     \dest, t9
    ext     t9, \src, 16, 4		//jednotky
    addu    \dest, t9    
.endm
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="_GET_SEC_FROM_BCD">
.macro _GET_SEC_FROM_BCD src, dest
    //sec 0-59
    ext     \dest, \src, 12, 4		//desitky
    li      t9, 10
    mul     \dest, t9
    ext     t9, \src, 8, 4		//jednotky
    addu    \dest, t9
.endm
//</editor-fold>    
    
//<editor-fold defaultstate="collapsed" desc="_DISABLE_INT">
.macro _DISABLE_INT
//zakaza interrupt (DI)	      , zalohu bitu do t9
//meni t8, t9, v0    
    
    mfc0    t9, _CP0_STATUS
    ext     t9, t9, 0, 1
    di
  
.endm 
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="_RESTORE_INT">
.macro _RESTORE_INT    
//restore interrupt  , zaloha v t9   
   
    beqz    t9, 1001f			//skok pokud nema povolit interrupt (zaloha EI v t9)
    nop
    ei
    
1001:
    
.endm    
    
//</editor-fold>  
    
//<editor-fold defaultstate="collapsed" desc="_SYSKEY_WRLOCK">
.macro _SYSKEY_WRLOCK   
//nuluj RTCCON1.WRLock, pouze po syskey    
    
1001:
    //nastavi RTCCON.3, ceka dokud neni nastaven
    move    v0, zero
    sw      v0, (SYSKEY)
    li      v0, 0xaa996655
    sw      v0, (SYSKEY)
    li      v0, 0x556699aa
    sw      v0, (SYSKEY)
    
    //nuluj WRLOCK
    li      v0, WRLOCK			//WRLOCK=0
    sw      v0, (RTCCON1CLR)
    sync

    lw      v0, (RTCCON1)		//ceka na WRLOCK=0 (obcas se musi opakovat unlock 2x)
    li	    v1, WRLOCK
    and	    v0, v1
    bnez    v0, 1001b			//skok, WRLOCK neni 0
    nop
 
.endm    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="_DATETIME_SYNC">

.macro _DATETIME_SYNC
//ceka, dokud RTCSTAT.b2 != 0 (pri 1 probiha zmena RTC)
    
1001:    
    lw	    v0, (RTCSTAT)
    li	    v1, TIME_SYNC
    and	    v0, v1
    bnez    v0, 1001b
    nop
    
.endm  
    
//</editor-fold>
  
//<editor-fold defaultstate="collapsed" desc="_ALARM_SYNC">

.macro _ALARM_SYNC    
//ceka, dokud RTCSTAT.b1 != 0 (pri 1 probiha zmena RTC)
    
1001:    
    lw	    v0, (RTCSTAT)
    li	    v1, ALARM_SYNC
    and	    v0, v1
    bnez    v0, 1001b
    nop
    
.endm  
    
//</editor-fold>
    
//</editor-fold>
   
//globals
    
//<editor-fold defaultstate="collapsed" desc="rtcInit">
.align 2    
.global rtcInit
.ent rtcInit
rtcInit:
//nastavi time_ms (long int) a date_century
//vraci v0=ok, muze pokracovat, 1=chyba, nelze spustiti OS 

    #define	RTC_INTERRUPT_PRIORITY		2
    #define	RTC_INTERRUPT_SUBPRIORITY	1	
    
    _PUSH   ra
    //POZOR, v prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma suspend
    
    //test, zda je RTC modul zapnuty, pokud ano, nemeni nastaveni date/time
    //RTC zustava zapnuty pri RESETu i probuzeni ze sleep modu
    lw	    v0, (RTCCON1)
    ext	    v0, v0, 15, 1		//RTCCON.15=ON
    bnez    v0, 1f			//skok, RTC modul je zapnuty
    //nop
    
    //<editor-fold defaultstate="collapsed" desc="interrupt priority...">
#ifdef PIC32MM0064
    //priority IPC3.b18-20
    li	    t9, (0b111 << 18)
    sw	    t9, (IPC3CLR)					//nuluj bity
    li	    t9, (RTC_INTERRUPT_PRIORITY << 18)
    sw	    t9, (IPC3SET)					//nastav bity
    
    //subpriority IPC3.b16-17
    li	    t9, (0b11 << 16)
    sw	    t9, (IPC3CLR)					//nuluj bity
    li	    t9, (RTC_INTERRUPT_SUBPRIORITY << 16)
    sw	    t9, (IPC3SET)					//nastav bity
   
    //IEC0.b14, IFS0.b14
    li	    v0, (1 << 14)
    sw	    v0, (IFS1CLR)					//flag
    sw	    v0, (IEC1SET)					//enable
#endif
    
#ifdef PIC32MM0256
    //priority IPC8.b2-4
    li	    t9, (0b111 << 2)
    sw	    t9, (IPC8CLR)					//nuluj bity
    li	    t9, (RTC_INTERRUPT_PRIORITY << 2)
    sw	    t9, (IPC8SET)					//nastav bity
    
    //subpriority IPC8.b0-1
    li	    t9, (0b11 << 0)
    sw	    t9, (IPC8CLR)					//nuluj bity
    li	    t9, (RTC_INTERRUPT_SUBPRIORITY << 0)
    sw	    t9, (IPC8SET)					//nastav bity
   
    //IEC1.b0, IFS1.b0
    li	    v0, (1 << 0)
    sw	    v0, (IFS1CLR)					//flag
    sw	    v0, (IEC1SET)					//enable
#endif    
    
#ifdef PIC32MZ   
 
#endif 
    
    //</editor-fold>
    
    //t9=stav CP0.STATUS.EI, disable interrupt
    //nastav WRLock=0
    _DISABLE_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK			//meni v0, v1
    _DATETIME_SYNC			//nemi v0, v1
    _ALARM_SYNC				//meni v0, v1

    //set time, date
    li      v0, 0x00000000              //00:00:00
    sw      v0, (RTCTIME)
    sw	    v0, (ALMTIME)
    sw	    v0, (ALMDATE)
    li      v0, 0x01010102              //2001, leden, 1, den v tydnu 7=SO (1=NE, 2=PO)
    sw      v0, (RTCDATE)

    //input clock LPRC, 32kHz
    li	    v0, RTCCON2_SET		//LPRC
    sw	    v0, (RTCCON2)
    
    //ON=1
    li      v0, RTCCON1_SET		//ON=1, WRLock=1, ALARM interrupt
    sw      v0, (RTCCON1)
    
    li	    v0, RTCCON1_ON
    sw	    v0, (RTCCON1SET)

    //WRLOCK=1
    li	    v0, WRLOCK			//set RTCCON1.WRLOCK
    sw      v0, (RTCCON1SET)

     //restore interrupt
    _RESTORE_INT			//ze zalohy t9, meni v0
    
1:    
    //nastav day_ms, aby byla synchronizonava s RTC
    bal	    setDayMs
    nop
    
    _POP    ra
    jrc     ra
    
.end rtcInit
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="rtcGetTime">

.align 2    
.global rtcGetTime
.ent rtcGetTime
rtcGetTime:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    v0, (RTCTIME)
    
    jrc	    ra
    
.end rtcGetTime 
    
//</editor-fold>
  
//<editor-fold defaultstate="collapsed" desc="rtcGetDate">
    
.align 2    
.global rtcGetDate
.ent rtcGetDate
rtcGetDate:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    v0, (RTCDATE)
    
    jrc	    ra
    
.end rtcGetDate  
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="rtcSetTime">
    
.align 2
.global rtcSetTime    
.ent rtcSetTime
rtcSetTime:
//a0=BCD time
    
//bcd format
//   des  jed  des  jed  des  jed
//0b RRRR.RRRR-MMMM.MMMM-DDDD.DDDD-xxxx xWWW - week day
//0b HHHH.HHHH-MMMM.MMMM-SSSS.SSSS-xxxx xxxx    
    
    _PUSH   ra

    
    
    //POZOR, v dalsim prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma s
    _DISABLE_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK			//meni v0, v1
    _DATETIME_SYNC			//nemi v0, v1
    _ALARM_SYNC				//meni v0, v1

    //set time, date
    sw      a0, (RTCTIME)
    
    //WRLOCK=1
    li	    v0, WRLOCK			//set RTCCON1.WRLOCK
    sw      v0, (RTCCON1SET)
    
    //restore interrupt
    _RESTORE_INT			//ze zalohy t9, meni v0    
    
    bal	    setDayMs
    nop
    
    _POP    ra
    jrc     ra
    
.end rtcSetTime 
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="rtcSetDate">
    
.align 2
.global rtcSetDate    
.ent rtcSetDate
rtcSetDate:
//a0=BCD time
    
//bcd format
//   des  jed  des  jed  des  jed
//0b RRRR.RRRR-MMMM.MMMM-DDDD.DDDD-xxxx xWWW - week day
//0b HHHH.HHHH-MMMM.MMMM-SSSS.SSSS-xxxx xxxx    
   
    
    //POZOR, v dalsim prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma s
    _DISABLE_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK			//meni v0, v1
    _DATETIME_SYNC			//nemi v0, v1
    _ALARM_SYNC				//meni v0, v1

    //set time, date
    sw      a0, (RTCDATE)
    
    //WRLOCK=1
    li	    v0, WRLOCK			//set RTCCON1.WRLOCK
    sw      v0, (RTCCON1SET)
    
    //restore interrupt
    _RESTORE_INT			//ze zalohy t9, meni v0    
    
    jrc     ra
    
.end rtcSetDate 
    
//</editor-fold>   
    
//<editor-fold defaultstate="collapsed" desc="rtcGetHalfSecond">

.align 2
.global rtcGetHalfSecond
.ent rtcGetHalfSecond
rtcGetHalfSecond:    
    
    //RTCSTAT.b0
    lw	    v0, (RTCSTAT)
    ext	    v0, v0, 0, 1
    
    jrc	    ra
    
.end rtcGetHalfSecond  
    
//</editor-fold>       

//<editor-fold defaultstate="collapsed" desc="rtcRegTimeAlarm">
    
.align 2
.global rtcRegTimeAlarm
.ent rtcRegTimeAlarm
rtcRegTimeAlarm:
//a0=adresa fce
//a1=hour
//a2=minute
    
    _PUSH   ra
    
    move    a3, a2		    //a3=minute
    move    a2, a1		    //a2=hour

    bal	    getProcID		    //meni v0, v1
    sll	    a1, v0, 8		    //procID do b8-b15
    ori	    a1, RTC_TALARM_ID	    
        
    //a0=fn, 
    //a1=eventID, procID, (var2=0, var3=0)
    //a2=hour   (var0) 
    //a3=minute (var1)
    bal	    addRegEvent		    //meni t9, v0, v1
    
    //v0=0 error, jinak adresa polozky
    
    _POP    ra
    jrc	    ra    
    
.end rtcRegTimeAlarm   

//</editor-fold>
 
   
//locals
    
//<editor-fold defaultstate="collapsed" desc="setDayMs">
.align 2     
//.global setDayMs    
.ent setDayMs
setDayMs:
//meni a0, a1, v0, v1, (volane fce t0-t3, t8, t9)    
    
    //RTC sec * 1000
    //+ RTC min * 60
    //+ RTC hod * 60
    
    lw      a0, (RTCTIME)
    //src, dest
    _GET_SEC_FROM_BCD a0, t0		//meni t9
    _GET_MIN_FROM_BCD a0, t1		//meni t9
    _GET_HOUR_FROM_BCD a0, t2		//meni t9
    
    mtlo    t0				//Lo=sec
    mthi    zero			//Hi=0
    
    li	    t9, 60
    maddu   t1, t9	//k Lo prida minute  * 60
    
    li	    t9, 3600
    maddu   t2, t9	//k Lo prida hour  * 3600
    
    //Lo obsahuje pocet second od zacatku dne
    li	    t9, 1000
    mflo    t8		//t8=Lo
    
    mul	    t9, t8	//second * 1000
    
    sw	    t9, (day_ms)
    
    jrc	    ra
    
.end setDayMs 
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="rtcEvents">
.ent rtcEvents
rtcEvents:
    
    //meni a0-a3, v0, v1, t6-t9
    _PUSH   ra
    
    lw      a0, (RTCTIME)
    _GET_MIN_FROM_BCD a0, t6		//meni t9, t6=minute
    _GET_HOUR_FROM_BCD a0, t7		//meni t9, t7=hour
    
    move    a0, zero			//hledani od zacatku regEventTable
    li	    a1, RTC_TALARM_ID	

1:  
    //a0=adresa zacetku hledani
    //a1=eventID
    bal	    getRegEvent			//meni v0, v1, t9, (vraci v0=adresa, nebo 0x0 - polozka neni)
    beqz    v0, 2f			//skok, nenasel polozku
    
    //nasel polozku v regEventTable
    move    a0, v0			//adresa polozky v regEventTable
    //var1=minute
    lw	    v1, RET_V1(a0)		//load minute
    bne	    v1, t6, 1b			//skok, jina minuta
    //var0=hour
    lw	    v1, RET_V0(a0)		//load hour
    bne	    v1, t7, 1b			//skok, jina hodina    
    
    //a0=adresa polozky v regEventTable (pouzije z ni procID a fn addr)
    //a1=p0
    //a2=p2
    //a3=p3
    move    a1, t7			//param hour
    move    a2, t6			//param minute
    lw      a3, (RTCDATE)		//RTC DATE
    bal	    raiseEventInt		//meni t9, v0, v1
    
    //a1 bylo zmemeno, dosadit eventID
    li	    a1, RTC_TALARM_ID	
    b16	    1b
    
2:  
    _POP    ra
    jrc	    ra
      
.end rtcEvents 
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="iVector_rtc (RTC alarm)">

.align 2    
.global iVector_rtc   
.ent iVector_rtc
iVector_rtc:
   
    //allarm jednou za minutu pri sec=00
    
    //nastav day_ms, podle aktualniho casu
    //musi uchovat Hi/Lo, jsou spolecne pro vsechny SRS
    mflo    s0			//uchova Lo
    mfhi    s1			//uchova Hi
    
    bal	    setDayMs		//meni HiLo
    
    mthi    s1			//obnov Hi
    mtlo    s0			//obnov Lo
    
    //obsluha udalosti rtcAlarm
    bal	    rtcEvents		//meni a0-a3, v0, v1, t6-t9
    
#ifdef PIC32MM0064    
    li	    v0, (1 << 14)	//nuluj flag
    sw	    v0, (IFS0CLR)
#endif
    
#ifdef PIC32MM0256    
    li	    v0, (1 << 0)	//nuluj flag
    sw	    v0, (IFS1CLR)
#endif    
    
    eret

.end iVector_rtc
    
//</editor-fold>    
    
#endif	    //RTC