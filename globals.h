//obsahuje globalni fce OS, vsechny soubory ".c" by mely inkludovat globals.h
//protoze tento soubor dale inkluduje def.h a structures.h
//asm soubory ".S" inkluduji jen def.h, pripadne asm_macro.S

#include "asm.h"
#include "def.h"
#include "struct.h"

#ifdef GRAPH
#include graphics_struct.h
#endif

//default app start param, hodnoty nastavuje fce main()
APP_START_PARAM defaultAppStartParam;                   

//test LED
volatile void* LED1_BASE=NULL;
uint LED1_BIT=0;
volatile void* LED2_BASE=NULL;
uint LED2_BIT=0;
volatile void* LED3_BASE=NULL;
uint LED3_BIT=0;
volatile void* LED4_BASE=NULL;
uint LED4_BIT=0;

//debug stacku
int checkStackSpaceValue=0x7FFFFFFF;

