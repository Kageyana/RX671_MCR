//====================================//
// インクルード
//====================================//
#include "gui.h"

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
/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowStartup
// 処理概要     起動画面を表示する
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GUI_ShowStartup(void)
{
	SSD1351fill(SSD1351_BLACK);
	SSD1351setCursor(10, 54);
	SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"RX671_MCR");
}
/////////////////////////////////////////////////////////////////////
// モジュール名  GUI_ShowMenu
// 処理概要     メニューを描画する
//              offset で指定した位置から項目を描画し
//              selected の項目を強調表示する
// 引数         items    : 文字列配列
//              count    : 要素数
//              selected : 選択中インデックス
//              offset   : 表示開始インデックス
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GUI_ShowMenu(const char **items, uint8_t count, uint8_t selected, uint8_t offset)
{
	// メニュー表示エリアを一旦クリアする
	// SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1, SSD1351_HEIGHT - 1, SSD1351_BLACK);

	// 表示できる最大行数を求め、offset から表示する数を決定
	uint8_t visible = MAX_VISIBLE_ITEMS;
	uint8_t show = (count - offset < visible) ? (count - offset) : visible;

	for(uint8_t i = 0; i < show; i++)
	{
		uint8_t idx = offset + i;
		// 表示行を設定
		SSD1351setCursor(2, (uint8_t)(i * MENU_ITEM_HEIGHT + MENU_START_Y));
		// 選択中は黄色で表示
		uint16_t color = (idx == selected) ? SSD1351_YELLOW : SSD1351_WHITE;
		SSD1351printf(Font_7x10, color, (uint8_t*)items[idx]);
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名  GUI_MenuSelect
// 処理概要     スイッチ入力に応じてメニューをスクロールしながら選択する
// 引数         items : 文字列配列
//              count : 要素数
// 戻り値       選択されたインデックス
/////////////////////////////////////////////////////////////////////
uint8_t GUI_MenuSelect(const char **items, uint8_t count)
{
	static uint8_t index = 0;
	static uint8_t top   = 0;

	GUI_ShowMenu(items, count, index, top);

	switch(swValTact)
	{
	case SW_UP:
		// UP が押された場合
		if(index == 0)
		{
			// 先頭からUPで末尾へ循環
			index = count - 1;
			if(count > MAX_VISIBLE_ITEMS)
			{
				top = count - MAX_VISIBLE_ITEMS;
			}
		}
		else
		{
			index--;
			// 表示範囲より上に移動した場合はスクロール
			if(index < top)
			{
				top--;
			}
		}
		GUI_ShowMenu(items, count, index, top);
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
		break;
	case SW_DOWN:
		// DOWN が押された場合
		if(index + 1 >= count)
		{
			// 末尾からDOWNで先頭へ循環
			index = 0;
			top   = 0;
		}
		else
		{
			index++;
			// 表示範囲を超えたら下方向へスクロール
			if(index >= top + MAX_VISIBLE_ITEMS)
			{
				top++;
			}
		}
		GUI_ShowMenu(items, count, index, top);
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
		break;
	case SW_PUSH:
		// 決定ボタンが押されたら現在の項目を返す
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
		return index;
	default:
		break;
	}

	return 0xFF;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowStatusBar
// 処理概要     ステータスバーにページ番号とバッテリー残量を表示する
// 引数         page : 表示するページ番号
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GUI_ShowStatusBar(uint8_t page)
{
	uint8_t percent = (uint8_t)((batteryVoltage / 12.0F) * 100.0F + 0.5F);
	if(percent > 100) percent = 100;
	SSD1351fillRectangle(0, 0, SSD1351_WIDTH - 1, 9, SSD1351_BLACK);
	SSD1351setCursor(2, 0);
	SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"P:%x", page);
	SSD1351setCursor(71, 0);
	SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"BAT:%3d%%", percent);
}

/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_EditContrastRGB
// 処理概要     RGBコントラストを編集する
// 引数         なし
// 戻り値       true:編集終了
/////////////////////////////////////////////////////////////////////
bool GUI_EditContrastRGB(void)
{
	static uint8_t contrast = 0x64;
	static bool init = false;

	if(!init)
	{
	SSD1351fill(SSD1351_BLACK);
	init = true;
	}

	SSD1351setCursor(2, 20);
	SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"RGB CONT");
	SSD1351setCursor(2, 36);
	SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"VAL:%3d", contrast);

	switch(swValTact)
	{
	case SW_LEFT:
		if(contrast > 0) contrast--;
		display_update_locked = true;
		bmi088_read_locked = true;
		// SPIバスがフリーになるまで待機
		while(!spi_ssd1351_tx_done || !spi_BMI088_tx_done);
		SSD1351setContrastRGB(contrast, contrast, contrast);
		display_update_locked = false;
		bmi088_read_locked = false;
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
	break;
	case SW_RIGHT:
		if(contrast < 255) contrast++;
		display_update_locked = true;
		bmi088_read_locked = true;
		// SPIバスがフリーになるまで待機
		while(!spi_ssd1351_tx_done || !spi_BMI088_tx_done);
		SSD1351setContrastRGB(contrast, contrast, contrast);
		display_update_locked = false;
		bmi088_read_locked = false;
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
	break;
	case SW_PUSH:
	R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
	init = false;
	return true;
	default:
	break;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_EditContrastMaster
// 処理概要     マスターコントラストを編集する
// 引数         なし
// 戻り値       true:編集終了
/////////////////////////////////////////////////////////////////////
bool GUI_EditContrastMaster(void)
{
	static uint8_t contrast = 0x08;
	static bool init = false;

	if(!init)
	{
	SSD1351fill(SSD1351_BLACK);
	init = true;
	}

	SSD1351setCursor(2, 20);
	SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"MASTER CONT");
	SSD1351setCursor(2, 36);
	SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"VAL:%3d", contrast);

	switch(swValTact)
	{
	case SW_LEFT:
		if(contrast > 0) contrast--;
		display_update_locked = true;
		bmi088_read_locked = true;
		// SPIバスがフリーになるまで待機
		while(!spi_ssd1351_tx_done || !spi_BMI088_tx_done);
		SSD1351setContrastMaster(contrast);
		display_update_locked = false;
		bmi088_read_locked = false;
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
	break;
	case SW_RIGHT:
		if(contrast < 255) contrast++;
		display_update_locked = true;
		bmi088_read_locked = true;
		// SPIバスがフリーになるまで待機
		while(!spi_ssd1351_tx_done || !spi_BMI088_tx_done);
		SSD1351setContrastMaster(contrast);
		display_update_locked = false;
		bmi088_read_locked = false;
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
	break;
	case SW_PUSH:
		R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
		init = false;
		return true;
	default:
	break;
	}

	return false;
}

