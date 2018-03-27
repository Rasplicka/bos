#include "xc.h"
#include "asm_macro.S"
#include "def.h"  

/*
 * Author Jiri Rasplicka 
 * Copyright (c) 2017, All rights reserved.      
 * timer1, used by BOS core (system timer)
 * This file is shared for the PIC32MM, PIC32MZ
 * Last update 1/2/2018    
 * Ver. 1.0    
     
time_ms (uint) cita pocet ms od zapnuti, po preteceni se nuluje (perioda cca 49 dnu)
day_ms (uint) pocita pocet ms od zacatku dne, bezi pouze pokud je definovano RTC, ktery jej kazdou minutu synchronizuje  
spolu s RTC DD/MM/YYYY urcuje den/cas
    
Interval events pracuji tak, ze po registraci (systemTimerRegInterval) odmeruji stanoveny
cas a po jeho uplynuti vola raiseEventInt. Systemovy proces pak tuto udalost spusti.
   
interval interruptu timer1 je nastaven v TIMER1_INTERVAL, pro MM 10ms, pro MZ 1ms. (o tuto hodnotu se zvysuje time_ms i day_ms)     

ukazka pouziti casovacu viz. doc/system timer     
 */    
 

.set micromips
.section .asm_timer1, code 
    
//<editor-fold defaultstate="collapsed" desc="timer1Init">
    
.align 2
.global timer1Init
.ent timer1Init
timer1Init:
    //timer1 muze byt napajen z LPRC (interni Low Power RC) 32kHz, nebo z SecondaryOSC (externi) - 32.768kHz
    //stejne jako RTCC
    //v idle modu stale bezi
    
    //MM - src muze byt LPRC, nebo SOSC (pak je nutne pouzit krystal a upravit cfg-bits, zapnout SOSC)
    //MZ - src muze byt PBCLK3, nebo SOSC
    
    #define	TIMER1_INTERRUPT_PRIORITY	2
    #define	TIMER1_INTERRUPT_SUBPRIORITY	1
     
    sw	    zero, (time_ms)			//time_ms (uint) cita pocet ms od zapnuti
    //sw	    zero, (day_ms)			//day_ms (uint) pocita pocet ms od zacatku dne, pouze pokud je definovano RTC
    
    sw	    zero, (T1CON)			//timer1 OFF (ON bit=0)
    sw	    zero, (TMR1)
   
    //<editor-fold defaultstate="collapsed" desc="PIC32MM">
#ifdef PIC32MM    
    
    //TIMER1_INTERVAL (def.h) definuje interval v ms (10-MM, 1-MZ)
    //pro 32.0kHz PR1=320 - 10ms, PR1=32 - 1ms 
    #define	PR1_VALUE			(32000/(1000/TIMER1_INTERVAL))

    //sw    zero, (T1CON)	    //timer1 OFF (ON bit=0)
    li	    t9, 0x2202		    //0x2202 pri SIDL=1 (stop in idle/sleep)
    sw	    t9, (T1CON)		    //setting (ON=0), src=LPRC (32kHz), div=1:1, idle-run, 
    
    //<editor-fold defaultstate="collapsed" desc="PIC32MM0064">
#ifdef PIC32MM0064 
    //IPC/IEC/IFS
    
    //priority IPC2.b26-28
    li	    t9, (0b111 << 26)
    sw	    t9, (IPC2CLR)					//nuluj bity
    li	    t9, (TIMER1_INTERRUPT_PRIORITY << 26)
    sw	    t9, (IPC2SET)					//nastav bity
    
    //subpriority IPC2.b24-25
    li	    t9, (0b11 << 24)
    sw	    t9, (IPC2CLR)					//nuluj bity    
    li	    t9, (TIMER1_INTERRUPT_SUBPRIORITY << 24)
    sw	    t9, (IPC2SET)					//nastav bity
    
    
    //IFS0.b11=0, IEC0.b11=1
    li	    t9, (1<<11)
    sw	    t9, (IFS0CLR)
    sw	    t9, (IEC0SET)
    
