#ifndef ENCODER_H_
#define ENCODER_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
#include <stdlib.h>
//====================================//
// シンボル定義
//====================================//
#define PULSE_METER 		14590	// 1m走行時のカウント
#define PULSE_MILLIMETER 	14.59F	// 1mmのカウント↑を1/1000

#define MTU_CHANNEL MTU2
//====================================//
// グローバル変数の宣言
//====================================//
extern int16_t encCurrent;
extern int32_t encTotal;
//====================================//
// プロトタイプ宣言
//====================================//
void InitEncoder(void);
void GetEncoderVal(void);
int32_t encMM(uint16_t mm);

#endif // ENCODER_H_
