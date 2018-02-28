//obsahuje globalni definice pro C/C++ i ASM

#define PIC32MM0256
//#define PIC32MM

// <editor-fold defaultstate="collapsed" desc="CPU">
#ifdef PIC32MZ

//XTAL 24MHz
#define     CFG_POSC_XTAL               24
#define     CFG_CLKSYS_POSC

//XTAL 12MHz
//#define     CFG_POSC_XTAL               12
//#define     CFG_CLKSYS_POSC

//FRC 8MHz
//#define     CFG_CLKSYS_FRC


//XTAL SOSC 37.768kHz (RTC, timer1)
//CFG_SOSC_ENABLE povoli sec. oscilator, pokud je pripojen XTAL 32.768kHz
//jinak muze byt priveden ext. signal 32.768kHz na SOSCO
//pokud ma RTC tento signal pouzit, musi byt definovano SOSC_CLK_TO_RTC
//jinak pouzije LPRC (timer1 vzdy pouziva PBCLK3)
//#define     CFG_SOSC_ENABLE                                   //SOSCO + SOSCI je pripojen XTAL 32.768kHz      
//#define     SOSC_CLK_TO_RTC                                   //na vstupu SOSCO je 32.768kHz, pouzije ho RTC         

//watch dog timer
#define     WATCHDOG_TIMER

#endif
//#define PIC32MM0064
//#define PIC32MM0064_28pin
//#define PIC32MM0256
//#define PIC32MM0256_36pin


//watch dog timer
//#define     WATCHDOG_TIMER

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="System">
//#define     SIMULATOR

#define		RAM_BASE                    0x80000000
#define     SRS_STACK_SIZE              512         //velikost oblasti zasobniku pro interrupt level 1(MM), 1-7(MZ)
#define     STACK_CHECK_VALUE           0xF010E020

#define     SAFE_PROCESS                            //povoluje ochranu prepinani procesu, kdy CPU timer spusti interrupt, bezi-li proces prilis dlouho
#define     SAFE_MODE_TIME_LIMIT_VALUE  0xFFFFF     //hodnota do CP0_COMPARE, pri prekroceni nastave chyba (interrupt CPU_TIMER)
#define     ENABLE_CHECK_STACK_OVERFLOW             //povoluje kontrolovat stack overflow


#ifdef PIC32MM0256
//------------------------------------------------------------------------------
//BOS used RAM cca 10kB

#define		RAM_SIZE                32*1024 
#define     STACK_SIZE              8*1024
#define		REG_EVENT_TABLE_CAPA    20              //max. pocet registraci udalosti (polozka 16 bytes) 320 B
#define		EVENT_CACHE_CAPA        20              //velikost cache udalosti        (polozka 20 bytes) 400 B
#define     PROC_T_CAPA             8               //kapacita proc_t (polozka 96 Bytes) 768 B

#define     TIMER1_INTERVAL         10              //interval ms
//------------------------------------------------------------------------------
#endif

#ifdef PIC32MM0064
//------------------------------------------------------------------------------
//BOS used RAM cca 4kB

#define		RAM_SIZE                8*1024
#define     STACK_SIZE              3*1024
#define		REG_EVENT_TABLE_CAPA    16              //max. pocet registraci udalosti (polozka 16 bytes) 256 B
#define		EVENT_CACHE_CAPA        12              //velikost cache udalosti        (polozka 20 bytes) 240 B
#define     PROC_T_CAPA             4               //kapacita proc_t (polozka 96 Bytes) 384 B

#define     TIMER1_INTERVAL         10              //interval ms
//------------------------------------------------------------------------------
#endif

#ifdef PIC32MZ
//------------------------------------------------------------------------------      
//BOS used RAM cca 68kB (app. stack ~4kB)

#define     EBASE                   0x9D000000
#define		RAM_SIZE                512*1024        //512kB
#define     STACK_SIZE              64*1024         //64kB (SRS stacks 7*512 = 3.5kB)
#define		REG_EVENT_TABLE_CAPA    64              //max. pocet registraci udalosti (polozka 16 bytes) 1024 B
#define		EVENT_CACHE_CAPA        64              //velikost cache udalosti        (polozka 20 bytes) 1280 B
#define     PROC_T_CAPA             16              //kapacita proc_t (polozka 96 Bytes) 1.5kB