#endif
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="PIC32MM0256">
#ifdef PIC32MM0256    
    //IPC/IEC/IFS
    
    //priority IPC4.b10-12
    li	    t9, (0b111 << 10)
    sw	    t9, (IPC4CLR)					//nuluj bity
    li	    t9, (TIMER1_INTERRUPT_PRIORITY << 10)
    sw	    t9, (IPC4SET)					//nastav bity
    
    //subpriority IPC4.b8-9
    li	    t9, (0b11 << 8)
    sw	    t9, (IPC4CLR)					//nuluj bity
    li	    t9, (TIMER1_INTERRUPT_SUBPRIORITY << 8)
    sw	    t9, (IPC4SET)					//nastav bity
    
    
    //IFS0.b17=0, IEC0.b17=1
    li	    t9, (1<<17)
    sw	    t9, (IFS0CLR)
    sw	    t9, (IEC0SET)
    
#endif    
    //</editor-fold>

    
    //PR1 je komparativni reg. (pro 32.0kHz a 10ms = 320) 
    li	    t9, PR1_VALUE
    sw	    t9, (PR1)
    
    //T1CON.b15 = ON
    li	    t9, (1<<15)
    sw	    t9, (T1CONSET)
    
#endif	//PIC32MM
    //</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="PIC32MZ">
#ifdef PIC32MZ
    //TIMER1_INTERVAL (def.h) definuje interval v ms (10-MM, 1-MZ)
    //CPU=200MHz, PBCLK=100MHz, T1_PR1_VALUE=12500 (1ms)
    //!!!pri zmene TIMER1_INTERVAL=10 je PR1_VALUE prilis velke (zmenit divide v T1CON)
    //pri pouziti SOSC je nutne v sleep modu pozastavit timer1, protoze SOSC bezi stale
    
    //SLEEP mode timer1 je pozastaven
    //IDLE  mode timer1 bezi dal (a spousti events)
    
    //timer1 pouziva PBCLK3 (100MHz)
    #define	T1_PBCLK3		    100000000
    #define	T1_PR1_VALUE		    (T1_PBCLK3 / 8 /(1000/TIMER1_INTERVAL))    //divide 8
    //#define PR1_VALUE			    (T1_PBCLK3 / 64 /(1000/TIMER1_INTERVAL))   //divide 64
    li	    t9, 0x0010
    sw	    t9, (T1CON)			    //setting (ON=0), src=PBCLK3 (100MHz), div=1:8, idle-run, 
    
    //OFF004
    li	    v1, EBASE
    la	    v0, iVector_timer1
    subu    v0, v1
    sw	    v0, (OFF004)
    
    //IPC/IEC/IFS
    
    //priority IPC1.b2-4
    li	    t9, (0b111 << 2)
    sw	    t9, (IPC1CLR)					//nuluj bity
    li	    t9, (TIMER1_INTERRUPT_PRIORITY << 2)
    sw	    t9, (IPC1SET)					//nastav bity
    
    //subpriority IPC1.b0-1
    li	    t9, (0b11 << 0)
    sw	    t9, (IPC1CLR)					//nuluj bity    
    li	    t9, (TIMER1_INTERRUPT_SUBPRIORITY << 0)
    sw	    t9, (IPC1SET)					//nastav bity
    
    
    //IFS0.b4=0, IEC0.b4=1
    li	    t9, (1<<4)
    sw	    t9, (IFS0CLR)
    sw	    t9, (IEC0SET)    
    
    
    //PR1 je komparativni reg. (pro 32.0kHz a 10ms = 320) 
    li	    t9, T1_PR1_VALUE
    sw	    t9, (PR1)
    
    //T1CON.b15 = ON
    li	    t9, (1<<15)
    sw	    t9, (T1CONSET)
    
#endif	//PIC32MZ  
    //</editor-fold>
  
    jrc	    ra
    
.end timer1Init   
    
//</editor-fold>
 
//<editor-fold defaultstate="collapsed" desc="systemTimerRegInterval">
    
.align 2
.global systemTimerRegInterval
.ent systemTimerRegInterval
systemTimerRegInterval:
//a0=adresa fce, a1=time ms
    
    _PUSH   ra
    
    move    a2, a1		    //interval ms
    move    a3, a1		    //prvni nastaveni intervalu ms
    bal	    getProcID		    //meni v0, v1
    sll	    a1, v0, 8		    //procID do b8-b15
    ori	    a1, TIMER1_EVENT_ID	    
    
    //a0=fn, 
    //a1=eventID, procID, (var2=0, var3=0)
    //a2=time (var0) 
    //a3=zero (var1)
    bal	    addRegEvent		    //meni t9, v0, v1
    
    //v0=0 error, jinak adresa polozky
    
    _POP    ra
    jrc	    ra    
    
