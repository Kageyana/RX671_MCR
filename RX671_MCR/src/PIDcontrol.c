//====================================//
// インクルード
//====================================//
#include "PIDcontrol.h"
#include <stdint.h>
//====================================//
// グローバル変数の宣言
//====================================//
pidParam lineTraceCtrl = {"line", KP1, KI1, KD1, 0, 0};
pidParam veloCtrl = {"speed", KP2, KI2, KD2, 0, 0};
pidParam angleCtrl = {"angle", KP3, KI3, KD3, 0, 0};

uint8_t targetSpeed;	// 目標速度
float 	targetAngle;	// 目標角速度

///////////////////////////////////////////////////////////////////////////
// モジュール名 getAnalogSensor
// 処理概要     アナログセンサのアナログ値で取得
// 引数         なし
// 戻り値       センサ値
///////////////////////////////////////////////////////////////////////////
int16_t getAnalogSensor(void)
{
	return lineSenVal[1] - lineSenVal[4];
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 setTargetSpeed
// 処理概要     目標速度の設定
// 引数         目標速度の整数倍値
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setTargetSpeed(float speed)
{
	targetSpeed = (int16_t)(speed * PALSE_MILLIMETER);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 setTargetAngle
// 処理概要     目標サーボ角度の設定
// 引数         目標サーボ角度[deg]
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void setTargetAngle(float angle)
{
	targetAngle = angle;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlTrace
// 処理概要     ライントレース時の制御量の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlTrace(void)
{
	int32_t iP, iD, iI, iRet, Dev, Dif;
	static int32_t traceBefore;
	
	//サーボモータ用PWM値計算
	Dev = getAnalogSensor();
	// I成分積算
	lineTraceCtrl.Int += (double)Dev * 0.001;
	if ( lineTraceCtrl.Int > 10000 ) lineTraceCtrl.Int = 10000;		// I成分リミット
	else if ( lineTraceCtrl.Int < -10000 ) lineTraceCtrl.Int = -10000;
	Dif = ( Dev - traceBefore ) * 1;	// dゲイン1/1000倍

	iP = lineTraceCtrl.kp * Dev;			   // 比例
	iI = lineTraceCtrl.ki * lineTraceCtrl.Int; // 積分
	iD = lineTraceCtrl.kd * Dif;			   // 微分
	iRet = iP + iI + iD;
	iRet = iRet >> 10; // PWMを0～1000近傍に収める

	// PWMの上限の設定
	if (iRet > 900)
		iRet = 900;
	if (iRet < -900)
		iRet = -900;

	lineTraceCtrl.pwm = iRet;
	traceBefore = Dev; // 次回はこの値が1ms前の値となる
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlSpeed
// 処理概要     モーターの制御量の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlSpeed(void)
{
	// int32_t iP, iI, iD, iRet, Dev, Dif;
	// static int16_t targetSpeedBefore, encoderBefore;

	// // 駆動モーター用PWM値計算
	// Dev = (int16_t)targetSpeed - encCurrentN; // 偏差
	// // 目標値を変更したらI成分リセット
	// if (targetSpeed != targetSpeedBefore)
	// 	veloCtrl.Int = 0;

	// veloCtrl.Int += (float)Dev * 0.001; // 時間積分
	// Dif = Dev - encoderBefore;			// 微分　dゲイン1/1000倍

	// iP = veloCtrl.kp * Dev;			 // 比例
	// iI = veloCtrl.ki * veloCtrl.Int; // 積分
	// iD = veloCtrl.kd * Dif;			 // 微分
	// iRet = iP + iI + iD;
	// iRet = iRet >> 1;

	// // PWMの上限の設定
	// if (iRet > 900)
	// 	iRet = 900;
	// if (iRet < -900)
	// 	iRet = -900;

	// veloCtrl.pwm = iRet;
	// encoderBefore = Dev;			 // 次回はこの値が1ms前の値となる
	// targetSpeedBefore = targetSpeed; // 前回の目標値を記録
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 motorControlAngle
// 処理概要     角度制御時の制御量の計算
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void motorControlAngle(void)
{
	// float iP, iI, iD, Dev, Dif;
	// static float angleBefore;
	// static float targetAngleBefore;
	// int32_t iRet;

	// Dev = (targetAngle - BMI088val.angle.z) * 20; // 目標値-現在値
	// // I成分積算
        // angleCtrl.Int += Dev * 0.005;
        // // 目標値を変更したらI成分リセット
        // // if ( targetAngle != targetAngleBefore ) angleCtrl.Int = 0;
	// Dif = (Dev - angleBefore) * 1; // dゲイン1/1000倍

	// iP = angleCtrl.kp * Dev;		   // 比例
	// iI = angleCtrl.ki * angleCtrl.Int; // 積分
	// iD = angleCtrl.kd * Dif;		   // 微分
	// iRet = (int32_t)iP + iI + iD;
	// iRet = iRet >> 2; // PWMを0～1000近傍に収める

	// // PWMの上限の設定
	// if (iRet > 900)
	// 	iRet = 900;
	// if (iRet < -900)
	// 	iRet = -900;

	// angleCtrl.pwm = iRet;
	// angleBefore = Dev;				 // 次回はこの値が1ms前の値となる
	// targetAngleBefore = targetAngle; // 前回の目標値を記録
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 writePIDparameters
// 処理概要     PIDゲインをSDカードに記録する
// 引数         pid:pidParam型の変数
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void writePIDparameters(pidParam *pid)
{
	// FIL fil;
	// FRESULT fresult;
	// uint8_t fileName[20] = PATH_SETTING;
	// int16_t ret = 0;

	// // ファイル読み込み
	// strcat(fileName, pid->name);								 // ファイル名追加
	// strcat(fileName, ".txt");									 // 拡張子追加
	// fresult = f_open(&fil, fileName, FA_OPEN_ALWAYS | FA_WRITE); // ファイルを開く

	// if (fresult == FR_OK)
	// {
	// 	f_printf(&fil, "%03d,%03d,%03d", pid->kp, pid->ki, pid->kd);
	// }

	// f_close(&fil);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 readPIDparameters
// 処理概要     PIDゲインをSDカードから読み取る
// 引数         pid:pidParam型の変数
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void readPIDparameters(pidParam *pid)
{
	// FIL fil;
	// FRESULT fresult;
	// uint8_t fileName[20] = PATH_SETTING;
	// TCHAR gain[20];
	// int16_t ret = 0;

	// // ファイル読み込み
	// strcat(fileName, pid->name);								  // ファイル名追加
	// strcat(fileName, ".txt");									  // 拡張子追加
	// fresult = f_open(&fil, fileName, FA_OPEN_EXISTING | FA_READ); // ファイルを開く

	// if (fresult == FR_OK)
	// {
	// 	f_gets(gain, sizeof(gain), &fil);						// 文字列取得
	// 	sscanf(gain, "%d,%d,%d", &pid->kp, &pid->ki, &pid->kd); // 文字列→数値
	// }

	// f_close(&fil);
}