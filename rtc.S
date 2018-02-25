#include "xc.h"
#include "def.h"    
#include "asm_macro.S"    
   
/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0    
 * RTC, used by BOS core
 * This file is shared to the PIC32MM, PIC32MZ
 */
    
//date a time se cte z RTC modulu
//presny cas (ms 32-bit) od zacatku dne je v day_ms, kterou incrementuje timer1
//day_ms se synchronizuje s RTC kazdou minutu (alarm interrupt)    
//(doba spusteni OS je v time_ms)

//Poskytuje event - RTC ALARM, v zadanou hodinu a minutu
//do fce predava i aktualni datum v BCD formatu (obsah RTCDATE)     
     
//bcd format detail:
//   b31       b23       b15       b7      b0
//   des  jed  des  jed  des  jed
//0b RRRR.RRRR-MMMM.MMMM-DDDD.DDDD-xxxx xWWW		//RTCDATE, WWW = week day
//      0    5    1    0    2    4         1		//05/10/24 Sunday     
     
//0b HHHH.HHHH-MMMM.MMMM-SSSS.SSSS-xxxx xxxx		//RTCTIME
//      1    4    3    5    0    2			//14:35:02
 
    
#ifdef RTC

.set reorder
.set micromips
.section .asm_rtc, code  
    
#ifdef PIC32MM 
    
    #define	    WRLOCK	    0x800		//RTCCON1.b11  
    #define	    TIME_SYNC	    0x4
    #define	    ALARM_SYNC	    0x2
    #define	    RTCCON1_SET	    0xC3FF0000		//ALARM enable, ON, ... 
    #define	    RTCCON1_ON	    0x00008000	    
    #define	    RTCCON2_SET	    0x3E800001		//clock=Low Power RC oscilator (32.0kHz)  
    
#endif	    //MM   
  
#ifdef PIC32MZ 
    
    #ifdef  SOSC_CLK_TO_RTC
	//SOSC 32.768kHz
	#define	    RTCCON_SET	    0x200		//SOSCO input (zkusebni deska nema ext. s.osc.)
    #else
	//LPRC
	#define	    RTCCON_SET	    0x0			//LPRC
    #endif	
    
    #define	    WRLOCK	    0x8			//RTCCON.b4  
    #define	    TIME_SYNC	    0x4
    #define	    RTCCON_ON	    0x8000
    #define	    RTC_ALRM	    0xC3FF		//alarm every minute
    
#endif	    //MZ     
    
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
    
//<editor-fold defaultstate="collapsed" desc="_GET_Y_FROM_BCD">
.macro _GET_Y_FROM_BCD src, dest
    //do t0 year 2000-2099
    ext     \dest, \src, 28, 4		    //4 bits desitky
    li	    t9, 10
    mul     \dest, t9              
    ext     t9, \src, 24, 4		    //4 bits jednotky
    addu    \dest, t9			    //dest=rok 0-99
.endm
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="_GET_M_FROM_BCD">
.macro _GET_M_FROM_BCD src, dest
    //month 1-12
    ext     \dest, \src, 20, 4	    //4 bits desitky
    li      t9, 10
    mul     \dest, t9
    ext     t9, \src, 16, 4         //4 bits jednotky
    addu    \dest, t9
.endm
//</editor-fold>
     
//<editor-fold defaultstate="collapsed" desc="_GET_D_FROM_BCD">
.macro _GET_D_FROM_BCD src, dest    
    //day 1-31
    ext     \dest, \src, 12, 4	    //4 bits desitky
    li      t9, 10
    mul     \dest, t9
    ext     t9, \src, 8, 4	    //4 bits jednotky
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
    //nop
    ei
    
1001:
    
.endm    
    
//</editor-fold>  
    
//<editor-fold defaultstate="collapsed" desc="_DISABLE_DMA_INT">
.macro _DISABLE_DMA_INT
//zakaze DMA (nastavi SUSPEND), zalohu DMACON do t8 
//zakaza interrupt (DI)	      , zalohu bitu do t9
//meni t8, t9, v0    
    
    mfc0    t9, _CP0_STATUS
    ext     t9, t9, 0, 1
    di

    //zaloha DMACON
    lw      t8, (DMACON)
    //ext	    t8, t8, 12, 1 
    
    //DMA suspend 
    addiu   v0, zero, 0x1000			//set b12 (suspend)
    sw      v0, (DMACONSET)
    
