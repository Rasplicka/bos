

//<editor-fold defaultstate="collapsed" desc="STACK _POP/_PUSH">

.macro _PUSH reg
    addiu   sp, sp, -4
    sw      \reg, 0(sp)
.endm

.macro _POP reg
    lw      \reg, 0(sp)
    addiu   sp, sp, 4
.endm
	
.macro _PUSH_S03RA
    addiu   sp, -20
    swm32   s0 - s3, ra, (sp)
.endm
	
.macro _POP_S03RA
    lwm32   s0 - s3, ra, (sp)
    addiu   sp, 20
.endm
  
.macro _PUSH_S04RA
    addiu   sp, -24
    swm32   s0 - s4, ra, (sp)
.endm
	
.macro _POP_S04RA
    lwm32   s0 - s4, ra, (sp)
    addiu   sp, 24
.endm    
    
.macro _PUSH_S07RA
    addiu   sp, -36
    swm32   s0 - s7, ra, (sp)
.endm
	
.macro _POP_S07RA
    lwm32   s0 - s7, ra, (sp)
    addiu   sp, 36
.endm      
    
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="podminene skoky _B_xxx">
.macro _B_FBT first_reg, val_reg, addr
//skok, first > val
//meni t9
    
    subu    t9, \first_reg, \val_reg
    bgtz    t9, \addr
    //nop
    
.endm    
    
.macro _B_FBTC first_reg, val_const, addr
//skok, first > val_const
//meni t9
    
    li	    t9, \val_const
    subu    t9, \first_reg, t9
    bgtz    t9, \addr
    //nop
    
.endm  
    
.macro _B_FBE first_reg, val_reg, addr
//skok, first >= val_reg
//meni t9

    subu    t9, \first_reg, \val_reg
    bgez    t9, \addr
    //nop
    
.endm     
    
.macro _B_FBEC first_reg, val_const, addr
//skok, first >= val_const
//meni t9
    
    li	    t9, \val_const
    subu    t9, \first_reg, t9
    bgez    t9, \addr
    //nop
    
.endm      
    
.macro _B_FLT first_reg, val_reg, addr
//skok, first < val
//meni t9
    
    subu    t9, \first_reg, \val_reg
    bltz    t9, \addr
    //nop
    
.endm  
    
.macro _B_FLTC first_reg, val_const, addr
//skok, first < val_const
//meni t9
    
    li	    t9, \val_const
    subu    t9, \first_reg, t9
    bltz    t9, \addr
    //nop
    
.endm     
    
.macro _B_FLE first_reg, val_reg, addr
//skok, first <= val_reg
//meni t9

    subu    t9, \first_reg, \val_reg
    blez    t9, \addr
    //nop
    
.endm 
    
.macro _B_FLEC reg, const, addr
    
    li	    t9, \const
    subu    t9, \reg, t9 
    blez    t9, \addr
    //nop
    
.endm    
    
.macro _B_EQC	reg, const, addr    
//skok, reg = const    
//meni t9
    
    li	    t9, \const
    beq	    \reg, t9, \addr
    //nop
    
.endm
    
.macro _B_NEQC reg, const, addr    
//skok, reg != const    
//meni t9
    
    li	    t9, \const
    bne	    \reg, t9, \addr
    //nop
    
.endm    
    
   
.macro _SET_IF_EQUI reg, val, set_reg, set_result
//pokud reg obsahuje hodnotu val, nastavi do set_reg hodnotu set_result    
//_SET_IF_EQUI v1, 8, v0, 0
    
    li	    t9, \val
    bne	    \reg, \val, 1001f
    //nop
    
    //reg=val, nastav set_reg
    li	    \set_reg, \set_result
    
1001:    
    
.endm    
   
.macro _SET_B_IF_EQUI reg, val, set_reg, set_result, branch
//pokud reg obsahuje hodnotu val, nastavi do set_reg hodnotu set_result    
//_SET_IF_EQUI v1, 8, v0, 0
    
    li	    t9, \val
    bne	    \reg, \val, 1001f
    //nop
    
    //reg=val, nastav set_reg
    li	    \set_reg, \set_result
    //nop
    b16	    \branch
    //nop
    
