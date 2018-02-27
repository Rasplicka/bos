#include "xc.h"
#include "asm_macro.S"
#include "def.h"       

 /*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS module, digital input pins scan
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */
    
//<editor-fold defaultstate="collapsed" desc="popis">
    
//pracuje v modu 0, 1, nebo 2
//mod 0
//je-li btn stisknuto delsi dobu, odesle jeden event UBTN_DOWN_LONG
//a po uvolneni UBTN_UP_LONG    
//app reaguje na tyto udalosti, tzn. ze reaguje az na UP (nebo UBTN_DOWN_LONG, nikoliv na DOWN)    
//app rozlisuje mezi kratkym a dlouhym stisknutim    
//mod 1
//je-li btn stisknuto delsi dobu, odesilaji se periodicky UBTN_REPEAT (prvni pauza je delsi)    
//app reaguje na UBTN_DOWN a UBTN_REPEAT    
//app nerozlisuje kratke a dlouhe stisknuti  
    
//mod 0+1 pri kratkem stisknuti odesila UBTN_DOWN a UBTN_UP v obou modech    
    
//mod 2
//mod pro vstupni pin. Odesila pouze UBTN_DOWN a UBTN_UP pri zmene stavu    
    
//param0 je UBTN_UP/UBTN_DOWN/UBTN_LONG_DOWN/UBTN_LONG_UP/UBTN_REPEAT
//param1 je value (= kod tlacitka, viz. buttons_table)    
    
//stav je potvrzeny, pokud je prectena 10x stejna hodnota    
    
//max. pocet buttons je omezen na BUTTONS_TABLE_CAPA (mozno zmenit)   
//kazde tlacitko se definuje v buttons_table
    
//app, ktera chce prijimat events tohoto modulu, musi volat ubtn_regEvent(&fn)
//pak je fn volana pri kazde event
//void fn(int event, int button_value, char cnt)    
//cnt je poradi REPEAT, nebo DOWN_LONG (0-0xFF)
    
//fce ubtn_start nastavuje TRIS(input), ANLSEL(digital), CNPU(pull-up povoli), CNPD(pull-down zakaze)    
//tlacitko se pripojuje mezi pin a zem    
    
    
    
//POZOR po dlouhem stisku (odesila DOWN_LONG) a uvolneni neodesila UP, ale pouze UP_LONG    
//stisk                                                                                                  uvolneni    
//DOWN...................................................................................................
//..............REPEAT(0)....REPEAT(1)....REPEAT(2)....REPEAT(3)....REPEAT(4)....REPEAT(5)....REPEAT(6)..
//...........................DOWN_LONG(0)...........................DOWN_LONG(1).........................UP_LONG    

//stisk                    uvolneni    
//DOWN..........REPEAT(0)..UP
    
//stisk   uvolneni    
//DOWN....UP       
    
//</editor-fold>
    
#ifdef UBTN
  
//<editor-fold defaultstate="collapsed" desc="definition">
 
#define		LONG_DOWN_MS		3000			//po 3000 ms detekuje dlouhe stisknuti (mod 0)
#define		REPEAT_FIRST_MS		2000			//po 2000 ms poprve odesila REPEAT event (mod 0)
#define		REPEAT_NEXT_MS		200			//po 200 ms druhe a dalsi REPEAT event (mod 0)
    
#define		BUTTONS_TABLE_CAPA	((buttons_table_end-buttons_table)/8) //8 
#define		ITEM_SIZE		12				//polozka v items: rep_time[4B], long_time[4B], rep_count[1B], long_count[1B], up_state[1B], down_state[1B]
#define		BUTTONS_TABLE_ISIZE	8				//velikost polozky v buttons_table    
     
#define		REP_TIME_OFF		0
#define		LONG_TIME_OFF		4
#define		REP_COUNT_OFF		8
#define		LONG_COUNT_OFF		9
#define		UP_STATE_OFF		10
#define		DOWN_STATE_OFF		11  
 
 //</editor-fold>

//<editor-fold defaultstate="collapsed" desc="RAM vars">
   
.section .ubtn.data    
items:		.space			BUTTONS_TABLE_CAPA * ITEM_SIZE   
    
//</editor-fold>
  
.set reorder    
.set micromips   
.section .ubtn, code
.align 2
    
//<editor-fold defaultstate="collapsed" desc="BUTTONS TABLE">
    
