//
// GBA.h
//
// GBA開発用ヘッダファイル
// メモリ番地、各レジスタ用フラグ、マクロほか定数の定義
//
// Modified by Hayato Kohara
//

#ifndef _GBA_H_
#define _GBA_H_

// 型・マクロ定義

typedef unsigned short hword;       // Half word (2 bytes)
typedef unsigned char  byte;        // Byte

#define gba_register(p)  *((volatile hword*) p) // I/O register access 2 bytes
#define gba_register4(p) *((volatile int*) p)   // I/O register access 4 bytes

// GBA memory map
    // IOベースレジスタ
#define IOBASE          0x04000000  // I/O register base address
    // パレット
#define BG_PALETTE      0x05000000  // Background palette RAM
#define OBJ_PALETTE     0x05000200  // Object palette RAM
    // VRAM
#define VRAM            0x06000000  // Video RAM frame buffer
#define VRAM_TILE(n)  (VRAM+n*0x4000) // Tile data base address
#define VRAM_MAP(n)   (VRAM+n*0x0800) // Map data base address
    // オブジェクト(スプライト)
#define OBJ_CHAR_TILE   0x06010000  // Object character base in tile mode
#define OBJ_CHAR_BITMAP 0x06014000  // Object character base in bitmap mode
    // OAM
#define OAM             0x07000000  // Object Attribute Memory

// LCD parameters

#define LCD_WIDTH   240     // 240 dots / horizontal line
#define LCD_HEIGHT  160     // 160 dots / vertical line
#define LCD_RWIDTH  30      // Real width in tile
#define LCD_RHEIGHT 20      // Real height in tile
#define LCD_VWIDTH  32      // Virtual width in tile
#define LCD_VHEIGHT 32      // Virtual height in tile
#define LCD_TILE_BYTE   (8 * 8)     // Byte size of single tile
#define LCD_TILE_HWORD  (8 * 4)     // Half word size of single tile
#define LCD_PAL_COLORS  256     // Total color number per palette
#define RGB(r, g, b)    (((b) << 10) + ((g) << 5) + (r))

// LCD controller

#define LCD_CTRL    (IOBASE+ 0x00)  // LCD control
#define LCD_BLANK 0x0080      //   Forced blank
#define LCD_BG0EN 0x0100      //   Enable BG0
#define LCD_BG1EN 0x0200      //   Enable BG1
#define LCD_BG2EN 0x0400      //   Enable BG2
#define LCD_BG3EN 0x0800      //   Enable BG3
#define LCD_OBJEN 0x1000      //   Enable OBJ
#define LCD_OBJMAP2D  0       //   OBJ 2D mapping
#define LCD_OBJMAP1D  0x0040      //   OBJ 1D mapping
#define LCD_MODE0 0       //   Video mode 0
#define LCD_MODE1 1       //   Video mode 1
#define LCD_MODE2 2       //   Video mode 2
#define LCD_MODE3 3       //   Video mode 3
#define LCD_MODE4 4       //   Video mode 4
#define LCD_MODE5 5       //   Video mode 5
#define LCD_STATUS  (IOBASE+ 0x04)  // LCD status
#define LCD_VCINTEN   0x0020      //   Enable V-counter match INT
#define LCD_HBINTEN   0x0010      //   Enable H-blank INT
#define LCD_VBINTEN   0x0008      //   Enable V-blank INT
#define LCD_BG0     (IOBASE+ 0x08)  // LCD BG0 control
#define LCD_BG1     (IOBASE+ 0x0A)  // LCD BG1 control
#define LCD_BG2     (IOBASE+ 0x0C)  // LCD BG2 control
#define LCD_BG3     (IOBASE+ 0x0E)  // LCD BG3 control
#define LCD_SIZE3232  0x0000      //   BG size 32 x 32 tiles
#define LCD_SIZE6432  0x4000      //   BG size 64 x 32 tiles
#define LCD_SIZE3264  0x8000      //   BG size 32 x 64 tiles
#define LCD_SIZE6464  0xC000      //   BG size 64 x 64 tiles
#define LCD_COLOR16   0       //   BG 16 colors x 16 palettes
#define LCD_COLOR256  0x0080      //   BG 256 colors x 1 palette
#define LCD_BGTILE(n) (n << 2)    //   BG tile base block #
#define LCD_BGMAP(n)  (n << 8)    //   BG map base block #
#define LCD_BG0_HOFFSET (IOBASE+ 0x10)  // LCD BG0 Horizontal offset
#define LCD_BG0_VOFFSET (IOBASE+ 0x12)  // LCD BG0 Vertical offset
#define LCD_BG1_HOFFSET (IOBASE+ 0x14)  // LCD BG1 Horizontal offset
#define LCD_BG1_VOFFSET (IOBASE+ 0x16)  // LCD BG1 Vertical offset
#define LCD_BG2_HOFFSET (IOBASE+ 0x18)  // LCD BG2 Horizontal offset
#define LCD_BG2_VOFFSET (IOBASE+ 0x1A)  // LCD BG2 Vertical offset
#define LCD_BG3_HOFFSET (IOBASE+ 0x1C)  // LCD BG3 Horizontal offset
#define LCD_BG3_VOFFSET (IOBASE+ 0x1E)  // LCD BG3 Vertical offset