.end systemTimerRegInterval    
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="iVector_timer1">
.align 2    
.global iVector_timer1   
.ent iVector_timer1
iVector_timer1:
    
#ifdef PIC32MM0064 
    #define	    IFS_BIT	    11
    #define	    IFS_REG_CLR	    IFS0CLR     
#endif
    
#ifdef PIC32MM0256 
    #define	    IFS_BIT	    17
    #define	    IFS_REG_CLR	    IFS0CLR     
#endif    
#ifdef PIC32MZ
    #define	    IFS_BIT	    4
    #define	    IFS_REG_CLR	    IFS0CLR 
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
#endif    
    
    
    lw	    v0, (time_ms)	    
    addiu   v0, TIMER1_INTERVAL		//time_ms += TIMER1_INTERVAL (1/10ms)
    sw	    v0, (time_ms)
    
//<editor-fold defaultstate="collapsed" desc="NETCOM">
#if defined USE_UARTNETCOM 
    lw	    v0, (netcom_ms)
    beqz    v0, 1f				//skok, nenastavuje time
    
    addiu   v0, TIMER1_INTERVAL
    sw	    v0, (netcom_ms)
    li	    v1, 50
    bne	    v0, v1, 1f
    
    //nastala udalost
    la	    v0, netcomNotRespond
    _CALL_C v0, 0
    1:
    
    
    #if defined NETCOM_DEVID==1 
	//pouze DevID=1, init bus
	lhu	v0, (netcomStratup_ms)
	addiu   v0, TIMER1_INTERVAL
	sh	v0, (netcomStratup_ms)
        
	li	v1, 100
	bne	v0, v1, 1f
	//netcomStratup_ms = 0, startUp com
	//.. vola start fci
  
	sh	zero, (netcomStratup_ms)	//nastav novy cas
        1:    
   
    #endif  //NETCOM_DEVID==1 
	
#endif  //USE_UARTNETCOM  
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="events">
    
    la	    t0, regEventTable
    addiu   t1, t0, (REG_EVENT_TABLE_ISIZE * REG_EVENT_TABLE_CAPA)
    li	    t3, TIMER1_EVENT_ID
    b16	    2f			//skok, je zadan zacatek hledani (naposledy nalezena)
    
1:    
    addiu   t0, REG_EVENT_TABLE_ISIZE	//dalsi polozka
    beq	    t0, t1, 5f			//skok, je na konci    
    
2:    
    lbu	    t9, RET_EVENTID(t0)		//load eventID (0=empty)
    beqz    t9, 5f			//skok, konec	
    bne	    t9, t3, 1b			//skok, jine eventID  
    
    //ok, nasel polozku
    //var0=nastaveny cas, var1=aktualni cas
    lw	    t2, RET_V1(t0)	
    addiu   t2, -TIMER1_INTERVAL	//t2=nova hodnota	    
    bgtz    t2, 3f			//skok, cas nevyprsel, udalost nenastala
    
    //nastala event
    
    move    a0, t0			//a0=adresa polozky v regEventTable, a1-a3 params
    lw	    a1, RET_V0(t0)		//a1=interval
    //a2-a3 nenastavuje
    bal	    raiseEventInt		//meni t9, v0, v1
    
    //nove nastaveni var1 na puvodni hodnotu
    lw	    t2, RET_V0(t0)
    
3:
    //v teto polozce jeste nenastala event
    sw	    t2, RET_V1(t0)		//save hodnotu

    b16	    1b				//next
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="clr FLAG, eret">
    //konec, nuluj flag IFS0.4
5:    
    
    li	    v0, (1 << IFS_BIT)
    sw      v0, (IFS_REG_CLR)
    sync
    
#ifdef PIC32MZ    
    _INT_EPILOGUE
#endif    
    
    eret
//</editor-fold>

  
.end iVector_timer1
  
//</editor-fold>            