//<editor-fold defaultstate="collapsed" desc="MACRO">
.macro _DEFINE_BUTTON port, bit, mode, value
    .word	\port		//PORTx_BASE, base addr portu 
    .hword	\bit 		//bit_index (A2)
    .byte	\mode 		//mode 0=down, repeat, long down, up, long up, 1=down, up
    .byte 	\value 		//value, ktera bude odeslana v event, param1
.endm
//</editor-fold>    
    
buttons_table:
//tabulka obsahuje popis vsech tlacitek, je zakoncena 0x0 (velikost se urci automaticky: buttons_table_end - buttons_table)
    //		   word        hword    byte	byte
    //             port base   pin      mode	value
    
    /*
    _DEFINE_BUTTON PORTA_BASE, BIT2,	0,	32		//A.2
    _DEFINE_BUTTON PORTA_BASE, BIT3,	0,	33		//A.3
    */
    
    //PIC32MZ test buttons
    _DEFINE_BUTTON PORTB_BASE, BIT12,	0,	5		//B.12
    _DEFINE_BUTTON PORTB_BASE, BIT13,	0,	15		//B.13
    _DEFINE_BUTTON PORTB_BASE, BIT14,	0,	25		//B.14
    
 //konec tabulky, musi byt zakonceno 0x0!
buttons_table_end:    
.word	0x0		
   
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="ubtnStart">
.align 2    
.global ubtnStart
.ent ubtnStart    
ubtnStart:
//nastavi bit v portu odpovidajicim tlacitku: TRIS=1(input), ANSEL=0(digital), CNPU=1(pull-up rezistor)    
//tlacitko se zapoji mezi pin a GND, stisknuti = LOG 0
//STATE nastavi na UP   
//definice tlacitek je v buttons_table 
//STATE 0xF0=UP, 0x0F=DOWN (mode 0,2), 0x0E=DOWN (mode 1)    

    li16    a0, 1
    bal	    setCanSleep
    li16    a0, 1
    bal	    setCanIdle
    
    //nastav TRIS bit=1 (input), ANSEL bit=0 digital
    la	    t0, buttons_table
    la	    s0, items		    //RAM tabulka items
    addiu   s1, zero, 0xF	    //button UP state (nastavi u vsech tlacitek)
    
1:  //cyklus pro vsechny tlacitka
    //nastav default hodnoty, nastav port
    lw	    t1, (t0)		//t1=base addr registru portu
    beqz    t1, 9f		//skok, konec buttons_table
    //nop
    
    lhu	    v0, 4(t0)		//load bit_mask

    //TRIS=1 (IN), ANSEL=0 (DIGITAL), PULL-UP
    //TRISbit=1
    sw32    v0, (TRIS_OFFSET + SET_OFFSET)(t1)
    //ANSELbit=0 
    sw32    v0, (ANSEL_OFFSET + CLR_OFFSET)(t1)
    //CNPU=1 (pull-up)
    sw32    v0, (CNPU_OFFSET + SET_OFFSET)(t1)
    //CNPD=0 (pull-down)
    sw32    v0, (CNPD_OFFSET + CLR_OFFSET)(t1)    
    
    sb      s1, UP_STATE_OFF(s0)	    //UP_state=0xF  (button=UP)
    sb	    zero, DOWN_STATE_OFF(s0)	    //DOWN_state=0x0
    
    //next button
    addiu   t0, BUTTONS_TABLE_ISIZE
    addiu   s0, ITEM_SIZE	
	
    b16	    1b
9:    
    
    //doEvents
    la	    a0, ubtnGo
    bal	    doEventsL
    
.end ubtnStart  
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="ubtnGo">
.align 2    
.ent ubtnGo
ubtnGo:
//odesila event
//1. btn_down,      param0=1, param1=buttons_table[6]    - okamzik stisknuti
//2. btn_up,        param0=2, param1=buttons_table[6]	 - uvolneni po beznem stisknuti   
//3. btn_down_long, param0=3, param1=buttons_table[6]    - trva down delsi dobu (pouze jeden event, nikoliv opakovane) mod 0
//4. btn_up_long,   param0=4, param1=buttons_table[6]    - uvolneni po dlouhem stisknuti mod 0
//5. btn_repeat     param0=5, param1=buttons_table[6]    - opakovane down mod 1
    
//STATE 0xF0=UP, 0x0F=DOWN (mode 0,2), 0x0E=DOWN (mode 1)    
    
    la	    s0, items
    addiu   s1, zero, 0xF	    //hodnota potvrzeny stav
    la	    s2, buttons_table

