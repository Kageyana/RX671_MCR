//====================================//
// インクルード
//====================================//
#include "switch.h"
//====================================//
// グローバル変数の宣言
//====================================//
// スイッチ関連
uint8_t swValTact;
uint8_t swValRotary;
/////////////////////////////////////////////////////////////////////
// モジュール名 getSwitches
// 処理概要  	スイッチの読み込み(10msごとに実行)
// 引数     	なし
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
void GetSwitches(void)
{
	// 5方向タクトスイッチ読み込み
	swValTact = GetSW5aAxisTact();
	// メインボード上のタクトスイッチ読み込み
	swValRotary = GetSWRotary();
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getSW5aAxisTact
// 処理概要     タクトスイッチ値を16進数で取得
// 引数         なし
// 戻り値       スイッチ値 0～7
///////////////////////////////////////////////////////////////////////////
uint8_t GetSW5aAxisTact(void)
{
	uint8_t ret = SW_NONE;

	if(TACTSW1 == 0) ret = SW_UP;
	if(TACTSW2 == 0) ret = SW_PUSH;
	if(TACTSW3 == 0) ret = SW_LEFT;
	if(TACTSW4 == 0) ret = SW_RIGHT;
	if(TACTSW5 == 0) ret = SW_DOWN;	
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 getSWMainTact
// 処理概要     タクトスイッチ値を16進数で取得
// 引数         なし
// 戻り値       スイッチ値 0～7
///////////////////////////////////////////////////////////////////////////
uint8_t GetSWRotary(void)
{
	uint8_t ret = 0x0;

	if(DIPSW1 == 0) ret |= 0x01;
	if(DIPSW2 == 0) ret |= 0x02;
	if(DIPSW4 == 0) ret |= 0x04;
	if(DIPSW8 == 0) ret |= 0x08;

	return ret;
}