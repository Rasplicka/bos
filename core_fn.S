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
    
//.set mips32    
.set micromips     
    
//.section os_alloc_stack_here
//tuto sekci linker vlozi jako posledni v data RAM
//definuje prvni volne misto v RAM    
//stack_area:	    .dword	    0x0	    //dword proto, aby byla zarovnana na 8 bytes
//v pripade, ze by linker umistoval tuto sekci jinam, bylo by mozne fci malloc alokovat
//libovolna data v heap, kterou linker vzdy umistuje hned za data. Adresa techto dat
//by definovala zacatek oblasti stacku    
    
.section .asm_fn, code     
    
//<editor-fold defaultstate="collapsed" desc="startupStack">
.align 2
.global startupStack
.ent startupStack
startupStack:
    
    //nastavi hodnotu sp pro startup OS (na TOP adresu stack_area)
    
    //sp SRS[0], docasny zasobnik
    la	    v0, stack_area
    li	    v1, STACK_SIZE
    addu    sp, v0, v1 
    
    jrc	    ra
    
.end startupStack  
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="clearProcTable">
.align 2
.global clearSystemData
.ent clearSystemData
clearSystemData: 
//a0=startAddr
//a1=size
    
    //Tato fce nuluje obsah dat pro OS (sekce .os)
    
    addu    a1, a0		//after
    
1:    
    sw	    zero, (a0)
    addiu   a0, 4
    bne	    a0, a1, 1b
  
    jrc	    ra
    
.end clearSystemData   

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
    
    //nop
    b16	    \collide
    //nop
    
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
    //nop
    b16	    2f 
    //nop

1:
    //pouzije dalsi polozku proc_t, za jejiz stack se pokusi vlozit novy stack
    
    lbu32    t9, TH_T_ID*4(t7)
    beqz    t9, 6f 			//skok, polozka neni pouzita
    //nop
    
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
    //nop
    beq	    t6, a1, 4f			//skok, toto je nova polozka (pro kterou se hleda stack)	    
    //nop
    beq	    t6, t7, 4f			//skok, za touto polozkou je test oblast (proto nemuze byt v kolizi)
    //nop
    lw32    v1, TH_T_START_SP*4(t6)	//stack TOP (after)
    lw32    v0, TH_T_STACK_BASE*4(t6)	//stack BOTTOM (start)
    
    //macro, v pripade kolize skok na 5f
    //start1, after1, start2, after2, err
    _TEST_COLLISION v0, v1, t4, t5, 5f 
    
4:    
    //ok, neni v kolizi, nebo prekoceni testu
    addiu   t6, PROC_T_ISIZE
    bne	    t6, t1, 3b			//skok, dalsi polozka proc_t
    //nop
    
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
    //nop
    
    //probehl test na zacatek oblasti 
    move    t7, t0
    //nop
    b16	    1b
    //nop
    
6:     
    addiu   t7, PROC_T_ISIZE
    bne	    t7, t1, 1b 
    //nop
    
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
    //nop
    
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
    li	    k0, SAFE_MODE_TIME_LIMIT_VALUE
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
    //nop						//musi zde zustat
    
.end startEvents
//</editor-fold>        
    
//<editor-fold defaultstate="collapsed" desc="doEvents">     
.align 2	
.global doEvents    
.ent doEvents    
doEvents:
    //prepnuti procesu (volano z processu)
    //ra obsahuje navratovou adresu
    
#ifdef SAFE_PROCESS    
    //nuluj count, nastav compare 
    mtc0    zero, _CP0_COUNT 
#endif    
    
    lw	    t8, (proc_t_pos)		    //process table
    
    //<editor-fold defaultstate="collapsed" desc="STACK_OVERFLOW">
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
    //nop
    
1:    
    //chyba stack overflow
    la	    t9, (processException)   //adresa error fce
    li	    a0, ERR_STACK_OVERFLOW        //param (stack overflow)

    
    jrc	    t9
    //nop
    
2:    
    
#endif 
    //</editor-fold>
   
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
    
//skok sem po chybe v procesu (pri restartApp), neuklada stav regs    
.global doEventsError
doEventsError:    
    
    //<editor-fold defaultstate="collapsed" desc="next process">
    //vyber dalsi proces, t8=adresa aktualni polozky v proc_t ------------------
    lw	    v0, (proc_t_after)
