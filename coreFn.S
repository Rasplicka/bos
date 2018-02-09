#include "xc.h"
#include "def.h"  
#include "asm_macro.S"
   

//.set mips32    
#define MICROMIPS  
.set micromips     
    
//.section os_alloc_stack_here
//tuto sekci linker vlozi jako posledni v data RAM
//definuje prvni volne misto v RAM    
//stack_area:	    .dword	    0x0	    //dword proto, aby byla zarovnana na 8 bytes
//v pripade, ze by linker umistoval tuto sekci jinam, bylo by mozne fci malloc alokovat
//libovolna data v heap, kterou linker vzdy umistuje hned za data. Adresa techto dat
//by definovala zacatek oblasti stacku    
    
.section .asm_fn, code     
    
//<editor-fold defaultstate="collapsed" desc="clearProcTable">
.align 2
.global clearProcTable
.ent clearProcTable
clearProcTable: 
    
    //Tato fce nuluje obsah proc_t
    
    la	    t0, proc_t			//proc_t
    li	    t1, PROC_T_CAPA
    li	    t9, PROC_T_ISIZE
    mul	    t1, t9
    addu    t1, t0			//proc_t after
    
1:    
    sw32    zero, (t0)
    addiu   t0, 4
    bne	    t0, t1, 1b
    nop
    
    jrc	    ra
    
.end clearProcTable   

//</editor-fold>
 
//<editor-fold defaultstate="collapsed" desc="allocStack">   
    
.macro _TEST_COLLISION st1, af1, st2, af2, collide
    //st1 = start oblasti 1
    //af1 = after oblast 1
    //st2 = start oblasti 2
    //af2 = after oblast 2
    
    //meni t9
    _BAL_FBE \st2, \af1, 1001f		//meni t9
    _BAL_FLE \af2, \st1, 1001f		//meni t9
    
    nop
    b16	    \collide
    nop
    
1001:
   
.endm     
    
.align 2
.global allocStack
.ent allocStack
allocStack:
//a0=pozadovana velikost stacku
//a1=thread table, adresa akt. polozky 
    
    //meni t0-t9, v0, v1
    //vraci 0=ok, 1=chyba nenasel volny stack
    
    //Prochazi polozky proc_t a hleda oblast v stack_area, kterou muze pouzit.
    //
    //Nejprve urci zacatek oblasti stacku pro app (za interrupt stack)
    //Pokusi se pouzit tuto oblast od zacatku
    //Je-li zacatek obsazeny, prochazi jednotlive polozky proc_t a pokusi se stack umistit 
    //tesne za stack kazde z nich.
    //V t4 ma pocatecni adresu a v t5 adresu za testovanou oblasti
    //Oblast testuje tak, ze prochazi polozky v proc_t a kontroluje zda nejsou v kolizi
    
    //Tato metoda funguje i v pripade, ze dochazi k ukonceni procesu a start novych za behu OS,
    //ale neprovadi se defragmentace oblasti
    
    
    //top adresu oblasti vlozi do TH_T_START_SP, coz je vrchol zasobniku, do ktereho ale tento
    //proces nikdy nezapisuje, protoze PUSH provede nejprve SP-4. TH_T_START_SP - tato hodnota 
    //se pouzije pro inicializaci registru sp pri startu procesu.
    //Top adresa je zaroven dno dalsiho zasobniku, kde ma tento nasledujici ulozenu STACK_CHECK_VALUE
    //pro kontrolu stack-overflow
    //stack-overflow se kontroluje pri prepnuti procesu tak, ze prave ukonceny proces musi mit
    //v TH_T_STACK_BASE (dno stacku) prave STACK_CHECK_VALUE, pokud ne, pak ji musel prespat
    //a dostat se tak mimo svuj zasobnik
    
    //nastav proc_t-start(t0) a proc_t-after(t1)
    la	    t0, proc_t			//proc_t-start
    li	    t1, PROC_T_CAPA
    li	    t9, PROC_T_ISIZE
    mul	    t1, t9
    addu    t1, t0			//proc_t-after
    
    //nastav oblast pro app stack app-stack-start(t2) a stack_area-after(t3)
    la	    t2, stack_area		//zacatek oblasti zasobniku v RAM
    li	    t9, STACK_SIZE
    addu    t3, t2, t9			//stack_area-after
    
    //zjisti pocet SRS 
    mfc0    v0, _CP0_SRSCTL
    ext	    v0, v0, 26, 4		//Number of Shadow Set (MM=1, MZ=7)
    li	    v1, SRS_STACK_SIZE		//dolni cast stack_area pro interrupt stack (interrupt level 1 MM, nebo 1-7 MZ)
    mul	    v0, v1			//SIZE * Number of SRS
    addu    t2, v0			//app-stack-start
   
    //t0 = proc_t, t1=proc_t after
    //t2 = stack_start (za SRS), t3=stack_after

    //pokusi se alokovat prvni cast app-stack-start 
    move    t4, t2			//testovana oblast (start) do t4
    move    t7, zero			
    nop
    b16	    2f 
    nop

