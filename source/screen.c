//
// screen.c
//
// BGスクリーンドライバ
//
// by Hayato Kohara
//

#include "gba.h"
#include "screen.h"
#include "game.h"

/*
    画像データ
    Gritで色深度8bit(=1Byte)、8bit(=1Byte, char)配列でタイルマップを作成
    grit ./resouce/bg.png -o ./source/bg.h -gt -gB8 -ftc -gu8
*/
#include "bg.h"     // 画像とパレットデータ
#include "bgmap.h"  // bg0Map[],bg1Map[],bg2Map[]

// Screen information

static screen screens[4] = {
    { (hword*) VRAM_MAP(21), 0, 0 }, // Page 0
    { (hword*) VRAM_MAP(22), 0, 0 }, // Page 1
    { (hword*) VRAM_MAP(23), 0, 0 }, // Page 2
    { (hword*) VRAM_MAP(24), 0, 0 }, // Page 3
};

static hword* tile = (hword*) VRAM_TILE(0);    // タイルは0番ブロックに1つのみ
static hword* bcolor = (hword*) BG_PALETTE;    // 0x05000000

// BG1 スクロール用
static char  bg1_offset;
static short bg1_counter;
static char  bg1_anim;

// BG2,3 スクロール用
static char ls_counter;
static char bg2_offset;
static char bg3_offset;

// 画面振動用
static char rumble;

void screen_init()
{
    int i;

    // パレットの設定
    for (i = 0; i < LCD_PAL_COLORS; i++)
        bcolor[i] = bgPal[i];

    // タイルのデータの読み込み(仮)
    // 表示するタイルによって使用するVRAMの大きさが異なるので要検討
    for (i = 0; i < bgTilesLen>>1; i++) // char型のタイルデータをhword型に書き込むため
        tile[i] = bgTiles[2*i] + (bgTiles[2*i + 1] << 8);

    // BGを有効化
    gba_register(LCD_BG0)  = LCD_SIZE3232  | LCD_COLOR256  |
                             LCD_BGTILE(0) | LCD_BGMAP(21) | PRIORITY_1;
    gba_register(LCD_BG1)  = LCD_SIZE3232  | LCD_COLOR256  |
                             LCD_BGTILE(0) | LCD_BGMAP(22) | PRIORITY_2;
    gba_register(LCD_BG2)  = LCD_SIZE3232  | LCD_COLOR256  |
                             LCD_BGTILE(0) | LCD_BGMAP(23) | PRIORITY_3;
    gba_register(LCD_BG3)  = LCD_SIZE3232  | LCD_COLOR256  |
                             LCD_BGTILE(0) | LCD_BGMAP(24) | PRIORITY_4;
    gba_register(LCD_CTRL) = LCD_BG0EN | LCD_BG1EN | LCD_BG2EN | LCD_BG3EN | LCD_MODE0;

}

void screen_title_init()
{
    int i;

    // BG2,3 スクロールの初期化
    ls_counter = 0;
    bg2_offset = 0;
    bg3_offset = 0;

    rumble = 0;

    gba_register(LCD_BG1_HOFFSET) = 0; // スクロール(8を加えてループ位置を調整)
    gba_register(LCD_BG2_VOFFSET) = 0;
    gba_register(LCD_BG3_VOFFSET) = 0;

    // BG0,2,3のマッピング(仮)
    for (i = 0; i < LCD_VWIDTH * LCD_VHEIGHT; i++) {
        screens[BG_TEXT].map[i]       = 0;
        screens[BG_MAPCHIP].map[i]    = 0;
        screens[BG_LANDSCAPE1].map[i] = 0;
        screens[BG_LANDSCAPE2].map[i] = 0;
        if (bg0TitleMap[i]!=0)
            screens[BG_TEXT].map[i]       = bg0TitleMap[i] - 1;
        if (bg1TitleMap[i]!=0)
            screens[BG_MAPCHIP].map[i]    = bg1TitleMap[i] - 1;
        if (bg2Map[i]!=0)
            screens[BG_LANDSCAPE1].map[i] = bg2Map[i] - 1;
        if (bg3Map[i]!=0)
            screens[BG_LANDSCAPE2].map[i] = bg3Map[i] - 1;
    }
}