1001:
    //cakat, dokud se DMA neuvolni
    lw      v0, (DMACON)
    ext     v0, v0, 11, 1			//DMA busy
    bnez    v0, 1001b
    //nop    
    
.endm 
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="_RESTORE_DMA_INT">
.macro _RESTORE_DMA_INT    
//restore DMA suspend, zaloha v t8 (cely DMACON)
//restore interrupt  , zaloha v t9   
   
    sw	    t8, (DMACON)
    //beqz    t8, 1001f			//skok, pokud DMA bylo SUSPEND, zaloha v t8
    //nop
    //nuluj DMA SUSPEND
    //addiu   v0, zero, 0x1000		//DMACON.12 = SUSPEND, nulovat
    //sw      v0, (DMACONCLR)

//1001:    
    beqz    t9, 1002f			//skok pokud nema povolit interrupt (zaloha EI v t9)
    //nop
    ei
    
1002:
.endm    
    
//</editor-fold>      
    
//<editor-fold defaultstate="collapsed" desc="_SYSKEY_WRLOCK_ENABLE">
.macro _SYSKEY_WRLOCK_ENABLE   
//nuluj RTCCON1.WRLock, pouze po syskey    
    
#ifdef PIC32MM    
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
    //nop
#endif    

#ifdef PIC32MZ    
1001:
    //nastavi RTCCON.3, ceka dokud neni nastaven
    move    v0, zero
    sw      v0, (SYSKEY)
    li      v0, 0xaa996655
    sw      v0, (SYSKEY)
    li      v0, 0x556699aa
    sw      v0, (SYSKEY)
    
    //nastav RTCCON.3 (RTCWREN)
    li16    v0, WRLOCK			//SET RTCCON.WREN
    sw      v0, (RTCCONSET)
    sync

    lw      v0, (RTCCON)		//ceka na WREN=1 (obcas se musi opakovat unlock 2x)
    li	    v1, WRLOCK
    and	    v0, v1    
    beqz    v0, 1001b
    //nop   

1002:
    //cekani na sync, RTCCON.2
    lw      v0, (RTCCON)            //ceka na sync=0
    ext     v0, v0, 2, 1
    bnez    v0, 1002b
    
    
#endif     
    
.endm    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="_WRLOCK_DISABLE">
.macro _WRLOCK_DISABLE    
    
#ifdef PIC32MM
    //WRLOCK=1
    li	    v0, WRLOCK			//set RTCCON1.WRLOCK
    sw      v0, (RTCCON1SET)    
#endif     
    
#ifdef PIC32MZ     
    //WRLOCK=0
    li	    v0, WRLOCK			//set RTCCON.WRLOCK
    sw      v0, (RTCCONCLR)
#endif 
    
.endm  
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="_DATETIME_SYNC">

.macro _DATETIME_SYNC

#ifdef PIC32MM    
    //ceka, dokud RTCSTAT.b2 != 0 (pri 1 probiha zmena RTC)
1001:    
    lw	    v0, (RTCSTAT)
    li	    v1, TIME_SYNC
    and	    v0, v1
    bnez    v0, 1001b
    //nop
#endif     
    
#ifdef PIC32MZ
    //cekani na sync, RTCCON.2
1001:     
    lw      v0, (RTCCON)            //ceka na sync=0
    li	    v1, TIME_SYNC
    and	    v0, v1
    bnez    v0, 1001b
    //nop 
#endif 
    
.endm  
    
//</editor-fold>
  
//<editor-fold defaultstate="collapsed" desc="_ALARM_SYNC">

.macro _ALARM_SYNC    

#ifdef PIC32MM     
1001:  
    //ceka, dokud RTCSTAT.b1 != 0 (pri 1 probiha zmena RTC)
    lw	    v0, (RTCSTAT)
    li	    v1, ALARM_SYNC
    and	    v0, v1
    bnez    v0, 1001b
    //nop
#endif 
    
#ifdef PIC32MZ
    
#endif     
    
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

    #define	RTC_INTERRUPT_PRIORITY		1
    #define	RTC_INTERRUPT_SUBPRIORITY	2	
    
    _PUSH   ra
    //POZOR, v prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma suspend
    
//<editor-fold defaultstate="collapsed" desc="PIC32MM">
    
