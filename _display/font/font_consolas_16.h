
//font consolas 16, color format 1-bit, 8x16, 

const FONT_HEAD font_consolas16 __section(".rodata") __attribute__((aligned(4))) = {
    0x1,                //fileID
    0x1,                //format
    8,                  //blockSize
    0,                  //compresion
    8,                  //width
    16,                 //Height (16)
    20,                 //itemSize
    0x20,               //firstAscii code
    0x80,               //firstVarAscii
    0,                  //style
    12,                 //lineOffset
    32,                 //dataOffset
    0,                  //dummy int
    "consolas16",       //name
};
const char font_consolas16_data[] __section(".rodata") = {
    //font data
    //ascii, width, data size16 (data + 4)
    
    //space
    0x20, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       
    //!
    0x21, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,        
       
    //"
    0x22, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x6c, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //#
    0x23, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0xfe, 0x24, 0x48, 0xfe, 0x48, 0x48, 0x48, 0x00, 0x00, 0x00,            
    
    //$
    0x24, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x08, 0x3c, 0x48, 0x50, 0x70, 0x1c, 0x16, 0x12, 0x12, 0x7c, 0x10, 0x10, 0x00,
    
    //%
    0x25, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x62, 0x94, 0x9c, 0x68, 0x10, 0x10, 0x26, 0x69, 0x49, 0x86, 0x00, 0x00, 0x00,
    
    //&
    0x26, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x30, 0x48, 0x48, 0x78, 0x30, 0x74, 0x94, 0x8c, 0x8c, 0x76, 0x00, 0x00, 0x00,
    
    //'
    0x27, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //(
    0x28, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x08, 0x04,
    
    //)
    0x29, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40,
    
    //*
    0x2A, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x08, 0x2a, 0x1c, 0x1c, 0x2a, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //+
    0x2B, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xfe, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
    
    //,
    0x2C, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x30, 0x00,
    
    //-
    0x2D, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //.
    0x2E, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
    
    ///
    0x2F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x02, 0x04, 0x04, 0x0c, 0x08, 0x18, 0x10, 0x10, 0x20, 0x20, 0x40, 0x00, 0x00,
    
    //0
    0x30, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x3c, 0x24, 0x42, 0x46, 0x5a, 0x62, 0x42, 0x24, 0x3c, 0x00, 0x00, 0x00,
    
    //1
    0x31, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x28, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00, 0x00,
    
    //2
    0x32, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x02, 0x02, 0x04, 0x0c, 0x18, 0x30, 0x7e, 0x00, 0x00, 0x00,
    
    //3
    0x33, 0x8, 0x14, 0x0,       
    0x00, 0x00, 0x00, 0x00, 0x78, 0x04, 0x04, 0x04, 0x38, 0x04, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00,
    
    //4
    0x34, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x28, 0x68, 0x48, 0x88, 0xfe, 0x08, 0x08, 0x00, 0x00, 0x00,
    
    //5
    0x35, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7c, 0x40, 0x40, 0x40, 0x78, 0x04, 0x04, 0x0c, 0x78, 0x00, 0x00, 0x00,
    
    //6
    0x36, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x1c, 0x20, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x66, 0x3c, 0x00, 0x00, 0x00,
    
    //7
    0x37, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7e, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x30, 0x00, 0x00, 0x00,
    
    //8
    0x38, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x66, 0x18, 0x66, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00,
    
    //9
    0x39, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0x42, 0x42, 0x46, 0x3a, 0x02, 0x04, 0x38, 0x00, 0x00, 0x00,
    
    //:
    0x3A, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
    
    //;
    0x3B, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08, 0x30, 0x00,
    
    //<
    0x3C, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00,
    
    //=
    0x3D, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //>
    0x3E, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00,
    
    //?
    0x3F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x30, 0x08, 0x04, 0x04, 0x04, 0x38, 0x20, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00,
    
    //@
    0x40, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x1c, 0x22, 0x41, 0x41, 0x9d, 0xb5, 0xa5, 0xa5, 0xa5, 0xbe, 0x80, 0x44, 0x38,
    
    //A
    0x41, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x28, 0x28, 0x6c, 0x44, 0x44, 0xfe, 0x82, 0x82, 0x00, 0x00, 0x00,
    
    //B
    0x42, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x46, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00, 0x00,
    
    //C
    0x43, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x1c, 0x22, 0x40, 0x40, 0x40, 0x40, 0x40, 0x22, 0x1c, 0x00, 0x00, 0x00,
    
    //D
    0x44, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x00,
    
    //E
    0x45, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7c, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x7c, 0x00, 0x00, 0x00,
    
    //F
    0x46, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7c, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,
    
    //G
    0x47, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x80, 0x80, 0x9c, 0x84, 0x84, 0x44, 0x3c, 0x00, 0x00, 0x00,
    
    //H
    0x48, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00,
    
    //I
    0x49, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00,
    
    //J
    0x4A, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00,
    
    //K
    0x4B, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00, 0x00,
    
    //L
    0x4C, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3e, 0x00, 0x00, 0x00,
    
    //M
    0x4D, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x26, 0x66, 0x5a, 0x5a, 0x5a, 0x52, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00,
    
    //N
    0x4E, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x62, 0x62, 0x52, 0x52, 0x52, 0x4a, 0x4a, 0x46, 0x46, 0x00, 0x00, 0x00,
    
    //O
    0x4F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38, 0x00, 0x00, 0x00,
    
    //P
    0x50, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7c, 0x46, 0x42, 0x42, 0x46, 0x78, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,
    
    //Q
    0x51, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x78, 0x10, 0x0e, 0x00,
    
    //R
    0x52, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x78, 0x48, 0x4c, 0x44, 0x46, 0x00, 0x00, 0x00,
    
    //S
    0x53, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x3c, 0x40, 0x40, 0x60, 0x1c, 0x06, 0x02, 0x02, 0x7c, 0x00, 0x00, 0x00,
    
    //T
    0x54, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
    
    //U
    0x55, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3c, 0x00, 0x00, 0x00,
    
    //V
    0x56, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x63, 0x22, 0x22, 0x36, 0x14, 0x14, 0x18, 0x00, 0x00, 0x00,
    
    //W
    0x57, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x69, 0x6b, 0x2a, 0x3a, 0x36, 0x36, 0x00, 0x00, 0x00,
    
    //X
    0x58, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0xc6, 0x64, 0x28, 0x18, 0x18, 0x38, 0x24, 0x46, 0xc3, 0x00, 0x00, 0x00,
    
    //Y
    0x59, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x82, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
    
    //Z
    0x5A, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x7e, 0x04, 0x04, 0x08, 0x18, 0x10, 0x20, 0x20, 0x7e, 0x00, 0x00, 0x00,
    
    //[
    0x5B, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1c,
    
    //
    0x5C, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x40, 0x20, 0x20, 0x10, 0x10, 0x18, 0x08, 0x0c, 0x04, 0x04, 0x02, 0x00, 0x00,
    
    //]
    0x5D, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38,
    
    //^
    0x5E, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x28, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //_
    0x5F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    
    //`
    0x60, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //a
    0x61, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x04, 0x3c, 0x44, 0x4c, 0x3c, 0x00, 0x00, 0x00,
    
    //b
    0x62, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x5c, 0x66, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x00,
    
    //c
    0x63, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x20, 0x40, 0x40, 0x40, 0x60, 0x3c, 0x00, 0x00, 0x00,
    
    //d
    0x64, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x1e, 0x22, 0x42, 0x42, 0x42, 0x66, 0x3a, 0x00, 0x00, 0x00,
    
    //e
    0x65, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0x42, 0x7e, 0x40, 0x60, 0x3e, 0x00, 0x00, 0x00,
    
    //f
    0x66, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x0e, 0x10, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
    
    //g
    0x67, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x44, 0x44, 0x44, 0x78, 0x40, 0x7c, 0x42, 0x42, 0x3c,
    
    //h
    0x68, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x58, 0x64, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00,
    
    //i
    0x69, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7e, 0x00, 0x00, 0x00,
    
    //j
    0x6A, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x7c, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0c, 0x78,
    
    //k
    0x6B, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00, 0x00, 0x00,
    
    //l
    0x6C, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7e, 0x00, 0x00, 0x00,
    
    //m
    0x6D, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xda, 0x92, 0x92, 0x92, 0x92, 0x92, 0x00, 0x00, 0x00,
    
    //n
    0x6E, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x64, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00,
    
    //o
    0x6F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0x42, 0x42, 0x42, 0x66, 0x3c, 0x00, 0x00, 0x00,
    
    //p
    0x70, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x66, 0x42, 0x42, 0x42, 0x44, 0x78, 0x40, 0x40, 0x40,
    
    //q
    0x71, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x22, 0x42, 0x42, 0x42, 0x66, 0x3a, 0x02, 0x02, 0x02,
    
    //r
    0x72, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x64, 0x44, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,
    
    //s
    0x73, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x40, 0x40, 0x38, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00,
    
    //t
    0x74, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0xfc, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1c, 0x00, 0x00, 0x00,
    
    //u
    0x75, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4c, 0x34, 0x00, 0x00, 0x00,
    
    //v
    0x76, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x32, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00, 0x00,
    
    //w
    0x77, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x82, 0x92, 0xaa, 0xaa, 0xaa, 0x44, 0x00, 0x00, 0x00,
    
    //x
    0x78, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x24, 0x18, 0x18, 0x24, 0x66, 0xc3, 0x00, 0x00, 0x00,
    
    //y
    0x79, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x36, 0x14, 0x14, 0x14, 0x08, 0x08, 0x10, 0x70,
    
    //z
    0x7A, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x08, 0x08, 0x10, 0x20, 0x20, 0x7c, 0x00, 0x00, 0x00,
    
    //{
    0x7B, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x0c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x60, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0c,
    
    //|
    0x7C, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    
    //}
    0x7D, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x30, 0x08, 0x08, 0x08, 0x08, 0x08, 0x06, 0x08, 0x08, 0x08, 0x08, 0x08, 0x30,
    
    //~
    0x7E, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x92, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00,
    
    //�
    0x7F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    
            
    //diakritika        
    //�
    0xE1, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x18, 0x24, 0x04, 0x3c, 0x44, 0x4c, 0x3c, 0x00, 0x00, 0x00,
    
    //�
    0xE9, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x3c, 0x66, 0x42, 0x7e, 0x40, 0x60, 0x3e, 0x00, 0x00, 0x00,
    
    //�
    0xED, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7e, 0x00, 0x00, 0x00,
    
    //�
    0xF3, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x3c, 0x66, 0x42, 0x42, 0x42, 0x66, 0x3c, 0x00, 0x00, 0x00,
    
    //�
    0xFA, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4c, 0x34, 0x00, 0x00, 0x00,
    
    //�
    0xFD, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x22, 0x22, 0x36, 0x14, 0x14, 0x14, 0x08, 0x08, 0x10, 0x70,            

    //e s hackem
    0xD8, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x14, 0x08, 0x00, 0x3c, 0x66, 0x42, 0x7e, 0x40, 0x60, 0x3e, 0x00, 0x00, 0x00,
    
    //u s krouzkem
    0xDE, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x10, 0x28, 0x10, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4c, 0x34, 0x00, 0x00, 0x00,
  
    //�
    0xC1, 0x8, 0x14, 0x0,
    0x00, 0x04, 0x08, 0x00, 0x18, 0x28, 0x28, 0x6c, 0x44, 0x44, 0xfe, 0x82, 0x82, 0x00, 0x00, 0x00,
    
    //�
    0xC9, 0x8, 0x14, 0x0,
    0x00, 0x08, 0x10, 0x00, 0x7c, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x7c, 0x00, 0x00, 0x00,
    
    //�
    0xCD, 0x8, 0x14, 0x0,
    0x00, 0x08, 0x10, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00,
    
    //�
    0xD3, 0x8, 0x14, 0x0,
    0x00, 0x08, 0x10, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38, 0x00, 0x00, 0x00,
    
    //�
    0xDA, 0x8, 0x14, 0x0,
    0x00, 0x08, 0x10, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x46, 0x3c, 0x00, 0x00, 0x00,
    
    //�
    0xDD, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x08, 0x10, 0x82, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
    
    //c s hackem
    0x9F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x14, 0x08, 0x00, 0x3c, 0x20, 0x40, 0x40, 0x40, 0x60, 0x3c, 0x00, 0x00, 0x00,
    
    //d s hackem
    0xD4, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x14, 0x0a, 0x02, 0x02, 0x1e, 0x22, 0x42, 0x42, 0x42, 0x66, 0x3a, 0x00, 0x00, 0x00,
    
    //n s hackem
    0xE5, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x64, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00,
    
    //r s hackem
    0xFE, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x28, 0x10, 0x00, 0x58, 0x64, 0x44, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,

    //s s hackem
    0xE7, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x28, 0x10, 0x00, 0x3c, 0x40, 0x40, 0x38, 0x04, 0x04, 0x78, 0x00, 0x00, 0x00,

    //t s hackem
    0x9C, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x14, 0x28, 0x20, 0xfc, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1c, 0x00, 0x00, 0x00,

    //z s hackem
    0xA7, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x28, 0x10, 0x00, 0x7c, 0x08, 0x08, 0x10, 0x20, 0x20, 0x7c, 0x00, 0x00, 0x00,
    
    //C s hackem
    0xAC, 0x8, 0x14, 0x0,
    0x00, 0x14, 0x08, 0x00, 0x1c, 0x22, 0x40, 0x40, 0x40, 0x40, 0x40, 0x22, 0x1c, 0x00, 0x00, 0x00,
    
    //D s hackem
    0xD2, 0x8, 0x14, 0x0,
    0x00, 0x28, 0x10, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x00,
    
    //N s hackem
    0xD5, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x00, 0x62, 0x62, 0x52, 0x52, 0x52, 0x4a, 0x4a, 0x46, 0x46, 0x00, 0x00, 0x00,
    
    //R s hackem
    0xFC, 0x8, 0x14, 0x0,
    0x00, 0x28, 0x10, 0x00, 0x78, 0x44, 0x44, 0x44, 0x78, 0x48, 0x4c, 0x44, 0x46, 0x00, 0x00, 0x00,
    
    //S s hackem
    0xE6, 0x8, 0x14, 0x0,
    0x00, 0x28, 0x10, 0x00, 0x3c, 0x40, 0x40, 0x60, 0x1c, 0x06, 0x02, 0x02, 0x7c, 0x00, 0x00, 0x00,
    
    //T s hackem
    0x9B, 0x8, 0x14, 0x0,
    0x00, 0x28, 0x10, 0x00, 0xfe, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00,
    
    //Z s hackem
    0xA6, 0x8, 0x14, 0x0,
    0x00, 0x28, 0x10, 0x00, 0x7e, 0x04, 0x04, 0x08, 0x18, 0x10, 0x20, 0x20, 0x7e, 0x00, 0x00, 0x00,

    //? posledni znak pouzije, pokud nenjade ascii code
    0x3F, 0x8, 0x14, 0x0,
    0x00, 0x00, 0x00, 0x30, 0x08, 0x04, 0x04, 0x04, 0x38, 0x20, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00,
    
    //konec
    0x0, 0x0, 0x0, 0x0
    
};
