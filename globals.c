#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

/*
 * Author Jiri Rasplicka, 2017, (all rights reserved)
 * Version 1.0       
 * BOS core
 * This file is shared to the PIC32MM, PIC32MZ
 * 
 */

void globalsBeforeProcess()
{
    //tato fce se vola jako inicializace systemu, tesne pred prvnim spustenim (registraci) procesu

}

void globalsAfterProcess()
{
    //tato fce se vola jako inicializace systemu, tesne po registraci vsech procesu, ale pred prvnim spustenim procesu
}