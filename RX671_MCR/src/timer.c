//====================================//
// インクルード
//====================================//
#include "timer.h"
#include "emergencyStop.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint16_t cnt10 = 0;
volatile uint16_t cnt0 = 0;
bool initIMU = false;
/////////////////////////////////////////////////////////////////////
// モジュール名 interrupt1ms
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void interrupt1ms(void * pdata)
{
	cnt0++;
	cnt10++;
	cntGUI++;

	if(!loggingSDcard && !display_update_locked && cnt10 != 1 && cnt10 != 2)
	{
		SSD1351updateScreen();
	}
	
	R_SDC_SD_1msInterval();	// SDカードの1ms間隔処理
	GetEncoderVal();		// エンコーダ値の取得

	GetLineSenADVal();		// ラインセンサのAD値取得

	motorControlTrace();	// ライントレース制御
	motorControlSpeed();	// 速度制御
	motorControlAngle();	// サーボ角度制御

	// 走行前に処理
	if (patternTrace < 10 || patternTrace > 100)
	{
		countDown();
		GetSwitches();			// スイッチの状態取得
	}
	// 走行中に処理
	if (patternTrace > 10 && patternTrace < 100)
	{
		// 緊急停止処理
		// if (cntEmcStopAngleX()) emcStop = STOP_ANGLE_X;
		// if (cntEmcStopAngleY()) emcStop = STOP_ANGLE_Y;
		// if (cntEmcStopEncStop()) emcStop = STOP_ENCODER_STOP;
		// if (cntEmcStopLineSensor()) emcStop = STOP_LINESENSOR;
		// if (judgeOverSpeed()) emcStop = STOP_OVERSPEED;
		if (cntEmcStopDist()) emcStop = STOP_DISTANCE;

		MotorPwmOut(veloCtrl.pwm, veloCtrl.pwm, veloCtrl.pwm, veloCtrl.pwm);
	}

	switch (cnt10)
	{
	case 1:
		if(!calibratIMU && !bmi088_read_locked)
		{
		   BMI088getGyro(); // 角速度取得
		   BMI088getTemp();
		   BMI088getAccele();
		   calcDegrees();   // 角度計算
		}
		break;
	case 2:
		if(calibratIMU)
		{
			calibrationIMU();
		}
		break;
	case 3:
		if(initSDcard && loggingSDcard)
		{
			f_printf(&file, formatLog
					,cnt0
					,BMI088val.accele.x
					,BMI088val.accele.y
					,BMI088val.accele.z
					,BMI088val.gyro.x
					,BMI088val.gyro.y
					,BMI088val.gyro.z
					,BMI088val.angle.x
					,BMI088val.angle.y
					,BMI088val.angle.z
					,BMI088val.temp);
		}
		break;
	case 10:
			cnt10 = 0;
			break;
	}
}