1001:    
    
.endm      
    
    
//</editor-fold>    	

//<editor-fold defaultstate="collapsed" desc="_MIPS32_TO_MICROMIPS">
.macro _MIPS32_TO_MICROMIPS
	
	.align 2
        .set noreorder

        .word 0x10000003	    //pokud je v microMIPS mode, tento code nic nedela
        .word 0x00000000	    //pokud je v MIPS32 mode, provede skok +16

        .set    micromips
    li      t9, 2		    //microMIPS instrukce
    b	    1001f		    //microMIPS instrukce skoku
    nop

        .align 2		    // Device not in proper ISA mode
        .set nomicromips
    la      t9, (1001f)		    //MIPS32 instrukce
    jr	    t9			    //MIPS32 instrukce skoku, _startup zacina na liche adrese, prepne do microMIPS
    nop
	.align 2
        .set micromips
	
1001:
    
	.set reorder
.endm  
    
//</editor-fold>    
    
//<editor-fold defaultstate="collapsed" desc="_INT_PROLOGUE, _INT_EPILOGUE">
.macro _INT_PROLOGUE 
//toto makro nuluje STATUS.EXL, nastavi STATUS.IPL, zalohuje EPC
//aby v prubehu interruptu mohl nastat jiny interrupt s vyssi prioritou (pouze MZ)
    
    //meni v0, v1
    
    //EPC - zaloha
    mfc0    v0, _CP0_EPC
    _PUSH   v0
    
    //SRSCTL.PSS - zaloha
    mfc0    v0, _CP0_SRSCTL
    ext	    v0, v0, 6, 4
    _PUSH   v0
    
    //VIEW_IPL - zaloha
    mfc0    v0, _CP0_VIEW_IPL
    _PUSH   v0
    
    //nastav status IPL
    mfc0    v0, _CP0_CAUSE		    //aktualni IPL
    ext	    v0, v0, 10, 8		    //aktualni IPL
    
    //STATUS.EXL nuluj
    mfc0    v1, _CP0_STATUS
    //ehb
    ins	    v1, v0, 10, 9
    ins	    v1, zero, 1, 1
    mtc0    v1, _CP0_STATUS    
    
.endm 
    
.macro _INT_EPILOGUE
//ukoncuje interrupt, kde bylo volano _INT_PROLOGUE (pouze MZ)    
    
    //meni v0, v1
    
    li16    v1, 1
    mfc0    v0, _CP0_STATUS
    ins	    v0, v1, 1, 1
    mtc0    v0, _CP0_STATUS
    
    _POP    v0
    mtc0    v0, _CP0_VIEW_IPL		    //obnov IPL ve STATUS
    
    _POP    v1
    mfc0    v0, _CP0_SRSCTL
    ins	    v0, v1, 6, 4
    mtc0    v0, _CP0_SRSCTL		    //obnov PSS
    
    _POP    v1
    mtc0    v1, _CP0_EPC		    //obnov EPC
    
.endm 
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="_CALL_C">
.macro _CALL_C addr_reg, arg_cnt
    //volaci c/c++ fce z assembleru
    //pozn.:
    //c/c++ fce uklada nad svuj stack params a0-a3 
    //proto se sp posune, aby fce nezapisovala nad svuj stack
    //v c/c++ sem totiz volajici uklada parametry (ale nevim, proc volana fce dela to same,
    //tj. kopiruje a0-a3 do stacku volajici fce)    
    //volani jalr zajisti prepnuti ISA (micromips/mips32)
    
    addiu   sp, (-4 * \arg_cnt)	    	    //viz. pozn
    jalr    \addr_reg			    //skok do fce
    addiu   sp, (4 * \arg_cnt)		    //viz. pozn
    
.endm  
    
//</editor-fold>
  