next:
    lw	    t1, (s2)		    //t1=base addr registru portu
    beqz    t1, end		    //skok konec
    
    //zjisti akt. stav pinu do v0
    lw32    v0, PORT_OFFSET(t1)		    //load PORTx
    lhu	    v1, 4(s2)			    //bit mask
    and	    v0, v1			    //v0=stav bitu (v0=0x0 pin LOW)
    
    beqz    v0, pin_down		    //skok, pin=0-down
    
    //pin je up
    lbu	    t9, UP_STATE_OFF(s0)
    bne	    t9, s1, up_test_change	    //skok, potencialni zmena stavu
    
    //trva stav UP
    sb	    zero, DOWN_STATE_OFF(s0)	    //down_state=0
    b16	    loop
    
up_test_change:
    //mozna zmena stavu na UP
    //t9=up_state
    addiu   t9, 1
    sb	    t9, UP_STATE_OFF(s0)	    //save novy up_state
    bne	    t9, s1, loop		    //skok, zmena stavu zatim nenastala (up_state != 0xF)
    
    //zmena stavu na UP
    sb	    zero, DOWN_STATE_OFF(s0)	    //down_state=0
    lbu	    t9, LONG_COUNT_OFF(s0)
    beqz    t9, send_up			    //skok, pri down nenastalo long_down, bude odesilat up
    
    //send long_up
    li16    a0, UBTN_UP_LONG		    //btn up long
    lbu	    a1, 7(s2)			    //value z buttons_table
    move    a2, zero
    bal	    runEvent			    //meni a0-a3, v0, v1, t5-t9

    b16	    loop
    
send_up:
    //send up
    li16    a0, UBTN_UP			    //btn up long
    lbu	    a1, 7(s2)			    //value z buttons_table
    move    a2, zero
    bal	    runEvent			    //meni a0-a3, v0, v1, t5-t9

    b16	    loop
    
pin_down:
    lbu	    t9, DOWN_STATE_OFF(s0)
    bne	    t9, s1, down_test_change	    //skok, potencialni zmena stavu
    
    //trva stav down
    sb	    zero, UP_STATE_OFF(s0)	    //up_state=0
    
    lbu	    t9, 6(s2)			    //load mode
    bnez    t9, loop			    //skok, mode=1
    
    lw32    a0, REP_TIME_OFF(s0)	    //load repeatTime
    
    //a0=time (vraci rozdil od now [ms])
    bal	    compareTimeMs		    //meni t8, t9
    
    lbu	    s3, REP_COUNT_OFF(s0)	    //load repeatCount
    beqz    s3, rep_0			    //skok, repeatCount=0 	
    
    //repeatCount > 0
    addiu   v0, (-REPEAT_NEXT_MS)
    bltz    v0, test_long		    //skok, nevyprsel cas
    
send_rep:    
    //cas vyprsel, odeslat repeat
    li16    a0, UBTN_REPEAT		    //btn repeat
    lbu	    a1, 7(s2)			    //value z buttons_table    
    move    a2, s3			    //repeat count
    bal	    runEvent			    //meni a0-a3, v0, v1, t5-t9
    
    lw      t9, (time_ms)
    sw32    t9, REP_TIME_OFF(s0)	    //nastav now repeatTime
    addiu   s3, 1			    //repeatC++
    andi    s3, 0xFF			    //nuluj b8-b31	
    beqz    s3, test_long		    //skok, pokud doslo k preteceni repeatC

    sb	    s3, REP_COUNT_OFF(s0)	    //uloz repeatCount

    b16	    test_long
   
rep_0:
    //repeat_count=0, v0=pocet ms
    addiu   v0, (-REPEAT_FIRST_MS)
    bgez    v0, send_rep		    //skok, cas vyprsel
    
test_long:    
    //test cas long
    lw32    a0, LONG_TIME_OFF(s0)	    //load longTime
    
    //a0=time (vraci rozdil od now [ms])
    bal	    compareTimeMs		    //meni t8, t9
    
    addiu   v0, (-LONG_DOWN_MS)
    bltz    v0, loop			    //skok, cas jeste nevyprsel
    
    //cas vyprsel
    lbu	    s3, LONG_COUNT_OFF(s0)	    //load longCount
    li16    a0, UBTN_DOWN_LONG		    //btn long down
    lbu	    a1, 7(s2)			    //value z buttons_table    
    move    a2, s3			    //longCount
    bal	    runEvent			    //meni a0-a3, v0, v1, t5-t9
    
    lw      t9, (time_ms)
    sw32    t9, LONG_TIME_OFF(s0)	    //nastav now longTime
    addiu   s3, 1			    //longC++
    andi    s3, 0xFF			    //nuluj b8-b31
    beqz    s3, loop			    //skok, pokud doslo k preteceni longC
    sb	    s3, LONG_COUNT_OFF(s0)	    //uloz repeatCount

    b16	    loop
    
