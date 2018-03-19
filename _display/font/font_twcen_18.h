//!!! PRED #include "fontxxx.h" musi byt includovan display.h s definici IMAGE_HEAD 

//SPECIALNI ZNAKY (diakritika)
//Zlutouck� kun, cepice
//char txt[] = {'\xA6','l','u','\x9C','o','u','\x9F','k','�',' ','k','\xDE','\xE5',',',' ','\x9F','e','p','i','c','e'};
//0xD8 e s hackem
//0xDE u s krouzkem
//0x9F c s hackem
//0xD4 d s hackem
//0xE5 n s hackem
//0xFE r s hackem
//0xE7 s s hackem
//0x9C t s hackem
//0xA7 z s hackem
//0xAC C s hackem
//0xD2 D s hackem
//0xD5 N s hackem
//0xFC R s hackem
//0xE6 S s hackem
//0x9B T s hackem
//0xA6 Z s hackem

//znaky ktere jsou soucasti znakove sady 128-255
//�(0xE1), �(0xE9), �(0xED), �(0xF3), �(0xFA), �(0xFD)
//�(0xC1), �(0xC9), �(0xCD), �(0xD3), �(0xDA), �(0xDD)

//fixed font, fileID = 0x1
//font s pevnou velikosti znaku, vsechny znaky maji stejnou velikost
//musi obsahovat vsechny znaky mezi firstAsciiCode a firstVarAscii
//v head musi byt udaj itemSize
//adresa znaku se vypocita z poradi a velikosti
//znaky s ascii code >= firstVarAscii se vyhledaji
//soubor nemuze byt komprimovan

//Variable font, fileID=0x2
//font s promennou velikosti (sirkou) znaku, kazdy znak muze mit jiny pocet bytes
//znak se hleda prochazenim celeho souboru od zacatku
//soubor muze byt komprimovan
//nemusi obsahovat vsechny znaky, a nemusi byt razeny podle ascii code

//Zdroj: Tw Cen MT Condensed