// Screen module

typedef struct {
  hword* map;   // Map data pointer
  int x, y; // Position
 } screen;

#define BG_BLACK    0       // BG color: black
#define BG_WHITE    1       // BG color: white

#define BG_TEXT         0       // テキストレイヤー
#define BG_MAPCHIP      1       // マップチップレイヤー
#define BG_LANDSCAPE1   2       // 背景レイヤー1
#define BG_LANDSCAPE2   3       // 背景レイヤー2

// Keypad

#define KEY_STATUS  (IOBASE+ 0x130) // Key status
#define KEY_INTCNT  (IOBASE+ 0x132) // Key interrupt controller
#define KEY_INTAND    0x8000      //   Mode flag (0: OR, 1:AND)
#define KEY_INTEN     0x4000      //   Interrupt request enable
#define KEY_L         0x200       //   L
#define KEY_R         0x100       //   R
#define KEY_DOWN      0x080       //   Down
#define KEY_UP        0x040       //   Up
#define KEY_LEFT      0x020       //   Left
#define KEY_RIGHT     0x010       //   Right
#define KEY_START     0x008       //   Start
#define KEY_SELECT    0x004       //   Select
#define KEY_B         0x002       //   B
#define KEY_A         0x001       //   A
#define KEY_ALL       0x3FF       // All key bits

// Timer controller

#define TMR_COUNT0  (IOBASE+ 0x100) // Timer 0 counter
#define TMR_CTRL0   (IOBASE+ 0x102) // Timer 0 control
#define TMR_COUNT1  (IOBASE+ 0x104) // Timer 1 counter
#define TMR_CTRL1   (IOBASE+ 0x106) // Timer 1 control
#define TMR_COUNT2  (IOBASE+ 0x108) // Timer 2 counter
#define TMR_CTRL2   (IOBASE+ 0x10A) // Timer 2 control
#define TMR_COUNT3  (IOBASE+ 0x10C) // Timer 3 counter
#define TMR_CTRL3   (IOBASE+ 0x10E) // Timer 3 control

#define TMR_ENABLE    0x0080      // Timer enable
#define TMR_INTREN    0x0040      // Timer interrupt request enable
#define TMR_CASCADE   0x0004      // Cascade mode (non prescalar mode)
#define TMR_1CLOCK    0x0000      // 1 clock
#define TMR_64CLOCK   0x0001      // 64 clocks
#define TMR_256CLOCK  0x0002      // 256 clocks
#define TMR_1024CLOCK 0x0003      // 1024 clocks

// Interrupt controller

