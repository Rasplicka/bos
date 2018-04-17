//obsahuje globalni definice pro C/C++ i ASM

#define PIC32MM0064
//#define SIMULATOR

// <editor-fold defaultstate="collapsed" desc="CPU">

    // <editor-fold defaultstate="collapsed" desc="CPU MZ">
#ifdef PIC32MZ

//XTAL 24MHz
#define     CFG_POSC_XTAL               24
#define     CFG_CLKSYS_POSC

//XTAL 12MHz
//#define     CFG_POSC_XTAL               12
//#define     CFG_CLKSYS_POSC

//FRC 8MHz
//#define     CFG_CLKSYS_FRC

#endif
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="CPU MM">
#ifdef PIC32MM

//externi XTAL 8MHz
//#define     CFG_POSC_XTAL               8

//interni FRC 8MHz (muze byt pouzit USB modul v modu DEVICE bez krystalu)
#define     CFG_FRC

#endif
    // </editor-fold>


//CFG_SOSC_ENABLE povoli sec. oscilator, pokud je pripojen XTAL 32.768kHz
//#define     CFG_SOSC_ENABLE                                   //SOSCO + SOSCI je pripojen XTAL 32.768kHz      

//pokud ma RTC modul pouzit signal z SOSC (jinak pouziva LPRC)
//#define     SOSC_CLK_TO_RTC                                   //na vstupu SOSCO je 32.768kHz, pouzije ho RTC 

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="System">
#define		RAM_BASE                    0x80000000
#define     SRS_STACK_SIZE              512         //velikost oblasti zasobniku pro interrupt level 1(MM), 1-7(MZ)
#define     STACK_CHECK_VALUE           0xF010E020

//#define     SAFE_PROCESS                          //povoluje ochranu prepinani procesu, kdy CPU timer spusti interrupt, bezi-li proces prilis dlouho
#define     SAFE_MODE_TIME_LIMIT_VALUE  0xFFFFF     //hodnota do CP0_COMPARE, pri prekroceni nastave chyba (interrupt CPU_TIMER)
#define     ENABLE_CHECK_STACK_OVERFLOW           //povoluje kontrolovat stack overflow

//watch dog timer
//#define     WATCHDOG_TIMER

#ifdef PIC32MM0256
//------------------------------------------------------------------------------
//BOS used RAM cca 10kB

#define		RAM_SIZE                32*1024 
#define     STACK_SIZE              8*1024
#define     SYSPROC_STACK_SIZE      1024            //velikost zasobniku systemProcess
#define		REG_EVENT_TABLE_CAPA    20              //max. pocet registraci udalosti (polozka 16 bytes) 320 B
#define		EVENT_CACHE_CAPA        20              //velikost cache udalosti        (polozka 20 bytes) 400 B
#define     PROC_T_CAPA             8               //kapacita proc_t (polozka 96 Bytes) 768 B

#define     TIMER1_INTERVAL         1              //interval ms
//------------------------------------------------------------------------------
#endif

#ifdef PIC32MM0064
//------------------------------------------------------------------------------
//BOS used RAM cca 4kB

#define		RAM_SIZE                8*1024
#define     STACK_SIZE              4*1024
#define     SYSPROC_STACK_SIZE      512            //velikost zasobniku systemProcess
#define		REG_EVENT_TABLE_CAPA    12              //max. pocet registraci udalosti (polozka 16 bytes) 256 B
#define		EVENT_CACHE_CAPA        8              //velikost cache udalosti        (polozka 20 bytes) 240 B
#define     PROC_T_CAPA             4               //kapacita proc_t (polozka 96 Bytes) 384 B

#define     TIMER1_INTERVAL         1              //interval ms
//------------------------------------------------------------------------------
#endif

#ifdef PIC32MZ
//------------------------------------------------------------------------------      
//BOS used RAM cca 68kB (app. stack ~4kB)

#define     EBASE                   0x9D000000
#define		RAM_SIZE                512 * 1024      //512kB
#define     STACK_SIZE              64  * 1024      //64kB (SRS stacks 7*512 = 3.5kB)
#define     SYSPROC_STACK_SIZE      4   * 1024      //velikost zasobniku systemProcess
#define		REG_EVENT_TABLE_CAPA    64              //max. pocet registraci udalosti (polozka 16 bytes) 1024 B
#define		EVENT_CACHE_CAPA        64              //velikost cache udalosti        (polozka 20 bytes) 1280 B
#define     PROC_T_CAPA             16              //kapacita proc_t (polozka 96 Bytes) 1.5kB

#define     TIMER1_INTERVAL         1               //interval ms
//------------------------------------------------------------------------------
#endif


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="NETCOM">
//@UARTNETCOM

#define         NETCOM_UART             1   //UART1
#define         UART1_USE

#define         NETCOM_MAXID            8   //max. ID na sbernici
#define         NETCOM_CAN_BE_MASTER
//#define         NETCOM_ONE_MASTER

#define         NETCOM_DATAOUT_CAPA     8   //8 polozek dataOut
#define         NETCOM_DATASET_CAPA     4   //polozek v setData, (0-255)
#define         NETCOM_DATAGET_CAPA     4   //polozek v getData, (0-31)

//</editor-fold>

// <editor-fold defaultstate="collapsed" desc="DISPLAY, GRAPHICS, TOUCHPAD">

/*@graphics*/
//#define             USE_GRAPHICS                    //pouzije graphics

/*@display_ili9341*/
//#define             USE_DISP9341                    //pouzije driver displeje
/*@touchpad_xpt2046*/
//#define             USE_TOUCHPAD_XPT2046           //aktivuje modul touchpadu, typ XPT2046

