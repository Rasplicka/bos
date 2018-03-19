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

#ifdef USE_GRAPHICS

//extern GRAPHICS;

extern GRAPHICS graphics;
extern DISPLAY sysDisplay;

// <editor-fold defaultstate="collapsed" desc="Fonts">
#ifdef FONT_ARIAL_18
    extern IMAGE_SRC font_arial_18;
#endif
#ifdef FONT_DLG_18
    extern IMAGE_SRC font_dlg_18;
#endif    
#ifdef FONT_CONSOLAS_16
    extern IMAGE_SRC font_consolas_16;
#endif    
#ifdef FONT_CONSOLAS_20
    extern IMAGE_SRC font_consolas_20;
#endif      
#ifdef FONT_CONSOLAS_28
    extern IMAGE_SRC font_consolas_28;
#endif      
#ifdef FONT_CONSOLAS_36
    extern IMAGE_SRC font_consolas_36;
#endif      
#ifdef FONT_YGM_16
    extern IMAGE_SRC font_ygm_16;
#endif
#ifdef FONT_YGM_20
    extern IMAGE_SRC font_ygm_20;
#endif
#ifdef FONT_YGM_28
    extern IMAGE_SRC font_ygm_28;
#endif
#ifdef FONT_YGM_36
    extern IMAGE_SRC font_ygm_36;
#endif
#ifdef FONT_YGM_46
    extern IMAGE_SRC font_ygm_46;
#endif    
#ifdef FONT_YGM_78
    extern IMAGE_SRC font_ygm_78;
#endif    

#ifdef FONT_TWCEN_18
    extern IMAGE_SRC font_twcen_18;
#endif      
#ifdef FONT_TWCEN_22
    extern IMAGE_SRC font_twcen_22;
#endif      
#ifdef FONT_TWCEN_28
    extern IMAGE_SRC font_twcen_28;
#endif      
#ifdef FONT_TWCEN_36
    extern IMAGE_SRC font_twcen_36;
#endif      
#ifdef FONT_TWCEN_46
    extern IMAGE_SRC font_twcen_46;
#endif      
#ifdef FONT_TWCEN_80
    extern IMAGE_SRC font_twcen_80;
#endif      
    
// </editor-fold>

    
#endif    