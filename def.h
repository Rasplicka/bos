//obsahuje globalni definice pro C/C++ i ASM

//#define PIC32MM0256
//#define PIC32MM

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

#define PIC32MM0256
#define TEST_BOARD_BOS1

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
//#define     SIMULATOR

#define		RAM_BASE                    0x80000000
#define     SRS_STACK_SIZE              512         //velikost oblasti zasobniku pro interrupt level 1(MM), 1-7(MZ)
#define     STACK_CHECK_VALUE           0xF010E020

#define     SAFE_PROCESS                          //povoluje ochranu prepinani procesu, kdy CPU timer spusti interrupt, bezi-li proces prilis dlouho
#define     SAFE_MODE_TIME_LIMIT_VALUE  0xFFFFF     //hodnota do CP0_COMPARE, pri prekroceni nastave chyba (interrupt CPU_TIMER)
#define     ENABLE_CHECK_STACK_OVERFLOW           //povoluje kontrolovat stack overflow

//watch dog timer
#define     WATCHDOG_TIMER

#ifdef PIC32MM0256
//------------------------------------------------------------------------------
//BOS used RAM cca 10kB

#define		RAM_SIZE                32*1024 
#define     STACK_SIZE              8*1024
#define     SYSPROC_STACK_SIZE      1024            //velikost zasobniku systemProcess
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
#define     SYSPROC_STACK_SIZE      1024            //velikost zasobniku systemProcess
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

// <editor-fold defaultstate="collapsed" desc="Error">

#define     ERR_CODE_TIME_LIMIT_EXCEED          1
#define     ERR_CODE_GENERAL_EXCEPTION          2
#define     ERR_CODE_TRAP                       3
#define     ERR_STACK_OVERFLOW                  4

#define     ON_ERROR_RESET_SYSTEM               0
#define     ON_ERROR_RESET_PROCESS              1
#define     ON_ERROR_REMOVE_PROCESS             2

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="UART">
//@UARTNETCOM
#define             USE_UARTNETCOM

//@NETCOM_param
#define             NETCOM_DEVID            1   //ID modulu           
#define             NETCOM_MAXID            8   //max. ID na sbernici

#define             NETCOM_DATAOUT_CAPA     8   //8 polozek dataOut
#define             NETCOM_DATAOUT_ISIZE    4

#define             NETCOM_DATAIN_CAPA      4   //8 polozek dataIn, pipe 1-8
#define             NETCOM_DATAIN_ISIZE     4

#define             NETCOM_BUFFER_PIPE_LEN  32  //velikost pipe bufferu 



//</editor-fold>


// <editor-fold defaultstate="collapsed" desc="DISPLAY, GRAPHICS, TOUCHPAD">

/*@graphics*/
#define             USE_GRAPHICS                    //pouzije graphics

/*@display_ili9341*/
#define             USE_DISP9341                    //pouzije driver displeje
/*@touchpad_xpt2046*/
#define             USE_TOUCHPAD_XPT2046           //aktivuje modul touchpadu, typ XPT2046

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
#define     SPI2_USE                               //aktivuje SPI2 (fce driveru pro SPI2)
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
#define     CN_EVENT_ID             15
#define     TOUCHPAD_EVENT_ID       16

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
#define     CNCON_OFFSET    0x70                        //change notification control
#define     CNEN_OFFSET     0x80                        //change notification enable    //MZ
#define     CNEN0_OFFSET    0x80                        //change notification enable    //MM
#define     CNSTAT_OFFSET   0x90                        ////change notification status
#define     CNEN1_OFFSET    0xA0                        //change notification enable    //MM    
#define     CNF_OFFSET      0xB0                        //change notification IF        //MM

#define	    CLR_OFFSET	    0x4                         //offset CLR registru
#define	    SET_OFFSET	    0x8                         //offset SET registru
#define	    INV_OFFSET	    0xC                         //offset INV registru

#define     CNPU_SET        CNPU_OFFSET + SET_OFFSET    //offset reg. pro SET pull-up
#define     CNPU_CLR        CNPU_OFFSET + CLR_OFFSET    //offset reg. pro CLR pull-up
#define     CNPD_SET        CNPD_OFFSET + SET_OFFSET    //offset reg. pro SET pull-down
#define     CNPD_CLR        CNPD_OFFSET + CLR_OFFSET    //offset reg. pro CLR pull-down
#define     ODC_SET         ODC_OFFSET + SET_OFFSET     //offset reg. pro SET open drain
#define     ODC_CLR         ODC_OFFSET + CLR_OFFSET     //offset reg. pro CLR open drain

#ifdef PIC32MM0256

#define     PORTA_BASE      0xBF802BB0
#define     PORTB_BASE      0xBF802CB0
#define     PORTC_BASE      0xBF802DB0
#define     PORTD_BASE      0xBF802EB0

