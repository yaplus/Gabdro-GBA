#ifndef _ITEM_H_
#define _ITEM_H_

#define ITEM_SIZE      16  // グラフィックの大きさ

#define ITEM_WIDTH    16  // 衝突判定の大きさ
#define ITEM_HEIGHT   26  // 衝突判定の大きさ

#define ITEM_MARGIN   10   // 風船とアイテムの高さの差

extern void item_load_chr();  // 画像の読み込み
extern void item_init();      // 敵の初期化
extern void item_fin();       // 敵の終了
extern void item_update();    // 敵の更新

// 敵オブジェクトへの問い合わせ
extern int item_collide(char, char, int, int);
extern void item_set_flag(char);
extern void item_reverse();

#endif
