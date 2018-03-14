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
	    
	    
cnStartPortA(0);							//aktivuje CM fci  (pracuje i v SLEEP, SIDL=0)
cnEnable(PORTA_BASE, BIT0 | BIT1);					//povoli PORTA.0, PORTA.1	    
cnStartPortB(0);	
cnEnable(PORTB_BASE, BIT4 | BIT5);					//povoli PORTB.4, PORTB.5      
	   
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
    
#if defined PIC32MZ && defined PORT_CN    

.set micromips 
.set reorder			//vklada nop do branch delay slotu
.section .asm_cn, code      
    
#define		    CN_INTERRUPT_PRIORITY	2
#define		    CN_INTERRUPT_SUBPRIORITY	3
   
#define		    CNA_IFS_BIT		22
#define		    CNA_IFS_REG_CLR	IFS3CLR     
#define		    CNA_IEC_REG_CLR	IEC3CLR 
    
#define		    CNB_IFS_BIT		23
#define		    CNB_IFS_REG_CLR	IFS3CLR     
#define		    CNB_IEC_REG_CLR	IEC3CLR 
    
#define		    CNC_IFS_BIT		24
#define		    CNC_IFS_REG_CLR	IFS3CLR     
#define		    CNC_IEC_REG_CLR	IEC3CLR     

#define		    CND_IFS_BIT		25
#define		    CND_IFS_REG_CLR	IFS3CLR     
#define		    CND_IEC_REG_CLR	IEC3CLR      
   
#define		    CNE_IFS_BIT		26
#define		    CNE_IFS_REG_CLR	IFS3CLR     
#define		    CNE_IEC_REG_CLR	IEC3CLR 
    
#define		    CNF_IFS_BIT		27
#define		    CNF_IFS_REG_CLR	IFS3CLR     
#define		    CNF_IEC_REG_CLR	IEC3CLR     
    
#define		    CNG_IFS_BIT		28
#define		    CNG_IFS_REG_CLR	IFS3CLR     
#define		    CNG_IEC_REG_CLR	IEC3CLR   
    
#define		    CNH_IFS_BIT		29
#define		    CNH_IFS_REG_CLR	IFS3CLR     
#define		    CNH_IEC_REG_CLR	IEC3CLR    
    
#define		    CNJ_IFS_BIT		30
#define		    CNJ_IFS_REG_CLR	IFS3CLR     
#define		    CNJ_IEC_REG_CLR	IEC3CLR  
    
#define		    CNK_IFS_BIT		31
#define		    CNK_IFS_REG_CLR	IFS3CLR     
#define		    CNK_IEC_REG_CLR	IEC3CLR      
   
    
#ifdef PORT_CNA    
//<editor-fold defaultstate="collapsed" desc="cnStartPortA">
.global cnStartPortA
.align 2    
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
    la	    v0, iVector_cnPortA
    subu    v0, v1
    sw	    v0, (OFF118)
    
    //priority IPC29.b18-20
    li	    t9, (0b111 << 18)
    sw	    t9, (IPC29CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 18)
    sw	    t9, (IPC29SET)					//nastav bity
    
    //subpriority IPC29.b16-17
    li	    t9, (0b11 << 16)
    sw	    t9, (IPC29CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 16)
    sw	    t9, (IPC29SET)					//nastav bity
    
    
    //IFS3.b22=0, IEC3.b22=1
    li	    t9, (1<<CNA_IFS_BIT)
    sw	    t9, (CNA_IFS_REG_CLR)
    sw	    t9, (CNA_IEC_REG_CLR)    
    
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
    
    //IEC3.b22=0
    li	    t9, (1<<CNA_IFS_BIT)
    sw	    t9, (CNA_IEC_REG_CLR)				//disable interrupt
    
    jrc	    ra
    
