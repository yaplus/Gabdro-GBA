//
// sprite.c
//
// OBJドライバ
//
// by Hayato Kohara
//

#include "gba.h"

// Global variables

static int next_char = 0; // 次にキャラクタが登録されるID位置

// (32 bytes/character)
// Setup sprite
void sprite_setup(int spnum, int shape, int priority, int chnum)
{
    int i, j;
    volatile sprite* sp; // コンパイラによる最適化を抑制

    sp = (sprite*) OAM;
    sp += spnum;
    sp->attr0 = OBJ_COLOR_256 + OBJ_SW_TURNOFF;
    sp->attr1 |= 0;
    sp->attr2 = ((priority & 3) << 10);
    sp->attr2 |= (chnum & OBJ_CHAR_MASK);
    sp->padd  = 0;

    i = shape >> 2;  // 4の商
    sp->attr0 |= (i << 14);
    j = shape & 3;   // 4の剰余
    sp->attr1 |= (j << 14);
}

// スプライトの表示/非表示
void sprite_display(int spnum, int disp)
{
    volatile sprite* sp; // コンパイラによる最適化を抑制
    sp = (sprite*) OAM;
    sp += spnum;

    if (disp)
        sp->attr0 &= ~OBJ_SW_MASK;
    else
        sp->attr0 |= OBJ_SW_TURNOFF;
}

// スプライトの座標の移動
void sprite_move(int spnum, int x, int y)
{
    volatile sprite* sp; // コンパイラによる最適化を抑制
    sp = (sprite*) OAM;
    sp += spnum;

    sp->attr1 &= ~OBJ_X_MASK;
    sp->attr1 |= (x & OBJ_X_MASK);
    sp->attr0 &= ~OBJ_Y_MASK;
    sp->attr0 |= (y & OBJ_Y_MASK);
}

// スプライトの反転
void sprite_flip(int spnum, int dir)
{
    volatile sprite* sp; // コンパイラによる最適化を抑制
    sp = (sprite*) OAM;
    sp += spnum;

    sp->attr1 ^= dir;
}

// スプライトのアニメーション
void sprite_animate(int spnum, int chnum)
{
    volatile sprite* sp; // コンパイラによる最適化を抑制
    sp = (sprite*) OAM;
    sp += spnum;

    sp->attr2 &= ~OBJ_CHAR_MASK;
    sp->attr2 |= (chnum & OBJ_CHAR_MASK);
}

// 画像の読み込み
// return value  : キャラクタ(画像)ID
// character* ch : 転送する画像データ
// int size      : number of sprites (ex. 16x16 -> 4)
int sprite_load(character* ch, int size)
{
    int i, j, start;
    volatile character* ptr; // コンパイラによる最適化を抑制

    start = next_char;
    if ((start + (size * 2)) > OBJ_MAX_CHARS)
        return -1;
    ptr = (character*) OBJ_CHAR_TILE;
    ptr += (next_char / 2);
    for (i = 0; i < size; i++) {
        for (j = 0; j < 32; j++)
            ptr->data[ j ] = ch[ i ].data[ j ];
        ptr++;
        next_char += 2;
    }
    return start;
}

// スプライトモジュールの初期化
void sprite_init(hword* obj_palette)
{
    int i;
    volatile hword* pal = (hword*) OBJ_PALETTE;

    // Initialize OBJ palettes

    for (i = 0; i < LCD_PAL_COLORS; i++)
        *pal++ = obj_palette[ i ];

    // Initialize sprites

    for (i = 0; i < OBJ_MAX_SPRITES; i++)
        sprite_setup(i, OBJ_8x8, PRIORITY_1, 0);

    // Turn on OBJ screen!

    gba_register(LCD_CTRL) |= LCD_OBJEN + LCD_OBJMAP1D;
}
