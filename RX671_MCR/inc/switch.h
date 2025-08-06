#ifndef SWITCH_H_
#define SWITCH_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
//====================================//
// シンボル定義
//====================================//
#define SW_NONE 0x0
#define SW_UP 0x1
#define SW_PUSH 0x2
#define SW_LEFT 0x3
#define SW_RIGHT 0x4
#define SW_DOWN 0x5

/************* 自動生成関数 **************/
// タクトスイッチ
#define TACTSW1 PORT5.PIDR.BIT.B6 // 上
#define TACTSW2 PORT1.PIDR.BIT.B3 // 押し込み
#define TACTSW3 PORT2.PIDR.BIT.B4 // 左
#define TACTSW4 PORT1.PIDR.BIT.B2 // 右
#define TACTSW5 PORT5.PIDR.BIT.B4 // 下
// ロータリーコードスイッチ
#define DIPSW1 PORT9.PIDR.BIT.B1
#define DIPSW2 PORT9.PIDR.BIT.B2
#define DIPSW4 PORT9.PIDR.BIT.B3
#define DIPSW8 PORT9.PIDR.BIT.B0
/****************************************/

//====================================//
// グローバル変数の宣言
//====================================//
extern uint8_t swValTact;
extern uint8_t swValRotary;
//====================================//
// プロトタイプ宣言
//====================================//
uint8_t GetSW5aAxisTact(void);
uint8_t GetSWRotary(void);
void GetSwitches(void);
#endif // SWITCH_H_