down_test_change:
    //mozna zmena stavu na down
    //t9=down_state
    addiu   t9, 1
    sb	    t9, DOWN_STATE_OFF(s0)	    //save novy down_state
    bne	    t9, s1, loop		    //skok, zmena stavu zatim nenastala (down_state != 0xF)

    //nastala zmena na down
    sb	    zero, UP_STATE_OFF(s0)	    //up_state=0
    lw      t9, (time_ms)		    //now	
    sw32    t9, REP_TIME_OFF(s0)	    //reteatTime=now
    sw32    t9, LONG_TIME_OFF(s0)	    //longTime=now
    sb	    zero, REP_COUNT_OFF(s0)	    //nuluj repeatCount
    sb	    zero, LONG_COUNT_OFF(s0)	    //nuluj longCount
    
    //send down
    li16    a0, UBTN_DOWN		    //btn down
    lbu	    a1, 7(s2)			    //value z buttons_table
    move    a2, zero
    bal	    runEvent			    //meni a0-a3, v0, v1, t5-t9
    
    //pokracuje loop
   
loop:    
    //next button
    addiu   s0, ITEM_SIZE		    //items ++
    addiu   s2, BUTTONS_TABLE_ISIZE	    //button_table ++

    b16	    next
    
end:    
    //doEvents
    la	    a0, ubtnGo
    bal	    doEventsL

.end ubtnGo  
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="ubtnRegEvent">
    
.align 2
.global	ubtnRegEvent
.ent ubtnRegEvent
ubtnRegEvent:
//a0=adresa fce
//a1=value min
//a2=value max    
    
    _PUSH   ra
    
    
    move    a3, a2		    //value max		    
    move    a2, a1		    //value min		    

    bal	    getProcID		    //meni v0, v1
    sll	    a1, v0, 8		    //procID do b8-b15
    ori	    a1, UBTN_EVENT_ID	 
    
        
    //a0=fn, 
    //a1=eventID, procID, (var2=0, var3=0)
    //a2=value min (var0) 
    //a3=value max (var1)
    bal	    addRegEvent		    //meni t9, v0, v1
    
    //v0=0 error, jinak adresa polozky
    
    _POP    ra
    jrc	    ra    
     
.end ubtnRegEvent 
    
//</editor-fold>
    
//<editor-fold defaultstate="collapsed" desc="runEvent">
    
.align 2    
.ent runEvent
runEvent:
//a0=udalost UBTN_UP/DOWN/LONG_DOWN/LONG_UP/REPEAT
//a1=button value z buttons_table 
//a2=cnt - index opakovani REPEAT a LONG_DOWN (0-0xFF) 
    
    
    //meni a0-a3, v0, v1, t5-t9
    _PUSH   ra
    
    move    t5, a0
    move    t6, a1
    move    t7, a2
    
    move    a0, zero			//hledani od zacatku regEventTable
    li	    a1, UBTN_EVENT_ID	

1:  
    //a0=adresa zacetku hledani
    //a1=eventID
    bal	    getRegEvent			//meni v0, v1, t9, (vraci v0=adresa, nebo 0x0 - polozka neni)
    beqz    v0, 2f			//skok, nenasel polozku
    
    //nasel polozku v regEventTable
    move    a0, v0			//adresa polozky v regEventTable
    
    //test, zda value (t6) je v rozsahu var0-var1
    lbu	    t8, RET_V0(a0)
    //first_reg, val_reg, addr
    _B_FLT t6, t8, 1b			//meni t9, skok pri value(t6) < min(t8)
    
    lbu	    t8, RET_V1(a0)
    //first_reg, val_reg, addr
    _B_FBT t6, t8, 1b			//meni t9, skok pri value(t6) > max(t8)
    
    //a0=adresa polozky v regEventTable (pouzije z ni procID a fn addr)
    //a1=p0
    //a2=p2
    //a3=p3
    move    a1, t5
    move    a2, t6
    move    a3, t7
    bal	    raiseEvent			//meni t9, v0, v1
    
    //a1 bylo zmemeno, dosadit eventID
    li	    a1, UBTN_EVENT_ID	
    b16	    1b
    
2:  
    _POP    ra
    jrc	    ra
    
.end runEvent   
    
//</editor-fold>
 
#endif	//UBTN   