1:
    //pouzije dalsi polozku proc_t, za jejiz stack se pokusi vlozit novy stack
    
    lbu32    t9, TH_T_ID*4(t7)
    beqz    t9, 6f 			//skok, polozka neni pouzita
    nop
    
    lw	    t4, TH_T_START_SP*4(t7)	//top stacku = start adresa noveho stacku

    
    //nyni bude prochazi vsechny polozky proc_t, vynecha aktualni (a1), (s7) a prazdnou
    //a bude zjistovat, zda novy stack (t4-t5) neni v kolizi s jiz existujicim
2:     
    //nastav t5 (start+size)
    addu    t5, t4, a0			//t5 new-stack-after addr.
    move    t6, t0			//t6 = testovana polozka proc_t (nyni nastav prvni)
    
    //kontrola, zda testovana oblast neni mimo stack_area
    //reg1, reg2, skok
    _BAL_FBT t5, t3, 5f			//skok, testovana oblast je mimo stack_area
    
3:  //prochazeni proc_t    
    lbu32    t9, TH_T_ID*4(t6)
    beqz    t9, 4f			//skok, prazdna polozka
    nop
    beq	    t6, a1, 4f			//skok, toto je nova polozka (pro kterou se hleda stack)	    
    nop
    beq	    t6, t7, 4f			//skok, za touto polozkou je test oblast (proto nemuze byt v kolizi)
    nop
    lw32    v1, TH_T_START_SP*4(t6)	//stack TOP (after)
    lw32    v0, TH_T_STACK_BASE*4(t6)	//stack BOTTOM (start)
    
    //macro, v pripade kolize skok na 5f
    //start1, after1, start2, after2, err
    _TEST_COLLISION v0, v1, t4, t5, 5f 
    
4:    
    //ok, neni v kolizi, nebo prekoceni testu
    addiu   t6, PROC_T_ISIZE
    bne	    t6, t1, 3b			//skok, dalsi polozka proc_t
    nop
    
    //probehl test vsech polozek, neni zadna kolize
    //t4=start oblasti (stack BOTTOM), t5=after (stack TOP)
    
    //BOTTOM
    sw32    t4, TH_T_STACK_BASE*4(a1)	//save bottom addr
    li	    v1, STACK_CHECK_VALUE	//check_value
    sw32    v1, (t4)			//save check_value
    
    //TOP
    sw32    t5, TH_T_SP*4(a1)		//save top addr
    sw32    t5, TH_T_START_SP*4(a1)	//save top addr
    
    li16    v0, 0			//return 0 (ok)
    jrc	    ra
    
5: 
    //dalsi polozka proc_t
    bnez    t7, 6f			//skok, pokud neprobihal test zacatku oblasti
    nop
    
    //probehl test na zacatek oblasti 
    move    t7, t0
    nop
    b16	    1b
    nop
    
6:     
    addiu   t7, PROC_T_ISIZE
    bne	    t7, t1, 1b 
    nop
    
    //konec, nenasel volnou oblast
    li16    v0, 1			//return 1 (error)
    jrc	    ra
   
.end allocStack    
//</editor-fold>    
    
//<editor-fold defaultstate="collapsed" desc="setSrsValue">         
    
.align 2    
.global setSrsValue    
.ent setSrsValue 
setSrsValue:
    
    //nastavi sp pro SRS[0] na TOP stack_area, tj docasny zasobnik pro inicializaci OS
    //nastavi SP (a GP) pro SRS[1-7], zasobniky jsou vyhrazeny na adrese stack_area,
    //velikost kazdeho je definovana SRS_STACK_SIZE (smerem nahoru)
    //nasleduji zasobniky pro jednotlive procesy 
    //celkova velikost oblasti pro zasobniky je v STACK_SIZE
    
    //zjisti pocet SRS 
    mfc0    v0, _CP0_SRSCTL
    ext	    v0, v0, 26, 4		//Number of Shadow Set (MM=1, MZ=7)
    
    move    v1, zero			//SRS index
    la	    t8, stack_area
    li	    t7, SRS_STACK_SIZE

1:    
    addiu   v1, 1			//SRS index++
    
    //nastav previous SRS=v1
    mfc0    t9, _CP0_SRSCTL
    ins	    t9, v1, 6, 4
    mtc0    t9, _CP0_SRSCTL
    ehb
    
    //zapis gp
    wrpgpr  gp, gp
    
    //zapis sp (dolni cast stack_area je vyhrazena pro zasobnik SRS[1])
    addu    t8, t7			//horni hranice stacku
    wrpgpr  sp, t8
    
    bne	    v1, v0, 1b			//skok, existuje dalsi SRS
    nop
    
    //konec
    jrc	    ra
    
