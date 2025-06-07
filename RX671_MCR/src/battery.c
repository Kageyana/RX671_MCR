//====================================//
// インクルード
//====================================//
#include "battery.h"
//====================================//
// グローバル変数の宣言
//====================================//
uint16_t batteryVal;
float batteryVoltage;
/////////////////////////////////////////////////////////////////////
// モジュール名 GetBatteryVoltage
// 処理概要     バッテリーのAD値取得と電圧計算
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GetBatteryVoltage(void)
{
	GET_BATTERY_VAL;	// バッテリーのAD値取得
	batteryVoltage = ((float)batteryVal / 4096.0F) * 3.3F; // AD値を電圧に変換
    batteryVoltage = (float)batteryVoltage * BATTERY_VOLTAGE_DIVIDER; // バッテリー電圧計算
}
