#include "xc.h"
#include "def.h"    
#include "asm_macro.S"    
   
/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0    
 * Change Notification, used by BOS core
 * This file is shared to the PIC32MM, PIC32MZ
 */
    
/*
def.h:
#define	    CN
#define	    CNA
#define	    CNB
	    
	    
cnStartPortA();							//aktivuje CM fci 
cnEnable(PORTA_BASE, BIT0 | BIT1, CN_STYLE.LOW_TO_HIGH);	//povoli PORTA.0, PORTA.1, zmena 0>1	    
cnStartPortB();	
cnEnable(PORTB_BASE, BIT4 | BIT5, CN_STYLE.ANY_CHANGE);		//povoli PORTB.4, PORTB.5, kazda zmena stavu      
	   
cnRegEvent(&fn1, PORTA_BASE);
cnRegEvent(&fn2, PORTB_BASE);    
   
void fn1(uint base, uint stat)	    
{
    if((stat & BIT1) != 0)
    {
	//zmena na PORTA.0
    }
}
	    
void fn2(uint base, uint stat)	    
{
    if((stat & BIT4) != 0)
    {
	//zmena na PORTB.4
    }
}	    
*/
    
#if defined PIC32MM && defined PORT_CN    
    
.set micromips 
.set reorder			//vklada nop do branch delay slotu
.section .asm_cn, code      
    
#define	    CN_INTERRUPT_PRIORITY	7
#define	    CN_INTERRUPT_SUBPRIORITY	3
    
#define	    CN_IFS_REG_CLR		IFS0CLR		    //spolecny pro A,B,C,D
#define	    CN_IEC_REG_CLR		IEC0CLR		    //spolecny pro A,B,C,D
#define	    CN_IEC_REG_SET		IEC0SET		    //spolecny pro A,B,C,D
    
#define	    CNA_IFS_BIT			8
#define	    CNB_IFS_BIT			9
#define	    CNC_IFS_BIT			10
#define	    CND_IFS_BIT			11

    
#ifdef PORT_CNA    
//<editor-fold defaultstate="collapsed" desc="cnStartPortA">
.global cnStartPortA
.align 2    
.ent cnStartPortA
cnStartPortA:
    //nastavi registr CNCONx, kde je ON (b15) a STYLE (b11)
    
    li	    t8, (PORTA_BASE + CNCON_OFFSET)
    
    sw	    zero, (t8)			//nuluj CNCON
    li	    v0, (1 << 15)		//ON b15
    li	    v1, (1 << 11)		//style
    or	    v0, v1
    sw	    v0, (t8)			//nastav CNCON.ON, STYLE
    
    //priority IPC2.b2-4
    li	    t9, (0b111 << 2)
    sw	    t9, (IPC2CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 2)
    sw	    t9, (IPC2SET)					//nastav bity
    
    //subpriority IPC2.b0-1
    li	    t9, (0b11 << 0)
    sw	    t9, (IPC2CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 0)
    sw	    t9, (IPC2SET)					//nastav bity
    
    
    //IFS, IEC
    li	    t9, (1<<CNA_IFS_BIT)
    sw	    t9, (CN_IFS_REG_CLR)
    sw	    t9, (CN_IEC_REG_SET)    
    
    jrc	    ra
    
.end cnStartPortA 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="cnStopPortA">
.global cnStopPortA
.align 2     
.ent cnStopPortA
cnStopPortA:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTA_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC
    li	    t9, (1<<CNA_IFS_BIT)
    sw	    t9, (CN_IEC_REG_CLR)				//disable interrupt
    
    jrc	    ra
    
.end cnStopPortA 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortA">
.align 2 
.global iVector_cnPortA  
.ent iVector_cnPortA
iVector_cnPortA:
    
    //CNF ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTA_BASE
    lw	    a1, (CNFA)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //nuluj CNF register
    sw	    zero, (CNFA)
    
    //clear flag
    li	    v0, (1 << CNA_IFS_BIT)
    sw      v0, (CN_IFS_REG_CLR)     
    sync			
    
    eret    
    
.end iVector_cnPortA 
    
//</editor-fold> 
#endif
    