.end setSrsValue
    
//</editor-fold>         
   
//<editor-fold defaultstate="collapsed" desc="startEvents">    
.align 2    
.global startEvents
.ent startEvents    
startEvents:
    //start, spusti prvni proces v process table
    
#ifdef SAFE_PROCESS    
    //nuluj count
    mtc0    zero, _CP0_COUNT 
    ehb
    
    //nastav compare
    li	    k0, SAFE_PROCESS_VALUE
    mtc0    k0, _CP0_COMPARE
    ehb
    
#endif      
    
    //proc_t_pos
    la	    k0, proc_t				//process table
    sw	    k0, (proc_t_pos)			//uloz prvni pozici
    
    //proc_t_after
    li	    k1, (PROC_T_ISIZE * PROC_T_CAPA)	//proc_t size
    addu    k1, k0
    sw	    k1, (proc_t_after)
    
    lw32    ra, TH_T_RA*4(k0)			//load start addr
    lw32    sp, TH_T_SP*4(k0)			//load sp
    
    jrc	    ra
    nop						//musi zde zustat
    
.end startEvents
//</editor-fold>        
    
//<editor-fold defaultstate="collapsed" desc="doEvents">     
.align 2	
.global doEvents    
.ent doEvents    
doEvents:
    //prepnuti procesu
    //ra obsahuje navratovou adresu
    
#ifdef SAFE_PROCESS    
    //nuluj count
    mtc0    zero, _CP0_COUNT 
#endif    
    
    lw	    t8, (proc_t_pos)		    //process table
    
#ifdef ENABLE_CHECK_STACK_OVERFLOW
    //test, zda dno (nejnizsi word) ve stacku obsahuje kontrolni hodnotu
    //pokud ne = stack overflow
    lw	    t6, TH_T_STACK_BASE*4(t8)	    //adresa stack_base
    lw	    t5, (t6)			    //load data
    li	    t7, STACK_CHECK_VALUE
    bne	    t5, t7, 1f			    //skok, chyba
    nop
    
    //test, zda neni sp <= stack_base
    subu    t6, sp, t6
    bgtz    t6, 2f			    //skok, ok
    nop
    
1:    
    //chyba stack overflow
    la	    t9, (processException)   //adresa error fce
    li	    a0, ERROR_STACK_OVERFLOW        //param (stack overflow)

    
    jrc	    t9
    nop
    
2:    
    
#endif   
    
    //<editor-fold defaultstate="collapsed" desc="save regs">
    swp     v0, TH_T_V0*4(t8)	    //v0, v1
    swp     a0, TH_T_A0*4(t8)	    //a0, a1
    swp     a2, TH_T_A2*4(t8)	    //a2, a3

    swm32   s0-s7, TH_T_S0*4(t8)	    //s0-s7

    swp     gp, TH_T_GP*4(t8)	    //gp, sp
    swp     fp, TH_T_FP*4(t8)	    //fp, ra

    //HI/LO
    //mflo    t0
    //mfhi    t1
    //swp	    t0, TH_T_LO*4(t8)
    //</editor-fold>
    
.global doEventsError
doEventsError:    
    
    //vyber dalsi proces, t8=adresa aktualni polozky v proc_t ------------------
    lw	    v0, (proc_t_after)
1:    
    addu    t8, PROC_T_ISIZE	    //dalsi polozka proc_t
    bne	    t8, v0, 2f		    //skok, neni za proc_t
    nop
    
    //je za posledni polozkou, nastav prvni
    la	    t8, proc_t		    //nastav prvni polozku proc_t

2:    
    lbu	    t9, TH_T_ID(t8)	    //load procID
    beqz    t9, 1b		    //skok, polozka je prazdna
    nop

    //t8=adresa polozky v proc_t
    sw	    t8, (proc_t_pos)	    //ulozit adresu akt. polozky proc_t
    //--------------------------------------------------------------------------
    
    //<editor-fold defaultstate="collapsed" desc="load regs"> 
    //HI/LO
    //lwp	    v0, TH_T_LO*4(t8)
    //mtlo    v0
    //mthi    v1 

    //lw      $1,  TH_T_REGS + 4 (t8)       //at
    lwp     v0, TH_T_V0*4(t8)	    //v0, v1
    lwp     a0, TH_T_A0*4(t8)	    //a0, a1
    lwp     a2, TH_T_A2*4(t8)	    //a2, a3

    //s0-s7 v memory model
    lwm32   s0-s7, TH_T_S0*4(t8)

    lwp     gp, TH_T_GP*4(t8)	    //gp, sp
    lwp     fp, TH_T_FP*4(t8)	    //fp, ra
    
    //</editor-fold>   

    jrc	    ra
    nop