1:    
    addu    t8, PROC_T_ISIZE	    //dalsi polozka proc_t
    bne	    t8, v0, 2f		    //skok, neni za proc_t
    //nop
    
    //je za posledni polozkou, nastav prvni
    la	    t8, proc_t		    //nastav prvni polozku proc_t

2:    
    lbu	    t9, TH_T_ID(t8)	    //load procID
    beqz    t9, 1b		    //skok, polozka je prazdna
    //nop

    //t8=adresa polozky v proc_t
    sw	    t8, (proc_t_pos)	    //ulozit adresu akt. polozky proc_t
    //--------------------------------------------------------------------------
    //</editor-fold>
    
#ifdef SAFE_PROCESS    
    //nastav compare 
    lw32    t9, TH_T_LIMIT*4(t8)
    mtc0    t9, _CP0_COMPARE
#endif      
    
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
    //nop
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
    
    move    ra, a0			    //param v a0 pouzije jako ra (navratova adresa)
    
#ifdef MICROMIPS    
    ori	    ra, 0x1			    //micromips (licha adresa zajisti prepnuti do micromips po "jrc ra")
#endif    

    lw	    t8, (proc_t_pos)		    //process table
    
    //<editor-fold defaultstate="collapsed" desc="STACK_OVERFLOW">
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
    //nop
    
1:    
    //chyba stack overflow
    la	    t9, (processException)   //adresa error fce
    li	    a0, ERR_STACK_OVERFLOW        //param (stack overflow)

    
    jrc	    t9
    //nop
    
2: 
#endif  
    //</editor-fold>
    
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
    
    //<editor-fold defaultstate="collapsed" desc="next process">
    //vyber dalsi proces, t8=adresa aktualni polozky v proc_t ------------------
    lw	    v0, (proc_t_after)
1:    
    addu    t8, PROC_T_ISIZE	    //dalsi polozka proc_t
    bne	    t8, v0, 2f		    //skok, neni za proc_t
    //nop
    
    //je za posledni polozkou, nastav prvni
    la	    t8, proc_t		    //nastav prvni polozku proc_t

2:    
    lbu	    t9, TH_T_ID(t8)	    //load procID
    beqz    t9, 1b		    //skok, polozka je prazdna
    //nop

    //t8=adresa polozky v proc_t
    sw	    t8, (proc_t_pos)	    //ulozit adresu akt. polozky proc_t
    //--------------------------------------------------------------------------    
    //</editor-fold>
    
#ifdef SAFE_PROCESS    
    //nastav compare 
    lw32    t9, TH_T_LIMIT*4(t8)
    mtc0    t9, _CP0_COMPARE
#endif      
    
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
    //nop
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
  
//<editor-fold defaultstate="collapsed" desc="restartApp, restartAppId">
.align 2
.global restartApp
.ent restartApp
restartApp:
    //restaruje aktualni App
    //muze byt volano i v obsluze events (systemovy process)
    //ukonci process, kteremu patri udalost, ne system process
    
    bal	    getProcID				    //aktualni procID, respektuje beh v events
    move    a0, v0
    
.global restartAppId   
restartAppId:    
//a0=procID
    
    move    s0, a0				    //s0=zaloha procID
    bal	    getProcTableItem			    //zjisti proc_t polozku procesu
    
    //v0=polozka proc_t
    lw32    v1, TH_T_START_SP(v0)
    sw32    v1, TH_T_SP(v0)			    //default sp
    
    lw32    v1, TH_T_START_ADDR(v0)	
    sw32    v1, TH_T_RA(v0)			    //default ra
    
    move    a0, s0				    //a0=procID
    bal	    removeRegEventProcID		    //remove reg events    
    move    a0, s0				    //a0=procID
    bal	    removeEventCacheProcID		    //remove raised events
    
    //zjisti, zda je normalni beh, nebo system process - pri event
    la	    v0, proc_t
    lw	    v1, (proc_t_pos)			    //proc_t_pos=proc_t (prvni process = system process)
    bne	    v0, v1, 1f				    //skok, normalni beh   

    //system process, volano z event
    //navrat do system procesu
    la	    ra, systemProcessResumeError
    jrc	    ra
    
1:    
    //normalni beh
    la	    ra, (doEventsError)		//prepnuti procesu, neuklada regs
    jrc	    ra
    
.end restartApp 
    