void screen_title_update()
{
    // BG2, 3 スクロール, 点滅カウント
    ls_counter++;
    bg2_offset++;
    if (ls_counter & 1) bg3_offset++;
    int i;
    for (i = 510; i < 540; i++) {
        screens[BG_TEXT].map[i] = 0;
        if ((ls_counter & 63) < 40)
            screens[BG_TEXT].map[i] = bg0TitleMap[i] - 1;
    }

    gba_register(LCD_BG2_HOFFSET) = bg2_offset;
    gba_register(LCD_BG3_HOFFSET) = bg3_offset;
}

void screen_ready_init()
{
    int i;

    // BG2,3 スクロールの初期化
    ls_counter = 0;
    rumble     = 0;

    // BG0,2,3のマッピング(仮)
    for (i = 0; i < LCD_VWIDTH * LCD_VHEIGHT; i++) {
        screens[BG_TEXT].map[i]       = 0;
        screens[BG_MAPCHIP].map[i]    = 0;
        if (bg0ReadyMap[i]!=0)
            screens[BG_TEXT].map[i]       = bg0ReadyMap[i] - 1;
    }
}

void screen_ready_update()
{
    // 点滅カウント
    ls_counter++;
    int i;
    for (i = 360; i < 390; i++) {
        screens[BG_TEXT].map[i] = 0;
        if ((ls_counter & 31) < 20)
            screens[BG_TEXT].map[i] = bg0ReadyMap[i] - 1;
    }

    if (ls_counter == 128) {
        screen_game_init();
        game_set_state(GAME);
    }
}

void screen_game_init()
{
    int i;

    // BG1 スクロールの初期化
    bg1_offset  = 0;
    bg1_counter = 0;
    bg1_anim    = 0;
    rumble      = 0;

    // BG0,2,3のマッピング(仮)
    for (i = 0; i < LCD_VWIDTH * LCD_VHEIGHT; i++) {
        screens[BG_TEXT].map[i]       = 0;
        screens[BG_MAPCHIP].map[i]    = 0;
        if (bg0Map[i]!=0)
            screens[BG_TEXT].map[i]       = bg0Map[i] - 1;
    }
}

void screen_game_update()
{
    // BG1 スクロール
    bg1_offset++;
    int i;
    if ((bg1_offset & 7) == 7) { // はみ出たぶんを1列ずらす
        for (i = 0; i < LCD_RHEIGHT; i++) {
            if (bg1Map[(1024*i) + bg1_counter])                      
                screens[BG_MAPCHIP].map[(LCD_VWIDTH*i) + ((bg1_offset-1)>>3)] = 64; // (>>3) = (/8)
            else
                screens[BG_MAPCHIP].map[(LCD_VWIDTH*i) + ((bg1_offset-1)>>3)] = 0;
        }
        bg1_counter++;
        bg1_anim = (bg1_anim+1) & 7;
        if (bg1_counter>1023) bg1_counter=0;
    }
    gba_register(LCD_BG1_HOFFSET) = bg1_offset + 8; // スクロール(8を加えてループ位置を調整)

    // BG2, 3 スクロール
    ls_counter++;
    if (ls_counter & 3) bg2_offset++;
    if (ls_counter & 1) bg3_offset++;
    gba_register(LCD_BG2_HOFFSET) = bg2_offset;
    gba_register(LCD_BG3_HOFFSET) = bg3_offset;

    // 画面振動
    if (rumble) {
        gba_register(LCD_BG2_VOFFSET) = bg2_offset & 3;
        gba_register(LCD_BG3_VOFFSET) = bg2_offset & 3;
    } else {
        gba_register(LCD_BG2_VOFFSET) = 0;
        gba_register(LCD_BG3_VOFFSET) = 0;
    }

    // コインのアニメーション
    for (i = 0; i < LCD_VWIDTH * LCD_VHEIGHT; i++) {
        if (screens[BG_MAPCHIP].map[i] != 0)
            screens[BG_MAPCHIP].map[i] = 64 + bg1_anim;
    }
}