.end doEvents    
//</editor-fold>    
    
//<editor-fold defaultstate="collapsed" desc="doEventsL">     
.align 2    
.global doEventsL    
.ent doEventsL 
doEventsL:
    //prepnuti procesu
    //a0 obsahuje navratovou adresu, kam se skoci v dalsim cyklu
    	
#ifdef SAFE_PROCESS    
    //nuluj count
    mtc0    zero, _CP0_COUNT 
#endif      
    
    move    ra, a0			    //a0 pouzije jako ra
    
#ifdef MICROMIPS    
    ori	    ra, 0x1			    //micromips
#endif    

    lw	    t8, (proc_t_pos)		    //process table
    
#ifdef ENABLE_CHECK_STACK_OVERFLOW
    //test, zda dno (nejnizsi word) ve stacku obsahuje kontrolni hodnotu
    //pokud ne = stack overflow
    lw	    t6, TH_T_STACK_BASE*4(t8)	    //adresa stack_base
    lw	    t5, (t6)			    //load data
    li	    t7, STACK_CHECK_VALUE
    bne	    t5, t7, 1f			    //skok, chyba
    nop
    
    //test, zda neni sp <= stack_base
    subu    t6, sp, t6
    bgtz    t6, 2f			    //skok, ok
    nop
    
1:    
    //chyba stack overflow
    la	    t9, (processException)   //adresa error fce
    li	    a0, ERROR_STACK_OVERFLOW        //param (stack overflow)

    
    jrc	    t9
    nop
    
2: 
#endif      
    
    //<editor-fold defaultstate="collapsed" desc="save regs">
    swp     v0, TH_T_V0*4(t8)	    //v0, v1
    swp     a0, TH_T_A0*4(t8)	    //a0, a1
    swp     a2, TH_T_A2*4(t8)	    //a2, a3

    swm32   s0-s7, TH_T_S0*4(t8)	    //s0-s7

    //26,27 = t8,t9
    swp     gp, TH_T_GP*4(t8)	    //gp, sp
    swp     fp, TH_T_FP*4(t8)	    //fp, ra

    //HI/LO
    //mflo    t0
    //mfhi    t1
    //swp	    t0, TH_T_LO*4(t8)
    //</editor-fold>
    
    //vyber dalsi proces, t8=adresa aktualni polozky v proc_t ------------------
    lw	    v0, (proc_t_after)
1:    
    addu    t8, PROC_T_ISIZE	    //dalsi polozka proc_t
    bne	    t8, v0, 2f		    //skok, neni za proc_t
    nop
    
    //je za posledni polozkou, nastav prvni
    la	    t8, proc_t		    //nastav prvni polozku proc_t

2:    
    lbu	    t9, TH_T_ID(t8)	    //load procID
    beqz    t9, 1b		    //skok, polozka je prazdna
    nop

    //t8=adresa polozky v proc_t
    sw	    t8, (proc_t_pos)	    //ulozit adresu akt. polozky proc_t
    //--------------------------------------------------------------------------    
    
    //<editor-fold defaultstate="collapsed" desc="load regs"> 
    //HI/LO
    //lwp	    v0, TH_T_LO*4(t8)
    //mtlo    v0
    //mthi    v1 

    lwp     v0, TH_T_V0*4(t8)	    //v0, v1
    lwp     a0, TH_T_A0*4(t8)	    //a0, a1
    lwp     a2, TH_T_A2*4(t8)	    //a2, a3

    //s0-s7 v memory model
    lwm32   s0-s7, TH_T_S0*4(t8)

    lwp     gp, TH_T_GP*4(t8)	    //gp, sp
    lwp     fp, TH_T_FP*4(t8)	    //fp, ra
    
    //</editor-fold>   

    jrc	    ra
    nop
.end doEventsL    
//</editor-fold>        
    
//<editor-fold defaultstate="collapsed" desc="getGP">         
.align 2    
.global getGP    
.ent getGP 
getGP:
    //vraci hodnotu GP, nastavi se pro kazdy process v proc_t
    move    v0, gp
    jrc	    ra
    
.end getGP    
//</editor-fold>   
    
//<editor-fold defaultstate="collapsed" desc="general_exception">
.align 2    
.global _general_exception_handler    
.ent _general_exception_handler    
_general_exception_handler:    
    
.global general_exception
general_exception:

    //nastavi parameter a vola fci processException(code)
    //param pro fci processGeneralException
    li	    a0, ERROR_GENERAL_EXCEPTION
    
    //EPC=processException (ERET do teto fce)
    la	    v0, processException
    mtc0    v0, _CP0_EPC
    ehb
    
    eret
   
.end _general_exception_handler  
    
//</editor-fold>
      
    
