//obsahuje globalni fce OS, vsechny soubory ".c" by mely inkludovat globals.h
//protoze tento soubor dale inkluduje def.h a structures.h
//asm soubory ".S" inkluduji jen def.h, pripadne asm_macro.S

#include "asm.h"
#include "def.h"
#include "struct.h"


//@default_app start param, hodnoty nastavuje fce main()
APP_START_PARAM defaultAppStartParam;                   

//@test_LED
PIN_INFO LED1;
PIN_INFO LED2;
PIN_INFO LED3;
PIN_INFO LED4;

//debug stacku
int checkStackSpaceValue=0x7FFFFFFF;                    //max. int

#ifdef NETCOM_UART
    extern NETCOM_DATAOUT* netcomDataOut[];
    extern NETCOM_DATAIN* netcomDataSet[];
    extern NETCOM_DATAIN* netcomDataGet[];
    extern void netcomSetData(NETCOM_DATAOUT* data);
    extern void netcomGetData(NETCOM_DATAOUT* data);
    
    char netcom_devId=0;    
    int netcomChkError=0;
    int netcomSendDataError=0;
    int netcomBusError=0;
    int netcomTxBytes=0;
    int netcomRxBytes=0;
    int netcomTxCTRL=0;
    int netcomRxCTRL=0;
#endif 

#ifdef USE_GRAPHICS
    //@all_displays

    //defined in graphics.c
    extern GRAPHICS graphics;
    extern DISPLAY sysDisplay;

    //Sets information about the font
    //@param head FONT_HEAD*, address of font head
    //@param src IMAGE_SRC*, address of structure, that represents the font data
    extern void setFontSrc(const void* head, void* src);
    
    //Sets information about the image
    //@param head IMAGE_HEAD*, address of image head
    //@param src IMAGE_SRC*, address of structure, that represents the image data
    extern void setImageSrc(const void* head, void* src);

    #if defined USE_DISP9341
        //pouze barevne (ILI9341)

        //Return short (16-bit) representation of RGB color
        //@param red 5-bit value, 0-31
        //@param green 6-bit value, 0-63
        //@param blue 5-bit value, 0-31
        extern short RGB16(int red, int green, int blue);
    
        //Sets color map
        //@param src Address of IMAGE_SRC structure, that represents image/font data
        //@param map Color map
        extern void setColorMap(void* src, const short* map);
                      
    #endif  //USE_DISP9341

// <editor-fold defaultstate="collapsed" desc="Fonts">
#ifdef FONT_ARIAL_18
    extern IMAGE_SRC font_arial_18;
#endif
#ifdef FONT_DLG_22
    extern IMAGE_SRC font_dlg_22;
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
    
#ifdef SPFONT_FIXED_8
    extern IMAGE_SRC spfont_fixed_8;
#endif    

#ifdef SPFONT_FIXED_16
    extern IMAGE_SRC spfont_fixed_16;
#endif       

#ifdef SPFONT_FIXED_32
    extern IMAGE_SRC spfont_fixed_32;
#endif       
    
#ifdef SPFONT_FIXED_48
    extern IMAGE_SRC spfont_fixed_48;
#endif       
    
#ifdef SPFONT_ROCK_32
    extern IMAGE_SRC spfont_rock_32;
#endif       
    
#ifdef SPFONT_ROCK_48
    extern IMAGE_SRC spfont_rock_48;
#endif
    
// </editor-fold>
    
#endif    