#define INT_VECTOR  0x03007FFC  // Interrupt address in internal WRAM
#define INT_IME     (IOBASE+ 0x208) // IME: Interrupt Master Enable
#define INT_IME_OFF   0       //   Disable
#define INT_IME_ON    1       //   Enable
#define INT_IE      (IOBASE+ 0x200) // IE: Interrupt Enable
#define INT_IF      (IOBASE+ 0x202) // IF: Interrupt request Flag
#define INT_PAK   0x2000      //   Game pak
#define INT_KEY   0x1000      //   Key
#define INT_DMA3  0x0800      //   DMA ch3
#define INT_DMA2  0x0400      //   DMA ch2
#define INT_DMA1  0x0200      //   DMA ch1
#define INT_DMA0  0x0100      //   DMA ch0
#define INT_SIO   0x0080      //   Serial I/O
#define INT_TMR3  0x0040      //   Timer ch3
#define INT_TMR2  0x0020      //   Timer ch2
#define INT_TMR1  0x0010      //   Timer ch1
#define INT_TMR0  0x0008      //   Timer ch0
#define INT_VCOUNT    0x0004      //   V-counter matching
#define INT_HBLANK    0x0002      //   Horizontal blanking
#define INT_VBLANK    0x0001      //   Vertical blanking

// Direct sound
#define SND_MASTER  (IOBASE+ 0x84)  // Master sound control
#define SND_ENABLE    0x80          // Enable sound circuit

#define SND_DOUTPUT (IOBASE+ 0x82)  // Direct sound output control
    //   Direct sound B
#define SND_DBRESET   0x8000        // Reset FIFO
#define SND_DBTIMER1  0x4000        // Select timer1 (0: timer0)
#define SND_DBLEFT    0x2000        // Output to left channel
#define SND_DBRIGHT   0x1000        // Output to right channel
#define SND_DBFULL    0x0008        // Full range (0: 50%)
    //   Direct sound A
#define SND_DARESET   0x0800        // Reset FIFO
#define SND_DATIMER   0x0400        // Select timer1 (0: timer0)
#define SND_DALEFT    0x0200        // Output to left channel
#define SND_DARIGHT   0x0100        // Output to right channel
#define SND_DAFULL    0x0004        // Full range (0: 50%)

#define SND_DAFIFO  (IOBASE+ 0xA0)  // Direct sound A FIFO
#define SND_DBFIFO  (IOBASE+ 0xA4)  // Direct sound B FIFO

// DMA channels
    // DMA channel 1
#define DMA_SRCAD1  (IOBASE+ 0xBC)  // Source address
#define DMA_DSTAD1  (IOBASE+ 0xC0)  // Destination address
#define DMA_COUNT1  (IOBASE+ 0xC4)  // Word count
#define DMA_CTRL1   (IOBASE+ 0xC6)  // DMA control
    // DMA channel 2
#define DMA_SRCAD2  (IOBASE+ 0xC8)  // Source address
#define DMA_DSTAD2  (IOBASE+ 0xCC)  // Destination address
#define DMA_COUNT2  (IOBASE+ 0xD0)  // Word count
#define DMA_CTRL2   (IOBASE+ 0xD2)  // DMA control
    // DMA channel 3
#define DMA_SRCAD3  (IOBASE+ 0xD4)  // Source address
#define DMA_DSTAD3  (IOBASE+ 0xD8)  // Destination address
#define DMA_COUNT3  (IOBASE+ 0xDC)  // Word count
#define DMA_CTRL3   (IOBASE+ 0xDE)  // DMA control
#define DMA_ENABLE    0x8000      //   DMA enable
#define DMA_INTREN    0x4000      //   Interrupt request enable
    //   DMA start timing
#define DMA_STARTIMD  0x0000      //     Start immediately
#define DMA_STARTVBL  0x1000      //     Start during a V-blank
#define DMA_STARTHBL  0x2000      //     Start during a H-blank
#define DMA_STARTFIFO 0x3000      //     Start when requested by FIFO
#define DMA_SIZE32    0x0400      //   Transfer type (0: 16bit)
#define DMA_REPEAT    0x0200      //   DMA repeat
    //   Source address control
#define DMA_SRCINC    0x0000      //     Increment
#define DMA_SRCDEC    0x0080      //     Decrement
#define DMA_SRCFIX    0x0100      //     Fixed
    //   Destination address control
#define DMA_DSTINC    0x0000      //     Increment
#define DMA_DSTDEC    0x0020      //     Decrement
#define DMA_DSTFIX    0x0040      //     Fixed
#define DMA_DSTREL    0x0060      //     Increment and reload

// Object data (sprites)

// スプライト画像データ
typedef struct {
    hword data[32]; // 8x8=64 px -> 64 byte = 32 hword (1hwordに2px分)
 } character;

