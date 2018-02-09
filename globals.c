#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

const APP_START_PARAM defaultAppStartParam = { ERROR_RESET_PROCESS, ERROR_RESET_PROCESS, SAFE_PROCESS_VALUE };


void globalsBeforeProcess()
{
    //tato fce se vola jako inicializace systemu, tesne pred prvnim spustenim (registraci) procesu
   
}

void globalsAfterProcess()
{
    //tato fce se vola jako inicializace systemu, tesne po registraci vsech procesu, ale pred prvnim spustenim procesu
}