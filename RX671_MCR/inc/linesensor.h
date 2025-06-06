#ifndef LINESENSOR_H_
#define LINESENSOR_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
//====================================//
// シンボル定義
//====================================//
#define LINESENSOR_NUM 7
//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t lineSenVal[LINESENSOR_NUM];
//====================================//
// プロトタイプ宣言
//====================================//
void GetLineSenADVal(void);

#endif // LINESENSOR_H_
