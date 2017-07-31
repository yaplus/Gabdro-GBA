//
// player.c
//
// プレイヤーオブジェクト管理
//
// by Hayato Kohara
//

#include "gba.h"
#include "player.h"
#include "sprite.h"
#include "screen.h"
#include "enemy.h"
#include "item.h"
#include "game.h"

/*
    画像データ
    Gritで色深度8bit(=1Byte)、8bit(=1Byte, char)配列でタイルマップを作成
    grit ./resouce/chr.png -o ./source/chr.h -gt -gB8 -ftc -gu8
*/
#include "chr.h"

// 各種定数(8+8bit固定小数点数)
#define K 0x000C    // 空気抵抗定数
#define G 0x0008    // 重力加速度
#define A 0x0010    // 上昇加速度

// スプライト関連
static int spnum = 0;   // プレイヤーのスプライト番号
static int chnum[8];    // キャラクタ(グラフィック)登録番号

// キー入力関連
short  key;         // キー入力
short  prev_key;    // 前フレームのキー入力

// プレイヤーに関するフラグ
// 0bit目: 向き(0->左)
// 1bit目: 上昇アニメーションの有無
// 2bit目: 無敵状態か否か
// 3bit目: アイテムを所持しているか
// 4bit目: 背景が揺れるか
static char flags;

// 座標関連
static fixed pos;   // 現在の座標
static fixed prev;  // 前フレームの座標

// アニメーションカウンタ
static int anim;

// ダメージカウンタ
static int dmg;

// 振動カウンタ
static char rumble;

// スコア
static int score;

// 残機
static short lives;

void player_load_chr()
{
    int i;

    // OAM
    for (i=0;i<8;i++) { // 画像データの読み込み
        chnum[i] = sprite_load((character*)chrTiles + (OBJ_SIZE_32x32 * i), OBJ_SIZE_32x32);
    }
}

void player_init()  // プレイヤーの初期化
{

    // 座標の初期化
    pos.b.xh  = prev.b.xh = 20;
    pos.b.yh  = prev.b.yh = 20;

    // アニメーションの初期化(右向き)
    sprite_flip(spnum, OBJ_FLIP_HORIZONTAL);
    anim = 0;

    // ダメージカウントの初期化
    dmg = 0;

    // フラグの初期化
    flags = 0;

    // スコアの初期化
    score = 0;

    // 残機の初期化
    lives = 3;

    // 振動の初期化
    rumble = 0;

    sprite_setup(spnum, OBJ_32x32, PRIORITY_2, chnum[1]);   // スプライトに画像を設定
    sprite_display(spnum, OBJ_TURNON);                      // スプライトの表示有効化
    sprite_move(spnum, pos.b.xh, pos.b.yh);
}

void player_fin()  // プレイヤーの初期化
{
    sprite_display(spnum, OBJ_TURNOFF); // スプライトの表示無効化
}

void player_update()
{
    if (lives>0) {
        player_living();
    } else {
        player_dying();
    }
}

