#ifndef GUI_H_
#define GUI_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"

#include "ssd1351.h"
#include "switch.h"
#include "battery.h"
#include "bmi088.h"
#include "linesensor.h"
#include "encoder.h"
#include <stdbool.h>
#include <stdint.h>
//=====================================//
// シンボル定義
//=====================================//
// メニュー表示に関する定数
// MENU_START_Y : メニューエリアの開始Y座標
// MENU_ITEM_HEIGHT : 1行あたりの高さ
// MAX_VISIBLE_ITEMS : 一度に表示できる行数
#define MENU_START_Y 12
#define MENU_ITEM_HEIGHT 12
#define MAX_VISIBLE_ITEMS (((SSD1351_HEIGHT - MENU_START_Y) / MENU_ITEM_HEIGHT) + 1)

//====================================//
// グローバル変数の宣言
//====================================//
extern volatile uint32_t cntGUI;	// GUI用カウンタ
//====================================//
// プロトタイプ宣言
//====================================//
void GUI_wait(uint32_t ms);
void GUI_ShowStartup(void);
void GUI_ShowMenu(const char **items, uint8_t count, uint8_t selected, uint8_t offset);
uint8_t GUI_MenuSelect(const char **items, uint8_t count);
void GUI_ShowStatusBar(uint8_t page);
bool GUI_EditContrast(void);
void GUI_DrawTestPattern(uint8_t y_start);
bool GUI_DisplayInverse(void);
bool GUI_ShowQRcode(void);
bool GUI_ShowSensors(void);
#endif // GUI_H_