#ifdef PIC32MM    
    
    //test, zda je RTC modul zapnuty, pokud ano, nemeni nastaveni date/time
    //RTC zustava zapnuty pri RESETu i probuzeni ze sleep modu (nuluje ho pouze PowerOnReset)
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
    
    //</editor-fold>
    
    //t9=stav CP0.STATUS.EI, disable interrupt
    //nastav WRLock=0
    _DISABLE_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK_ENABLE			//meni v0, v1
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
    _WRLOCK_DISABLE
    //li	    v0, WRLOCK			//set RTCCON1.WRLOCK
    //sw      v0, (RTCCON1SET)

     //restore interrupt
    _RESTORE_INT			//ze zalohy t9, meni v0

#endif	    //PIC32MM
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="PIC32MZ">
#ifdef PIC32MZ   
    
    //test, zda je RTC modul zapnuty, pokud ano, nemeni nastaveni date/time
    //RTC zustava zapnuty pri RESETu i probuzeni ze sleep modu (nuluje ho pouze PowerOnReset)
    lw	    v0, (RTCCON)
    ext	    v0, v0, 15, 1		//RTCCON.15=ON
    bnez    v0, 1f
    //nop
    
    //<editor-fold defaultstate="collapsed" desc="interrupt priority...">
    
    //OFF166
    li	    v1, EBASE
    la	    v0, iVector_rtc
    subu    v0, v1
    sw	    v0, (OFF166)
    
     //priority IPC41.b18-20
    li	    t9, (0b111 << 18)
    sw	    t9, (IPC41CLR)					//nuluj bity
    li	    t9, (RTC_INTERRUPT_PRIORITY << 18)
    sw	    t9, (IPC41SET)					//nastav bity
    
    //subpriority IPC41.b16-17
    li	    t9, (0b11 << 16)
    sw	    t9, (IPC41CLR)					//nuluj bity
    li	    t9, (RTC_INTERRUPT_SUBPRIORITY << 16)
    sw	    t9, (IPC41SET)					//nastav bity
   
    //IEC5.b6, IFS5.b6
    li	    v0, (1 << 6)
    sw	    v0, (IFS5CLR)					//flag
    sw	    v0, (IEC5SET)					//enable
    
    //</editor-fold>

    //t9=stav CP0.STATUS.EI, disable interrupt
    //t8=stav DMA.SUSPEND
    _DISABLE_DMA_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK_ENABLE			//meni v0, v1
    //_DATETIME_SYNC			//nemi v0, v1
    //_ALARM_SYNC				//meni v0, v1    
    
    //set time, date
    li      v0, 0x00000000              //00:00:00
    sw      v0, (RTCTIME)
    sw	    v0, (ALRMTIME)
    sw	    v0, (ALRMDATE)
    li      v0, 0x01010102              //2001, leden, 1, den v tydnu 7=SO (1=NE, 2=PO)
    sw      v0, (RTCDATE)
    
    //set alarm interrupt
    li	    v0, RTC_ALRM
    sw	    v0, (RTCALRM)
    
    //nastavi CLKSEL 0x0=LPRC, 0x200=SOSCO
    li	    v0, RTCCON_SET
    sw	    v0, (RTCCONSET)
    
    //ON=1
    li	    v0, RTCCON_ON
    sw	    v0, (RTCCONSET)
    
    //WRLOCK=0
    _WRLOCK_DISABLE
    
    _RESTORE_DMA_INT			//meni t8, t9, v0
    
#endif	    //PIC32MZ  
    
//</editor-fold>
    
1:      
    //nastav day_ms, aby byla synchronizonava s RTC
    bal	    setDayMs
    
    _POP    ra
    jrc     ra
    
.end rtcInit
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="rtcGetTimeBCD">

.align 2    
.global rtcGetTimeBCD
.ent rtcGetTimeBCD
rtcGetTimeBCD:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    v0, (RTCTIME)
    
    jrc	    ra
    
.end rtcGetTimeBCD 
    
//</editor-fold>
  
//<editor-fold defaultstate="collapsed" desc="rtcGetDateBCD">
    
.align 2    
.global rtcGetDateBCD
.ent rtcGetDateBCD
rtcGetDateBCD:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    v0, (RTCDATE)
    
    jrc	    ra
    
.end rtcGetDateBCD  
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="rtcSetTimeBCD">
    
.align 2
.global rtcSetTimeBCD    
.ent rtcSetTimeBCD
rtcSetTimeBCD:
//a0=BCD time
    