#define     TIMER1_INTERVAL         1               //interval ms
//------------------------------------------------------------------------------
#endif


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Error">
#define     ERR_CODE_TIME_LIMIT_EXCEED          1
#define     ERR_CODE_GENERAL_EXCEPTION          2
#define     ERR_CODE_TRAP                       3
#define     ERR_STACK_OVERFLOW                  4

#define     ON_ERROR_RESET_SYSTEM               0
#define     ON_ERROR_RESET_PROCESS              1
#define     ON_ERROR_REMOVE_PROCESS             2

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DISPLAY, GRAPHICS, TOUCHPAD">
#define             USE_GRAPHICS                    //pouzije graphics

//ili9341
//#define             USE_SYSTEMFONT_ARIAL            //pouzije system font arial18 - std font
//#define             SYSDISPLAY_9341SPI              //definuje typ systemoveho displeje (ili9341)
//#define             USE_DISP9341                    //pouzije driver displeje
//ssd1306
#define             USE_SYSTEMFONT_FIXEDx           //pouzije system font fixed16 - nestandardni font (pro ssd1306)
#define             SYSDISPLAY_1306SPI              //definuje typ systemoveho displeje (ssd1306)
#define             USE_DISP1306                    //pouzije driver displeje

#define             USE_TOUCHPAD                    //aktivuje vyssi fce touchpadu (nezavisle na konkretnim typu)
#define             TOUCHPAD_XPT2046_INIT           //aktivuje modul touchpadu, typ XPT2046
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Periph driver (SPI, I2C, ...)">
//SPI ---------------------------------------------------
//driver je pouzit, pokud pouziva alespon jeden SPI kanal
//#define     SPI1_INIT                              //pouziva SPI1 (fce driveru pro SPI1)
#define     SPI2_INIT                                //aktivuje SPI2 (fce driveru pro SPI2)
//#define     SPI3_INIT                              //pouziva SPI3 (fce driveru pro SPI3)


//I2C -----------------------------------------------------
//rada MM ma I2C pouze ve verzi PIC32MM0256 (0064 nikoliv)
#define I2C1_INIT                                   //aktivuje I2C driver
//#define I2C2_INIT
//#define I2C3_INIT
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Proc table">
#define     PROC_T_ISIZE    96          //velikost polozky v proc_t

//cislovani pro pouziti v c/c++ (index word), v asm se musi vynasobit 4
//
#define     TH_T_ID         0    
#define     TH_T_START_ADDR 1       //pouzije se pro restart procesu
#define     TH_T_START_SP   2       //pouzije se pro restart procesu
#define     TH_T_STACK_BASE 3       //zde je check value

#define     TH_T_A0			4      //offset a0
#define     TH_T_A1			5
#define     TH_T_A2			6
#define     TH_T_A3			7      //offset a3

#define	    TH_T_V0			8
#define	    TH_T_V1			9

#define	    TH_T_S0			10	
#define	    TH_T_S2			12    
#define	    TH_T_S4			14
#define	    TH_T_S6			16

#define	    TH_T_GP			18
#define	    TH_T_SP			19
#define	    TH_T_FP			20
#define	    TH_T_RA			21

#define     TH_T_LIMIT      22
#define     TH_T_EXIT       23
//#define	    TH_T_LO			30
//#define	    TH_T_HI			31
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Events">
//regEventTable a eventCache
#define		REG_EVENT_TABLE_ISIZE	16
#define		RET_EVENTID		0
#define		RET_PROCID		1
#define		RET_V2			2
#define		RET_V3			3
#define		RET_V0			4
#define		RET_V1			8
#define		RET_FN			12

