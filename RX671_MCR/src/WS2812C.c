//====================================//
// インクルード
//====================================//
#include "WS2812C.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint8_t LED_Data[MAX_LED][4];
volatile uint16_t pwmData[(24 * MAX_LED) + RESET_LEN] = {0};
volatile bool datasentflag = false;
volatile bool dma_done_flag = false;
volatile bool lineflag = false;
///////////////////////////////////////////////////////////////////////////
// モジュール名 setLED
// 処理概要     色ごとの輝度を設定する
// 引数         LEDnum:設定するLEDの番号(0から) rgb:色ごとの輝度(0～255)
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setLED(int LEDnum, int Red, int Green, int Blue) {
    if (!datasentflag) {
        LED_Data[LEDnum][0] = LEDnum;
        LED_Data[LEDnum][1] = Green;
        LED_Data[LEDnum][2] = Red;
        LED_Data[LEDnum][3] = Blue;
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 sendLED
// 処理概要     setLEDで設定したRGBデータを送信する
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void sendLED(void)
{
    volatile uint32_t index = 0;
    volatile uint32_t color;

    if (!datasentflag)
    {
        for (int i = 0; i < MAX_LED; i++)
        {
            color = ((LED_Data[i][1] << 16) | (LED_Data[i][2] << 8) | LED_Data[i][3]);
            for (int j = 23; j >= 0; j--)
            {
                pwmData[index++] = (color & (1 << j)) ? PWM_1 : PWM_0;
            }
        }

        for (int i = 0; i < RESET_LEN; i++)
		{
			pwmData[index++] = 0;
		
		}

        datasentflag = true;

        // 転送長をセットする方法は設定によって異なります
        // 必要なら DMCNT / DMCRA レジスタを直接書き換える（上級向け）

		DMAC0.DMSAR = (uint32_t*)&pwmData;
        DMAC0.DMDAR = (uint32_t*)&TPU5.TGRB;
		DMAC0.DMCRA = index;
        R_Config_DMAC0_Start();  // DMA転送開始（トリガは自動 or ソフト）
        R_Config_TPU5_Start();   // PWM出力スタート

        // DMA完了はフラグまたは割り込みで検知
		dma_done_flag = false;
        while (!dma_done_flag);  // 割り込み内でセットされるフラグ

        R_Config_TPU5_Stop();
        R_Config_DMAC0_Stop();
		PORTB.PODR.BIT.B6 = 0;   // 明示的にLow
		PORTB.PDR.BIT.B6 = 1;    // 出力モード
        R_BSP_SoftwareDelay(120, BSP_DELAY_MICROSECS);
        datasentflag = false;
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 fullColorLED
// 処理概要     4つのLEDの色を順番に変える
// 引数         brightness:最大輝度(0～127) add 輝度の変化量
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void fullColorLED(uint8_t brightness, uint8_t add) {
    static RGBLED led[MAX_LED] = {1, 0, 0, 0};
    if (!lineflag) {
        for (int i = 0; i < MAX_LED; i++) led[i].pattern = i + 1;
        lineflag = true;
    }

    for (int i = 0; i < MAX_LED; i++) {
        r2b(&led[i], brightness, add);
        setLED(i, led[i].r, led[i].g, led[i].b);
    }
    sendLED();
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 r2b
// 処理概要     赤から青へ色をフルカラーに変える
// 引数         *led:RGB構造体のポインタ brightness:最大輝度(0～127) add 輝度の変化量
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void r2b(RGBLED *led, uint8_t brightness, uint8_t add) {
    switch (led->pattern) {
        case 1: led->r = brightness; led->g += add; led->b = 0;
                if (led->g >= brightness) { led->g = brightness; led->pattern = 2; } break;
        case 2: led->r -= add; led->g = brightness; led->b = 0;
                if (led->r <= 0) { led->r = 0; led->pattern = 3; } break;
        case 3: led->g = brightness; led->b += add;
                if (led->b >= brightness) { led->b = brightness; led->pattern = 4; } break;
        case 4: led->g -= add; led->b = brightness;
                if (led->g <= 0) { led->g = 0; led->pattern = 5; } break;
        case 5: led->r += add; led->b = brightness;
                if (led->r >= brightness) { led->r = brightness; led->pattern = 6; } break;
        case 6: led->r = brightness; led->b -= add;
                if (led->b <= 0) { led->b = 0; led->pattern = 1; } break;
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 led_out
// 処理概要     赤色で2進数表示
// 引数         表示したい2進数4bit
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void led_out(uint8_t data) {
    for (int i = 0; i < MAX_LED; i++)
        setLED(i, (data & (1 << i)) ? 10 : 0, 0, 0);
    sendLED();
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 clearLED
// 処理概要     LED全消灯
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void clearLED(void) {
    for (int i = 0; i < MAX_LED; i++)
        setLED(i, 0, 0, 0);
    sendLED();
}