//bcd format
//   des  jed  des  jed  des  jed
//0b RRRR.RRRR-MMMM.MMMM-DDDD.DDDD-xxxx xWWW - week day
//0b HHHH.HHHH-MMMM.MMMM-SSSS.SSSS-xxxx xxxx    
    
    _PUSH   ra
    
#ifdef PIC32MM
    
    //POZOR, v dalsim prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma s
    _DISABLE_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK_ENABLE			//meni v0, v1
    _DATETIME_SYNC			//nemi v0, v1
    _ALARM_SYNC				//meni v0, v1

    //set time, date
    sw      a0, (RTCTIME)
    
    //WRLOCK=1
    _WRLOCK_DISABLE
    //li	    v0, WRLOCK			//set RTCCON1.WRLOCK
    //sw      v0, (RTCCON1SET)
    
    //restore interrupt
    _RESTORE_INT			//ze zalohy t9, meni v0    
    
#endif
    
#ifdef PIC32MZ    
    //POZOR, v dalsim prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma s
    _DISABLE_DMA_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK_ENABLE			//meni v0, v1
    _DATETIME_SYNC			//nemi v0, v1
    _ALARM_SYNC				//meni v0, v1

    //set time, date
    sw      a0, (RTCTIME)
    
    //WRLOCK=0
    _WRLOCK_DISABLE
    //li	    v0, WRLOCK			//set RTCCON1.WRLOCK
    //sw      v0, (RTCCON1SET)
    
    //restore interrupt
    _RESTORE_DMA_INT			//ze zalohy t9, meni v0       
#endif    
    
    bal	    setDayMs
    //nop
    
    _POP    ra
    jrc     ra
    
.end rtcSetTimeBCD 
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="rtcSetDateBCD">
    
.align 2
.global rtcSetDateBCD    
.ent rtcSetDateBCD
rtcSetDateBCD:
//a0=BCD time
    
//bcd format
//   des  jed  des  jed  des  jed
//0b RRRR.RRRR-MMMM.MMMM-DDDD.DDDD-xxxx xWWW - week day
//0b HHHH.HHHH-MMMM.MMMM-SSSS.SSSS-xxxx xxxx    
   
#ifdef PIC32MM    
    //POZOR, v dalsim prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma s
    _DISABLE_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK			//meni v0, v1
    _DATETIME_SYNC			//nemi v0, v1
    _ALARM_SYNC				//meni v0, v1

    //set time, date
    sw      a0, (RTCDATE)
    
    //WRLOCK=1
    _WRLOCK_DISABLE
    
    //restore interrupt
    _RESTORE_INT			//ze zalohy t9, meni v0    
#endif
    
#ifdef PIC32MZ    
    //POZOR, v dalsim prubehu fce je t8 a t9 pouzito jako zaloha stavu interrupt a dma s
    _DISABLE_DMA_INT			//meni t8, t9, v0
    
    //nuluj RTCCON1.WRLOCK
    _SYSKEY_WRLOCK_ENABLE		//meni v0, v1
    _DATETIME_SYNC			//nemi v0, v1
    _ALARM_SYNC				//meni v0, v1

    //set time, date
    sw      a0, (RTCDATE)
    
    //WRLOCK=0
    _WRLOCK_DISABLE
    
    //restore interrupt
    _RESTORE_DMA_INT			//ze zalohy t9, meni v0    
#endif    
    
    jrc     ra
    
.end rtcSetDateBCD 
    
//</editor-fold>   
    
//<editor-fold defaultstate="collapsed" desc="rtcGetHalfSecond">

.align 2
.global rtcGetHalfSecond
.ent rtcGetHalfSecond
rtcGetHalfSecond:    
    
#ifdef PIC32MM    
    //RTCSTAT.b0
    lw	    v0, (RTCSTAT)
    ext	    v0, v0, 0, 1
#endif    
    
#ifdef PIC32MZ     
    lw	    v0, (RTCCON)
    ext	    v0, v0, 1, 1
#endif     
    
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

    
//<editor-fold defaultstate="collapsed" desc="rtcGetHour">
    
.align 2    
.global rtcGetHour
.ent rtcGetHour
rtcGetHour:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    t8, (RTCTIME)
    
    //src, dest
    _GET_HOUR_FROM_BCD t8, v0		//meni t9
    
    jrc	    ra
    
.end rtcGetHour  
    
//</editor-fold>    
   
//<editor-fold defaultstate="collapsed" desc="rtcGetMinute">
    