#ifdef PORT_CNB    
//<editor-fold defaultstate="collapsed" desc="cnStartPortB">
.global cnStartPortB
.align 2    
.ent cnStartPortB
cnStartPortB:
   
    //nastavi registr CNCONx, kde je ON (b15) a STYLE (b11)
    
    li	    t8, (PORTB_BASE + CNCON_OFFSET)
    
    sw	    zero, (t8)			//nuluj CNCON
    li	    v0, (1 << 15)		//ON b15
    li	    v1, (1 << 11)		//style
    or	    v0, v1
    sw	    v0, (t8)			//nastav CNCON.ON, STYLE
    
    //priority IPC2.b10-12
    li	    t9, (0b111 << 10)
    sw	    t9, (IPC2CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 10)
    sw	    t9, (IPC2SET)					//nastav bity
    
    //subpriority IPC2.b8-9
    li	    t9, (0b11 << 8)
    sw	    t9, (IPC2CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 8)
    sw	    t9, (IPC2SET)					//nastav bity
    
    
    //IFS, IEC
    li	    t9, (1<<CNB_IFS_BIT)
    sw	    t9, (CN_IFS_REG_CLR)
    sw	    t9, (CN_IEC_REG_SET)    
    
    jrc	    ra
    
.end cnStartPortB 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="cnStopPortB">
.global cnStopPortB
.align 2     
.ent cnStopPortB
cnStopPortB:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTB_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC
    li	    t9, (1<<CNB_IFS_BIT)
    sw	    t9, (CN_IEC_REG_CLR)				//disable interrupt
    
    jrc	    ra
    
.end cnStopPortB 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortB">
.align 2 
.global iVector_cnPortB   
.ent iVector_cnPortB
iVector_cnPortB:
    
    //CNF ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTB_BASE
    lw	    a1, (CNFB)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //nuluj CNF register
    sw	    zero, (CNFB)
    
    //clear flag
    li	    v0, (1 << CNB_IFS_BIT)
    sw      v0, (CN_IFS_REG_CLR)     
    sync			
    
    eret    
    
.end iVector_cnPortB 
    
//</editor-fold> 
#endif    

#ifdef PORT_CNC    
//<editor-fold defaultstate="collapsed" desc="cnStartPortC">
.global cnStartPortC
.align 2    
.ent cnStartPortC
cnStartPortC:
  
    //nastavi registr CNCONx, kde je ON (b15) a STYLE (b11)
    
    li	    t8, (PORTC_BASE + CNCON_OFFSET)
    
    sw	    zero, (t8)			//nuluj CNCON
    li	    v0, (1 << 15)		//ON b15
    li	    v1, (1 << 11)		//style
    or	    v0, v1
    sw	    v0, (t8)			//nastav CNCON.ON, STYLE
    
    //priority IPC2.b18-20
    li	    t9, (0b111 << 18)
    sw	    t9, (IPC2CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 18)
    sw	    t9, (IPC2SET)					//nastav bity
    
    //subpriority IPC2.b16-17
    li	    t9, (0b11 << 16)
    sw	    t9, (IPC2CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 16)
    sw	    t9, (IPC2SET)					//nastav bity
    
    
    //IFS, IEC
    li	    t9, (1<<CNC_IFS_BIT)
    sw	    t9, (CN_IFS_REG_CLR)
    sw	    t9, (CN_IEC_REG_SET)    
    
    jrc	    ra
    
.end cnStartPortC 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="cnStopPortC">
.global cnStopPortC
.align 2     
.ent cnStopPortC
cnStopPortC:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTC_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC
    li	    t9, (1<<CNC_IFS_BIT)
    sw	    t9, (CN_IEC_REG_CLR)				//disable interrupt
    
    jrc	    ra
    
.end cnStopPortC 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortC">
.align 2 
.global iVector_cnPortC      
.ent iVector_cnPortC
iVector_cnPortC:
    
    //CNF ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTC_BASE
    lw	    a1, (CNFC)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //nuluj CNF register
    sw	    zero, (CNFC)
    
    //clear flag
    li	    v0, (1 << CNC_IFS_BIT)
    sw      v0, (CN_IFS_REG_CLR)     
    sync			
    
    eret    
    
.end iVector_cnPortC 
    
//</editor-fold> 
#endif       