#define		EVENT_CACHE_ISIZE	20
#define		EVC_PROCID		0    
#define		EVC_P0			4
#define		EVC_P1			8
#define		EVC_P2			12
#define		EVC_FN			16  

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="EventID">
//ID event
#define     TIMER1_EVENT_ID		    1
#define     UBTN_EVENT_ID           5
#define     RTC_TALARM_ID           10

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Font (SSD1306)">
#define     FONT1306_MID    1
#define     FONT1306_BIG    2
#define     FILL1306        96
#define     IMAGE1306       97
#define     ZERO1306        98
#define     COMMAND1306     99
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="BIT definition">
//*******************************
//BIT definition
//*******************************
#define     BIT0           0b0000000000000001
#define     BIT1           0b0000000000000010
#define     BIT2           0b0000000000000100
#define     BIT3           0b0000000000001000
#define     BIT4           0b0000000000010000
#define     BIT5           0b0000000000100000
#define     BIT6           0b0000000001000000
#define     BIT7           0b0000000010000000
#define     BIT8           0b0000000100000000
#define     BIT9           0b0000001000000000
#define     BIT10          0b0000010000000000
#define     BIT11          0b0000100000000000
#define     BIT12          0b0001000000000000
#define     BIT13          0b0010000000000000
#define     BIT14          0b0100000000000000
#define     BIT15          0b1000000000000000
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="IO port">
#define	    ANSEL_OFFSET    0x0                         //offset ANSEL reg
#define	    TRIS_OFFSET	    0x10
#define	    PORT_OFFSET	    0x20 
#define     LAT_OFFSET      0x30                        //latch
#define     ODC_OFFSET      0x40                        //open drain
#define	    CNPU_OFFSET	    0x50                        //pull-up
#define	    CNPD_OFFSET	    0x60                        //pull-down

#define	    CLR_OFFSET	    0x4                         //offset CLR registru
#define	    SET_OFFSET	    0x8                         //offset SET registru
#define	    INV_OFFSET	    0xC                         //offset INV registru

#define     CNPU_SET        CNPU_OFFSET + SET_OFFSET    //offset reg. pro SET pull-up
#define     CNPU_CLR        CNPU_OFFSET + CLR_OFFSET    //offset reg. pro CLR pull-up
#define     CNPD_SET        CNPD_OFFSET + SET_OFFSET    //offset reg. pro SET pull-down
#define     CNPD_CLR        CNPD_OFFSET + CLR_OFFSET    //offset reg. pro CLR pull-down
#define     ODC_SET         ODC_OFFSET + SET_OFFSET     //offset reg. pro SET open drain
#define     ODC_CLR         ODC_OFFSET + CLR_OFFSET     //offset reg. pro CLR open drain

#ifdef PIC32MM0064

#define     PORTA_BASE      0xBF802600
#define     PORTB_BASE      0xBF802700
#define     PORTC_BASE      0xBF802800

#endif

#ifdef PIC32MM0256

#define     PORTA_BASE      0xBF802BB0
#define     PORTB_BASE      0xBF802CB0
#define     PORTC_BASE      0xBF802DB0

#endif

#ifdef PIC32MZ

#define     PORTA_BASE      0xBF860000
#define     PORTB_BASE      0xBF860100
#define     PORTC_BASE      0xBF860200
#define     PORTD_BASE      0xBF860300
#define     PORTE_BASE      0xBF860400
#define     PORTF_BASE      0xBF860500
#define     PORTG_BASE      0xBF860600
#define     PORTH_BASE      0xBF860700
#define     PORTJ_BASE      0xBF860800
#define     PORTK_BASE      0xBF860900

#endif

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="string/chars">
#define     _CHAR_SPACE     32
#define     _CHAR_MINUS     0x2D
// </editor-fold>


//*******************************************
//drivers
//*******************************************

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
#define     UBTN

#define     UBTN_DOWN       1
#define     UBTN_UP         2
#define     UBTN_DOWN_LONG  3
#define     UBTN_UP_LONG    4
#define     UBTN_REPEAT     5
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="disp1306">
#define     DISP1306
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="PWM">
#define     PWM_INIT                              //rizeni vykonu, pouziva CCP moduly
//#define     CCP_PWM_COUNT       2                 //pocet pouzitych CCP modulu pro rizeni vykonu PWM
//#define     CCP1_PWM                              //CCP1 pouzita pro pwm
//#define     CCP2_PWM                              //CCP2 pouzita pro pwm
//#define     CCP3_PWM                              //CCP3 pouzita pro pwm
//#define     CCP4_PWM
//#define     PWM_SOFT                              //aktivuje fci soft zmeny vykonu

#define     CCP_PWM_TABLE_ISIZE 20                  //velikost polozky
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="USB">
#define     USB_DEVICE_INIT

// </editor-fold>

#define     TEST_DRIVER_INIT
#define     TEST_DRIVER_VERSION     2

