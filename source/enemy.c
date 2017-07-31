//
// enemy.c
//
// 敵オブジェクト管理
//
// by Hayato Kohara
//

#include "gba.h"
#include "enemy.h"
#include "sprite.h"
#include "player.h"

/*
    画像データ
    Gritで色深度8bit(=1Byte)、8bit(=1Byte, char)配列でタイルマップを作成
    grit ./resouce/chr.png -o ./source/chr.h -gt -gB8 -ftc -gu8
*/
#include "chr.h"

// スプライト関連
static int enemy_spnum = 1;     // 敵キャラのスプライト番号
static int enemy_chnum[4];      // キャラクタ(グラフィック)登録番号
static int bullet_spnum = 2;    // 弾丸のスプライト番号
static int bullet_chnum[2];     // 弾丸のスプライト番号

// 座標関連
static fixed enemy_pos;   // 敵の座標
static fixed enemy_prev;  // 前フレームの敵の座標
static fixed bullet_pos;  // 弾丸の座標

// アニメーション関連
static int enemy_anim;
static int bullet_anim;
static char anim;       // カウンタ

static char enemy_acc;
static short bullet_spd;

void enemy_load_chr()
{
    int i;

    // OAM
    for (i=0;i<4;i++) { // 画像データの読み込み
        enemy_chnum[i] = sprite_load((character*)chrTiles + OBJ_SIZE_32x32 * (8 + i),
                                       OBJ_SIZE_32x32);
    }

    for (i=0;i<2;i++) { // 画像データの読み込み
        bullet_chnum[i] = sprite_load((character*)chrTiles + OBJ_SIZE_32x32 * 12 + OBJ_SIZE_16x16 * i,
                                       OBJ_SIZE_16x16);
    }
}

void enemy_init()  // 敵キャラの初期化
{
    enemy_anim  = 0;
    bullet_anim = 0;

    enemy_acc   = 7;
    bullet_spd = 0x200;
    
    // 座標の初期化
    enemy_pos.b.xh  = enemy_prev.b.xh = 208;
    enemy_pos.b.yh  = enemy_prev.b.yh = 20;
    bullet_pos.b.xh = enemy_pos.b.xh - 10;
    bullet_pos.b.yh = enemy_pos.b.yh + 10;

    sprite_setup(enemy_spnum, OBJ_32x32, PRIORITY_2, enemy_chnum[enemy_anim]);          // スプライトに画像を設定
    sprite_display(enemy_spnum, OBJ_TURNON);                                            // スプライトの表示有効化
    sprite_setup(bullet_spnum, OBJ_16x16, PRIORITY_2, bullet_chnum[bullet_anim]);       // スプライトに画像を設定
    sprite_display(bullet_spnum, OBJ_TURNON);                                           // スプライトの表示有効化
    sprite_move(enemy_spnum, enemy_pos.b.xh, enemy_pos.b.yh);
    sprite_move(bullet_spnum, bullet_pos.b.xh, bullet_pos.b.yh);
}

void enemy_fin()
{
    sprite_display(enemy_spnum, OBJ_TURNOFF);   // スプライトの表示無効化
    sprite_display(bullet_spnum, OBJ_TURNOFF);  // スプライトの表示無効化
}

void enemy_update()
{
    signed short vy = (signed short)(enemy_pos.w.y - enemy_prev.w.y);
    signed short ay;
    int s = player_get_score();
    if (s > 200) { // 加速
        enemy_acc = 12;
        if (s > 500) {
            bullet_spd = 0x280;
            if (s > 1000) {
                enemy_acc = 20;
                bullet_spd = 0x320;
            }
        }
    }

    // プレイヤーの挙動を追尾
    hword py = player_get_y();
    if (py > enemy_pos.w.y) {
        ay = enemy_acc;
    } else {
        ay = -enemy_acc;
    }
    vy += ay;

    if (vy >  0x0150) vy =  0x0150; // 下
    if (vy < -0x0150) vy = -0x0150; // 上

    // 新しい座標の算出と修正
    signed short ny = enemy_pos.w.y + vy;
    enemy_prev.w.y = enemy_pos.w.y;
    enemy_pos.w.y  = (unsigned short)ny;
    if (enemy_pos.b.yh > 0x80 || enemy_pos.b.yh < 0x02) {
        enemy_pos.w.y = enemy_prev.w.y;
    }

    // 弾丸の移動と座標の修正
    bullet_pos.w.x -= bullet_spd;
    if (bullet_pos.b.xh > 240) {
        bullet_pos.b.xh = enemy_pos.b.xh - 10;
        bullet_pos.b.yh = enemy_pos.b.yh + 10;
    }

    // アニメーション
    anim++;
    if ((anim & 7) == 7) {
        // 敵のアニメーション
        enemy_anim = (enemy_anim + 1) & 3;
        // 弾丸のアニメーション
        bullet_anim = (bullet_anim + 1) & 1;
    }
    sprite_animate(enemy_spnum, enemy_chnum[enemy_anim]); 
    sprite_animate(bullet_spnum, bullet_chnum[bullet_anim]);

    // スプライトの移動
    sprite_move(enemy_spnum, enemy_pos.b.xh, enemy_pos.b.yh);
    sprite_move(bullet_spnum, bullet_pos.b.xh, bullet_pos.b.yh);
}

// 衝突判定
int enemy_bullet_collide(char x, char y, int width, int height)
{
    return !(x          >= bullet_pos.b.xh + BULLET_WIDTH  ||
             x + width  <= bullet_pos.b.xh                 ||
             y          >= bullet_pos.b.yh + BULLET_HEIGHT ||
             y + height <= bullet_pos.b.yh);
}

void enemy_bullet_reverse()
{
    bullet_pos.b.xh = enemy_pos.b.xh - 10;
    bullet_pos.b.yh = enemy_pos.b.yh + 10;
}
