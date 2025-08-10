#ifndef CONTROL_H_
#define CONTROL_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
#include "r_cmt_rx_if.h"
#include "r_dmaca_rx_if.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "BMI088.h"
#include "ssd1351.h"
#include "WS2812C.h"
#include "switch.h"
#include "SDcard.h"
#include "encoder.h"
#include "Motor.h"
#include "timer.h"
#include "linesensor.h"
#include "setup.h"
#include "emergencyStop.h"
//====================================//
// シンボル定義
//====================================//
#define CMT_CHANNEL 0 // 1msタイマ割り込みに使用するCMTチャンネル番号

// 速度パラメータ関連
#define PARAM_STRAIGHT 2.0F
#define PARAM_CURVE 1.5F
#define PARAM_STOP 0.8F

// 角度
#define ANGLE_RIGHTCLANK		1350	// 右クランク旋回角度
#define ANGLE_LEFTCLANK		    -1350	// 左クランク旋回角度
#define ANGLE_RIGHTCHANGE		700	// 右レーンチェンジ旋回角度
#define ANGLE_LEFTCHANGE		-700	    // 右レーンチェンジ旋回角度

// カーブ関連
#define CURVE_R600_START		120		// R600開始AD値
#define CURVE_R450_START		600		// R450開始AD値
//====================================//
// グローバル変数の宣言
//====================================//
// パターン、モード関連
extern uint8_t patternTrace; // パターン番号
extern bool modeLOG;		// ログ取得状況
extern bool modeCurve;		// カーブモード
extern bool initMSD;		// microSD初期化状況
extern bool initIMU;		// IMU初期化状況

// タイマ関連
extern uint32_t cntRun;
extern int16_t countdown;

// エンコーダ関連
extern int32_t	enc1;				// 走行用距離カウント
extern int32_t	enc_slope;			// 坂上距離カウント

typedef struct
{
	float straight;
	float curve;
	float stop;
} speedParam;
//====================================//
// プロトタイプ宣言
//====================================//
void initSystem(void);
void loopSystem(void);
void emargencyStop(void);
void countDown(void);
void setEncoderVal(void);
void writeTgtspeeds(void);
void readTgtspeeds(void);

// マーカー関連
bool checkCrossLine( void );
bool checkRightLine( void );
bool checkLeftLine( void );

#endif // CONTROL_H_
