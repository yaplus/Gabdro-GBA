#ifndef _ENEMY_H_
#define _ENEMY_H_

#define CHR_SIZE        32  // グラフィックの大きさ

#define BULLET_WIDTH    16  // 衝突判定の大きさ
#define BULLET_HEIGHT   16  // 衝突判定の大きさ

extern void enemy_load_chr();  // 画像の読み込み
extern void enemy_init();      // 敵の初期化
extern void enemy_fin();       // 敵の終了
extern void enemy_update();    // 敵の更新

// 敵オブジェクトへの問い合わせ
extern int enemy_bullet_collide(char, char, int, int);
extern void enemy_bullet_reverse();

#endif
