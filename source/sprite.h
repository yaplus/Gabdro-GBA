#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "gba.h"

// スプライトの属性設定
extern void sprite_setup(int, int, int, int);

// スプライトの属性更新
extern void sprite_display(int, int);   // スプライトの表示
extern void sprite_move(int, int, int); // スプライトの移動
extern void sprite_flip(int, int);      // スプライトの反転
extern void sprite_animate(int, int);   // スプライトのアニメーション

// スプライトの画像読み込み
extern int sprite_load(character*, int);

// スプライトの初期化
extern void sprite_init(hword*);

#endif
