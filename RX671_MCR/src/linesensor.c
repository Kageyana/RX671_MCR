//====================================//
// インクルード
//====================================//
#include "linesensor.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint16_t lineSenVal[LINESENSOR_NUM];
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
}