//</editor-fold>
   
    
//<editor-fold defaultstate="collapsed" desc="defragTableW">
    
.align 2
.global defragTableW
.ent defragTableW
defragTableW:
//a0=adresa tabulky
//a1=item size [bytes] (delitelne 4)
//a2=max. pocet polozek, item cnt
//meni t7-t9, v0, v1, a0-a2
    
    //tabulka musi obsahovat polozky word (velikost polozky min. 1 word, delitelna 4)
    //posouva polozky v tabulce tak, aby mezi nimi nebyly neplatne polozky
    //tzn. prvni nulova polozka je konec tabulky
    //polozka musi zacinat word, ktery urcuje jeji platnost. Hodnota 0x0 = polozka je neplatna, hodnota != 0x0, polozka je platna
    
    
    //a0=src
    //t7=dest
    move    t7, a0
    
1:
    lw	    t9, (a0)		    //load src data
    beqz    t9, 3f		    //skok, src data=0
    //nop
    
    //src data neni 0
    bne	    t7, a0, 4f		    //skok src != dest, bude presun
    //nop
    
    //src=dest, zadny presun
2:    
    addu    t7, a1		    //dest++
    
3:
    addu    a0, a1		    //src++
    addiu   a2, -1		    //item cnt--
    bne	    a2, zero, 1b	    //neni konec
    //nop
    
    //je konec, prosel vsechny polozky
    jrc	    ra
    //nop
    
4:
    //presun dat ze src do dest
    move    v0, a0		    //src
    move    v1, t7		    //dest
    move    t8, a1		    //size bytes
    
5:    
    lw	    t9, (v0)
    sw	    t9, (v1)
    addiu   v0, 4
    addiu   v1, 4
    addiu   t8, -4
    bnez    t8, 5b		    //skok, polozka pokracuje
    //nop
    
    //src[0]=0x0
    sw	    zero, (a0)
    //nop
    b16	    2b
    //nop
    
.end defragTableW    
    
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="defragTableB">
    
.align 2
.global defragTableB
.ent defragTableB
defragTableB:
//a0=adresa tabulky
//a1=item size [bytes] (delitelne 1)
//a2=max. pocet polozek, item cnt
//meni t7-t9, v0, v1, a0-a2    
    
    //tabulka musi obsahovat polozky word (velikost polozky min. 1 word, delitelna 4)
    //posouva polozky v tabulce tak, aby mezi nimi nebyly neplatne polozky
    //tzn. prvni nulova polozka je konec tabulky
    //polozka musi zacinat word, ktery urcuje jeji platnost. Hodnota 0x0 = polozka je neplatna, hodnota != 0x0, polozka je platna
    
    
    //a0=src
    //t7=dest
    move    t7, a0
    
1:
    lbu	    t9, (a0)		    //load src data
    beqz    t9, 3f		    //skok, src data=0
    //nop
    
    //src data neni 0
    bne	    t7, a0, 4f		    //skok src != dest, bude presun
    //nop
    
    //src=dest, zadny presun
2:    
    addu    t7, a1		    //dest++
    
3:
    addu    a0, a1		    //src++
    addiu   a2, -1		    //item cnt--
    bne	    a2, zero, 1b	    //neni konec
    //nop
    
    //je konec, prosel vsechny polozky
    jrc	    ra
    //nop
    
4:
    //presun dat ze src do dest
    move    v0, a0		    //src
    move    v1, t7		    //dest
    move    t8, a1		    //size bytes
    
5:    
    lbu	    t9, (v0)
    sb	    t9, (v1)
    addiu   v0, 1
    addiu   v1, 1
    addiu   t8, -1
    bnez    t8, 5b		    //skok, polozka pokracuje
    //nop
    
    //src[0]=0x0
    sb	    zero, (a0)
    //nop
    b16	    2b
    //nop
    
    
.end defragTableB    
    
//</editor-fold>        
    
    
//<editor-fold defaultstate="collapsed" desc="compareTimerMs">
    
.align 2
.global compareTimerMs
.ent compareTimerMs    
compareTimerMs:
//a0=porovnavany cas (predpoklada se, ze je starsi, nez soucasny - timer_ms)
//vraci kolik ms uplynulo od zadaneho casu    
//meni t8,t9 (v0)    
    
    //je-li timer nastaven na 10ms, rozdil je po 10ms
    
    lw	    t9, (timer_ms)
    subu    v0, t9, a0
     
    bltz    v0, 1f
    //nop
    jrc	    ra
    
