#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "periph.h"
#include "def.h"
#include "asm.h"



void interrupt_Init()
{
#ifdef PIC32MZ

#endif

#ifdef PIC32MM
    
    //nepouziva c/c++ interrupt vector
    //fn.S obsahuje tabulku se skoky do interrupt fci, po skonceni provede ERET 
    //tabulka vektoru je nezavisla na EBASE (na rozdil od dokumentace), vzdy zacina na x09D00 0200
    //v tabulce je pouze skok do fn.S funkce (vlozi compilator prikazem napr.: extern void __attribute__((vector(21))) iVector21();)
    //vector spacing je 8 bytes, multivector
    //vsechny ILP pouzivaji SRS[1]
    //setSrsValue() zajisti, ze SRS[1] gp a sp budou nastaveny na pouziti v c/c++ kodu
    //SRS[1] ma vlastni zasobnik (512 Bytes), gp je nastaveno jako v SRS[0]
    
    
    //nastavi vychozi hodnoty GP a SP pro SRS[1]
    //var gp_value obsahuje hodnotu pro GP
    //vat sp_srs1_top obsahuje hodnotu pro SP
    setSrsValue();
    
    //Multivector, spacing 8 bytes, IPL 1-7 pouziva SRS[1]
    //Neobsahuje EI, STATUS.EI zustava 0 (interrupt disable)
    setInterrupt();
     
#endif        
        
}