.end cnStopPortA 
    
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortA">
.align 2 
.ent iVector_cnPortA
iVector_cnPortA:
    
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTA_BASE
    lw	    a1, (CNSTATA)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTA)			//cteni portu
    
    
    //clear flag
    li	    v0, (1 << CNA_IFS_BIT)
    sw      v0, (CNA_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
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
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTB_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF118
    li	    v1, EBASE
    la	    v0, iVector_cnPortB
    subu    v0, v1
    sw	    v0, (OFF119)
    
    //priority IPC29.b26-28
    li	    t9, (0b111 << 26)
    sw	    t9, (IPC29CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 26)
    sw	    t9, (IPC29SET)					//nastav bity
    
    //subpriority IPC29.b24-25
    li	    t9, (0b11 << 24)
    sw	    t9, (IPC29CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 24)
    sw	    t9, (IPC29SET)					//nastav bity
    
    
    //IFS3.b23=0, IEC3.b23=1
    li	    t9, (1<<CNB_IFS_BIT)
    sw	    t9, (CNB_IFS_REG_CLR)
    sw	    t9, (CNB_IEC_REG_CLR)    
    
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
    
    //IEC3.b23=0
    li	    t9, (1<<CNB_IFS_BIT)
    sw	    t9, (CNB_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortB 
    
//</editor-fold>   
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortB">
.align 2 
.ent iVector_cnPortB
iVector_cnPortB:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTB_BASE
    lw	    a1, (CNSTATB)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTB)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNB_IFS_BIT)
    sw      v0, (CNB_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
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
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTC_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortC
    subu    v0, v1
    sw	    v0, (OFF120)
    
    //priority IPC30.b2-4
    li	    t9, (0b111 << 2)
    sw	    t9, (IPC30CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 2)
    sw	    t9, (IPC30SET)					//nastav bity
    
    //subpriority IPC30.b0-1
    li	    t9, (0b11 << 0)
    sw	    t9, (IPC30CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 0)
    sw	    t9, (IPC30SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CNC_IFS_BIT)
    sw	    t9, (CNC_IFS_REG_CLR)
    sw	    t9, (CNC_IEC_REG_CLR)    
    
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
    
    //IEC3.b23=0
    li	    t9, (1<<CNC_IFS_BIT)
    sw	    t9, (CNC_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortC 
    
//</editor-fold>      
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortC">
.align 2 
.ent iVector_cnPortC
iVector_cnPortC:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTC_BASE
    lw	    a1, (CNSTATC)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTC)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNC_IFS_BIT)
    sw      v0, (CNC_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
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
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTD_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortD
    subu    v0, v1
    sw	    v0, (OFF121)
    
    //priority IPC30.b10-12
    li	    t9, (0b111 << 10)
    sw	    t9, (IPC30CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 10)
    sw	    t9, (IPC30SET)					//nastav bity
    
    //subpriority IPC30.b8-9
    li	    t9, (0b11 << 8)
    sw	    t9, (IPC30CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 8)
    sw	    t9, (IPC30SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CND_IFS_BIT)
    sw	    t9, (CND_IFS_REG_CLR)
    sw	    t9, (CND_IEC_REG_CLR)    
    
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
    
    //IEC3.b23=0
    li	    t9, (1<<CND_IFS_BIT)
    sw	    t9, (CND_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortD 
    
//</editor-fold>     
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortD">
.align 2 
.ent iVector_cnPortD
iVector_cnPortD:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTD_BASE
    lw	    a1, (CNSTATD)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTD)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CND_IFS_BIT)
    sw      v0, (CND_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
    eret    
    
.end iVector_cnPortD 
    
//</editor-fold>    
#endif
    
#ifdef PORT_CNE     
//<editor-fold defaultstate="collapsed" desc="cnStartPortE">
.global cnStartPortE
.align 2     
.ent cnStartPortE
cnStartPortE:
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTE_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortE
    subu    v0, v1
    sw	    v0, (OFF122)
    
    //priority IPC30.b18-20
    li	    t9, (0b111 << 18)
    sw	    t9, (IPC30CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 18)
    sw	    t9, (IPC30SET)					//nastav bity
    
    //subpriority IPC30.b16-17
    li	    t9, (0b11 << 16)
    sw	    t9, (IPC30CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 16)
    sw	    t9, (IPC30SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CNE_IFS_BIT)
    sw	    t9, (CNE_IFS_REG_CLR)
    sw	    t9, (CNE_IEC_REG_CLR)    
    
    jrc	    ra
    
.end cnStartPortE 
    
//</editor-fold>    
//<editor-fold defaultstate="collapsed" desc="cnStopPortE">
.global cnStopPortE
.align 2     
.ent cnStopPortE
cnStopPortE:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTE_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC3.b23=0
    li	    t9, (1<<CNE_IFS_BIT)
    sw	    t9, (CNE_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortE 
    
//</editor-fold>       
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortE">
.align 2 
.ent iVector_cnPortE
iVector_cnPortE:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTE_BASE
    lw	    a1, (CNSTATE)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTE)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNE_IFS_BIT)
    sw      v0, (CNE_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
    eret    
    
.end iVector_cnPortE 
    
//</editor-fold>      
#endif
    
#ifdef PORT_CNF     
//<editor-fold defaultstate="collapsed" desc="cnStartPortF">
.global cnStartPortF
.align 2     
.ent cnStartPortF
cnStartPortF:
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTF_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortF
    subu    v0, v1
    sw	    v0, (OFF123)
    
    //priority IPC30.b26-28
    li	    t9, (0b111 << 26)
    sw	    t9, (IPC30CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 26)
    sw	    t9, (IPC30SET)					//nastav bity
    
    //subpriority IPC30.b24-25
    li	    t9, (0b11 << 24)
    sw	    t9, (IPC30CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 24)
    sw	    t9, (IPC30SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CNF_IFS_BIT)
    sw	    t9, (CNF_IFS_REG_CLR)
    sw	    t9, (CNF_IEC_REG_CLR)    
    
    jrc	    ra
    
.end cnStartPortF 
    
//</editor-fold>    
//<editor-fold defaultstate="collapsed" desc="cnStopPortF">
.global cnStopPortF
.align 2     
.ent cnStopPortF
cnStopPortF:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTF_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC3.b23=0
    li	    t9, (1<<CNF_IFS_BIT)
    sw	    t9, (CNF_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortF 
    
//</editor-fold>      
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortF">
.align 2 
.ent iVector_cnPortF
iVector_cnPortF:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTF_BASE
    lw	    a1, (CNSTATF)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTF)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNF_IFS_BIT)
    sw      v0, (CNF_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
    eret    
    
.end iVector_cnPortF 
    
//</editor-fold>      
#endif
    
#ifdef PORT_CNG     
//<editor-fold defaultstate="collapsed" desc="cnStartPortG">
.global cnStartPortG
.align 2     
.ent cnStartPortG
cnStartPortG:
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTG_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortG
    subu    v0, v1
    sw	    v0, (OFF124)
    
    //priority IPC31.b2-4
    li	    t9, (0b111 << 2)
    sw	    t9, (IPC31CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 2)
    sw	    t9, (IPC31SET)					//nastav bity
    
    //subpriority IPC31.b0-1
    li	    t9, (0b11 << 0)
    sw	    t9, (IPC31CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 0)
    sw	    t9, (IPC31SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CNG_IFS_BIT)
    sw	    t9, (CNG_IFS_REG_CLR)
    sw	    t9, (CNG_IEC_REG_CLR)    
    
    jrc	    ra
    
.end cnStartPortG 
    
//</editor-fold>    
//<editor-fold defaultstate="collapsed" desc="cnStopPortG">
.global cnStopPortG
.align 2     
.ent cnStopPortG
cnStopPortG:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTG_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC3.b23=0
    li	    t9, (1<<CNG_IFS_BIT)
    sw	    t9, (CNG_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortG
    
//</editor-fold>      
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortG">
.align 2 
.ent iVector_cnPortG
iVector_cnPortG:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTG_BASE
    lw	    a1, (CNSTATG)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTG)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNG_IFS_BIT)
    sw      v0, (CNG_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
    eret    
    
.end iVector_cnPortG 
    
//</editor-fold>     
#endif
    
#ifdef PORT_CNH    
//<editor-fold defaultstate="collapsed" desc="cnStartPortH">
.global cnStartPortH
.align 2     
.ent cnStartPortH
cnStartPortH:
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTH_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortH
    subu    v0, v1
    sw	    v0, (OFF125)
    
    //priority IPC31.b10-12
    li	    t9, (0b111 << 10)
    sw	    t9, (IPC31CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 10)
    sw	    t9, (IPC31SET)					//nastav bity
    
    //subpriority IPC31.b8-9
    li	    t9, (0b11 << 8)
    sw	    t9, (IPC31CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 8)
    sw	    t9, (IPC31SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CNH_IFS_BIT)
    sw	    t9, (CNH_IFS_REG_CLR)
    sw	    t9, (CNH_IEC_REG_CLR)    
    
    jrc	    ra
    
.end cnStartPortH 
    
//</editor-fold>    
//<editor-fold defaultstate="collapsed" desc="cnStopPortH">
.global cnStopPortH
.align 2     
.ent cnStopPortH
cnStopPortH:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTH_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC3.b23=0
    li	    t9, (1<<CNH_IFS_BIT)
    sw	    t9, (CNH_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortH
    
//</editor-fold>       
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortH">
.align 2 
.ent iVector_cnPortH
iVector_cnPortH:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTH_BASE
    lw	    a1, (CNSTATH)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTH)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNH_IFS_BIT)
    sw      v0, (CNH_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
    eret    
    
.end iVector_cnPortH 
    
//</editor-fold>   
#endif    
  
#ifdef PORT_CNJ     
//<editor-fold defaultstate="collapsed" desc="cnStartPortJ">
.global cnStartPortJ
.align 2     
.ent cnStartPortJ
cnStartPortJ:
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTJ_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortJ
    subu    v0, v1
    sw	    v0, (OFF126)
    
    //priority IPC31.b18-20
    li	    t9, (0b111 << 18)
    sw	    t9, (IPC31CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 18)
    sw	    t9, (IPC31SET)					//nastav bity
    
    //subpriority IPC31.b16-17
    li	    t9, (0b11 << 16)
    sw	    t9, (IPC31CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 16)
    sw	    t9, (IPC31SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CNJ_IFS_BIT)
    sw	    t9, (CNJ_IFS_REG_CLR)
    sw	    t9, (CNJ_IEC_REG_CLR)    
    
    jrc	    ra
    
.end cnStartPortJ 
    
//</editor-fold>    
//<editor-fold defaultstate="collapsed" desc="cnStopPortJ">
.global cnStopPortJ
.align 2     
.ent cnStopPortJ
cnStopPortJ:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTJ_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC3.b23=0
    li	    t9, (1<<CNJ_IFS_BIT)
    sw	    t9, (CNJ_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortJ
    
//</editor-fold>      
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortJ">
.align 2 
.ent iVector_cnPortJ
iVector_cnPortJ:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTJ_BASE
    lw	    a1, (CNSTATJ)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTJ)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNJ_IFS_BIT)
    sw      v0, (CNJ_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
    eret    
    
.end iVector_cnPortJ 
    
//</editor-fold>      
#endif    
    
#ifdef PORT_CNK     
//<editor-fold defaultstate="collapsed" desc="cnStartPortK">
.global cnStartPortK
.align 2     
.ent cnStartPortK
cnStartPortK:
//a0=SIDL (0=pracuje v IDLE/SLEEP, 1=nepracuje v IDLE/SLEEP)
    
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTK_BASE + CNCON_OFFSET)
    
    //b.13=SIDL
    andi    a0, 0x1			//ponecha pouze b0
    sll	    a0, 13			//SIDL
    
    //b.15=ON
    li	    t9, (1 << 15)		//ON b15
    or	    a0, t9			//ON + SIDL
    
    sw	    zero, (t8)			//nuluj CNSTAT
    sw	    a0, (t8)			//nastav CNSTAT
    
    //OFF120
    li	    v1, EBASE
    la	    v0, iVector_cnPortK
    subu    v0, v1
    sw	    v0, (OFF127)
    
    //priority IPC31.b26-28
    li	    t9, (0b111 << 26)
    sw	    t9, (IPC31CLR)					//nuluj bity
    li	    t9, (CN_INTERRUPT_PRIORITY << 26)
    sw	    t9, (IPC31SET)					//nastav bity
    
    //subpriority IPC31.b24-25
    li	    t9, (0b11 << 24)
    sw	    t9, (IPC31CLR)					//nuluj bity    
    li	    t9, (CN_INTERRUPT_SUBPRIORITY << 24)
    sw	    t9, (IPC31SET)					//nastav bity
    
    
    //IFS3.b24=0, IEC3.b24=1
    li	    t9, (1<<CNK_IFS_BIT)
    sw	    t9, (CNK_IFS_REG_CLR)
    sw	    t9, (CNK_IEC_REG_CLR)    
    
    jrc	    ra
    