1:
    //doslo k preteceni timer_ms
    li	    t8, 0xFFFFFFFF
    subu    v0, t8, a0 
    addu    v0, t9
    jrc	    ra
    
.end compareTimerMs 
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="pauseEvents">    
.align 2
.global pauseEvents
.ent pauseEvents
pauseEvents:
//a0=pauza ms
    
#ifdef SIMULATOR
    //simulator nemeri cas
    jrc	    ra
#endif    
    
    move    a2, ra		    //zaloha ra
    move    v1, a0		    //pocet ms cekani
    lw	    a0, timer_ms	    //pocatecni time_ms
    
1:
    //test, zda je jiz Threading (muze volat doEvents)
    lbu	    t9, (SYSTEM_STATUS)	    //SYSTEM_STATUS.b0=Threading
    ext	    t9, t9, 0, 1	    //ext b0
    beqz    t9, 2f
    //nop
    
    //doEvents
    bal	    doEvents		    //neuklada obsah t0-t9
    //nop
    
2:    
    //porovnava aktualni time_ms s time_ms v a0 (pocatecni cas)
    bal	    compareTimerMs	    //meni t9, t8, v0 vraci kolik uplynulo ms (po 10 ms)
    //nop
    
    subu    v0, v1
    bltz    v0, 1b		    //skok, neuplynul cas
    //nop
    
    move    ra, a2		    //obnov ra
    jrc	    ra
    
.end pauseEvents    
    
//</editor-fold>
    
//errors
    
//<editor-fold defaultstate="collapsed" desc="iVector0 CPU Timer">
.align 2    
.global iVector0
.ent iVector0
iVector0:
    
    //CPU timer interrupt, interrupt level 1, pouziva SRS[1]
    //aktualni proces trva prilis dlouho, neprovedl doEvents
    //COUNT a COMPARE necha bez zmeny

    
#if defined PIC32MM0256    
    #define	    IFS_BIT	    0			//bit v IFS registru
    #define	    IFS_REG_CLR	    IFS0CLR		//IFS CLR register
#elif defined PIC32MM0064
    #define	    IFS_BIT	    0
    #define	    IFS_REG_CLR	    IFS0CLR     
#elif defined PIC32MZ
    //...
#endif    
    
    //nuluj flag
    li16    v0, (1 << IFS_BIT)	    
    sw	    v0, (IFS_REG_CLR)
    
    //param pro fci processException
    li	    s1, ERR_CODE_TIME_LIMIT_EXCEED
    
    //pokracuje v generalException
    b	    general_exception_shared_point
    
    //EPC=processException (ERET do teto fce)
    //la	    v0, processException
    //mtc0    v0, _CP0_EPC
    //ehb
    
    //eret
    
.end iVector0  
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="general_exception">
.align 2    
.global _general_exception_handler    
.ent _general_exception_handler    
_general_exception_handler:    
    
.global general_exception
general_exception:

    //nastavi parameter a vola fci processException(code)
    //param pro fci processException
  
    //<editor-fold defaultstate="collapsed" desc="err code">
    //zjisti, zda nastal TRAP (v c/c++ volani fce trap())
    li	    s1, ERR_CODE_TRAP
    
    mfc0    t9, _CP0_CAUSE		//cause.b0-4
    ehb
    
#ifdef PIC32MM    
    ext	    t9, t9, 2, 4
#else 
    //!!!zkontrolovat na MZ, zda trap = b0-4 (podle doc. ano)
    //ale je divny, pro MM ma b2-5 
    ext	    t9, t9, 0, 5
#endif    
    
    li	    t8, 0x0D			//trap code=0x0D
    beq	    t8, t9, 1f			//skok, je trap
  
    //neni trap
    li	    s1, ERR_CODE_GENERAL_EXCEPTION
    //</editor-fold>
   
