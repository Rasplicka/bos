#include "xc.h"
#include "def.h"    
#include "asm_macro.S"    
   
/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0    
 * Change Notification, used by BOS core
 * This file is shared to the PIC32MM, PIC32MZ
 */
    
    
.set micromips 
.set reorder			//vklada nop do branch delay slotu
    
.section .asm_cn, code      
    
#define		    CN_INTERRUPT_PRIORITY	2
#define		    CN_INTERRUPT_SUBPRIORITY	3
    
//<editor-fold defaultstate="collapsed" desc="cnStartPortA">
.global cnStartPortA
.ent cnStartPortA
cnStartPortA:
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTA_BASE + CNCON_OFFSET)
    
    
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF118
    li	    v1, EBASE
    la	    v0, cnAInterrupt
    subu    v0, v1
    sw	    v0, (OFF118)
    
    //priority IPC29.b18-20
    li	    t9, (0b111 << 18)
    sw	    t9, (IPC29CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 2)
    sw	    t9, (IPC29SET)					//nastav bity
    
    //subpriority IPC29.b16-17
    li	    t9, (0b11 << 16)
    sw	    t9, (IPC29CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 0)
    sw	    t9, (IPC29SET)					//nastav bity
    
    
    //IFS3.b22=0, IEC3.b22=1
    li	    t9, (1<<22)
    sw	    t9, (IFS3CLR)
    sw	    t9, (IEC3SET)    
    
    jrc	    ra
    
.end cnStartPortA 
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="cnStopPortA">
.global cnStopPortA
.ent cnStopPortA
cnStopPortA:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTA_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC3.b22=0
    li	    t9, (1<<22)
    sw	    t9, (IEC3CLR)				//disable interrupt
    
    jrc	    ra
    
.end cnStopPortA 
    
//</editor-fold>


//<editor-fold defaultstate="collapsed" desc="cnEnable">
.global cnEnable
.ent cnEnable
cnEnable:
//a0=port_base
//a1=bits
    
    //povoli ChangeNotification pro zadany port/bit
    //cnStartPortA(0);					    //zaktivuje fci CN (SIDL=0, pracuje v IDLE/SLEEP)
    //cnEnable(PORTA_BASE, BIT0 | BIT1);		    //povoli PORTA.0, PORTA.1
    
    addiu   a0, (CNENA_OFFSET + SET_OFFSET)
    sw	    a1, (a0)
    
    jrc	    ra
    
.end cnEnable 
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="cnDisble">
.global cnDisble
.ent cnDisble
cnDisble:
//a0=port_base
//a1=bits
    
    //zakaze ChangeNotification pro zadany port/bit
    
    addiu   a0, (CNENA_OFFSET + CLR_OFFSET)
    sw	    a1, (a0)
    
    jrc	    ra
    
.end cnDisble 
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="cnRegEvent">
.global cnRegEvent
.ent cnRegEvent
cnRegEvent:
//a0=adresa fce
//a1=port_base
    
    _PUSH   ra
    
    move    a2, a1		    //a2=port_base
    move    a3, zero

    bal	    getProcID		    //meni v0, v1
    sll	    a1, v0, 8		    //procID do b8-b15
    ori	    a1, CN_EVENT_ID	    
        
    //a0=fn, 
    //a1=eventID, procID, (var2=0, var3=0)
    //a2=port_base  (var0) 
    //a3=0	    (var1)
    bal	    addRegEvent		    //meni t9, v0, v1
    
    //v0=0 error, jinak adresa polozky
    
    _POP    ra
    jrc	    ra    
    
.end cnRegEvent
    
//</editor-fold>

    
//<editor-fold defaultstate="collapsed" desc="cnEvents">
.ent cnEvents
cnEvents:
//a0=port_base  (adresa base)  
//a1=port_state (hodnota CNSTATx)    
    
    //meni a0-a3, v0, v1, t6-t9
    _PUSH   ra
    
    move    t6, a0			//zaloha a0
    move    t7, a1			//zaloha a1
    move    a0, zero			//hledani od zacatku regEventTable
    li	    a1, CN_EVENT_ID		//time alarm eventID

1:  
    //a0=adresa zacetku hledani
    //a1=eventID
    bal	    getRegEvent			//meni v0, v1, t9, (vraci v0=adresa, nebo 0x0 - polozka neni)
    beqz    v0, 2f			//skok, nenasel polozku
    
    //nasel polozku v regEventTable
    move    a0, v0			//adresa polozky v regEventTable
    //var0=port
    lw	    v1, RET_V0(a0)		//load port
    bne	    v1, t6, 1b			//skok, jiny port
   
    //a0=adresa polozky v regEventTable (pouzije z ni procID a fn addr)
    //a1=p0
    //a2=p2
    //a3=p3
    move    a1, t6			//param port_base
    move    a2, t7			//param port_state
    bal	    raiseEventInt		//meni t9, v0, v1
    
    //a1 bylo zmemeno, dosadit eventID
    li	    a1, CN_EVENT_ID	
    b16	    1b
    
2:  
    _POP    ra
    jrc	    ra
      
.end cnEvents 
    
//</editor-fold>    
    
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortA">
.ent iVector_cnPortA
iVector_cnPortA:
    
#ifdef PIC32MM0064 
    //#define	    RTC_IFS_BIT		14
    //#define	    RTC_IFS_REG_CLR	IFS0CLR     
#endif
    
#ifdef PIC32MM0256 
    //#define	    RTC_IFS_BIT		0
    //#define	    RTC_IFS_REG_CLR	IFS1CLR     
#endif    
    
#ifdef PIC32MZ
    #define	    CN_IFS_BIT		22
    #define	    CN_IFS_REG_CLR	IFS29CLR 
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
#endif  
    
    li	    a0, PORTA_BASE
    lw	    a1, (CNSTATA)
    bal	    cnEvents		//meni a0-a3, v0, v1, t6-t9
    
    
    //clear flag
    li	    v0, (1 << CN_IFS_BIT)
    sw      v0, (CN_IFS_REG_CLR)     
    sync			
    
#ifdef PIC32MZ    
    _INT_EPILOGUE
#endif 
    
    eret    
    
.end iVector_cnPortA 
    
//</editor-fold>
   
    