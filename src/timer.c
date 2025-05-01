//====================================//
// インクルード
//====================================//
#include "timer.h"
#include "BMI088.h"
#include "ssd1351.h"
#include <stdint.h>
//====================================//
// グローバル変数の宣言
//====================================//
uint16_t cnt10 = 0;
uint16_t cnt0 = 0;
uint8_t useIMU = 0;
uint8_t useDisplay = 0;
volatile bool update_display = false;
/////////////////////////////////////////////////////////////////////
// モジュール名 Interrupt1ms
// 処理概要     タイマー割り込み(1ms)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void Interrupt1ms(void)
{
	cnt0++;
	cnt10++;

	SSD1351_UpdateScreen_Chunked();	

	switch (cnt10)
	{
	case 1:
		
		if(useDisplay == 0 && useIMU == 0)
		{
			useIMU = 1;
			BMI088_getGyro(); // 角速度取得
			// BMI088_getTemp();
			// BMI088_getAccele();
			calcDegrees();	 // 角度計算
			useIMU = 0;
		}
		
		break;
	case 2:
		// if(useIMU == 0 && useDisplay == 0)
		// {
		// 	useDisplay = 1;
		// 	SSD1351_UpdateScreen_Chunked();	
		// 	useDisplay = 0;
		// }
		
		break;
	case 10:
		cnt10 = 0;
		break;
	}
}