general_exception_shared_point:    
1:  
    
    //<editor-fold defaultstate="collapsed" desc="userAppError">
    //a0=err code
    mfc0    s2, _CP0_EPC		//do a1 adresu chyby
    
    //zjisti procID
    bal	    getProcID
    move    s0, v0			//do a2 procID
    
    //s0=procID, s1=err code, s2=bad addr

    //volat user fci userAppError (status.EXL=1, umoznuje HALT)
    move    a0, s0			//procID
    move    a1, s1			//code
    move    a2, s2			//addr
    bal	    userAppError
    //</editor-fold>

    
    //<editor-fold defaultstate="collapsed" desc="najdi proc_t polozku">
    move    a0, s0			//procID
    bal	    getProcTableItem		
    beqz    v0, 9f			//chyba nenasel polozku v proc_t (nemelo by nikdy nastat)
    move    s3, v0			//proc_t item, zaloha do s3
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="zjisti ON_ERROR behavior, sw RESET">
    move    a0, s3			//a0=proc_t adresa polozky procesu
    move    a1, s1			//err code
    bal	    getErrorBehavior
    
    //v0=behavior
    beqz    v0, 9f			//skok, behavior = reset
    
    //RESET PROCESS, nebo REMOVE PROCESS
    move    s4, v0			//s4 = zaloha behavior
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="remove events procesu">
    move    a0, s0			//a0=procID
    bal	    removeRegEventProcID	//remove reg events    
    move    a0, s0			//a0=procID
    bal	    removeEventCacheProcID	//remove raised events
    
    li16    v0, ON_ERROR_RESET_PROCESS
    bne	    v0, s4, 3f			//skok, neni RESET PROCESS
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="RESET PROCESS">
    //RESET PROCESS
    lw32    v0, TH_T_START_SP(s3)	
    sw32    v0, TH_T_SP(s3)		//SP=default
    lw32    v0, TH_T_START_ADDR(s3)	
    sw32    v0, TH_T_RA(s3)		//RA=default

    b16	    4f
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="REMOVE PROCESS">
3:
    //REMOVE PROCESS
    sw32    zero, TH_T_ID(s3)		//nuluje procID
    //</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="ERET, normalni beh, nebo system process">
4:
    //zjisti, kde nastala chyba (normalni beh, nebo system process - pri event)
    la	    v0, proc_t
    lw	    v1, (proc_t_pos)		//proc_t_pos=proc_t (prvni process = system process)
    bne	    v0, v1, 5f			//skok, chyba pri normalnim behu
    
    //chyba v obsluze event, zpet do system process
    la	    v0, systemProcessResumeError
    mtc0    v0, _CP0_EPC
    ehb
    eret

5:
    //chyba pri normalnim behu, spustit dalsi proces 
    la	    v0, (doEventsError)
    mtc0    v0, _CP0_EPC
    ehb
    eret 
    //</editor-fold>
 
    //<editor-fold defaultstate="collapsed" desc="reset, unknown error">
9:    
    //reset, nebo neznama chyba
    la	    v0, softReset
    jrc	    v0
    //</editor-fold>
   
.end _general_exception_handler  
    
//</editor-fold>
     
    
//<editor-fold defaultstate="collapsed" desc="getProcID">
.align 2    
.global getProcID    
.ent getProcID
getProcID:     
    //vraci v0 = aktualni procID
  
    lw	    v0, (proc_t_pos)
    la	    v1, proc_t
    bne	    v0, v1, 1f			    //skok, neni prvni (system) proces
    
    //systemProcess, aktualni procID je v errorProcID
    lw	    v0, (errorProcID)
    jrc	    ra
    
1:
    //bezi jiny nez systemProcess, precte ID z proc_t_pos
    lbu	    v0, TH_T_ID(v0)
    jrc	    ra
    
.end getProcID  
    
//</editor-fold>  

//<editor-fold defaultstate="collapsed" desc="getProcTableItem">
.align 2    
.global getProcTableItem    
.ent getProcTableItem
getProcTableItem: 
//a0=procID
    la	    v0, proc_t
    addiu   v1, v0, (PROC_T_ISIZE * PROC_T_CAPA)
    b16	    2f
    
1:
    addiu   v0, PROC_T_ISIZE
    beq	    v0, v1, 9f
2:    
    lbu	    t9, TH_T_ID(v0)
    bne	    t9, a0, 1b
    
    //ok, nasel polozku
    jrc	    ra				    //v0=adresa polozky
    
9:
    //nenasel polozku
    move    v0, zero
    jrc	    ra
    
.end getProcTableItem  
    
//</editor-fold>
  
