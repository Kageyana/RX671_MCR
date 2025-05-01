#ifndef TIME_H_
#define TIME_H_
//=====================================//
// インクルード
//=====================================//
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
extern uint8_t useIMU, useDisplay;
extern volatile bool update_display;
//====================================//
// プロトタイプ宣言
//====================================//
void Interrupt1ms(void);

#endif // TIME_H_