void screen_over_init()
{
    int i;
    ls_counter = 0;
    // BG0,2,3のマッピング(仮)
    for (i = 0; i < LCD_VWIDTH * LCD_VHEIGHT; i++) {
        screens[BG_TEXT].map[i]       = 0;
        screens[BG_MAPCHIP].map[i]    = 0;
        if (bg0OverMap[i]!=0)
            screens[BG_TEXT].map[i] = bg0OverMap[i] - 1;
    }
}

void screen_over_update()
{
    ls_counter++;
    int i;
    for (i = 510; i < 540; i++) {
        screens[BG_TEXT].map[i] = 0;
        if ((ls_counter & 63) < 40)
            screens[BG_TEXT].map[i] = bg0OverMap[i] - 1;
    }
}

void screen_draw_score(int a, int b, int c, int d)
{
    screens[BG_TEXT].map[23] = a + 1;
    screens[BG_TEXT].map[24] = b + 1;
    screens[BG_TEXT].map[25] = c + 1;
    screens[BG_TEXT].map[26] = d + 1;
}

void screen_draw_over_score(int a, int b, int c, int d)
{
    screens[BG_TEXT].map[436] = a + 1;
    screens[BG_TEXT].map[437] = b + 1;
    screens[BG_TEXT].map[438] = c + 1;
    screens[BG_TEXT].map[439] = d + 1;
}

void screen_draw_over_hscore(int a, int b, int c, int d)
{
    screens[BG_TEXT].map[372] = a + 1;
    screens[BG_TEXT].map[373] = b + 1;
    screens[BG_TEXT].map[374] = c + 1;
    screens[BG_TEXT].map[375] = d + 1;
}

void screen_draw_over_new()
{
    screens[BG_TEXT].map[377] = 54;
    screens[BG_TEXT].map[378] = 55;
    screens[BG_TEXT].map[441] = 53;
}

void screen_draw_hscore(int a, int b, int c, int d)
{
    screens[BG_TEXT].map[52] = a + 1;
    screens[BG_TEXT].map[53] = b + 1;
    screens[BG_TEXT].map[54] = c + 1;
    screens[BG_TEXT].map[55] = d + 1;
}

void screen_draw_lives(int n)
{
    screens[BG_TEXT].map[5] = n + 1;
}

void screen_draw_item()
{
    screens[BG_TEXT].map[576] = 144;
    screens[BG_TEXT].map[577] = 145;
    screens[BG_TEXT].map[608] = 146;
    screens[BG_TEXT].map[609] = 147;
    screens[BG_TEXT].map[610] = 148;
}

void screen_clear_item()
{
    screens[BG_TEXT].map[576] = 0;
    screens[BG_TEXT].map[577] = 0;
    screens[BG_TEXT].map[608] = 0;
    screens[BG_TEXT].map[609] = 0;
    screens[BG_TEXT].map[610] = 0;
}

void screen_set_rumble(char b)
{
    if (b)
        rumble = 1;
    else
        rumble = 0;
}

char screen_get_bg1_offset() {
    return bg1_offset;
}

char screen_is_bg1_mapchip(char ix, char iy)
{
    return screens[BG_MAPCHIP].map[(LCD_VWIDTH*iy) + ix] != 0;
}

char screen_check_bg1_mapchip(char ix, char iy)
{
    if (screen_is_bg1_mapchip(ix,iy)) {
        screens[BG_MAPCHIP].map[(LCD_VWIDTH*iy) + ix] = 0;
        return 1;
    }
    return 0;
}