/*@display_ssd1306*/
//#define             USE_DISP1306                    //pouzije driver displeje

/*@FONT (standard)*/
//#define             FONT_ARIAL_18           //color format 1bit, variable width, height 18px, ascii 0x20 - 0x7E
//#define             FONT_DLG_22             //color format 4bit, variable width, height 22px, ascii 0x20 - 0x7E + ceska diakritika (Velikost cca 14kB)
//#define             FONT_CONSOLAS_16        //color format 1bit, width 8px, height 16px, ascii 0x20 - 0x7E + ceska diakritika
//#define             FONT_CONSOLAS_20        //color format 1bit, width 12px, height 20px, ascii 0x20 - 0x7E + ceska diakritika  (velikost cca 4kB)
//#define             FONT_CONSOLAS_28        //color format 1bit, width 13px, height 28px, ascii 0x20 - 0x7E + ceska diakritika
//#define             FONT_CONSOLAS_36        //color format 1bit, width 20px, height 36px, ascii 0x20 - 0x7E + ceska diakritika  (velikost cca 12kB)
//#define             FONT_YGM_16             //color format 4bit, variable width, height 16px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 6kB)
//#define             FONT_YGM_20             //color format 4bit, variable width, height 20px, ascii 0x20 - 0x7E + ceska diakritika
//#define             FONT_YGM_28             //color format 4bit, variable width, height 28px, ascii 0x20 - 0x7E + ceska diakritika
//#define             FONT_YGM_36             //color format 4bit, variable width, height 36px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 20kB)
//#define             FONT_YGM_46             //color format 4bit, variable width, height 46px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 30kB)
//#define             FONT_YGM_78             //color format 4bit, variable width, height 78px, ascii ' ','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':' (velikost cca 10kB)
//#define             FONT_TWCEN_18           //color format 4bit, variable width, height 18px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 5kB)
#define             FONT_TWCEN_22           //color format 4bit, variable width, height 22px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 7kB)
//#define             FONT_TWCEN_28           //color format 4bit, variable width, height 28px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 11kB)
//#define             FONT_TWCEN_36           //color format 4bit, variable width, height 36px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 16kB)
//#define             FONT_TWCEN_46           //color format 4bit, variable width, height 46px, ascii 0x20 - 0x7E + ceska diakritika (velikost cca 25kB)
//#define             FONT_TWCEN_80           //color format 4bit, variable width, height 16px, ascii ' ','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':' (velikost cca 7kB)

/*@FONT (SSD1306)*/
//#define             SPFONT_FIXED_8          //1-bit, width 8px, height 8px, ascii 0x20 - 0x7E (1.2kB)
//#define             SPFONT_FIXED_16         //1-bit, width 8px, height 16px, ascii 0x20 - 0x7E + ceska diakritika (3kB)
//#define             SPFONT_FIXED_32         //1-bit, width 16px, height 32px, ascii 0x20 - 0x7E (7kB)
//#define             SPFONT_FIXED_48         //1-bit, width 22px, height 48px, ascii ' ','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':' (3kB)
//#define             SPFONT_ROCK_32          //1-bit, width 16px, height 32px, ascii 0x20 - 0x7E (7kB)
//#define             SPFONT_ROCK_48          //1-bit, width 22px, height 48px, ascii ' ','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':' (3kB)

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Periph driver (SPI, I2C, ...)">
//SPI ---------------------------------------------------
//driver je pouzit, pokud pouziva alespon jeden SPI kanal
//#define     SPI1_USE                               //pouziva SPI1 (fce driveru pro SPI1)
//#define     SPI2_USE                               //aktivuje SPI2 (fce driveru pro SPI2)
//#define     SPI3_USE                               //pouziva SPI3 (fce driveru pro SPI3)
//#define     SPI4_USE                               //aktivuje SPI2 (fce driveru pro SPI2)
//#define     SPI5_USE                               //aktivuje SPI2 (fce driveru pro SPI2)
//#define     SPI6_USE                               //aktivuje SPI2 (fce driveru pro SPI2)

//I2C -----------------------------------------------------
//rada MM ma I2C pouze ve verzi PIC32MM0256 (0064 nikoliv)
//#define I2C1_INIT                                   //aktivuje I2C driver
//#define I2C2_INIT
//#define I2C3_INIT
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ADC">
#define     ADC_SCAN_INIT

//vstupy, ktere jsou skenovany
#ifdef PIC32MM0064
    #define     AN8
    #define     AN9
    #define     AN10
#endif
#ifdef PIC32MM0256
    #define     AN4
    #define     AN5
    #define     AN6
    #define     AN7

    //#define     ANVDDCORE         //Vcc CORE (AN27)
    #define     ANVBG               //internal ref. 1.2V (AN28)
    //#define     ANVSS             //GND (AN29)
    //#define     ANVDD             //Vcc (AN30)
#endif

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="RTC">
#define     RTC
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ubtn">
//#define     UBTN
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="PWM">
//#define     PWM_INIT                              //rizeni vykonu, pouziva CCP moduly
//#define     CCP_PWM_COUNT       2                 //pocet pouzitych CCP modulu pro rizeni vykonu PWM
//#define     CCP1_PWM                              //CCP1 pouzita pro pwm
//#define     CCP2_PWM                              //CCP2 pouzita pro pwm
//#define     CCP3_PWM                              //CCP3 pouzita pro pwm
//#define     CCP4_PWM
//#define     PWM_SOFT                              //aktivuje fci soft zmeny vykonu

//#define     CCP_PWM_TABLE_ISIZE 20                  //velikost polozky
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="USB">
//#define     USB_DEVICE_INIT

// </editor-fold>