void player_living()
{
    key = gba_register(KEY_STATUS);

    signed short vx = (signed short)(pos.w.x - prev.w.x);
    signed short vy = (signed short)(pos.w.y - prev.w.y);

    signed short ax = ((-K) * vx) >> 8; // 小数点位置を戻すため8ビット右シフト
    signed short ay = G;

    /*
    // デバッグ用チート
    if (!(key & KEY_SELECT) && (prev_key & KEY_SELECT)) {
        // 残機増える
        if (lives<9) {
            lives++;
        }
    }
    if (!(key & KEY_START) && (prev_key & KEY_START)) {
        // スコア増える
        if (score<9900) {
            score = score + 50;
        }
    }
    if (!(key & KEY_L) && (prev_key & KEY_L)) {
        // アイテム使える
        flags |=  PLAYER_ITEM;
    }
    */

    // アイテムの使用
    if (!(key & KEY_B) && (prev_key & KEY_B) && (flags & PLAYER_ITEM)) {
        int i,j;
        flags &= ~PLAYER_ITEM;
        // 振動フラグON
        flags |= PLAYER_RUMBLE;
        screen_set_rumble(1);
        // コインの回収
        for (i = 0; i < LCD_VWIDTH; i++) {
            for (j = 0; j < LCD_RHEIGHT; j++) {
                if (screen_check_bg1_mapchip(i, j) && score < 9999) score++;
            }
        }
    }

    // 移動
    if (!(key & KEY_RIGHT)) {
        ax += A;
        // グラフィックの反転 : ボタンが押されたとき、かつ向きが反転する場合
        if ((prev_key & KEY_RIGHT) && (flags & PLAYER_DIR)) {
            sprite_flip(spnum, OBJ_FLIP_HORIZONTAL);
            flags ^= PLAYER_DIR;
        }
    } else if (!(key & KEY_LEFT)) {
        ax -= A;
        // グラフィックの反転 : ボタンが押されたとき、かつ向きが反転する場合
        if ((prev_key & KEY_LEFT) && !(flags & PLAYER_DIR)) {
            sprite_flip(spnum, OBJ_FLIP_HORIZONTAL);
            flags ^= PLAYER_DIR;
        }
    }

    // 上昇
    if (!(key & KEY_A) && (prev_key & KEY_A)) {
        ay -= 0x0100;
        // アニメーションカウント開始
        flags |= PLAYER_ANIM;
    }

    if ((flags & PLAYER_ANIM) && (gba_register(TMR_COUNT0) & 0x03) == 0x03) {
        anim++;
        if (anim > 2) {
            anim = 0;
            flags &= ~PLAYER_ANIM;
        }
    }
    sprite_animate(spnum, chnum[anim+1]);

    // アイテムの使用による画面の振動
    if (flags & PLAYER_RUMBLE) {
        rumble++;
        if (rumble == 40) {
            flags &= ~PLAYER_RUMBLE;
            screen_set_rumble(0);
            rumble = 0;
        }
        sprite_animate(spnum, chnum[7]);
    }

    // 速度の算出
    vx += ax;
    vy += ay;
    
    // 一定速度以上は出ない
    if (vx >  0x0280) vx =  0x0280; // 右
    if (vx < -0x0280) vx = -0x0280; // 左
    if (vy >  0x1000) vy =  0x1000; // 下
    if (vy < -0x0180) vy = -0x0180; // 上

    // 新しい座標の確定
    signed short nx = pos.w.x + vx;
    prev.w.x = pos.w.x;
    pos.w.x  = (unsigned short)nx;

    signed short ny = pos.w.y + vy;
    prev.w.y = pos.w.y;
    pos.w.y  = (unsigned short)ny;

    
    // はみ出し防止
    if (pos.b.xh > 0xB0 || pos.b.xh < 0x02) {
        pos.w.x = prev.w.x;
    }
    if (pos.b.yh > 0x80 || pos.b.yh < 0x02) {
        pos.w.y = prev.w.y;
    }

    sprite_move(spnum, pos.b.xh, pos.b.yh);
    prev_key = key;

    // スコアと残機の表示
    int a,b,c,d;
    a = score / 1000;
    b = (score - a*1000) / 100;
    c = (score - a*1000 - b*100) / 10;
    d = score % 10;
    screen_draw_score(a, b, c, d);
    screen_draw_lives(lives);
    if (flags & PLAYER_ITEM)
        screen_draw_item();
    else
        screen_clear_item();

    // 弾丸との衝突判定
    if (!(flags & PLAYER_INV) &&
        enemy_bullet_collide(pos.b.xh + COLLIDE_X, pos.b.yh + COLLIDE_Y, COLLIDE_WIDTH, COLLIDE_HEIGHT)) {
        lives--;
        flags |= PLAYER_INV;
        enemy_bullet_reverse();
    }
    sprite_display(spnum, OBJ_TURNON);
    if (flags & PLAYER_INV) {
        dmg++;
        if (dmg<60) {
            sprite_animate(spnum, chnum[4 + (dmg & 1)]);
        }
        if (dmg==200) {
            dmg = 0;
            flags &= ~PLAYER_INV;
        }
        if ((gba_register(TMR_COUNT0) & 0x03) == 0x03) 
            sprite_display(spnum, OBJ_TURNOFF);
    }

    // アイテムとの衝突判定
    if (item_collide(pos.b.xh + COLLIDE_X, pos.b.yh + COLLIDE_Y, COLLIDE_WIDTH, COLLIDE_HEIGHT)) {
        item_set_flag(0);
        item_reverse();
        flags |=  PLAYER_ITEM;
    }

    // BG1との衝突判定
    // BG1におけるインデックスの導出
    char bg1_offset = screen_get_bg1_offset();
    char ix  = (char)((pos.b.xh + COLLIDE_X + bg1_offset) >> 3) + 1;    // 自機位置の左端 x方向
    char iy  = (char)((pos.b.yh + COLLIDE_Y) >> 3);                     // 自機位置の上端 y方向

    if (screen_check_bg1_mapchip(ix,   iy)   && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix+1, iy)   && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix+2, iy)   && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix,   iy+1) && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix+1, iy+1) && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix+2, iy+1) && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix,   iy+2) && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix+1, iy+2) && score < 9999) score++;
    if (screen_check_bg1_mapchip(ix+2, iy+2) && score < 9999) score++;
}

void player_dying()
{

    signed short vy = (signed short)(pos.w.y - prev.w.y);
    signed short ay = G+2;

    flags ^= PLAYER_INV;
    sprite_animate(spnum, chnum[6]);
    sprite_display(spnum, OBJ_TURNON);

    // 速度の算出
    vy += ay;
    
    // 一定速度以上は出ない
    if (vy >  0x1600) vy =  0x1600; // 下
    if (vy < -0x0180) vy = -0x0180; // 上

    // 新しい座標の確定

    signed short ny = pos.w.y + vy;
    prev.w.y = pos.w.y;
    pos.w.y  = (unsigned short)ny;

    
    // はみ出し防止
    if (pos.b.xh > 0xB0 || pos.b.xh < 0x02) {
        pos.w.x = prev.w.x;
    }
    if (pos.b.yh < 0x02) {
        pos.w.y = prev.w.y;
    }

    sprite_move(spnum, pos.b.xh, pos.b.yh);
    prev_key = key;

    // スコアと残機の表示
    int a,b,c,d;
    a = score / 1000;
    b = (score - a*1000) / 100;
    c = (score - a*1000 - b*100) / 10;
    d = score % 10;
    screen_draw_score(a, b, c, d);
    screen_draw_lives(lives);
    if (flags & PLAYER_ITEM)
        screen_draw_item();
    else
        screen_clear_item();

    // OVER画面への遷移
    if (pos.b.yh > 0xD0) {
        int a,b,c,d;
        a = score / 1000;
        b = (score - a*1000) / 100;
        c = (score - a*1000 - b*100) / 10;
        d = score % 10;
        if (!(flags&PLAYER_DIR))
            sprite_flip(spnum, OBJ_FLIP_HORIZONTAL);
        screen_over_init();
        screen_draw_over_score(a, b, c, d);
        game_set_hscore(score);
        game_set_state(OVER);
    }

}

short player_get_y()
{
    return pos.w.y;
}

int player_get_score()
{
    return score;
}