//<editor-fold defaultstate="collapsed" desc="getErrorBehavior">
.align 2    
.global getErrorBehavior   
.ent getErrorBehavior
getErrorBehavior:     
//a0=adresa polozky proc_t
//a1=err code
    
    lw	    t9, TH_T_ID(a0)		    //load word, b8-15 = behavior
    
    li16    v0, ERR_CODE_TIME_LIMIT_EXCEED
    bne	    v0, a1, 1f
    //CPU timer limit error
    ext	    v0, t9, 8, 2		    //b8-9
    b16	    4f
    
1:
    li16    v0, ERR_CODE_GENERAL_EXCEPTION
    bne	    v0, a1, 2f
    //General Exception
    ext	    v0, t9, 10, 2		    //b10-11
    b16	    4f    
    
2:
    li16    v0, ERR_CODE_TRAP
    bne	    v0, a1, 3f    
    //trap
    ext	    v0, t9, 12, 2		    //b12-13
    b16	    4f    
    
3:
    //stack overflow
    move    v0, zero			    //RESET_SYSTEM
    
4:    
    //v0=ON_ERROR, 0=RESET_SYSTEM, 1=RESET_PROCESS, 2=REMOVE_PROCESS
 
    jrc	    ra
    
.end getErrorBehavior 
    
//</editor-fold>
   
//events
    
//<editor-fold defaultstate="collapsed" desc="getRegEvent">
.align 2    
.global getRegEvent    
.ent getRegEvent
getRegEvent: 
//a0=adresa zacatku hledani
//a1=eventID    

    //fce hleda polozku podle eventID
    //vraci v0=adresa polozky, nebo 0=nenasel
    //hledani zacina na adrese a0 (je-li > 0, jinak od zacatku regEventTable)
    
    la	    v0, regEventTable
    addiu   v1, v0, (REG_EVENT_TABLE_ISIZE * REG_EVENT_TABLE_CAPA)
    
    beqz    a0, 2f			//skok, je zadan zacatek hledani (naposledy nalezena)
   
    move    v0, a0
   
1:    
    addiu   v0, REG_EVENT_TABLE_ISIZE	//dalsi polozka
    beq	    v0, v1, 9f			//skok, je na konci    
    
2:    
    lbu	    t9, (v0)			//load eventID (0=empty)
    beqz    t9, 9f			//skok, konec	
    bne	    t9, a1, 1b			//skok, jina polozka 

    //nasel polozku (v0)
    jrc	    ra
    
9:
    //nenasel dalsi polozku
    move    v0, zero
    jrc	    ra
    
.end getRegEvent  
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="addRegEvent">
.align 2    
.global addRegEvent    
.ent addRegEvent
addRegEvent:    
//a0 eventID(b0-7), procID(b8-15), var2(b16-23), var3(b24-31)
//a1 var0
//a2 var1
//a3 fn
    
    //meni t9, v0, v1
    //vraci v0=adr. polozky (OK), v0=0 (NULL=chyba), plna tabulka
    
    //prida polozku do globalni tabulky regEventTable, kde jsou
    //vsechny registrovane udalosti
    //pri ukonceni procesu, OS automaticky vyradi polozky tohoto procesu
    //tabulku pote defragmentuje
    //prvni eventID=0, polozka je volna
    //tato fce neni nikdy volana z interruptu (proto nemusi pracovat v rezimu DI)
    
//regEventTable:    
//b0 -   eventID  
//b1 -   procID
//b2 -   var2 - 8bit
//b3 -   var3 - 8bit   
//b4-7   var0 - 32bit
//b8-11  var1 - 32bit    
//b12-16 fn_addr    

    la	    v0, regEventTable
    addiu   v1, v0, (REG_EVENT_TABLE_ISIZE * REG_EVENT_TABLE_CAPA)
    b16	    2f
 
1:    
    addiu   v0, REG_EVENT_TABLE_ISIZE	//dalsi polozka
    beq	    v0, v1, 9f			//skok, je na konci
    
2:    
    lbu	    t9, (v0)			//load eventID (0=empty)
    bnez    t9, 1b			//skok, polozka neni prazdna

    //ok, polozka je prazdna
    sw	    a3, RET_FN(v0)		//fn addr
    sw	    a2, RET_V1(v0)		//fn addr
    sw	    a1, RET_V0(v0)		//fn addr
    sw	    a0, RET_EVENTID(v0)		//eventID, procID, var2, var3

    jrc	    ra
    
9:
    move    v0, zero
    jrc	    ra
    
