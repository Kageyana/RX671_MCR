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

#define TGR_LINESENSORS		59998	// ジェネラルレジスタ初期値(ラインセンサ) TGRA初期値-1
/*************************** 自動生成関数 **************************/
// MTUスタート
#define SET_MTU_LINESENSORS	R_Config_MTU7_Start()
#define PWM_LINESENSORS_OUT			MTU7.TGRD			// PWM出力
/******************************************************************************************/
//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t lineSenVal[LINESENSOR_NUM];
//====================================//
// プロトタイプ宣言
//====================================//
void InitLineSensors(void);
void GetLineSenADVal(void);
void PowerLineSensors(bool power);

#endif // LINESENSOR_H_
