#ifndef _PLAYER_H_
#define _PLAYER_H_

#define CHR_SIZE        32 // グラフィックの大きさ
#define COLLIDE_X       7  // 衝突判定のX方向オフセット
#define COLLIDE_Y       10 // 衝突判定のY方向オフセット
#define COLLIDE_WIDTH   16 // 衝突判定の大きさ
#define COLLIDE_HEIGHT  16 // 衝突判定の大きさ

#define PLAYER_DIR      0x01    // 方向フラグ(0:左, 1:右)
#define PLAYER_ANIM     0x02    // アニメーションフラグ(0:なし, 1:あり)
#define PLAYER_INV      0x04    // 無敵フラグ(0:なし, 1:あり)
#define PLAYER_ITEM     0x08    // アイテムフラグ(0:なし, 1:あり)
#define PLAYER_RUMBLE   0x10    // 振動フラグ(0:なし, 1:あり)

extern void player_load_chr();  // 画像の読み込み
extern void player_init();      // プレイヤーの初期化
extern void player_fin();       // プレイヤーの終了
extern void player_update();    // プレイヤーの更新
extern void player_living();    // プレイヤーの更新(プレイ中)
extern void player_dying();     // プレイヤーの更新(ミス時)

// プレイヤーオブジェクトへの問い合わせ
extern short player_get_y();
extern int player_get_score();

#endif