.end addRegEvent  
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="removeRegEventProcID">
.align 2    
.global removeRegEventProcID    
.ent removeRegEventProcID
removeRegEventProcID: 
//a0=procID
    
    //meni t7-t9, v0, v1, a0-a2 (defragTableW)
    
    //vola se pri ukonceni procesu procID
    //vymaze vrechny registrace se stejnym procID, z regEventTable
    //nakonec je regEventTable defragmentovana v rezimu DI
    
    la	    v0, regEventTable
    addiu   v1, v0, (REG_EVENT_TABLE_ISIZE * REG_EVENT_TABLE_CAPA)
    move    t8, zero			//status, zda byla vyrazena polozka
    b16	    2f
 
1:    
    addiu   v0, REG_EVENT_TABLE_ISIZE	//dalsi polozka
    beq	    v0, v1, 9f			//skok, je na konci
    
2:    
    lbu	    t9, RET_PROCID(v0)		//load procID
    bne	    t9, a0, 1b
    
    //stejne procID
    sb	    zero, RET_EVENTID(v0)	//eventID=0 (volna polozka)
    addiu   t8, 1			//status ++
    b16	    1b

9:
    beqz    t8, 10f			//skok, nenastala zmena
    _PUSH   ra
    
    //defrag eventCache
    la	    a0, regEventTable			//a0=adresa tabulky
    li	    a1, REG_EVENT_TABLE_ISIZE		//a1=item size [bytes] (delitelne 4)
    li	    a2, REG_EVENT_TABLE_CAPA		//a2=max. pocet polozek, item cnt
    di
    ehb
    bal	    defragTableW
    ei
    
    _POP    ra
    
10:    
    jrc	    ra
    
.end removeRegEventProcID 
    
//</editor-fold>    
    
//<editor-fold defaultstate="collapsed" desc="removeEventCacheProcID">
.align 2    
.global removeEventCacheProcID    
.ent removeEventCacheProcID
removeEventCacheProcID: 
//a0=procID
    
    //meni t8, t9, v0, v1
    //vymaze vrechny cekajici udalosti v eventCache
    //v eventCache nastavi vsechny polozky daneho procesu na 0xFF - dokoncene
    //polozky z eventCache se takto vyrazuji, aby v pripade raiseEvent z interruptu
    //byly nove polozky pridavany na konec
    //systemProcess nejprve vykona vsechny polozky (0 < procID > 0xFF) a pak je nuluje (pri DI)
    //proto se nemusi eventCache defragmentovat
    
    la	    v0, eventCache
    addiu   v1, v0, (EVENT_CACHE_ISIZE * EVENT_CACHE_CAPA)
    li	    t8, 0xFF
    b16	    2f
 
1:    
    addiu   v0, EVENT_CACHE_ISIZE	//dalsi polozka
    beq	    v0, v1, 9f			//skok, je na konci
    
2:    
    lbu	    t9, (v0)			//load procID
    bne	    t9, a0, 1b
    
    //stejne procID
    sb	    t8, EVC_PROCID(v0)		//procID=0xFF (dokoncena polozka)
    b16	    1b

9:

    jrc	    ra    
    
.end removeEventCacheProcID 
    
//</editor-fold>    
    
//<editor-fold defaultstate="collapsed" desc="raiseEvent">
.align 2    
.global raiseEvent    
.ent raiseEvent
raiseEvent:  
//a0=adresa polozky v regEventTable (pouzije z ni procID a fn addr)
//a1=p0
//a2=p2
//a3=p3
    
    //meni t9, v0, v1
    //vraci v0=1 OK, v0=1 chyba, eventCache je plna
    
    //vyvolani udalosti v a0
    //prochazi eventCache a hleda volnou polozku
    //tu vyplni daty
    //pracuje v rezimu DI, jinak by v prubehu mohl interupt menit eventCache (take muze volat raiseEvent)

    
    //eventCache
    la	    v0, eventCache
    addiu   v1, v0, (EVENT_CACHE_ISIZE * EVENT_CACHE_CAPA)
    di
    ehb
    b16	    2f
  
1:
    //polozka neni prazdna
    addiu   v0, EVENT_CACHE_ISIZE
    beq	    v0, v1, 9f 
    
