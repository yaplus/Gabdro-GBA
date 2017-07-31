#ifndef _GAME_H_
#define _GAME_H_

enum state {TITLE, READY, GAME, OVER};

extern enum state game_get_state();     // 今の状態を問い合わせる
extern void game_set_state(enum state); // 状態の変更
extern void game_set_hscore(int);       // ハイスコアの設定
extern void game_init();                // ゲームの初期化
extern void game_update();              // ゲームの更新

#endif
