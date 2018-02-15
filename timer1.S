#include "xc.h"
#include "asm_macro.S"
#include "def.h"  

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * timer1, used by BOS core (system timer)
 * This file is shared to the PIC32MM, PIC32MZ
     
 timer_ms (uint) cita pocet ms od zapnuti pouziva jej systemovy citac (delay, interval), po preteceni se nuluje (perioda cca 49 dnu)
 day_ms (uint) pocita pocet ms od zacatku dne, bezi pouze pokud je definovano RTC, ktery jej kazdou minutu synchronizuje  
 spolu s RTC DD/MM/YYYY urcuje den/cas
 udalosti spoustene v konkretni cas pouzivaji day_ms    
     
systemove citace pracuji tak, ze po registraci (systemTimerRegInterval, nebo systemTimerRegDelay) odmeruji stanoveny
cas a po jeho uplynuti nastavi state byte. Aplikace testuje hodnotu state bytu a podle toho pozna ze nastala udalost
(je to tak proto, aby timer1_interrupt probehl co nejrychleji)     
 
systemTimerRegDelay po uplynuti provede automaticky systemTimerUnregInterval, zatimco systemTimerRegInterval bezi trvale
a systemTimerUnregInterval musi provest aplikace.
     
Max. pocet registrovanych udalosti pro casovac je v TIMER1_EVENT_CAPA
   
interval interruptu timer je nastaven v TIMER1_INTERVAL, pro MM 10ms, pro MZ 1ms. (o tuto hodnotu se zvysuje timer_ms i day_ms)     

ukazka pouziti casovacu viz. doc/system timer     
 */    
 

.set micromips
.section .asm_timer1, code 
    
#define	TIMER1_EVENT_ID		    1
    
//<editor-fold defaultstate="collapsed" desc="timer1Init">
    
.align 2
.global timer1Init
.ent timer1Init
timer1Init:
    //timer1 muze byt napajen z LPRC (interni Low Power RC) 32kHz, nebo z SecondaryOSC (externi) - 32.768kHz
    //stejne jako RTCC
    //v idle modu stale bezi
    
    //TIMER1_INTERVAL (def.h) definuje interval v ms (10-MM, 1-MZ)
    //pro 32.0kHz PR1=320 - 10ms, PR1=32 - 1ms 
    #define	PR1_VALUE			(32000/(1000/TIMER1_INTERVAL))
    
    //src muze byt LPRC, nebo SOSC (pak je nutne pouzit krystal a upravit cfg-bits, zapnout SOSC)
    #define	TIMER1_SRC_LPRC
    //#define	TIMER1_SRC_SOSC
    
    #define	TIMER1_INTERRUPT_PRIORITY	2
    #define	TIMER1_INTERRUPT_SUBPRIORITY	0
     
    sw	    zero, (timer_ms)			//timer_ms (uint) cita pocet ms od zapnuti
    sw	    zero, (day_ms)			//day_ms (uint) pocita pocet ms od zacatku dne, pouze pokud je definovano RTC
    
   
    //<editor-fold defaultstate="collapsed" desc="PIC32MM">
#ifdef PIC32MM    

    sw	    zero, (T1CON)	    //timer1 OFF (ON bit=0)
    sw	    zero, (TMR1)
    li	    t9, 0x0202
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
    
    move    a2, a1		    //time ms
    bal	    getProcID		    //meni v0, v1
    sll	    a1, v0, 8		    //procID do b8-b15
    ori	    a1, TIMER1_EVENT_ID	    
    move    a3, zero
    
    //a0=fn, 
    //a1=eventID, procID, (var2=0, var3=0)
    //a2=time (var0) 
    //a3=zero (var1)
    bal	    addRegEvent
    
    //v0=0 error, jinak adresa polozky
    
    jrc	    ra    
    
    
    
//a0=adresa status byte
//a1=interval ms 
//vraci v0=1 OK, v0=0 chyba, plna tabulka  
    
//po uplynuti casu nastavi status byte=1, nulovat ho pak musi app 
//udalost nastava periodicky, pro zruseni ji musi app unreg
    
    /*
    la	    v1, (timer1_events)
    addiu   v0, zero, TIMER1_EVENT_CAPA

1:    
    lw	    t9, (v1)
    bnez    t9, 2f
    //nop
    
    //volna polozka
    sw	    a0, (v1)				//adresa status byte
    sw	    a1, 4(v1)				//default value
    sw	    a1, 8(v1)				//value
   
    li16    v0, 1				//vraci OK
    jrc	    ra
    
2:
    addiu   v1, TIMER1_EVENT_ISIZE
    addiu   v0, -1
    bnez    v0, 1b
    //nop
    
    //v0=0, vraci chybu
    jrc	    ra
    */
    
.end systemTimerRegInterval    
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="systemTimerRegDelay">
    
.align 2
.global systemTimerRegDelay
.ent systemTimerRegDelay
systemTimerRegDelay:
//a0=adresa status byte
//a1=delay ms 
//vraci v0=1 OK, v0=0 chyba, plna tabulka 
    
//po uplynuti casu nastavi status byte=1, nulovat ho pak musi app  
//udalost nastane pouze jednou, app ji pak nemusi unreg (provede se automaticky)    
    
    
    la	    v1, (timer1_events)
    addiu   v0, zero, TIMER1_EVENT_CAPA