//RP IN
#define     RP1_IN          1
#define     RP2_IN          2
#define     RP3_IN          3
#define     RP4_IN          4
#define     RP5_IN          5
#define     RP6_IN          6
#define     RP7_IN          7
#define     RP8_IN          8
#define     RP9_IN          9
#define     RP10_IN         10
#define     RP11_IN         11
#define     RP12_IN         12
#define     RP13_IN         13
#define     RP14_IN         14
#define     RP15_IN         15
#define     RP16_IN         16
#define     RP17_IN         17
#define     RP18_IN         18
#define     RP19_IN         19
#define     RP20_IN         20
#define     RP21_IN         21
#define     RP22_IN         22
#define     RP23_IN         23
#define     RP24_IN         24

#define     INT4_IO         RPINR1bits.INT4R
#define     ICM1_IO         RPINR2bits.ICM1R
#define     ICM2_IO         RPINR2bits.ICM2R
#define     ICM3_IO         RPINR3bits.ICM3R
#define     ICM4_IO         RPINR3bits.ICM4R
#define     OCFA_IO         RPINR5bits.OCFAR
#define     OCFB_IO         RPINR5bits.OCFBR
#define     TCKIA_IO        RPINR6bits.TCKIAR
#define     TCKIB_IO        RPINR6bits.TCKIBR
#define     ICM5_IO         RPINR7bits.ICM5R
#define     ICM6_IO         RPINR7bits.ICM6R
#define     ICM7_IO         RPINR7bits.ICM7R
#define     ICM8_IO         RPINR7bits.ICM8R
#define     ICM9_IO         RPINR8bits.ICM9R
#define     U3RX_IO         RPINR8bits.U3RXR
#define     U2RX_IO         RPINR9bits.U2RXR
#define     U2CTS_IO        RPINR9bits.U2CTSR
#define     U3CTS_IO        RPINR10bits.U3CTSR
#define     SDI2_IO         RPINR11bits.SDI2R
#define     SCK2IN_IO       RPINR11bits.SCK2INR
#define     SS2IN_IO        RPINR11bits.SS2INR
#define     CLCINA_IO       RPINR12bits.CLCINAR
#define     CLCINB_IO       RPINR12bits.CLCINBR

//RP OUT
#define     DEFAULT_IO      0
#define     C1OUT_IO        1
#define     C2OUT_IO        3
#define     C3OUT_IO        3
#define     U2TX_IO         4
#define     U2RST_IO        5
#define     U3TX_IO         6
#define     U3RST_IO        7
#define     SDO2_IO         8
#define     SCK2OUT_IO      9
#define     SS2OUT_IO       10
#define     OCM4_IO         11
#define     OCM5_IO         12
#define     OCM6_IO         13
#define     OCM7_IO         14
#define     OCM8_IO         15
#define     OCM9_IO         16
#define     CLC1OUT_IO      17
#define     CLC2OUT_IO      18
#define     CLC3OUT_IO      19
#define     CLC4OUT_IO      20

#define     RP1_OUT         RPOR0bits.RP1R
#define     RP2_OUT         RPOR0bits.RP2R
#define     RP3_OUT         RPOR0bits.RP3R
#define     RP4_OUT         RPOR0bits.RP4R
#define     RP5_OUT         RPOR1bits.RP5R
#define     RP6_OUT         RPOR1bits.RP6R
#define     RP7_OUT         RPOR1bits.RP7R
#define     RP8_OUT         RPOR1bits.RP8R
#define     RP9_OUT         RPOR2bits.RP9R
#define     RP10_OUT        RPOR2bits.RP10R
#define     RP11_OUT        RPOR2bits.RP11R
#define     RP12_OUT        RPOR2bits.RP12R
#define     RP13_OUT        RPOR3bits.RP13R
#define     RP14_OUT        RPOR3bits.RP14R
#define     RP15_OUT        RPOR3bits.RP15R
#define     RP16_OUT        RPOR3bits.RP16R
#define     RP17_OUT        RPOR4bits.RP17R
#define     RP18_OUT        RPOR4bits.RP18R
#define     RP19_OUT        RPOR4bits.RP19R
#define     RP20_OUT        RPOR4bits.RP20R
#define     RP21_OUT        RPOR5bits.RP21R
#define     RP22_OUT        RPOR5bits.RP22R
#define     RP23_OUT        RPOR5bits.RP23R
#define     RP24_OUT        RPOR5bits.RP24R

#endif

#ifdef PIC32MM0064

#define     PORTA_BASE      0xBF802600
#define     PORTB_BASE      0xBF802700
#define     PORTC_BASE      0xBF802800

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

//port Change Notification
#define     PORT_CN
#ifdef PORT_CN
    #define     PORT_CNA
    #define     PORT_CNB
    #define     PORT_CNC
    #define     PORT_CND
    #define     PORT_CNE
    #define     PORT_CNF
    #define     PORT_CNG
    #define     PORT_CNH
    #define     PORT_CNJ
    #define     PORT_CNK
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
//#define     UBTN

#define     UBTN_DOWN       1
#define     UBTN_UP         2
#define     UBTN_DOWN_LONG  3
#define     UBTN_UP_LONG    4
#define     UBTN_REPEAT     5
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="dispay">

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