const FONT_HEAD font_twcen18 __section(".rodata") __attribute__((aligned(4))) = {
    //pouze cisla: +,-./0123456789: 
    0x2,                //fileID
    0x4,                //format
    8,                  //blockSize
    1,                  //compresion
    0,                  //width
    18,                 //Height 
    0,                  //itemSize
    0x20,               //firstAscii code
    0x20,               //firstVarAscii
    0,                  //style
    14,                 //lineOffset
    32,                 //dataOffset
    0,                  //dummy int
    "twcen18",       //name
};
const char font_twcen18_data[] __section(".rodata") ={
    //font data
    //ascii, width, data size16 (data + 4)

    //space 
    0x20, 3, 0x6, 0x0,
    0x1b, 0x00,
    //!
    0x21, 3, 0x19, 0x0,
    0x06, 0x00, 0xf4, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x02, 0x00, 0xff, 0xe4, 0x06, 0x00,
    0x0,
    //"
    0x22, 6, 0x15, 0x0,
    0x09, 0x00, 0xf4, 0x3f, 0x94, 0xf4, 0x3f, 0x94, 0xf4, 0x0e, 0x74, 0xf4, 0x0c, 0x43, 0xe2, 0x21, 0x00,
    0x0,
    //#
    0x23, 11, 0x45, 0x0,
    0x12, 0x00, 0xf7, 0x3c, 0x10, 0x0c, 0x40, 0x00, 0x04, 0xc0, 0x00, 0xd2, 0x02, 0x00, 0xfb, 0x79, 0x00, 0x3c, 0x10, 0x3f, 0x04, 0xff, 0xfb, 0x40, 0x00, 0xd2, 0x00, 0xa7, 0x02, 0x00, 0xf5, 0x3c, 0x10, 0x0c, 0x40, 0x00, 0x04, 0xc0, 0x00, 0xd2, 0x00, 0x3f, 0x04, 0xff, 0xfb, 0x40, 0x0c, 0x40, 0x07, 0x90, 0x02, 0x00, 0xfd, 0xd2, 0x00, 0xa7, 0x02, 0x00, 0xfc, 0x3c, 0x10, 0x0c, 0x40, 0x17, 0x00,
    0x0,
    //$
    0x24, 6, 0x2b, 0x0,
    0x0a, 0x00, 0xde, 0x38, 0x00, 0x07, 0xff, 0xe2, 0x3e, 0x79, 0xc7, 0x4f, 0x58, 0x00, 0x3f, 0xc8, 0x00, 0x06, 0xfe, 0x40, 0x00, 0x4f, 0xf4, 0x00, 0x39, 0xe9, 0x00, 0x39, 0xcc, 0x2c, 0x69, 0xe9, 0x08, 0xff, 0xb1, 0x00, 0x38, 0x0a, 0x00,
    0x0,
    //%
    0x25, 11, 0x43, 0x0,
    0x16, 0x00, 0xf1, 0x04, 0xfb, 0x00, 0x03, 0xb0, 0x00, 0xc5, 0xc4, 0x00, 0x92, 0x00, 0x0c, 0x5c, 0x40, 0x49, 0x02, 0x00, 0xf0, 0xc5, 0xc4, 0x0a, 0x20, 0x00, 0x0c, 0x5d, 0x24, 0x73, 0xfd, 0x10, 0x4f, 0xb0, 0xa1, 0xa7, 0xa4, 0x02, 0x00, 0xf9, 0x77, 0x0a, 0x7a, 0x70, 0x00, 0x0a, 0x10, 0x02, 0xa7, 0xf5, 0x00, 0x07, 0x40, 0x0a, 0x7a, 0x40, 0x03, 0xa0, 0x00, 0x3f, 0xd1, 0x16, 0x00,
    0x0,
    //&
    0x26, 7, 0x28, 0x0,
    0x12, 0x00, 0xe1, 0x0d, 0xfb, 0x00, 0x07, 0xc0, 0xe4, 0x00, 0x7c, 0x3e, 0x40, 0x03, 0xfe, 0x90, 0x00, 0x0d, 0xf2, 0x00, 0x0c, 0xce, 0xc4, 0x83, 0xe4, 0x4f, 0xe7, 0x3f, 0x70, 0xce, 0x20, 0x7f, 0xfc, 0xe9, 0x0e, 0x00,
    //'
    0x27, 3, 0x10, 0x0,
    0x04, 0x00, 0xf9, 0x04, 0xf7, 0x4f, 0x73, 0xe4, 0x0d, 0x20, 0x10, 0x00,
    //(
    0x28, 3, 0x1c, 0x0,
    0x06, 0x00, 0xeb, 0x04, 0xb0, 0xc9, 0x0e, 0x44, 0xd1, 0x7c, 0x0a, 0x90, 0xa9, 0x0a, 0x90, 0x7c, 0x04, 0xd1, 0x3e, 0x20, 0xc7, 0x04, 0xb0, 0x00,
    //)
    0x29, 3, 0x1c, 0x0,
    0x06, 0x00, 0xed, 0x6b, 0x03, 0xe2, 0x0c, 0x70, 0x79, 0x04, 0xc0, 0x4d, 0x03, 0xe0, 0x3c, 0x04, 0xc0, 0x79, 0x0c, 0x73, 0xe2, 0x7b, 0x02, 0x00,
    //*
    0x2a, 5, 0x14, 0x0,
    0x0b, 0x00, 0xf5, 0xc4, 0x04, 0xd8, 0x7d, 0x04, 0xc9, 0x13, 0xc8, 0x7d, 0x00, 0xc4, 0x17, 0x00,
    //+
    0x2b, 11, 0x33, 0x0,
    0x18, 0x00, 0xfe, 0x0d, 0x20, 0x04, 0x00, 0xff, 0xd2, 0x04, 0x00, 0xfe, 0x0d, 0x20, 0x04, 0x00, 0xff, 0xd2, 0x02, 0x00, 0xff, 0x0a, 0x03, 0xff, 0xfe, 0xfd, 0x10, 0x02, 0x00, 0xff, 0xd2, 0x04, 0x00, 0xfe, 0x0d, 0x20, 0x04, 0x00, 0xff, 0xd2, 0x04, 0x00, 0xfe, 0x0d, 0x20, 0x1d, 0x00,
    0x0,
    //,
    0x2c, 3, 0xf, 0x0,
    0x12, 0x00, 0xfa, 0x04, 0x90, 0xa7, 0x0c, 0x13, 0x80, 0x03, 0x00,
    0x0,
    //-
    0x2d, 4, 0xb, 0x0,
    0x14, 0x00, 0xfe, 0x4f, 0xf7, 0x0e, 0x00,
    0x0,
    //.
    0x2e, 3, 0xb, 0x0,
    0x13, 0x00, 0xfe, 0x02, 0xe7, 0x06, 0x00,
    0x0,
    ///
    0x2f, 4, 0x21, 0x0,
    0x09, 0x00, 0xe8, 0x09, 0x00, 0x0a, 0x00, 0x38, 0x00, 0x47, 0x00, 0x92, 0x00, 0xa1, 0x00, 0x80, 0x04, 0x70, 0x07, 0x40, 0x0a, 0x10, 0x0a, 0x00, 0x38, 0x00, 0x74, 0x03, 0x00,
    0x0,
    //0
    0x30, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x03, 0xff, 0x80, 0x0c, 0x93, 0xe2, 0x0e, 0x70, 0xe7, 0x3e, 0x40, 0xe7, 0x3e, 0x40, 0xe9, 0x3e, 0x40, 0xe9, 0x3e, 0x40, 0xe7, 0x0e, 0x70, 0xe7, 0x0c, 0x93, 0xe4, 0x03, 0xff, 0x80, 0x0c, 0x00,
    0x0,
    //1
    0x31, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x04, 0xfd, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x0c, 0x00,
    0x0,
    //2
    0x32, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x07, 0xff, 0xb1, 0x0e, 0x73, 0xf7, 0x3f, 0x70, 0xe7, 0x00, 0x03, 0xe4, 0x00, 0x07, 0xd1, 0x00, 0x0e, 0x90, 0x00, 0x7f, 0x20, 0x00, 0xe9, 0x00, 0x07, 0xf2, 0x00, 0x0e, 0xff, 0xf4, 0x0c, 0x00,
    0x0,
    //3
    0x33, 6, 0x28, 0x0,
    0x0c, 0x00, 0xee, 0x03, 0xff, 0xb1, 0x0a, 0xc3, 0xf7, 0x0c, 0x90, 0xe7, 0x00, 0x03, 0xf7, 0x00, 0x7f, 0xb0, 0x00, 0x04, 0xf4, 0x02, 0x00, 0xf6, 0xe7, 0x3f, 0x70, 0xe7, 0x0e, 0x93, 0xe4, 0x07, 0xff, 0xb0, 0x0c, 0x00,
    //4
    0x34, 6, 0x26, 0x0,
    0x0d, 0x00, 0xe3, 0x0a, 0xf2, 0x00, 0x3f, 0xf2, 0x00, 0x7f, 0xf2, 0x00, 0xeb, 0xf2, 0x07, 0xc7, 0xf2, 0x0e, 0x47, 0xf2, 0x3f, 0xff, 0xf9, 0x00, 0x07, 0xf2, 0x00, 0x07, 0xf2, 0x00, 0x07, 0xf2, 0x0c, 0x00,
    //5
    0x35, 6, 0x28, 0x0,
    0x0c, 0x00, 0xee, 0x03, 0xff, 0xf7, 0x04, 0xf2, 0x00, 0x07, 0xd1, 0x00, 0x0a, 0xfe, 0x40, 0x00, 0x07, 0xf2, 0x00, 0x03, 0xf7, 0x02, 0x00, 0xf6, 0xe7, 0x00, 0x03, 0xe4, 0x00, 0x0a, 0xd1, 0x3f, 0xfb, 0x10, 0x0c, 0x00,
    //6
    0x36, 6, 0x26, 0x0,
    0x0d, 0x00, 0xe3, 0x3f, 0x70, 0x00, 0xad, 0x10, 0x03, 0xf9, 0x00, 0x07, 0xf2, 0x00, 0x0c, 0xff, 0xd1, 0x0e, 0x73, 0xe7, 0x3e, 0x40, 0xe9, 0x3e, 0x40, 0xe9, 0x0e, 0x73, 0xf7, 0x06, 0xff, 0xb0, 0x0c, 0x00,
    //7
    0x37, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe9, 0x0c, 0xff, 0xf9, 0x00, 0x03, 0xe4, 0x00, 0x04, 0xf2, 0x00, 0x0a, 0xc0, 0x00, 0x0e, 0x70, 0x00, 0x3e, 0x20, 0x00, 0x7d, 0x10, 0x00, 0xc9, 0x02, 0x00, 0xfc, 0xe4, 0x00, 0x04, 0xf2, 0x0d, 0x00,
    0x0,
    //8
    0x38, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x04, 0xff, 0xa0, 0x0c, 0x94, 0xf4, 0x0c, 0x93, 0xe4, 0x0c, 0x94, 0xf2, 0x03, 0xff, 0x80, 0x0c, 0x73, 0xe4, 0x3e, 0x40, 0xe7, 0x3f, 0x70, 0xe7, 0x0e, 0x93, 0xe4, 0x04, 0xff, 0xb0, 0x0c, 0x00,
    0x0,
    //9
    0x39, 6, 0x26, 0x0,
    0x0c, 0x00, 0xe3, 0x06, 0xff, 0xb0, 0x0e, 0x73, 0xe4, 0x3e, 0x40, 0xe7, 0x3f, 0x70, 0xe7, 0x0e, 0x73, 0xf7, 0x07, 0xff, 0xf4, 0x00, 0x0a, 0xd1, 0x00, 0x3f, 0x70, 0x00, 0xad, 0x10, 0x00, 0xe9, 0x0d, 0x00,
    //:
    0x3a, 3, 0x10, 0x0,
    0x0f, 0x00, 0xfe, 0x2e, 0x70, 0x02, 0x00, 0xfe, 0x02, 0xe7, 0x06, 0x00,
    //;
    0x3b, 3, 0x13, 0x0,
    0x0e, 0x00, 0xff, 0xcc, 0x03, 0x00, 0xfa, 0x07, 0x90, 0xc4, 0x3c, 0x16, 0x70, 0x03, 0x00,
    0x0,
    //<
    0x3c, 11, 0x31, 0x0,
    0x1f, 0x00, 0xfe, 0x08, 0xb0, 0x02, 0x00, 0xfd, 0x04, 0xce, 0x71, 0x02, 0x00, 0xfe, 0x6e, 0xd4, 0x02, 0x00, 0xfd, 0x0a, 0xf9, 0x10, 0x03, 0x00, 0xfe, 0xaf, 0xb1, 0x04, 0x00, 0xfd, 0x06, 0xed, 0x40, 0x04, 0x00, 0xfd, 0x4c, 0xe7, 0x10, 0x04, 0x00, 0xff, 0x8b, 0x1c, 0x00,
    0x0,
    //=
    0x3d, 11, 0x16, 0x0,
    0x21, 0x00, 0xff, 0x0c, 0x03, 0xff, 0xff, 0xfc, 0x11, 0x00, 0xff, 0x0c, 0x03, 0xff, 0xff, 0xfc, 0x27, 0x00,
    //>
    0x3e, 11, 0x32, 0x0,
    0x1c, 0x00, 0xfe, 0xa7, 0x10, 0x03, 0x00, 0xfd, 0x02, 0xae, 0xb3, 0x04, 0x00, 0xfd, 0x05, 0xde, 0x60, 0x04, 0x00, 0xfe, 0x2a, 0xf7, 0x03, 0x00, 0xfd, 0x02, 0xad, 0x70, 0x02, 0x00, 0xfe, 0x5d, 0xe6, 0x02, 0x00, 0xfd, 0x2a, 0xeb, 0x30, 0x02, 0x00, 0xfe, 0x0a, 0x71, 0x1f, 0x00,
    //?
    0x3f, 5, 0x22, 0x0,
    0x0a, 0x00, 0xec, 0x07, 0xdb, 0x13, 0xe2, 0xa7, 0x00, 0x0a, 0x90, 0x00, 0xa9, 0x00, 0x0c, 0x70, 0x0e, 0xe2, 0x00, 0xe4, 0x00, 0x0e, 0x40, 0x03, 0x00, 0xfe, 0x3e, 0x40, 0x0a, 0x00,
    //@
    0x40, 16, 0x7f, 0x0,
    0x12, 0x00, 0xfc, 0x08, 0xdd, 0xdf, 0xd6, 0x03, 0x00, 0xfa, 0x04, 0xe9, 0x10, 0x00, 0x2a, 0xd1, 0x02, 0x00, 0xfe, 0x4f, 0x40, 0x03, 0x00, 0xb3, 0x7d, 0x10, 0x00, 0xe4, 0x00, 0xaf, 0xf8, 0xac, 0x0c, 0x90, 0x07, 0xc0, 0x0c, 0xc1, 0x3f, 0xf9, 0x04, 0xd1, 0x0c, 0x40, 0x7d, 0x10, 0x07, 0xf7, 0x03, 0xe2, 0x0d, 0x20, 0xe9, 0x00, 0x04, 0xf4, 0x03, 0xe4, 0x3e, 0x23, 0xe4, 0x00, 0x04, 0xd1, 0x03, 0xe2, 0x3e, 0x23, 0xe4, 0x00, 0x0a, 0xc0, 0x07, 0xc0, 0x0e, 0x40, 0xe7, 0x00, 0x0e, 0x90, 0x3e, 0x40, 0x0c, 0x70, 0xad, 0x12, 0xdf, 0x92, 0xc7, 0x00, 0x04, 0xf2, 0x0a, 0xfe, 0x4c, 0xfe, 0x63, 0xe4, 0x00, 0xad, 0x10, 0x03, 0x00, 0xf7, 0x3f, 0x90, 0x00, 0x0a, 0xf9, 0x30, 0x00, 0x4a, 0xf7, 0x03, 0x00, 0xfb, 0x29, 0xef, 0xff, 0xe9, 0x30, 0x09, 0x00,
    0x0,
    //A
    0x41, 7, 0x2c, 0x0,
    0x0f, 0x00, 0xfb, 0x0e, 0x70, 0x00, 0x03, 0xfc, 0x02, 0x00, 0xe5, 0x7f, 0xd1, 0x00, 0x0a, 0xce, 0x40, 0x00, 0xe7, 0xc7, 0x00, 0x3e, 0x4a, 0x90, 0x04, 0xf2, 0x7d, 0x10, 0xaf, 0xff, 0xf2, 0x0c, 0x90, 0x3e, 0x40, 0xe7, 0x00, 0xe9, 0x0e, 0x00,
    //B
    0x42, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x0c, 0xff, 0xb0, 0x0c, 0x94, 0xe4, 0x0c, 0x90, 0xe7, 0x0c, 0x96, 0xf2, 0x0c, 0xff, 0x70, 0x0c, 0x96, 0xf4, 0x0c, 0x90, 0xe7, 0x0c, 0x90, 0xe9, 0x0c, 0x94, 0xf7, 0x0c, 0xff, 0xb0, 0x0c, 0x00,
    0x0,
    //C
    0x43, 6, 0x27, 0x0,
    0x0d, 0x00, 0xe7, 0x6f, 0xf7, 0x04, 0xf7, 0x00, 0x0a, 0xd1, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0a, 0xc0, 0x00, 0x04, 0xf4, 0x02, 0x00, 0xfe, 0x7f, 0xf7, 0x0c, 0x00,
    0x0,
    //D
    0x44, 7, 0x2b, 0x0,
    0x0e, 0x00, 0xde, 0x08, 0xbb, 0x60, 0x00, 0xcc, 0x0c, 0xc0, 0x0c, 0xc0, 0x4f, 0x20, 0xcc, 0x03, 0xf7, 0x0c, 0xc0, 0x0e, 0x70, 0xcc, 0x00, 0xe7, 0x0c, 0xc0, 0x3e, 0x40, 0xcc, 0x04, 0xf2, 0x0c, 0xc0, 0xcc, 0x00, 0x8b, 0xb6, 0x0f, 0x00,
    0x0,
    //E
    0x45, 5, 0x22, 0x0,
    0x0a, 0x00, 0xe7, 0x0e, 0xff, 0x90, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0xff, 0x70, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xef, 0xf9, 0x0a, 0x00,
    //F
    0x46, 5, 0x21, 0x0,
    0x0a, 0x00, 0xe8, 0x0e, 0xff, 0x90, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0xff, 0x70, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x0b, 0x00,
    0x0,
    //G
    0x47, 7, 0x2d, 0x0,
    0x0f, 0x00, 0xf5, 0xaf, 0xf8, 0x00, 0x7f, 0x27, 0xf4, 0x0a, 0xc0, 0x3a, 0x10, 0xc9, 0x02, 0x00, 0xfe, 0x0c, 0x90, 0x02, 0x00, 0xef, 0xc9, 0x3f, 0xf9, 0x0c, 0x90, 0x0e, 0x90, 0xac, 0x00, 0xe7, 0x07, 0xd1, 0x4f, 0x40, 0x0a, 0xff, 0x80, 0x0e, 0x00,
    0x0,
    //H
    0x48, 7, 0x2d, 0x0,
    0x0e, 0x00, 0xf1, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0x02, 0xff, 0xee, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0e, 0x00,
    0x0,
    //I
    0x49, 4, 0x1d, 0x0,
    0x08, 0x00, 0xec, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x08, 0x00,
    0x0,
    //J
    0x4a, 4, 0x1c, 0x0,
    0x09, 0x00, 0xed, 0xe9, 0x00, 0xe9, 0x00, 0xe9, 0x00, 0xe9, 0x00, 0xe9, 0x00, 0xe9, 0x00, 0xe9, 0x00, 0xe9, 0x00, 0xe9, 0xcf, 0xe2, 0x08, 0x00,
    //K
    0x4b, 7, 0x2c, 0x0,
    0x0e, 0x00, 0xdd, 0x0c, 0xc0, 0x3f, 0x70, 0xcc, 0x0a, 0xc0, 0x0c, 0xc3, 0xe4, 0x00, 0xcc, 0xac, 0x00, 0x0c, 0xee, 0x40, 0x00, 0xcc, 0xe4, 0x00, 0x0c, 0xca, 0xc0, 0x00, 0xcc, 0x3e, 0x40, 0x0c, 0xc0, 0xac, 0x00, 0xcc, 0x04, 0xf4, 0x0e, 0x00,
    //L
    0x4c, 5, 0x22, 0x0,
    0x0a, 0x00, 0xe7, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xef, 0xf9, 0x0a, 0x00,
    //M
    0x4d, 8, 0x31, 0x0,
    0x10, 0x00, 0xd8, 0x04, 0xf2, 0x0a, 0xd1, 0x07, 0xf4, 0x0c, 0xd1, 0x07, 0xf7, 0x0e, 0xf2, 0x0a, 0xf9, 0x3f, 0xf2, 0x0a, 0xcb, 0x4e, 0xe4, 0x0c, 0xac, 0x89, 0xe4, 0x0e, 0x7c, 0xd5, 0xe7, 0x0e, 0x7a, 0xf2, 0xc9, 0x3e, 0x47, 0xd1, 0xc9, 0x3e, 0x44, 0xc0, 0xcc, 0x10, 0x00,
    0x0,
    //N
    0x4e, 7, 0x2c, 0x0,
    0x0e, 0x00, 0xdd, 0x0e, 0x70, 0x3e, 0x20, 0xed, 0x13, 0xe2, 0x0e, 0xf4, 0x3e, 0x20, 0xed, 0x93, 0xe2, 0x0e, 0x9c, 0x4e, 0x20, 0xe7, 0xa9, 0xe2, 0x0e, 0x74, 0xee, 0x20, 0xe7, 0x0e, 0xf2, 0x0e, 0x70, 0x7f, 0x20, 0xe7, 0x03, 0xe2, 0x0e, 0x00,
    //O
    0x4f, 7, 0x2b, 0x0,
    0x0f, 0x00, 0xde, 0xcc, 0xd7, 0x00, 0xac, 0x04, 0xd1, 0x0c, 0x90, 0x3e, 0x40, 0xe7, 0x03, 0xe4, 0x0e, 0x70, 0x3e, 0x40, 0xe7, 0x03, 0xe4, 0x0e, 0x70, 0x3e, 0x40, 0xc9, 0x03, 0xe4, 0x0a, 0xc0, 0x4d, 0x10, 0x0c, 0xce, 0x70, 0x0e, 0x00,
    0x0,
    //P
    0x50, 6, 0x26, 0x0,
    0x0c, 0x00, 0xe3, 0x0c, 0xff, 0xb1, 0x0c, 0x93, 0xf7, 0x0c, 0x90, 0xc9, 0x0c, 0x90, 0xc9, 0x0c, 0x93, 0xf7, 0x0c, 0xff, 0xb0, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x0d, 0x00,
    //Q
    0x51, 7, 0x30, 0x0,
    0x0f, 0x00, 0xde, 0xdf, 0xf7, 0x00, 0xad, 0x17, 0xf2, 0x0c, 0x90, 0x3e, 0x40, 0xe7, 0x03, 0xe4, 0x0e, 0x70, 0x0e, 0x70, 0xe7, 0x00, 0xe7, 0x0e, 0x70, 0x3f, 0x70, 0xc9, 0x79, 0xe4, 0x0a, 0xc4, 0xef, 0x20, 0x0d, 0xff, 0xb0, 0x02, 0x00, 0xfe, 0x09, 0x20, 0x0a, 0x00,
    //R
    0x52, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x0c, 0xff, 0xb0, 0x0c, 0x93, 0xf7, 0x0c, 0x90, 0xe7, 0x0c, 0x94, 0xf7, 0x0c, 0xff, 0xb0, 0x0c, 0xce, 0x40, 0x0c, 0xac, 0x90, 0x0c, 0x97, 0xd1, 0x0c, 0x93, 0xe4, 0x0c, 0x90, 0xe9, 0x0c, 0x00,
    0x0,
    //S
    0x53, 5, 0x22, 0x0,
    0x0a, 0x00, 0xe7, 0x0a, 0xff, 0x43, 0xe4, 0x00, 0x4f, 0x20, 0x03, 0xf7, 0x00, 0x07, 0xf4, 0x00, 0x0c, 0xd1, 0x00, 0x0e, 0x70, 0x00, 0xc9, 0x00, 0x3f, 0x73, 0xff, 0xb1, 0x0a, 0x00,
    //T
    0x54, 5, 0x22, 0x0,
    0x0a, 0x00, 0xe7, 0x7f, 0xff, 0xd0, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x0a, 0x00,
    //U
    0x55, 7, 0x2c, 0x0,
    0x0e, 0x00, 0xdd, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0a, 0xd1, 0x7f, 0x20, 0x0d, 0xff, 0x70, 0x0e, 0x00,
    //V
    0x56, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x4f, 0x20, 0xac, 0x3e, 0x40, 0xc9, 0x0e, 0x70, 0xe7, 0x0c, 0x93, 0xe4, 0x07, 0xc4, 0xd1, 0x04, 0xd8, 0xc0, 0x03, 0xee, 0x90, 0x00, 0xef, 0x40, 0x00, 0xaf, 0x20, 0x00, 0x7d, 0x10, 0x0c, 0x00,
    0x0,
    //W
    0x57, 9, 0x36, 0x0,
    0x12, 0x00, 0xd3, 0x4d, 0x10, 0xc4, 0x0a, 0xc3, 0xe2, 0x0e, 0x70, 0xc9, 0x0e, 0x43, 0xf9, 0x0e, 0x70, 0xc7, 0x7d, 0xc3, 0xe2, 0x0a, 0x9a, 0x8c, 0x6d, 0x10, 0x7c, 0xc5, 0xc9, 0xc0, 0x04, 0xee, 0x2a, 0xd9, 0x00, 0x3f, 0xd1, 0x7f, 0x70, 0x00, 0xec, 0x04, 0xf4, 0x00, 0x0c, 0x90, 0x3e, 0x20, 0x12, 0x00,
    //X
    0x58, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x4f, 0x20, 0xc9, 0x0c, 0x93, 0xe4, 0x07, 0xdb, 0xc0, 0x03, 0xff, 0x70, 0x00, 0xaf, 0x20, 0x00, 0xcf, 0x20, 0x03, 0xff, 0x70, 0x0a, 0xc8, 0xd1, 0x0e, 0x73, 0xe4, 0x4d, 0x10, 0xcc, 0x0c, 0x00,
    0x0,
    //Y
    0x59, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x4f, 0x40, 0xcc, 0x0e, 0x93, 0xf7, 0x0a, 0xc4, 0xf2, 0x04, 0xfc, 0x90, 0x00, 0xef, 0x40, 0x00, 0xad, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x0c, 0x00,
    0x0,
    //Z
    0x5a, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x0e, 0xff, 0xf7, 0x00, 0x03, 0xe2, 0x00, 0x0a, 0xc0, 0x00, 0x0e, 0x70, 0x00, 0x4f, 0x20, 0x00, 0xa9, 0x00, 0x03, 0xe4, 0x00, 0x07, 0xd1, 0x00, 0x0c, 0x90, 0x00, 0x3f, 0xff, 0xf7, 0x0c, 0x00,
    0x0,
    //[
    0x5b, 4, 0x23, 0x0,
    0x08, 0x00, 0xe6, 0x0a, 0xf9, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0x70, 0x0a, 0xf9, 0x02, 0x00,
    0x0,
    // zpetne lomitko
    0x5c, 4, 0x23, 0x0,
    0x08, 0x00, 0xe6, 0x81, 0x00, 0x74, 0x00, 0x37, 0x00, 0x0a, 0x00, 0x08, 0x10, 0x07, 0x20, 0x04, 0x70, 0x00, 0x80, 0x00, 0xa1, 0x00, 0x72, 0x00, 0x44, 0x00, 0x38, 0x00, 0x09, 0x02, 0x00,
    0x0,
    //]
    0x5d, 4, 0x23, 0x0,
    0x08, 0x00, 0xe6, 0x4f, 0xf2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x00, 0xd2, 0x4f, 0xf2, 0x02, 0x00,
    0x0,
    //^
    0x5e, 8, 0x22, 0x0,
    0x0d, 0x00, 0xfe, 0x0e, 0xc0, 0x02, 0x00, 0xfe, 0x7f, 0xf2, 0x02, 0x00, 0xf1, 0xe7, 0xc9, 0x00, 0x04, 0xf2, 0x4f, 0x20, 0x0c, 0x90, 0x0e, 0x90, 0x4f, 0x40, 0x07, 0xd1, 0x24, 0x00,
    //_
    0x5f, 8, 0xa, 0x0,
    0x3c, 0x00, 0x04, 0xff, 0x08, 0x00,
    //`
    0x60, 5, 0xe, 0x0,
    0x0a, 0x00, 0xfb, 0x0c, 0xd1, 0x00, 0x0e, 0x70, 0x1e, 0x00,
    //a
    0x61, 6, 0x1e, 0x0,
    0x15, 0x00, 0xeb, 0x07, 0xfc, 0xf7, 0x0e, 0x74, 0xf7, 0x3f, 0x73, 0xf7, 0x3e, 0x43, 0xf7, 0x3f, 0x73, 0xf7, 0x0e, 0x94, 0xf7, 0x07, 0xfc, 0xf7, 0x0c, 0x00,
    //b
    0x62, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x00, 0x0e, 0xbf, 0xb1, 0x0e, 0x93, 0xe4, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x93, 0xe4, 0x0e, 0xbf, 0xd1, 0x0c, 0x00,
    0x0,
    //c
    0x63, 4, 0x17, 0x0,
    0x0e, 0x00, 0xf2, 0x07, 0xfd, 0x0e, 0x70, 0x4f, 0x40, 0x4f, 0x20, 0x4f, 0x40, 0x3f, 0x70, 0x07, 0xfd, 0x08, 0x00,
    0x0,
    //d
    0x64, 6, 0x26, 0x0,
    0x0d, 0x00, 0xe3, 0x03, 0xf7, 0x00, 0x03, 0xf7, 0x00, 0x03, 0xf7, 0x07, 0xfc, 0xf7, 0x0e, 0x94, 0xf7, 0x3f, 0x73, 0xf7, 0x3e, 0x43, 0xf7, 0x3f, 0x73, 0xf7, 0x0e, 0x94, 0xf7, 0x07, 0xfc, 0xf7, 0x0c, 0x00,
    //e
    0x65, 6, 0x1e, 0x0,
    0x15, 0x00, 0xeb, 0x03, 0xff, 0xb0, 0x0c, 0x93, 0xe4, 0x0e, 0x40, 0xe7, 0x3f, 0xff, 0xf9, 0x3e, 0x40, 0x00, 0x0c, 0x90, 0xc7, 0x04, 0xff, 0xb1, 0x0c, 0x00,
    //f
    0x66, 4, 0x1d, 0x0,
    0x08, 0x00, 0xec, 0x04, 0xfd, 0x0a, 0xd1, 0x0a, 0xc0, 0x4f, 0xfd, 0x0a, 0xc0, 0x0a, 0xc0, 0x0a, 0xc0, 0x0a, 0xc0, 0x0a, 0xc0, 0x0a, 0xc0, 0x08, 0x00,
    0x0,
    //g
    0x67, 6, 0x27, 0x0,
    0x15, 0x00, 0xe2, 0x0a, 0xfd, 0xf7, 0x0e, 0x94, 0xf7, 0x3f, 0x73, 0xf7, 0x3e, 0x43, 0xf7, 0x3f, 0x73, 0xf7, 0x0e, 0x74, 0xf7, 0x07, 0xfc, 0xf7, 0x00, 0x03, 0xf7, 0x3f, 0x73, 0xe4, 0x05, 0xbb, 0x60, 0x03, 0x00,
    0x0,
    //h
    0x68, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x00, 0x0e, 0xdf, 0xe2, 0x0e, 0x93, 0xe4, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0c, 0x00,
    0x0,
    //i
    0x69, 3, 0x17, 0x0,
    0x08, 0x00, 0xff, 0xe4, 0x02, 0x00, 0xf6, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x06, 0x00,
    0x0,
    //j
    0x6a, 3, 0x1b, 0x0,
    0x08, 0x00, 0xff, 0xe4, 0x02, 0x00, 0xf0, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0x00,
    0x0,
    //k
    0x6b, 6, 0x27, 0x0,
    0x0c, 0x00, 0xe2, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x00, 0x0e, 0x74, 0xf2, 0x0e, 0x7c, 0x90, 0x0e, 0xdf, 0x20, 0x0e, 0xdd, 0x10, 0x0e, 0x8e, 0x70, 0x0e, 0x77, 0xd1, 0x0e, 0x70, 0xe7, 0x0c, 0x00,
    0x0,
    //l
    0x6c, 3, 0x18, 0x0,
    0x06, 0x00, 0xf1, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x06, 0x00,
    //m
    0x6d, 9, 0x28, 0x0,
    0x20, 0x00, 0xe1, 0xcd, 0xfe, 0x9f, 0xe2, 0x0c, 0x90, 0xe9, 0x3f, 0x70, 0xc9, 0x0e, 0x70, 0xe7, 0x0c, 0x90, 0xe7, 0x0e, 0x70, 0xc9, 0x0e, 0x70, 0xe7, 0x0c, 0x90, 0xe7, 0x0e, 0x70, 0xc9, 0x0e, 0x70, 0xe7, 0x12, 0x00,
    //n
    0x6e, 6, 0x1e, 0x0,
    0x15, 0x00, 0xeb, 0x0e, 0xbf, 0xd2, 0x0e, 0x93, 0xe4, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0c, 0x00,
    //o
    0x6f, 6, 0x1e, 0x0,
    0x15, 0x00, 0xeb, 0x04, 0xff, 0x90, 0x0c, 0x73, 0xe4, 0x3e, 0x40, 0xe7, 0x3e, 0x40, 0xe7, 0x3f, 0x70, 0xe7, 0x0c, 0x93, 0xe4, 0x04, 0xff, 0x90, 0x0c, 0x00,
    //p
    0x70, 6, 0x26, 0x0,
    0x15, 0x00, 0xe3, 0x0e, 0xbf, 0xd1, 0x0e, 0x93, 0xe4, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x70, 0xe7, 0x0e, 0x93, 0xe4, 0x0e, 0xbf, 0xd1, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x00, 0x0e, 0x70, 0x04, 0x00,
    //q
    0x71, 6, 0x27, 0x0,
    0x15, 0x00, 0xe2, 0x07, 0xfc, 0xf7, 0x0e, 0x94, 0xf7, 0x3f, 0x73, 0xf7, 0x3e, 0x43, 0xf7, 0x3f, 0x73, 0xf7, 0x0e, 0x94, 0xf7, 0x07, 0xfc, 0xf7, 0x00, 0x03, 0xf7, 0x00, 0x03, 0xf7, 0x00, 0x03, 0xf7, 0x03, 0x00,
    0x0,
    //r
    0x72, 4, 0x17, 0x0,
    0x0e, 0x00, 0xf2, 0x3e, 0x9c, 0x3f, 0x90, 0x3e, 0x40, 0x3e, 0x40, 0x3e, 0x40, 0x3e, 0x40, 0x3e, 0x40, 0x08, 0x00,
    0x0,
    //s
    0x73, 5, 0x1a, 0x0,
    0x12, 0x00, 0xef, 0x4f, 0xf4, 0x0c, 0x90, 0x00, 0xcc, 0x00, 0x03, 0xfb, 0x00, 0x03, 0xf7, 0x00, 0x0c, 0x90, 0xaf, 0xe2, 0x0a, 0x00,
    //t
    0x74, 4, 0x1b, 0x0,
    0x0a, 0x00, 0xee, 0x07, 0xd1, 0x07, 0xd1, 0x4f, 0xfc, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x08, 0x00,
    0x0,
    //u
    0x75, 6, 0x1e, 0x0,
    0x15, 0x00, 0xeb, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x93, 0xe4, 0x07, 0xff, 0xb0, 0x0c, 0x00,
    //v
    0x76, 6, 0x1d, 0x0,
    0x15, 0x00, 0xec, 0x3e, 0x40, 0xe7, 0x0c, 0x93, 0xe4, 0x0a, 0xc7, 0xd1, 0x04, 0xdb, 0x90, 0x00, 0xef, 0x70, 0x00, 0xaf, 0x20, 0x00, 0x7c, 0x0d, 0x00,
    0x0,
    //w
    0x77, 8, 0x25, 0x0,
    0x1c, 0x00, 0xe4, 0x0e, 0x43, 0xb0, 0xa9, 0x0c, 0x74, 0xd1, 0xc4, 0x07, 0x97, 0xf5, 0xe2, 0x04, 0xda, 0xb9, 0xd1, 0x00, 0xed, 0x8d, 0x90, 0x00, 0xcc, 0x4f, 0x70, 0x00, 0x79, 0x0d, 0x20, 0x10, 0x00,
    0x0,
    //x
    0x78, 6, 0x1e, 0x0,
    0x15, 0x00, 0xeb, 0x0e, 0x40, 0xe7, 0x07, 0xd8, 0xd1, 0x00, 0xef, 0x70, 0x00, 0xad, 0x10, 0x03, 0xff, 0x70, 0x0a, 0xc7, 0xf2, 0x3e, 0x40, 0xc9, 0x0c, 0x00,
    //y
    0x79, 6, 0x27, 0x0,
    0x15, 0x00, 0xec, 0x4f, 0x20, 0xac, 0x0e, 0x70, 0xe7, 0x0a, 0x93, 0xe2, 0x07, 0xd8, 0xc0, 0x03, 0xee, 0x70, 0x00, 0xcf, 0x20, 0x00, 0x7c, 0x02, 0x00, 0xf9, 0xc7, 0x00, 0x03, 0xe2, 0x00, 0x07, 0xc0, 0x04, 0x00,
    0x0,
    //z
    0x7a, 5, 0x1b, 0x0,
    0x11, 0x00, 0xee, 0x03, 0xff, 0xf9, 0x00, 0x3e, 0x20, 0x07, 0xc0, 0x00, 0xe7, 0x00, 0x4f, 0x20, 0x0a, 0x90, 0x03, 0xff, 0xf7, 0x0a, 0x00,
    0x0,
    //{
    0x7b, 5, 0x2c, 0x0,
    0x08, 0x00, 0xdb, 0x04, 0xfd, 0x00, 0xe8, 0x00, 0x3f, 0x70, 0x03, 0xf7, 0x00, 0x3f, 0x70, 0x03, 0xe4, 0x00, 0x7d, 0x10, 0x7f, 0x40, 0x00, 0x7d, 0x10, 0x03, 0xe4, 0x00, 0x3f, 0x70, 0x03, 0xf7, 0x00, 0x3f, 0x70, 0x00, 0xe8, 0x00, 0x04, 0xfd,
    //|
    0x7c, 8, 0x47, 0x0,
    0x0d, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x02, 0x00, 0xfe, 0x07, 0x90, 0x09, 0x00,
    0x0,
    //}
    0x7d, 5, 0x2d, 0x0,
    0x07, 0x00, 0xda, 0x0a, 0xf8, 0x00, 0x04, 0xf2, 0x00, 0x3e, 0x40, 0x03, 0xe4, 0x00, 0x3e, 0x40, 0x03, 0xe4, 0x00, 0x0a, 0xc1, 0x00, 0x0a, 0xf0, 0x0a, 0xc1, 0x03, 0xe4, 0x00, 0x3e, 0x40, 0x03, 0xe4, 0x00, 0x3e, 0x40, 0x04, 0xf2, 0x0a, 0xf8, 0x00,
    0x0,
    //~
    0x7e, 11, 0x14, 0x0,
    0x2c, 0x00, 0xf5, 0x06, 0xef, 0xe9, 0x10, 0x58, 0x00, 0xa4, 0x02, 0xaf, 0xfb, 0x40, 0x2c, 0x00,
    //�
    0x7F, 6, 0x13, 0x0,
    0x07, 0x00, 0xf6, 0xff, 0x00, 0x0f, 0x00, 0xf0, 0x0f, 0x00, 0xf0, 0x00, 0xff, 0x25, 0x00,
    0x0,
    
    //�
    0xE1, 6, 0x28, 0x0,
    0x0b, 0x00, 0xff, 0x0f, 0x02, 0x00, 0xfd, 0xf0, 0x00, 0x0f, 0x04, 0x00, 0xeb, 0x07, 0xfc, 0xf7, 0x0e, 0x74, 0xf7, 0x3f, 0x73, 0xf7, 0x3e, 0x43, 0xf7, 0x3f, 0x73, 0xf7, 0x0e, 0x94, 0xf7, 0x07, 0xfc, 0xf7, 0x0c, 0x00,
    //�
    0xE9, 6, 0x28, 0x0,
    0x0b, 0x00, 0xff, 0x0f, 0x02, 0x00, 0xfd, 0xf0, 0x00, 0x0f, 0x04, 0x00, 0xeb, 0x03, 0xff, 0xb0, 0x0c, 0x93, 0xe4, 0x0e, 0x40, 0xe7, 0x3f, 0xff, 0xf9, 0x3e, 0x40, 0x00, 0x0c, 0x90, 0xc7, 0x04, 0xff, 0xb1, 0x0c, 0x00,
    //�
    0xED, 4, 0x1E, 0x0,
    0x07, 0x00, 0xfc, 0x0f, 0x00, 0xf0, 0x0f, 0x03, 0x00, 0xf2, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x0e, 0x70, 0x08, 0x00,
    //�
    0xF3, 6, 0x28, 0x0,
    0x0b, 0x00, 0xff, 0x0f, 0x02, 0x00, 0xfd, 0xf0, 0x00, 0x0f, 0x04, 0x00, 0xeb, 0x04, 0xff, 0x90, 0x0c, 0x73, 0xe4, 0x3e, 0x40, 0xe7, 0x3e, 0x40, 0xe7, 0x3f, 0x70, 0xe7, 0x0c, 0x93, 0xe4, 0x04, 0xff, 0x90, 0x0c, 0x00,
    //�
    0xFA, 6, 0x28, 0x0,
    0x0b, 0x00, 0xff, 0x0f, 0x02, 0x00, 0xfd, 0xf0, 0x00, 0x0f, 0x04, 0x00, 0xeb, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x93, 0xe4, 0x07, 0xff, 0xb0, 0x0c, 0x00,
    //�
    0xFD, 6, 0x31, 0x0,
    0x0b, 0x00, 0xff, 0x0f, 0x02, 0x00, 0xfd, 0xf0, 0x00, 0x0f, 0x04, 0x00, 0xec, 0x4f, 0x20, 0xac, 0x0e, 0x70, 0xe7, 0x0a, 0x93, 0xe2, 0x07, 0xd8, 0xc0, 0x03, 0xee, 0x70, 0x00, 0xcf, 0x20, 0x00, 0x7c, 0x02, 0x00, 0xf9, 0xc7, 0x00, 0x03, 0xe2, 0x00, 0x07, 0xc0, 0x04, 0x00,
    0x0,
    
    //e s hackem
    0xD8, 6, 0x25, 0x0,
    0x0d, 0x00, 0x02, 0xf0, 0xfe, 0x00, 0x0f, 0x04, 0x00, 0xeb, 0x03, 0xff, 0xb0, 0x0c, 0x93, 0xe4, 0x0e, 0x40, 0xe7, 0x3f, 0xff, 0xf9, 0x3e, 0x40, 0x00, 0x0c, 0x90, 0xc7, 0x04, 0xff, 0xb1, 0x0c, 0x00,
    0x0,
    //u s krouzkem
    0xDE, 6, 0x2b, 0x0,
    0x07, 0x00, 0xf6, 0xff, 0x00, 0x0f, 0x00, 0xf0, 0x0f, 0x00, 0xf0, 0x00, 0xff, 0x04, 0x00, 0xeb, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x93, 0xe4, 0x07, 0xff, 0xb0, 0x0c, 0x00,
    0x0,
    
    
    //c s hackem
    0x9F, 4, 0x1e, 0x0,
    0x08, 0x00, 0x02, 0x0f, 0xfe, 0x00, 0xf0, 0x02, 0x00, 0xf2, 0x07, 0xfd, 0x0e, 0x70, 0x4f, 0x40, 0x4f, 0x20, 0x4f, 0x40, 0x3f, 0x70, 0x07, 0xfd, 0x08, 0x00,
    //d s hackem
    0xD4, 6, 0x26, 0x0,
    0x0d, 0x00, 0xe3, 0xf3, 0xf7, 0x00, 0x0f, 0xf7, 0x00, 0x03, 0xf7, 0x07, 0xfc, 0xf7, 0x0e, 0x94, 0xf7, 0x3f, 0x73, 0xf7, 0x3e, 0x43, 0xf7, 0x3f, 0x73, 0xf7, 0x0e, 0x94, 0xf7, 0x07, 0xfc, 0xf7, 0x0c, 0x00,
    //n s hackem
    0xE5, 6, 0x25, 0x0,
    0x0d, 0x00, 0x02, 0xf0, 0xfe, 0x00, 0x0f, 0x04, 0x00, 0xeb, 0x0e, 0xbf, 0xd2, 0x0e, 0x93, 0xe4, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0e, 0x73, 0xf7, 0x0c, 0x00,
    0x0,
    //r s hackem
    0xFE, 4, 0x1e, 0x0,
    0x08, 0x00, 0x02, 0x0f, 0xfe, 0x00, 0xf0, 0x02, 0x00, 0xf2, 0x3e, 0x9c, 0x3f, 0x90, 0x3e, 0x40, 0x3e, 0x40, 0x3e, 0x40, 0x3e, 0x40, 0x3e, 0x40, 0x08, 0x00,
    //s s hackem
    0xE7, 5, 0x21, 0x0,
    0x0a, 0x00, 0x02, 0x0f, 0xfe, 0x00, 0x0f, 0x04, 0x00, 0xef, 0x4f, 0xf4, 0x0c, 0x90, 0x00, 0xcc, 0x00, 0x03, 0xfb, 0x00, 0x03, 0xf7, 0x00, 0x0c, 0x90, 0xaf, 0xe2, 0x0a, 0x00,
    0x0,
    //t s hackem
    0x9C, 4, 0x1c, 0x0,
    0x09, 0x00, 0xed, 0x0f, 0x07, 0xdf, 0x07, 0xd1, 0x4f, 0xfc, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x08, 0x00,
    //z s hackem
    0xA7, 5, 0x22, 0x0,
    0x0a, 0x00, 0x02, 0x0f, 0xfe, 0x00, 0x0f, 0x03, 0x00, 0xee, 0x03, 0xff, 0xf9, 0x00, 0x3e, 0x20, 0x07, 0xc0, 0x00, 0xe7, 0x00, 0x4f, 0x20, 0x0a, 0x90, 0x03, 0xff, 0xf7, 0x0a, 0x00,
    
    
    //�
    0xC1, 7, 0x38, 0x0,
    0x03, 0x00, 0xff, 0xf0, 0x02, 0x00, 0xff, 0xf0, 0x02, 0x00, 0xff, 0xf0, 0x05, 0x00, 0xfb, 0x0e, 0x70, 0x00, 0x03, 0xfc, 0x02, 0x00, 0xe5, 0x7f, 0xd1, 0x00, 0x0a, 0xce, 0x40, 0x00, 0xe7, 0xc7, 0x00, 0x3e, 0x4a, 0x90, 0x04, 0xf2, 0x7d, 0x10, 0xaf, 0xff, 0xf2, 0x0c, 0x90, 0x3e, 0x40, 0xe7, 0x00, 0xe9, 0x0e, 0x00,
    //�
    0xC9, 5, 0x2a, 0x0,
    0x02, 0x00, 0xfb, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 0x03, 0x00, 0xe7, 0x0e, 0xff, 0x90, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0xff, 0x70, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xe9, 0x00, 0x0e, 0x90, 0x00, 0xef, 0xf9, 0x0a, 0x00,
    //�
    0xCD, 4, 0x24, 0x0,
    0xfa, 0x00, 0xf0, 0x00, 0xf0, 0x00, 0xf0, 0x02, 0x00, 0xec, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x07, 0xd1, 0x08, 0x00,
    //�
    0xD3, 7, 0x37, 0x0,
    0x03, 0x00, 0xff, 0xf0, 0x02, 0x00, 0xff, 0xf0, 0x02, 0x00, 0xff, 0xf0, 0x05, 0x00, 0xde, 0xcc, 0xd7, 0x00, 0xac, 0x04, 0xd1, 0x0c, 0x90, 0x3e, 0x40, 0xe7, 0x03, 0xe4, 0x0e, 0x70, 0x3e, 0x40, 0xe7, 0x03, 0xe4, 0x0e, 0x70, 0x3e, 0x40, 0xc9, 0x03, 0xe4, 0x0a, 0xc0, 0x4d, 0x10, 0x0c, 0xce, 0x70, 0x0e, 0x00,
    0x0,
    //�
    0xDA, 7, 0x38, 0x0,
    0x03, 0x00, 0xff, 0xf0, 0x02, 0x00, 0xff, 0xf0, 0x02, 0x00, 0xff, 0xf0, 0x04, 0x00, 0xdd, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0c, 0xc0, 0x4f, 0x40, 0xcc, 0x04, 0xf4, 0x0a, 0xd1, 0x7f, 0x20, 0x0d, 0xff, 0x70, 0x0e, 0x00,
    //�
    0xDD, 6, 0x31, 0x0,
    0x02, 0x00, 0xff, 0x0f, 0x02, 0x00, 0xfd, 0xf0, 0x00, 0x0f, 0x04, 0x00, 0xe2, 0x4f, 0x40, 0xcc, 0x0e, 0x93, 0xf7, 0x0a, 0xc4, 0xf2, 0x04, 0xfc, 0x90, 0x00, 0xef, 0x40, 0x00, 0xad, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x00, 0x7d, 0x10, 0x0c, 0x00,
    0x0,
    
    //C s hackem
    0xAC, 6, 0x2e, 0x0,
    0x04, 0x00, 0x02, 0xf0, 0xfe, 0x00, 0x0f, 0x05, 0x00, 0xe7, 0x6f, 0xf7, 0x04, 0xf7, 0x00, 0x0a, 0xd1, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0c, 0x90, 0x00, 0x0a, 0xc0, 0x00, 0x04, 0xf4, 0x02, 0x00, 0xfe, 0x7f, 0xf7, 0x0c, 0x00,
    //D s hackem
    0xD2, 7, 0x33, 0x0,
    0x04, 0x00, 0x02, 0x0f, 0x02, 0x00, 0xff, 0x0f, 0x05, 0x00, 0xde, 0x08, 0xbb, 0x60, 0x00, 0xcc, 0x0c, 0xc0, 0x0c, 0xc0, 0x4f, 0x20, 0xcc, 0x03, 0xf7, 0x0c, 0xc0, 0x0e, 0x70, 0xcc, 0x00, 0xe7, 0x0c, 0xc0, 0x3e, 0x40, 0xcc, 0x04, 0xf2, 0x0c, 0xc0, 0xcc, 0x00, 0x8b, 0xb6, 0x0f, 0x00,
    0x0,
    //N s hackem
    0xD5, 7, 0x34, 0x0,
    0x04, 0x00, 0x02, 0x0f, 0x02, 0x00, 0xff, 0x0f, 0x05, 0x00, 0xdd, 0x0e, 0x70, 0x3e, 0x20, 0xed, 0x13, 0xe2, 0x0e, 0xf4, 0x3e, 0x20, 0xed, 0x93, 0xe2, 0x0e, 0x9c, 0x4e, 0x20, 0xe7, 0xa9, 0xe2, 0x0e, 0x74, 0xee, 0x20, 0xe7, 0x0e, 0xf2, 0x0e, 0x70, 0x7f, 0x20, 0xe7, 0x03, 0xe2, 0x0e, 0x00,
    //R s hackem
    0xFC, 6, 0x2e, 0x0,
    0x04, 0x00, 0x02, 0xf0, 0xfe, 0x00, 0x0f, 0x04, 0x00, 0xe2, 0x0c, 0xff, 0xb0, 0x0c, 0x93, 0xf7, 0x0c, 0x90, 0xe7, 0x0c, 0x94, 0xf7, 0x0c, 0xff, 0xb0, 0x0c, 0xce, 0x40, 0x0c, 0xac, 0x90, 0x0c, 0x97, 0xd1, 0x0c, 0x93, 0xe4, 0x0c, 0x90, 0xe9, 0x0c, 0x00,
    //S s hackem
    0xE6, 5, 0x29, 0x0,
    0x03, 0x00, 0x02, 0xf0, 0xfe, 0x00, 0xf0, 0x03, 0x00, 0xe7, 0x0a, 0xff, 0x43, 0xe4, 0x00, 0x4f, 0x20, 0x03, 0xf7, 0x00, 0x07, 0xf4, 0x00, 0x0c, 0xd1, 0x00, 0x0e, 0x70, 0x00, 0xc9, 0x00, 0x3f, 0x73, 0xff, 0xb1, 0x0a, 0x00,
    0x0,
    //T s hackem
    0x9B, 5, 0x29, 0x0,
    0x03, 0x00, 0x02, 0xf0, 0xfe, 0x00, 0xf0, 0x03, 0x00, 0xe7, 0x7f, 0xff, 0xd0, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x00, 0xe7, 0x00, 0x0e, 0x70, 0x0a, 0x00,
    0x0,
    //Z s hackem
    0xA6, 6, 0x2e, 0x0,
    0x04, 0x00, 0x02, 0xf0, 0xfe, 0x00, 0x0f, 0x04, 0x00, 0xe2, 0x0e, 0xff, 0xf7, 0x00, 0x03, 0xe2, 0x00, 0x0a, 0xc0, 0x00, 0x0e, 0x70, 0x00, 0x4f, 0x20, 0x00, 0xa9, 0x00, 0x03, 0xe4, 0x00, 0x07, 0xd1, 0x00, 0x0c, 0x90, 0x00, 0x3f, 0xff, 0xf7, 0x0c, 0x00,
    
    //? posledni znak se pouzije, pokud nenajde ascii code
    0x3f, 5, 0x22, 0x0,
    0x0a, 0x00, 0xec, 0x07, 0xdb, 0x13, 0xe2, 0xa7, 0x00, 0x0a, 0x90, 0x00, 0xa9, 0x00, 0x0c, 0x70, 0x0e, 0xe2, 0x00, 0xe4, 0x00, 0x0e, 0x40, 0x03, 0x00, 0xfe, 0x3e, 0x40, 0x0a, 0x00,
    //konec
    0x0, 0x0, 0x0, 0x0
};
