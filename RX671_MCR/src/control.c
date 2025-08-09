//====================================//
// インクルード
//====================================//
#include "control.h"
#include "PIDcontrol.h"
#include "ssd1351.h"
#include <stdbool.h>
//====================================//
// グローバル変数の宣言
//====================================//
// モード関連
uint8_t patternTrace = 0;	// パターン番号
bool modeCurve = false;		// カーブモード
// タイマ関連
int16_t countdown;

// 速度パラメータ関連
speedParam tgtParam = {
	PARAM_STRAIGHT,
	PARAM_CURVE,
	PARAM_STOP,
};
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
		// スタート前設定
		SetupUpdate(); // メニュー画面の更新処理
		if ( start && !modePushcart ) {
			lineTraceCtrl.Int = 0;		// 積分リセット
			
			countdown = 5000;							  // カウントダウンスタート
			SSD1351fillRectangle(0, 15, 127, 63, SSD1351_BLACK); // メイン表示空白埋め
			SSD1351setCursor(56, 28);
			SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"5");
			cnt1 = 0;
			patternTrace = 1;
			break;
		} else if ( start && modePushcart ) {
			// 手押しモードの場合すぐに通常トレース
			patternTrace = 11;
			break;
		}
		break;

	case 1:
		ServoPwmOut1( lineTraceCtrl.pwm );

		if (countdown == 4000)
		{
			SSD1351fillRectangle(0, 0, SSD1351_WIDTH - 1, SSD1351_HEIGHT -1, SSD1351_BLACK); // メイン表示空白埋め
			SSD1351setCursor(56, 28);
			SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"4");
		}
		if (countdown == 3000)
		{
			SSD1351fillRectangle(0, 0, SSD1351_WIDTH - 1, SSD1351_HEIGHT -1, SSD1351_BLACK); // メイン表示空白埋め
			SSD1351setCursor(56, 28);
			SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"4");
		}
		if (countdown == 2000)
		{
			SSD1351fillRectangle(0, 0, SSD1351_WIDTH - 1, SSD1351_HEIGHT -1, SSD1351_BLACK); // メイン表示空白埋め
			SSD1351setCursor(56, 28);
			SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"4");
			calibratIMU = true;	// IMUキャリブレーション開始
		}
		if (countdown == 1000)
		{
			SSD1351fillRectangle(0, 0, SSD1351_WIDTH - 1, SSD1351_HEIGHT -1, SSD1351_BLACK); // メイン表示空白埋め
			SSD1351setCursor(56, 28);
			SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"4");
		}

		// IMUのキャリブレーションが終了したら走行開始
		if (!calibratIMU && countdown == 0)
		{
			PowerLineSensors(1);   // ラインセンサ点灯

			// SDカードに変数保存
			bmi088_read_locked = false;
			display_update_locked = true;

			// PIDゲインを記録
			writePIDparameters(&lineTraceCtrl);
			writePIDparameters(&veloCtrl);
			writePIDparameters(&angleCtrl);

			// 変数初期化

			patternTrace = 11;
		}
		break;

		//-------------------------------------------------------------------
		// 【010】トレース処理
		//-------------------------------------------------------------------
		case 11:
			setTargetSpeed(tgtParam.straight);
			
			// クロスラインチェック
			if ( checkCrossLine() ) {
				enc1 = 0;
				patternTrace = 21;
				break;
			}
			// 右ハーフラインチェック
	   		if ( checkRightLine() ) {
				enc1 = 0;
				patternTrace = 51;
				break;
			}
			// 左ハーフラインチェック
	   		if ( checkLeftLine() ) {
				enc1 = 0;
				patternTrace = 61;
				break;
			}
			// 坂道チェック
			/*if ( EncoderTotal >= 5609 ) {
				if( checkSlope() == 1 || checkSlope() == -1 ) {
					pattern = 71;
					break;
				}
			}*/
			// カーブチェック
			if ( getServoAngle() >=  CURVE_R600_START || getServoAngle() <= -CURVE_R600_START ) {
				enc1 = 0;
				modeCurve = true;
				patternTrace = 12;
				break;
			}
			break;

	case 12:
		
		break;

	case 101:
		
		break;

	default:
		break;
	} // switch case
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 countDown
// 処理概要     カウントダウン
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void countDown(void)
{
	if (countdown > 0)
		countdown--;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkCrossLine
// 処理概要     クロスライン検知
// 引数         なし
// 戻り値       0:クロスラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkCrossLine( void )
{
	if ( sensor_inp() == 0x7 || sensor_inp() == 0x5) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkRightLine
// 処理概要     右ハーフライン検出処理
// 引数         なし
// 戻り値       0:右ハーフラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkRightLine( void )
{
	if ( sensor_inp() == 0x3 ) return true;
	else return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 checkLeftLine
// 処理概要     左ハーフライン検出処理
// 引数         なし
// 戻り値       0:左ハーフラインなし 1:あり
///////////////////////////////////////////////////////////////////////////
bool checkLeftLine( void )
{
	if ( sensor_inp() == 0x6 ) return true;
	else return false;
}