.align 2    
.global rtcGetMinute
.ent rtcGetMinute
rtcGetMinute:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    t8, (RTCTIME)
    
    //src, dest
    _GET_MIN_FROM_BCD t8, v0		//meni t9
    
    jrc	    ra
    
.end rtcGetMinute  
    
//</editor-fold>     
  
//<editor-fold defaultstate="collapsed" desc="rtcGetSecond">
    
.align 2    
.global rtcGetSecond
.ent rtcGetSecond
rtcGetSecond:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    t8, (RTCTIME)
    
    //src, dest
    _GET_SEC_FROM_BCD t8, v0		//meni t9
    
    jrc	    ra
    
.end rtcGetSecond  
    
//</editor-fold>     
    
//<editor-fold defaultstate="collapsed" desc="rtcGetYear">
    
.align 2    
.global rtcGetYear
.ent rtcGetYear
rtcGetYear:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    t8, (RTCDATE)
    
    //src, dest
    _GET_Y_FROM_BCD t8, v0		//meni t9
    
    jrc	    ra
    
.end rtcGetYear  
    
//</editor-fold>      
    
//<editor-fold defaultstate="collapsed" desc="rtcGetMonth">
    
.align 2    
.global rtcGetMonth
.ent rtcGetMonth
rtcGetMonth:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    t8, (RTCDATE)
    
    //src, dest
    _GET_M_FROM_BCD t8, v0		//meni t9
    
    jrc	    ra
    
.end rtcGetMonth  
    
//</editor-fold>     
    
//<editor-fold defaultstate="collapsed" desc="rtcGetDay">
    
.align 2    
.global rtcGetDay
.ent rtcGetDay
rtcGetDay:
    
    _DATETIME_SYNC			//nemi v0, v1
    lw	    t8, (RTCDATE)
    
    //src, dest
    _GET_D_FROM_BCD t8, v0		//meni t9
    
    jrc	    ra
    
.end rtcGetDay  
    
//</editor-fold>     
    
//<editor-fold defaultstate="collapsed" desc="rtcGetDayOfWeek">
    
.align 2    
.global rtcGetDayOfWeek
.ent rtcGetDayOfWeek
rtcGetDayOfWeek:
    
    //1=NE, 2=PO, ... 7=SO
    _DATETIME_SYNC			//nemi v0, v1
    lw	    v0, (RTCDATE)
    ext	    v0, v0, 0, 3
    
    jrc	    ra
    
.end rtcGetDayOfWeek  
    
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
    
    //lw	    t8, (day_ms)
    sw	    t9, (day_ms)		//rozdil cca 100ms / minutu
    
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
   
    
#ifdef PIC32MM0064 
    #define	    RTC_IFS_BIT		14
    #define	    RTC_IFS_REG_CLR	IFS0CLR     
#endif
    
#ifdef PIC32MM0256 
    #define	    RTC_IFS_BIT		0
    #define	    RTC_IFS_REG_CLR	IFS1CLR     
#endif    
    
#ifdef PIC32MZ
    #define	    RTC_IFS_BIT		6
    #define	    RTC_IFS_REG_CLR	IFS5CLR 
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
#endif  
    
       
    //nastav day_ms, podle aktualniho casu
    //musi uchovat Hi/Lo, jsou spolecne pro vsechny SRS
    mflo    s0			//uchova Lo
    mfhi    s1			//uchova Hi
    
    bal	    setDayMs		//meni HiLo
    
    mthi    s1			//obnov Hi
    mtlo    s0			//obnov Lo
    
    //obsluha udalosti rtcAlarm
    bal	    rtcEvents		//meni a0-a3, v0, v1, t6-t9
    
    
    //test
    /*
    li	    t9, 0xFFFFFF
1:
    addiu   t8, 1
    addiu   t8, 1
    addiu   t8, 1
    addiu   t8, 1
    addiu   t8, 1
    addiu   t8, 1
    
    addiu   t9, -1
    bnez    t9, 1b
    */

   
    //clear flag
    li	    v0, (1 << RTC_IFS_BIT)
    sw      v0, (RTC_IFS_REG_CLR)     
    sync			//bez sync je interrupt volan 2x (MZ)
    
#ifdef PIC32MZ    
    _INT_EPILOGUE
#endif 
    
    eret

.end iVector_rtc
    
//</editor-fold>    
    
#endif	    //RTC