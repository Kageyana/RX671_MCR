#ifndef BATTERY_H_
#define BATTERY_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
#include <stdint.h>
//====================================//
// シンボル定義
//====================================//
#define GET_BATTERY_VAL R_Config_S12AD1_Get_ValueResult(ADCHANNEL5, &batteryVal)

#define RESISTANCE_DIVIDER_R1 62000 // R1抵抗値
#define RESISTANCE_DIVIDER_R2 12000 // R2抵抗値
#define BATTERY_VOLTAGE_DIVIDER (RESISTANCE_DIVIDER_R1 + RESISTANCE_DIVIDER_R2) / RESISTANCE_DIVIDER_R2 // 電圧分圧比
#define BATTERY_VOLTAGE_MAX 12.0F // バッテリーの最大電圧
//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t batteryVal;
extern uint16_t internalRefVoltage; // AD内部基準電圧値
extern float batteryVoltage;
//====================================//
// プロトタイプ宣言
//====================================//
// バッテリーAD値の取得と電圧計算
void GetBatteryVoltage(void);

#endif // BATTERY_H_
