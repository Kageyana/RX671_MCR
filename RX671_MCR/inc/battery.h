#ifndef BATTERY_H_
#define BATTERY_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
//====================================//
// シンボル定義
//====================================//
#define GET_BATTERY_VAL R_Config_S12AD1_Get_ValueResult(ADCHANNEL5, &batteryval)
//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t batteryval;
extern float batteryVoltage;
//====================================//
// プロトタイプ宣言
//====================================//
// バッテリーAD値の取得と電圧計算
void GetBatteryADVal(void);

#endif // BATTERY_H_
