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

//====================================//
// グローバル変数の宣言
//====================================//
// パターン、モード関連
extern uint8_t patternTrace; // パターン番号
extern bool modeDSP;		 // LCD表示選択
extern bool modeLOG;		 // ログ取得状況
extern bool initMSD;		 // microSD初期化状況
extern bool initLCD;		 // LCD初期化状況
extern bool initIMU;		 // IMU初期化状況
extern bool initCurrent;	 // 電流センサ初期化状況

// タイマ関連
extern uint32_t cntRun;
extern int16_t countdown;
//====================================//
// プロトタイプ宣言
//====================================//
void initSystem(void);
void loopSystem(void);
void emargencyStop(void);
void countDown(void);
void checkCurve(void);
void getADC2(void);
void setEncoderVal(void);
void writeTgtspeeds(void);
void readTgtspeeds(void);

#endif // CONTROL_H_
