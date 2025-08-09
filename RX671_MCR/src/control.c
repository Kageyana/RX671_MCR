//====================================//
// インクルード
//====================================//
#include "control.h"
//====================================//
// グローバル変数の宣言
//====================================//
// モード関連
uint8_t patternTrace = 0; // パターン番号
// タイマ関連

///////////////////////////////////////////////////////////////////////////
// モジュール名 systemInit
// 処理概要     初期化処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void initSystem(void)
{
	volatile FRESULT res;

	// タイマ割り込み開始(1ms)
	R_CMT_CreatePeriodicAssignChannelPriority(1000
											, &interrupt1ms
											, CMT_CHANNEL
											, (cmt_priority_t)CMT_PRIORITY_5);
	R_DMACA_Init(); // DMAC内部情報を初期化

	R_Config_SCI2_Start();
	BMI088init();
	SSD1351init();
	GUI_ShowStartup();

	// 赤枠描画
	for(int x = 0; x < SSD1351_WIDTH; x++) {
		SSD1351drawPixel(x, 0, SSD1351_RED);
		SSD1351drawPixel(x, SSD1351_HEIGHT-1, SSD1351_RED);
	}

	for(int y = 0; y < SSD1351_HEIGHT; y++) {
		SSD1351drawPixel(0, y, SSD1351_RED);
		SSD1351drawPixel(SSD1351_WIDTH-1, y, SSD1351_RED);
	}

	R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);
	// calibratIMU = true;	// IMUキャリブレーション開始
	// while(calibratIMU);	// キャリブレーション完了待ち
	
	// SDカード初期化
	if(SDcardOpen() == SDC_SD_SUCCESS)
	{
		SDcardinit(); // SDカード初期化
	}
	if(initSDcard)
	{
		// PIDパラメータをSDカードから読み込み
		readPIDparameters(&lineTraceCtrl);
		readPIDparameters(&veloCtrl);
		readPIDparameters(&angleCtrl);
		// res = logCreate();
		// if (res == FR_OK)
		// {
		// 	loggingSDcard = 1;
		// 	cnt0 = 0;
		// }
	}

	// ロータリエンコーダ初期化
	InitEncoder();
	// モータドライバ初期化
	InitMotor();
	// ラインセンサ初期化
	InitLineSensors();

	R_Config_S12AD0_Start(); // A/D変換開始
	R_Config_S12AD1_Start(); // A/D変換開始
	
	// SSD1351drawImage(0, 0, 128, 128, imgQRgithub128x128);
	// initLED();
	// setLED(0, 255, 0, 0);
	// setLED(1, 255, 0, 0);
	// setLED(2, 255, 0, 0);
	// setLED(3, 255, 0, 0);
	// sendLED();
	cnt0 = 0;

	SSD1351fill(SSD1351_BLACK);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 systemLoop
// 処理概要     メインループ
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void loopSystem(void)
{

	// 緊急停止処理
	if (patternTrace > 10 && patternTrace < 100 && emcStop > 0)
	{
		patternTrace = 101;
	}

	switch (patternTrace)
	{
	case 0:
		SetupUpdate(); // メニュー画面の更新処理
		break;

	case 1:
		
		break;

	case 11:
		
		break;

	case 12:
		
		break;

	case 101:
		
		break;

	default:
		break;
	} // switch case
}