#ifndef _SCREEN_H_
#define _SCREEN_H_

// BGの初期化・更新
extern void screen_init();          // BGの初期化

extern void screen_title_init();    // TITLE画面のBGの初期化
extern void screen_title_update();  // TITLE画面のBGの更新

extern void screen_ready_init();    // READY画面のBGの初期化
extern void screen_ready_update();  // READY画面のBGの更新

extern void screen_game_init();     // GAME画面のBGの初期化
extern void screen_game_update();   // GAME画面のBGの更新

extern void screen_over_init();     // OVER画面のBGの初期化
extern void screen_over_update();   // OVER画面のBGの更新

// スコア・残機の描画
extern void screen_draw_score(int, int, int, int);
extern void screen_draw_hscore(int, int, int, int);
extern void screen_draw_over_score(int, int, int, int);
extern void screen_draw_over_hscore(int, int, int, int);
extern void screen_draw_over_new();
extern void screen_draw_lives(int);
extern void screen_draw_item();
extern void screen_clear_item();

// フラグの設定
extern void screen_set_rumble(char b);

// BG1への問い合わせ
extern char screen_get_bg1_offset();
extern char screen_is_bg1_mapchip(char, char);
extern char screen_check_bg1_mapchip(char, char);

#endif
