//obsahuje globalni fce OS, vsechny soubory ".c" by mely inkludovat globals.h
//protoze tento soubor dale inkluduje def.h a structures.h
//asm soubory ".S" inkluduji jen def.h, pripadne asm_macro.S

#include "asm.h"
#include "def.h"
#include "structures.h"

#ifdef GRAPH
#include graphics_struct.h
#endif

APP_START_PARAM defaultAppStartParam;                   //default app start param

volatile void* LED1_BASE=NULL;
uint LED1_BIT=0;
volatile void* LED2_BASE=NULL;
uint LED2_BIT=0;
volatile void* LED3_BASE=NULL;
uint LED3_BIT=0;
volatile void* LED4_BASE=NULL;
uint LED4_BIT=0;

int checkStackSpaceValue=0x7FFFFFFF;

