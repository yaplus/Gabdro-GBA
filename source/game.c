//
// game.c
//
// ゲームコントローラ
//
// by Hayato Kohara
//

#include "gba.h"
#include "chr.h"
#include "sprite.h"
#include "screen.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "game.h"

static enum state current_state;    // 現在の状態
static int key;                     // キー入力
static int prev_key;                // 前フレームのキー入力
static int hscore;                  // ハイスコア

enum state game_get_state()
{
    return current_state;
}

void game_set_state(enum state new_state)
{
    current_state = new_state;
}

void game_set_hscore(int score)
{
    if (score > hscore) {
        hscore = score;
        screen_draw_over_new();
    }
}

void game_init()
{
    hscore = 200;
    screen_init();                  // BGの初期化
    screen_title_init();            // TITLE画面のBGの初期化
    sprite_init((hword*)chrPal);    // スプライトの初期化・パレットの設定
    player_load_chr();              // プレイヤーのグラフィック読み込み
    enemy_load_chr();               // 敵キャラのグラフィック読み込み
    item_load_chr();                // アイテムのグラフィック読み込み
    game_set_state(TITLE);          // 状態の設定
}

void game_update()
{
    int a,b,c,d;
    switch (game_get_state()) {
        case TITLE:
            // タイトルの表示
            screen_title_update();
            a = hscore / 1000;
            b = (hscore - a*1000) / 100;
            c = (hscore - a*1000 - b*100) / 10;
            d = hscore % 10;
            screen_draw_hscore(a, b, c, d);
            key = gba_register(KEY_STATUS);
            if (!(key & KEY_START) && (prev_key & KEY_START)) {
                player_init();          // プレイヤーの初期化
                enemy_init();           // 敵キャラの初期化
                item_init();            // アイテムの初期化
                screen_ready_init();    // READY画面のBGの初期化
                game_set_state(READY);  // 状態の更新
            }
            break;
        case READY:
            // READY画面の表示
            screen_ready_update();      // BGの更新
            break;
        case GAME:
            // ゲームの更新
            screen_game_update();       // BGの更新
            player_update();            // プレイヤーの更新
            enemy_update();             // 敵キャラの更新
            item_update();              // アイテムの更新
            break;
        case OVER:
            // ゲームオーバーを表示
            screen_over_update();
            a = hscore / 1000;
            b = (hscore - a*1000) / 100;
            c = (hscore - a*1000 - b*100) / 10;
            d = hscore % 10;
            screen_draw_over_hscore(a, b, c, d);
            key = gba_register(KEY_STATUS);
            if (!(key & KEY_START) && (prev_key & KEY_START)) {
                screen_title_init();    // TITLE画面のBGの初期化
                player_fin();           // プレイヤーの終了
                enemy_fin();            // 敵キャラの終了
                item_fin();             // アイテムの終了
                game_set_state(TITLE);  // 状態の更新
            }
            break;
    }
    prev_key = key;
}