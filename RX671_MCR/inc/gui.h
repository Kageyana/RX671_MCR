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
#include <stdint.h>

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
bool GUI_EditContrastRGB(void);
bool GUI_EditContrastMaster(void);
void GUI_DrawTestPattern(uint8_t y_start);

#endif // GUI_H_
