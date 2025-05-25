//====================================//
// インクルード
//====================================//
#include "timer.h"
#include "BMI088.h"
#include "SDcard.h"
#include "ssd1351.h"
#include "r_sdc_sd_rx_if.h"
#include "switch.h"

#include "ff.h"
#include "diskio.h"

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
void interrupt1ms(void * pdata)
{
	cnt0++;
	cnt10++;

	// SSD1351updateScreen();
	getSwitches();
	R_SDC_SD_1msInterval();

	switch (cnt10)
	{
	case 1:
		
		if(BMI088val.Initialized && !calibratIMU)
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
	case 3:
		if(initSDcard && loggingSDcard)
		{
			f_printf(&file, "%d,%f,%f\n"
				,cnt0
				,BMI088val.gyro.z
				,BMI088val.temp);
		}
		break;
	case 10:
		cnt10 = 0;
		break;
	}
}
