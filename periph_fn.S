#include "xc.h"
#include "def.h"  
#include "asm_macro.S"
  
    
/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS core (asm helper functions)
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */

    
//!!! 2/2018 vyrazeny instrukce NOP v delay slotu, po skocich b16, beq, bne, bltz, bgtz, ...
//compiler xc32, v1.44 je vklada automaticky, dle typu CPU   
    
.set micromips     
.section .asm_fn, code     
    
//<editor-fold defaultstate="collapsed" desc="setPortDigOut">
.align 2    
.global setPortDigOut    
.ent setPortDigOut
setPortDigOut:    
//a0 base adresa portu, PORTA_BASE, PORTB_BASE, ...
//a1 pozadovane bity BIT0, BIT2 | BIT3, ...  
    
    //nastavuje pouze TRIS (in/out) a ANSEL (analog/digital)
    
    //napr.: setPortDigOut(POTRA_BASE, BIT1 | BIT4 | BIT8);
    
    //TRISbit=0
    sh32    a1, (TRIS_OFFSET + CLR_OFFSET)(a0)
    //ANSELbit=0 
    sh32    a1, (ANSEL_OFFSET + CLR_OFFSET)(a0)
    
    jrc	    ra
    
.end setPortDigOut 

//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="setPortDigIn">
.align 2    
.global setPortDigIn     
.ent setPortDigIn
setPortDigIn:    
//a0 base adresa portu, PORTA_BASE, PORTB_BASE, ...
//a1 pozadovane bity BIT0, BIT2 | BIT3, ...    
    
    //nastavuje pouze TRIS (in/out) a ANSEL (analog/digital)
    
    //napr.: setPortDigIn(POTRA_BASE, BIT1 | BIT4 | BIT8);
    
    //TRISbit=1
    sh32    a1, (TRIS_OFFSET + SET_OFFSET)(a0)
    //ANSELbit=0 
    sh32    a1, (ANSEL_OFFSET + CLR_OFFSET)(a0)
    
    jrc	    ra
    
.end setPortDigIn 
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="setPortAnalogIn">
.align 2    
.global setPortAnalogIn     
.ent setPortAnalogIn
setPortAnalogIn:    
//a0 base adresa portu, PORTA_BASE, PORTB_BASE, ...
//a1 pozadovane bity BIT0, BIT2 | BIT3, ...    
    
    //nastavuje pouze TRIS (in/out) a ANSEL (analog/digital)
    
    //napr.: setPortAnalogIn(POTRA_BASE, BIT1 | BIT4 | BIT8);
    
    //TRISbit=1
    sh32    a1, (TRIS_OFFSET + SET_OFFSET)(a0)
    //ANSELbit=1 
    sh32    a1, (ANSEL_OFFSET + CLR_OFFSET)(a0)
    
    jrc	    ra
    
.end setPortAnalogIn 
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="setPortOpenDrain">
.align 2    
.global setPortOpenDrain     
.ent setPortOpenDrain
setPortOpenDrain:    
//a0 base adresa portu, PORTA_BASE, PORTB_BASE, ...
//a1 pozadovane bity BIT0, BIT2 | BIT3, ... 
//a2 1=nastav ODC, 0=nuluj ODC (vypnuto)    
    
    //nastavuje hodnotu ODCx 
    
    //napr.: setPortOpenDrain(POTRA_BASE, BIT1 | BIT4 | BIT8, 1);		//nastav OpenDrain B.1, B.4, B.8
    
    beqz    a2, 1f
    nop
    
    //ODCx=1
    sh32    a1, (ODC_OFFSET + SET_OFFSET)(a0)
    jrc	    ra
    
1:
    //ODCx=0
    sh32    a1, (ODC_OFFSET + CLR_OFFSET)(a0)
    jrc	    ra
    
.end setPortOpenDrain
    
//</editor-fold>
  
//<editor-fold defaultstate="collapsed" desc="setPortPullUp">
.align 2    
.global setPortPullUp     
.ent setPortPullUp
setPortPullUp:    
//a0 base adresa portu, PORTA_BASE, PORTB_BASE, ...
//a1 pozadovane bity BIT0, BIT2 | BIT3, ... 
//a2 1=nastav PullUp, 0=nuluj PullUp (vypnuto)    
    
    //nastavuje hodnotu CNPUx 
    
    //napr.: setPortPullUp(POTRA_BASE, BIT1 | BIT4 | BIT8, 1);			//nastav PullUp B.1, B.4, B.8
    
    beqz    a2, 1f
    nop
    
    //CNPUx=1
    sh32    a1, (CNPU_OFFSET + SET_OFFSET)(a0)
    jrc	    ra
    
1:
    //CNPUx=0
    sh32    a1, (CNPU_OFFSET + CLR_OFFSET)(a0)
    jrc	    ra
    
.end setPortPullUp 
    
//</editor-fold>
 
//<editor-fold defaultstate="collapsed" desc="setPortPullDown">
.align 2    
.global setPortPullDown     
.ent setPortPullDown
setPortPullDown:    
//a0 base adresa portu, PORTA_BASE, PORTB_BASE, ...
//a1 pozadovane bity BIT0, BIT2 | BIT3, ... 
//a2 1=nastav PullDown, 0=nuluj PullDown (vypnuto)    
    
    //nastavuje hodnotu CNPDx 
    
    //napr.: setPortPullDown(POTRA_BASE, BIT1 | BIT4 | BIT8, 1);			//nastav PullDown B.1, B.4, B.8
    
    beqz    a2, 1f
    nop
    
    //CNPDx=1
    sh32    a1, (CNPD_OFFSET + SET_OFFSET)(a0)
    jrc	    ra
    
1:
    //CNPDx=0
    sh32    a1, (CNPD_OFFSET + CLR_OFFSET)(a0)
    jrc	    ra
    
.end setPortPullDown
    
//</editor-fold>    