#ifdef PORT_CND    
//<editor-fold defaultstate="collapsed" desc="cnStartPortD">
.global cnStartPortD
.align 2    
.ent cnStartPortD
cnStartPortD:
    
    //nastavi registr CNCONx, kde je ON (b15) a STYLE (b11)
    
    li	    t8, (PORTD_BASE + CNCON_OFFSET)
    
    sw	    zero, (t8)			//nuluj CNCON
    li	    v0, (1 << 15)		//ON b15
    li	    v1, (1 << 11)		//style
    or	    v0, v1
    sw	    v0, (t8)			//nastav CNCON.ON, STYLE
    
    //priority IPC2.b26-28
    li	    t9, (0b111 << 26)
    sw	    t9, (IPC2CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 26)
    sw	    t9, (IPC2SET)					//nastav bity
    
    //subpriority IPC2.b24-25
    li	    t9, (0b11 << 24)
    sw	    t9, (IPC2CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 24)
    sw	    t9, (IPC2SET)					//nastav bity
    
    
    //IFS, IEC
    li	    t9, (1<<CND_IFS_BIT)
    sw	    t9, (CN_IFS_REG_CLR)
    sw	    t9, (CN_IEC_REG_SET)    
    
    jrc	    ra
    
.end cnStartPortD 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="cnStopPortD">
.global cnStopPortD
.align 2     
.ent cnStopPortD
cnStopPortD:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTD_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC
    li	    t9, (1<<CND_IFS_BIT)
    sw	    t9, (CN_IEC_REG_CLR)				//disable interrupt
    
    jrc	    ra
    
.end cnStopPortD 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortD">
.align 2 
.global iVector_cnPortD      
.ent iVector_cnPortD
iVector_cnPortD:
    
    //CNF ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTD_BASE
    lw	    a1, (CNFD)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //nuluj CNF register
    sw	    zero, (CNFD)
    
    //clear flag
    li	    v0, (1 << CND_IFS_BIT)
    sw      v0, (CN_IFS_REG_CLR)     
    sync			
    
    eret    
    
.end iVector_cnPortD 
    
//</editor-fold> 
#endif      
    
//<editor-fold defaultstate="collapsed" desc="cnEnable">
.global cnEnable
.align 2     
.ent cnEnable
cnEnable:
//a0=port_base
//a1=bits
//a2=style	    1(0>1), 2(1>0), 3=both
    
    //povoli ChangeNotification pro zadany port/bit
    //cnStartPortA(0);						//zaktivuje fci CN (SIDL=0, pracuje v IDLE/SLEEP)
    //cnEnable(PORTA_BASE, BIT0 | BIT1, CN_STYLE.LOW_TO_HIGH);	//povoli PORTA.0, PORTA.1, zmena 0>1
    //cnEnable(PORTB_BASE, BIT4 | BIT5, CN_STYLE.ANY_CHANGE);	//povoli PORTB.4, PORTB.5, kazda zmena stavu    
    
    
    li	    v0, 0xFFFFFFFF 
    
    addiu   v1, a0, (CNEN0_OFFSET + SET_OFFSET)    
    //nastav CNEN0 (0>1), dle mode.b0
    ext	    t8, a2, 0, 1
    movn    t9, v0, t8			    //t9=0xFFFFFFFF
    movz    t9, zero, t8		    //t9=0x0
    and	    t9, a1			    //pouze vybrane bity
    sw	    t9, (v1)
    
    addiu   v1, a0, (CNEN1_OFFSET + SET_OFFSET)
    //nastav CNEN1 (1>0), dle mode.b1
    ext	    t8, a2, 1, 1
    movn    t9, v0, t8			    //t9=0xFFFFFFFF
    movz    t9, zero, t8		    //t9=0x0
    and	    t9, a1			    //pouze vybrane bity
    sw	    t9, (v1)    
    
    //nuluj flag
    addiu   v1, a0, (CNF_OFFSET + CLR_OFFSET)
    sw	    a1, (v1)
    
    jrc	    ra
    
.end cnEnable 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="cnDisble">
.global cnDisble
.align 2     
.ent cnDisble
cnDisble:
//a0=port_base
//a1=bits
    
    //zakaze ChangeNotification pro zadany port/bit
    
    //nuluj EN0
    addiu   v1, a0, (CNEN0_OFFSET + CLR_OFFSET)
    sw	    a1, (v1)
    //nuluj EN1
    addiu   v1, a0, (CNEN1_OFFSET + CLR_OFFSET)
    sw	    a1, (v1)    
    
    jrc	    ra
    
.end cnDisble 
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="cnRegEvent">
.global cnRegEvent
.align 2     
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
.align 2     
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
    
#endif //PIC32MM && PORT_CN      