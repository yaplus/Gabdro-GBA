//
// enemy.c
//
// 敵オブジェクト管理
//
// by Hayato Kohara
//

#include "gba.h"
#include "item.h"
#include "sprite.h"
#include "player.h"

/*
    画像データ
    Gritで色深度8bit(=1Byte)、8bit(=1Byte, char)配列でタイルマップを作成
    grit ./resouce/chr.png -o ./source/chr.h -gt -gB8 -ftc -gu8
*/
#include "chr.h"

// スプライト関連
static int balloon_spnum = 3;   // 風船のスプライト番号
static int balloon_chnum[2];    // キャラクタ(グラフィック)登録番号
static int item_spnum = 4;      // アイテムのスプライト番号
static int item_chnum;          // 弾丸のスプライト番号

// 座標関連
static fixed pos;   // アイテムの座標
static char theta;

// アニメーション関連
static int balloon_anim;
static char anim;

// 出現するかどうか
static char flag;
static int prev_sc; // 前フレームのスコア

void item_load_chr()
{
    int i;

    // OAM
    for (i=0;i<2;i++) { // 画像データの読み込み
        balloon_chnum[i] = sprite_load((character*)chrTiles + OBJ_SIZE_32x32 * 13 + OBJ_SIZE_16x16 * i,
                                       OBJ_SIZE_32x32);
    }

    item_chnum = sprite_load((character*)chrTiles + OBJ_SIZE_32x32 * 12 + OBJ_SIZE_16x16 * 2,
                             OBJ_SIZE_16x16);
}

void item_init()  // プレイヤーの初期化
{
    balloon_anim  = 0;
    anim          = 0;
    prev_sc       = 0;

    // 座標の初期化
    item_set_flag(0);
    item_reverse();

    sprite_setup(balloon_spnum, OBJ_16x16, PRIORITY_2, balloon_chnum[balloon_anim]);    // スプライトに画像を設定
    sprite_display(balloon_spnum, OBJ_TURNON);                                          // スプライトの表示有効化
    sprite_setup(item_spnum, OBJ_16x16, PRIORITY_2, item_chnum);                        // スプライトに画像を設定
    sprite_display(item_spnum, OBJ_TURNON);                                             // スプライトの表示有効化
    sprite_move(balloon_spnum, pos.b.xh, pos.b.yh);
    sprite_move(item_spnum, pos.b.xh, pos.b.yh + ITEM_MARGIN);
}

void item_fin()
{
    sprite_display(balloon_spnum, OBJ_TURNOFF);   // スプライトの表示無効化
    sprite_display(item_spnum, OBJ_TURNOFF);      // スプライトの表示無効化
}

void item_update()
{
    int s = player_get_score() % 500;
    if (prev_sc > s)
        item_set_flag(1);
    prev_sc = s;

    if (flag) {
        // 新しい座標の算出
        theta++;
        signed short nx = pos.w.x + (signed short)(-2 * sin_tbl[(int)theta]);
        pos.w.x  = (unsigned short)nx;

        pos.w.y = pos.w.y - 0x80;
        if (pos.b.yh == 0xE6) {
            item_set_flag(0);
            item_reverse();
        }
    }
    // アニメーション
    anim++;
    if ((anim & 7) == 7) {
        // 風船のアニメーション
        balloon_anim = (balloon_anim + 1) & 1;
    }
    sprite_animate(balloon_spnum, balloon_chnum[balloon_anim]); 
    // スプライトの移動
    sprite_move(balloon_spnum, pos.b.xh, pos.b.yh);
    sprite_move(item_spnum, pos.b.xh, pos.b.yh + ITEM_MARGIN);
}

// 衝突判定
int item_collide(char x, char y, int width, int height)
{
    return !(x          >= pos.b.xh + ITEM_WIDTH                ||
             x + width  <= pos.b.xh                             ||
             y          >= pos.b.yh + ITEM_HEIGHT               ||
             y + height <= pos.b.yh);
}

// フラグセット
void item_set_flag(char b)
{
    if (b)
        flag = 1;
    else
        flag = 0;
}

// アイテムの座標を戻す
void item_reverse()
{
    pos.b.xh = 192;
    pos.b.yh = 160;
    theta = 0;
}