// スプライト制御レジスタ
typedef struct {
    hword attr0;              // OBJ attribute 0
    hword attr1;              // OBJ attribute 1
    hword attr2;              // OBJ attribute 2
    hword padd;               // We ignore rotate/scale data
 } sprite;

#define OBJ_MAX_SPRITES 128     // Maximum number of sprites
#define OBJ_MAX_CHARS   1024    // Maximum number of 32B characters
#define OBJ_SHAPE_SQ    0x0000  // Shape: square type
#define OBJ_SHAPE_HR    0x4000  // Shape: horizontal rectangle type
#define OBJ_SHAPE_VR    0x8000  // Shape: vertical rectangle type
#define OBJ_COLOR_256   0x2000  // Color mode: 256 colors x 1 palette
#define OBJ_Y_MASK      0x00FF  // Y coordinate bit mask (lower 8 bits)
#define OBJ_SW_MASK     0x0300  // OBJ display switch bit mask
#define OBJ_SW_TURNOFF  0x0200  // Turn off OBJ

#define OBJ_X_MASK      0x01FF  // X coordinate bit mask (lower 9 bits)
#define OBJ_SIZE_MASK   0xC000  // OBJ size bit mask

#define OBJ_FLIP_HORIZONTAL 0x1000  // 水平反転
#define OBJ_FLIP_VERTICAL   0x2000  // 垂直反転

#define OBJ_CHAR_MASK   0x03FF  // Character# bit mask (lower 10 bits)
#define OBJ_PRIOR_MASK  0x0C00  // OBJ priority bit mask

// Object display switch

#define OBJ_TURNOFF 0
#define OBJ_TURNON  1

// Object size index

#define OBJ_8x8     0
#define OBJ_16x16   1
#define OBJ_32x32   2
#define OBJ_64x64   3
#define OBJ_16x8    4
#define OBJ_32x8    5
#define OBJ_32x16   6
#define OBJ_64x32   7
#define OBJ_8x16    8
#define OBJ_8x32    9
#define OBJ_16x32   10
#define OBJ_32x64   11

// スプライトのサイズ
#define OBJ_SIZE_8x8    1
#define OBJ_SIZE_16x16  4
#define OBJ_SIZE_32x32  16

// Priority index

#define PRIORITY_1  0       // 1st priority
#define PRIORITY_2  1       // 2nd priority
#define PRIORITY_3  2       // 3rd priority
#define PRIORITY_4  3       // 4th priority

// 固定小数点
// 必要に応じて上位ビットをsignedとすること
typedef struct {
    hword x;
    hword y;
} point;

typedef struct {
    byte xl;    // x小数部
    byte xh;    // x整数部
    byte yl;    // y小数部
    byte yh;    // y整数部
} fixed_pos;

typedef union {
    point     w;
    fixed_pos b;
} fixed;

static const hword sin_tbl[] = {
        0,    6,   12,   18,   25,   31,   37,   43,   49,   56,   62,   68,   74,   80,   86,   92,   97,  103,  109,  115,  120,  126,  131,  136,  142,  147,  152,  157,  162,  167,  171,  176,  181,  185,  189,  193,  197,  201,  205,  209,  212,  216,  219,  222,  225,  228,  231,  234,  236,  238,  241,  243,  244,  246,  248,  249,  251,  252,  253,  254,  254,  255,  255,  255,
      256,  255,  255,  255,  254,  254,  253,  252,  251,  249,  248,  246,  244,  243,  241,  238,  236,  234,  231,  228,  225,  222,  219,  216,  212,  209,  205,  201,  197,  193,  189,  185,  181,  176,  171,  167,  162,  157,  152,  147,  142,  136,  131,  126,  120,  115,  109,  103,   97,   92,   86,   80,   74,   68,   62,   56,   49,   43,   37,   31,   25,   18,   12,    6,
        0,   -6,  -12,  -18,  -25,  -31,  -37,  -43,  -49,  -56,  -62,  -68,  -74,  -80,  -86,  -92,  -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176, -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234, -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255,
     -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238, -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185, -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103,  -97,  -92,  -86,  -80,  -74,  -68,  -62,  -56,  -49,  -43,  -37,  -31,  -25,  -18,  -12,   -6
};

#endif