1:    
    lw	    t9, (v1)
    bnez    t9, 2f
    //nop
    
    //volna polozka
    sw	    a0, (v1)				//adresa status byte
    sw	    zero, 4(v1)				//default value = 0, neni repeat
    sw	    a1, 8(v1)				//value
   
    li16    v0, 1				//vraci OK
    jrc	    ra
    
2:
    addiu   v1, TIMER1_EVENT_ISIZE
    addiu   v0, -1
    bnez    v0, 1b
    //nop
    
    //v0=0, vraci chybu
    jrc	    ra     
    
    
.end systemTimerRegDelay
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="systemTimerUnregInterval">
    
.align 2
.global systemTimerUnregInterval
.ent systemTimerUnregInterval
systemTimerUnregInterval:
//a0=adresa status byte    
    
    _PUSH ra
    
    addiu   v0, zero, TIMER1_EVENT_CAPA
    la	    v1, timer1_events
1:    
    lw	    t9, (v1)
    bne	    t9, a0, 3f
    //nop
    
    //nasel polozku
    sw	    zero, (v1)	    //vymazat
    
    la	    a0, timer1_events
    li16    a1, TIMER1_EVENT_ISIZE
    addiu   a2, zero, TIMER1_EVENT_CAPA
    //a0=adresa tabulky, a1=velikost polozky, a2=max. pocet polozek
    la	    t9, (defragTableW)
    jalr    t9
    bal	    defragTableW
    //nop
    
2:
    _POP ra
    jrc	    ra
    //nop
    
3:
    addiu   v0, -1	    //count--
    addiu   v1, 4	    //item++ (event table)
    bnez    v0, 1b	    //skok na dalsi item
    //nop
    
    //nenasel polozku
    _POP    ra
    jrc	    ra
    //nop  
    
 
.end systemTimerUnregInterval  
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="iVector_timer1">
.align 2    
.global iVector_timer1   
.ent iVector_timer1
iVector_timer1:
    
    //#define INTERVAL	10		//10ms
    //timer1_events tabulka word[0]-adresa var, word[1]=default time, word[2]akt. value (citac) 
    
    lw	    v0, (timer_ms)	    
    addiu   v0, TIMER1_INTERVAL		//timer_ms += TIMER1_INTERVAL (10ms)
    sw	    v0, (timer_ms)
    
#ifdef RTC
    lw	    v0, (day_ms)
    addiu   v0, TIMER1_INTERVAL
    sw	    v0, (day_ms)
#endif    
   
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
    //a1-a3 nenastavuje
    //meni t9, v0, v1
    bal	    raiseEventInt
    
    //nove nastaveni var1 na puvodni hodnotu
    lw	    t2, RET_V0(t0)
    
3:
    //v teto polozce jeste nenastala event
    sw	    t2, RET_V1(t0)		//save hodnotu

    b16	    1b				//next
    
    //</editor-fold>

    
    /*
    
//word addr, word ms, word ms value    
    la	    v0, timer1_events
    addiu   v1, zero, TIMER1_EVENT_CAPA
    li16    a0, 1
    move    t0, zero			//priznak, zda byla vyrazena udalost
    	
1:    
    lw	    t9, (v0)			//adresa promene, kterou nastavi na 1, pokud nastala udalost
    beqz    t9, 4f			//skok, neplatna polozka, konec dat
    //nop
   
    //platna polozka
    lw	    t8, 8(v0)			//load aktualni value (citac)
    addiu   t8, -TIMER1_INTERVAL	//citac --
    sw	    t8, 8(v0)			//save citac
    bgtz    t8, 3f			//skok, neni 0, zadna udalost
    //nop
    
    //nastala udalost
    sb	    a0, (t9)			//nastav status byte registrovane udalosti
    lw	    t8, 4(v0)			//load default value (pokud je 0, neni udalost repeat)
    bnez    t8, 2f			//skok, je repeat
    //nop
    
    //t8=0, udalost neni repeat, zrusit 
    sw	    zero, (v0)
    addiu   t0, 1			//nastav priznak zruseni event

2:
    //je repeat, t8=default value (provede i kdyz neni repeat, nevadi to)
    sw	    t8, 8(v0)
    
3:
    //next
    addiu   v0, TIMER1_EVENT_ISIZE	//dalsi polozka
    addiu   v1, -1			//items--
    bnez    v1, 1b			//loop
    //nop
    
4:    
    beqz    t0, 5f			//skok, nebyla zrusena zadna udalost
    //nop
    //byla zrusena udalost (nebyla repeat), defrag. tabulku
    la	    a0, timer1_events
    addiu   a1, zero, TIMER1_EVENT_ISIZE
    addiu   a2, zero, TIMER1_EVENT_CAPA
    //a0=adresa tabulky, a1=velikost polozky, a2=max. pocet polozek (kapacita)
    bal	    defragTableW
    //nop
    
    */

5:    
    
    //<editor-fold defaultstate="collapsed" desc="clr FLAG">
    //konec, nuluj flag IFS0.4
#ifdef PIC32MZ    
    li16    v0, (1 << 4)
    sw      v0, (IFS0CLR)
#endif    
   
#ifdef PIC32MM0064   
    li	    v0, (1 << 11)
    sw      v0, (IFS0CLR)
#endif     
    
#ifdef PIC32MM0256   
    li	    v0, (1 << 17)
    sw      v0, (IFS0CLR)
#endif   
    //</editor-fold>

    eret
    //nop
  
.end iVector_timer1
  
//</editor-fold>            