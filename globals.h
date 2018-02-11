//obsahuje globalni fce OS, vsechny soubory ".c" by mely inkludovat globals.h
//protoze tento soubor dale inkluduje def.h a structures.h
//asm soubory ".S" inkluduji jen def.h, pripadne asm_macro.S

#include "asm.h"
#include "def.h"
#include "structures.h"

APP_START_PARAM defaultAppStartParam;                   //default app start param

uint LED1_BASE=NULL;
uint LED1_BIT=0;
uint LED2_BASE=NULL;
uint LED2_BIT=0;
uint LED3_BASE=NULL;
uint LED3_BIT=0;
uint LED4_BASE=NULL;
uint LED4_BIT=0;

void globalsBeforeProcess();
void globalsAfterProcess();