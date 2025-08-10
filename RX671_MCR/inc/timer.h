#ifndef TIMER_H_
#define TIMER_H_
//=====================================//
// インクルード
//=====================================//
#include "r_smc_entry.h"
#include "BMI088.h"
#include "SDcard.h"
#include "ssd1351.h"
#include "switch.h"
#include "encoder.h"
#include "battery.h"
#include "setup.h"
#include "control.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//=====================================//
// シンボル定義
//=====================================//

//====================================//
// グローバル変数の宣言
//====================================//
extern volatile uint16_t cnt0;
extern uint16_t cnt1;
extern bool initIMU;
//====================================//
// プロトタイプ宣言
//====================================//
void interrupt1ms(void * pdata);

#endif // TIMER_H_