.end cnStartPortK 
    
//</editor-fold>    
//<editor-fold defaultstate="collapsed" desc="cnStopPortK">
.global cnStopPortK
.align 2     
.ent cnStopPortK
cnStopPortK:
    //nastavi registr CNSTATx, kde je ON (b15) a SIDL (b13)
    
    li	    t8, (PORTK_BASE + CNCON_OFFSET) 
    sw	    zero, (t8)					//ON=0
    
    //IEC3.b23=0
    li	    t9, (1<<CNK_IFS_BIT)
    sw	    t9, (CNK_IEC_REG_CLR)			//disable interrupt
    
    jrc	    ra
    
.end cnStopPortK
    
//</editor-fold>      
//<editor-fold defaultstate="collapsed" desc="iVector_cnPortK">
.align 2 
.ent iVector_cnPortK
iVector_cnPortK:
     
    _MIPS32_TO_MICROMIPS  
    _INT_PROLOGUE
    
    //CNSTAT ma nastavene bity, kde doslo ke zmene stavu
    li	    a0, PORTK_BASE
    lw	    a1, (CNSTATK)
    bal	    cnEvents			//meni a0-a3, v0, v1, t6-t9
    
    //cteni portu nuluje CNSTAT register
    lw	    t9, (PORTK)			//cteni portu
    
    //clear flag
    li	    v0, (1 << CNK_IFS_BIT)
    sw      v0, (CNK_IFS_REG_CLR)     
    sync			
    
    _INT_EPILOGUE
    
    eret    
    
.end iVector_cnPortK 
    
//</editor-fold>       
#endif    
    
//<editor-fold defaultstate="collapsed" desc="cnEnable">
.global cnEnable
.align 2     
.ent cnEnable
cnEnable:
//a0=port_base
//a1=bits
    
    //povoli ChangeNotification pro zadany port/bit
    //cnStartPortA(0);					    //zaktivuje fci CN (SIDL=0, pracuje v IDLE/SLEEP)
    //cnEnable(PORTA_BASE, BIT0 | BIT1);		    //povoli PORTA.0, PORTA.1
    
    addiu   a0, (CNEN_OFFSET + SET_OFFSET)
    sw	    a1, (a0)
    
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
    
    addiu   a0, (CNEN_OFFSET + CLR_OFFSET)
    sw	    a1, (a0)
    
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
    
#endif //PIC32MZ && PORT_CN      