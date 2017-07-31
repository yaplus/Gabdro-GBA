//
// main.c
//
// ガヴドロアドバンス
//
// by Hayato Kohara
//

#include "gba.h"
#include "game.h"

void wait_until_vblank()
{
    while ((gba_register(LCD_STATUS) & 1) == 0);
}

void wait_while_vblank()
{
    while ((gba_register(LCD_STATUS) & 1));
}

int main()
{
    // 初期化
    gba_register(TMR_COUNT0) = 0;
    gba_register(TMR_CTRL0) = TMR_ENABLE + TMR_1024CLOCK;
    game_init();

    // メインループ
    for (;;) {
        wait_until_vblank();
        game_update();  // ゲームの更新
        wait_while_vblank();
    }
}
