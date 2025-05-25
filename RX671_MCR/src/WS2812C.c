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
// モジュール名 initLED
// 処理概要     LEDの初期化
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void initLED(void)
{
	volatile dmaca_return_t retDMA;
	dmaca_transfer_data_cfg_t td_cfg;

	retDMA = R_DMACA_Open(DMACA_CH0); // DMAC0をオープン

	td_cfg.transfer_mode = DMACA_TRANSFER_MODE_NORMAL;
	td_cfg.repeat_block_side = DMACA_REPEAT_BLOCK_DISABLE;
	td_cfg.data_size = DMACA_DATA_SIZE_WORD;
	td_cfg.act_source = (dmaca_activation_source_t)164; // TPU5の割り込みを使用
	td_cfg.request_source = DMACA_TRANSFER_REQUEST_PERIPHERAL;
	td_cfg.dtie_request = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
	td_cfg.esie_request = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
	td_cfg.rptie_request = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
	td_cfg.sarie_request = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
	td_cfg.darie_request = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
	td_cfg.src_addr_mode = DMACA_SRC_ADDR_INCR;
	td_cfg.src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
	td_cfg.des_addr_mode = DMACA_DES_ADDR_FIXED;
	td_cfg.des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
	td_cfg.offset_value = 0x00000000;
	td_cfg.interrupt_sel = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
	td_cfg.p_src_addr = (void *)&pwmData;
	td_cfg.p_des_addr = (void *)&TPU5.TGRB;
	td_cfg.transfer_count = 326; // 24bit * 4LED + RESET_LEN

	retDMA = R_DMACA_Create(DMACA_CH0, &td_cfg); // DMAC0の転送データ設定

	retDMA = R_DMACA_Int_Callback(DMACA_CH0,ledDMAinterrupt); // DMAC0の割り込みコールバック関数設定

}

void ledDMAinterrupt(void)
{
    if (DMAC0.DMSTS.BIT.DTIF == 1U)
    {
        DMAC0.DMSTS.BIT.DTIF = 0U;
        dma_done_flag = true;
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 setLED
// 処理概要     色ごとの輝度を設定する
// 引数         LEDnum:設定するLEDの番号(0から) rgb:色ごとの輝度(0～255)
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setLED(int LEDnum, int Red, int Green, int Blue)
{
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
	dmaca_stat_t dmac_status;
	volatile dmaca_return_t retDMA;

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

		DMAC0.DMCRA = index;
		retDMA = R_DMACA_Int_Enable(DMACA_CH0,15);	 // 割り込み優先度を設定
		retDMA = R_DMACA_Control(DMACA_CH0, DMACA_CMD_ENABLE, &dmac_status); // DMA転送開始
        // R_Config_DMAC0_Start();  // DMA転送開始（トリガは自動 or ソフト）
        R_Config_TPU5_Start();   // PWM出力スタート

        // DMA完了はフラグまたは割り込みで検知
		dma_done_flag = false;
        while (!dma_done_flag);  // 割り込み内でセットされるフラグ

        R_Config_TPU5_Stop();
        // R_Config_DMAC0_Stop();
		retDMA = R_DMACA_Control(DMACA_CH0, DMACA_CMD_DTIF_STATUS_CLR, &dmac_status); // DMA転送完了フラグクリア
		retDMA = R_DMACA_Control(DMACA_CH0, DMACA_CMD_ESIF_STATUS_CLR, &dmac_status); // DMAエスケープフラグクリア

        datasentflag = false;
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 fullColorLED
// 処理概要     4つのLEDの色を順番に変える
// 引数         brightness:最大輝度(0～127) add 輝度の変化量
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void fullColorLED(uint8_t brightness, uint8_t add)
{
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
void r2b(RGBLED *led, uint8_t brightness, uint8_t add)
{
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
void led_out(uint8_t data)
{
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
void clearLED(void)
{
    for (int i = 0; i < MAX_LED; i++)
        setLED(i, 0, 0, 0);
    sendLED();
}
