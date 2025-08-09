//====================================//
// インクルード
//====================================//
#include "linesensor.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint16_t lineSenVal[LINESENSOR_NUM];
/////////////////////////////////////////////////////////////////////
// モジュール名 InitLineSensors
// 処理概要     ラインセンサの初期化
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void InitLineSensors(void)
{
	SET_MTU_LINESENSORS; // ラインセンサのMTUを開始
	PowerLineSensors(0); // ラインセンサの電源をOFF
	MTU7.TGRA = TGR_LINESENSORS;
}

/////////////////////////////////////////////////////////////////////
// モジュール名 GetLineSenADVal
// 処理概要     ラインセンサのAD値を取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GetLineSenADVal(void)
{
	R_Config_S12AD0_Get_ValueResult(ADCHANNEL1, &lineSenVal[0]);
	R_Config_S12AD0_Get_ValueResult(ADCHANNEL2, &lineSenVal[1]);
	R_Config_S12AD0_Get_ValueResult(ADCHANNEL3, &lineSenVal[2]);
	R_Config_S12AD0_Get_ValueResult(ADCHANNEL4, &lineSenVal[3]);
	R_Config_S12AD0_Get_ValueResult(ADCHANNEL5, &lineSenVal[4]);
	R_Config_S12AD0_Get_ValueResult(ADCHANNEL6, &lineSenVal[5]);
	R_Config_S12AD0_Get_ValueResult(ADCHANNEL7, &lineSenVal[6]);
}/////////////////////////////////////////////////////////////////////
// モジュール名 GetLineSenADVal
// 処理概要     ラインセンサのAD値を取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void PowerLineSensors(bool power)
{
	if(power)
	{
		// MTU7.TGRA = TGR_MOTOR+1;
		PWM_LINESENSORS_OUT = TGR_LINESENSORS+2;
	}
	else
	{
		PWM_LINESENSORS_OUT = 0;
	}
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 sensor_inp
// 処理概要     デジタルセンサの値を16進数で取得
// 引数         なし
// 戻り値       センサ値0～
///////////////////////////////////////////////////////////////////////////
unsigned char sensor_inp(void) 
{
	char l, c, r;
	
	if (lineSenVal[5] < 350 ) r = 0x1;
	else r = 0;
	if (lineSenVal[2] < 350 ) c = 0x2;
	else c = 0;
	if (lineSenVal[0] < 350 ) l = 0x4;
	else l = 0;
	
	return l+c+r;
}