2:    
    lbu	    t9, EVC_PROCID(v0)		//load procID v eventCache
    bnez    t9, 1b			//skok, polozka neni volna    
    
    //ok, nasel volnou polozku
    lbu	    t9, RET_PROCID(a0)		//load procID
    sb	    t9, EVC_PROCID(v0)		//save procID
    
    ei					//polozka je nyni obsazena, muze povolit interrupt
    
    lw	    t9, RET_FN(a0)		//load fn addr
    sw	    t9, EVC_FN(v0)		//save fn addr
    
    sw	    a1, EVC_P0(v0)		//save params
    sw	    a2, EVC_P1(v0)
    sw	    a3, EVC_P2(v0)
  
    li16    v0, 1
    jrc	    ra
    
9:    
    ei
    move    v0, zero
    jrc	    ra

.end raiseEvent  
    
//</editor-fold>
   
//<editor-fold defaultstate="collapsed" desc="systemProcess">
.align 2    
.global systemProcess    
.ent systemProcess
systemProcess: 
    //systemovy proces
    //vzdy projde celou eventCache a vola jednotlive events - fn(p0, p1, p2)
    //pripadna chyba v event fn se povazuje za chybu daneho procesu, ne systemProcessu
    //pokud je v event fci doEvents(), dojde k beznemu prepnuti procesu, tzn. ze 
    //provadeni eventCache se pozastavi (a muze dojit k jejimu naplneni)
    //doEvents by se v event fci volat nemelo, ale chyba to neni.
    
    //systemProcess prochazi polozky a dokoncenou oznaci procID=0xFF, tzn. dokonceno
    //to je proto, aby pripadne raiseEvent z interruptu nepouzilo jiz dokoncene polozky
    //ale aby pridavalo nove events na konec tabulky.
    //kriticke casti systemProcess jsou v rezimu DI
    
    
    
    //la	    v0, systemProcessResumeError
    //sw	    v0, (onErrorResume)		    //meni chovani pro error
    
1:
    //eventCache
    la	    s0, eventCache
    addiu   s1, s0, (EVENT_CACHE_ISIZE * EVENT_CACHE_CAPA)   
    li	    s2, 0xFF
    di
    ehb

2:  //v rezimu DI
    lbu	    t9, EVC_PROCID(s0)
    beqz    t9, 5f			    //skok, prazdna polozka, konec eventCache
    beq	    t9, s2, 3f			    //skok, dokoncena polozka 
    
    //platna polozka
    ei
    lw	    v0, EVC_FN(s0)		    //fn addr
    lw	    a0, EVC_P0(s0)		    //p0
    lw	    a1, EVC_P1(s0)		    //p1
    lw	    a2, EVC_P2(s0)		    //p2
    
    sw	    t9, (errorProcID)		    //nastav aktualni procID
    jalr    v0				    //skok do fce
    
    //po navratu z event fn
    sb	    s2, EVC_PROCID(s0)		    //oznac jako dokonceno (ale ne empty, aby polozku neobsadilo raiseEvent z interruptu)
    di
    ehb
    
3:  //next    
    addiu   s0, EVENT_CACHE_ISIZE
    bne	    s0, s1, 2b
    
    //je za eventCache

5:    
    //konec, vsechny udalosti dokonceny, a jsou oznaceny jako neplatne - 0xFF
    //vymazat eventCache (v rezimu DI, nezmi nastat zadna zmena v eventCache)
    la	    s0, eventCache
6:    
    lbu	    t9, EVC_PROCID(s0)
    beqz    t9, 7f
    
    sb	    zero, EVC_PROCID(s0)
    addiu   s0, EVENT_CACHE_ISIZE	
    bne	    s0, s1, 6b
    
7:  //ok, cela eventCache je prazdna (nyni muze interrupt pridat novou polozku)
    ei
    //sw	    zero, (onErrorResume)	    //meni chovani pro error
    la	    a0, systemProcess		    //param, adresa pristiho cyklu
    bal	    doEventsL			    //prepni process
    
systemProcessResumeError:    
    //v eventCache byly polozky ukonceneho procesu nastaveny na 0xFF, neni treba defragmentovat
    //pozor, mohou byt zmeneny i s0-s7   
    //restart system processu
    lw	    t9, (proc_t_pos)
    lw32    sp, TH_T_START_SP(t9)	    //nastav sp na vychozi hodnotu
    b16	    1b
    
.end systemProcess  
    
//</editor-fold>    