//====================================//
// インクルード
//====================================//
#include "timer.h"
#include "BMI088.h"
#include "ssd1351.h"
#include "switch.h"
#include <stdbool.h>
#include <stdint.h>
//====================================//
// グローバル変数の宣言
//====================================//
uint16_t cnt10 = 0;
uint16_t cnt0 = 0;
bool initIMU = false;
volatile bool update_display = false;
/////////////////////////////////////////////////////////////////////
// モジュール名 interrupt1ms
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void interrupt1ms(void)
{
	cnt0++;
	cnt10++;

	SSD1351updateScreen();
	getSwitches();

	switch (cnt10)
	{
	case 1:
		
		if(initIMU && !calibratIMU)
		{
			BMI088getGyro(); // 角速度取得
			BMI088getTemp();
			BMI088getAccele();
			calcDegrees();	 // 角度計算
		}
		
		break;
	case 2:
		if(calibratIMU)
		{
			calibrationIMU();
		}
		break;
	case 10:
		cnt10 = 0;
		break;
	}
}
