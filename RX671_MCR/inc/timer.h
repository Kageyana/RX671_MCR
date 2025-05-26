#ifndef TIME_H_
#define TIME_H_
//=====================================//
// インクルード
//=====================================//
#include "r_smc_entry.h"
#include "BMI088.h"
#include "SDcard.h"
#include "ssd1351.h"
#include "switch.h"
#include "encoder.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//=====================================//
// シンボル定義
//=====================================//

//====================================//
// グローバル変数の宣言
//====================================//
extern uint16_t cnt0;
extern uint16_t cnt1;
extern bool initIMU;
extern volatile bool update_display;
//====================================//
// プロトタイプ宣言
//====================================//
void interrupt1ms(void * pdata);

#endif // TIME_H_