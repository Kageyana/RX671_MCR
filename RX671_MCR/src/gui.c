//====================================//
// インクルード
#include "gui.h"
#include "bmi088.h"
#include "images.h"
#include "ssd1351.h"
#include "switch.h"
#include "sys/types.h"
#include "timer.h"
#include <stdbool.h>
#include <stdint.h>

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
volatile uint32_t cntGUI;	// GUI用カウンタ
/////////////////////////////////////////////////////////////////////
// センサページ用の状態を保持する列挙体
typedef enum {
    SENSOR_MENU, // トップメニュー
    SENSOR_BAT,  // バッテリー電圧表示
    SENSOR_IMU,  // IMUの角度・温度・加速度表示
    SENSOR_ENC,  // エンコーダ値表示
    SENSOR_LINE  // ラインセンサ値表示
} SensorState;

// センサページの状態変数
static SensorState sensor_state = SENSOR_MENU;
static uint8_t     sensor_sel   = 0xff;
// モジュール名 GUI_wait
// 処理概要     指定ミリ秒だけ待機する
// 引数         ms: 待機時間(ミリ秒)
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GUI_wait(uint32_t ms)
{
	cntGUI = 0;
	while(cntGUI < ms);
}
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
	SSD1351line(0,MENU_START_Y-2, SSD1351_WIDTH-1, MENU_START_Y-2, SSD1351_WHITE);
}

/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_DrawTestPattern
// 処理概要     テレビのカラーバーを画面下部に表示する
// 引数         y_start : パターンを描画する開始Y座標
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GUI_DrawTestPattern(uint8_t y_start)
{
	const uint16_t colors[8] = {
			SSD1351_WHITE,
			SSD1351_YELLOW,
			SSD1351_CYAN,
			SSD1351_GREEN,
			SSD1351_MAGENTA,
			SSD1351_RED,
			SSD1351_BLUE,
			SSD1351_BLACK
	};
	uint8_t width = SSD1351_WIDTH / 8;
	for(uint8_t i = 0; i < 8; i++)
	{
		uint8_t x1 = i * width;
		uint8_t x2 = (i + 1) * width - 1;
		SSD1351fillRectangle(x1, y_start, x2, SSD1351_HEIGHT - 1, colors[i]);
	}
}

/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_EditContrast
// 処理概要     コントラストを編集する
// 引数         なし
// 戻り値       true:編集終了
/////////////////////////////////////////////////////////////////////
bool GUI_EditContrast(void)
{
	static uint8_t contrast[4] = {0x08, 0x64, 0x64, 0x64};
	static uint8_t index = 0; // 0:R 1:G 2:B
	static bool init = false;

	if(!init)
	{
		// 初期化処理
		SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
												SSD1351_HEIGHT - 1, SSD1351_BLACK);
		SSD1351setCursor(2, MENU_START_Y);
		SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"CONTRAST");
		GUI_DrawTestPattern(SSD1351_HEIGHT - 40);
		bmi088_read_locked = true;
		GUI_wait(200); // 200ms待機
		init = true;
	}

	const uint8_t *labels[] = {"MASTER","R","G","B"};
	for(uint8_t i = 0; i < 4; i++)
	{
		uint16_t color = (i == index) ? SSD1351_YELLOW : SSD1351_WHITE;
		if(i > 0)
		{
			SSD1351setCursor(2 + (40*(i-1)), MENU_START_Y + 24);
		}
		else
		{
			SSD1351setCursor(2, MENU_START_Y + 12);
		}
		SSD1351printf(Font_7x10, color, (uint8_t*)"%s:%3d", labels[i], contrast[i]);
	}

	switch(swValTact)
	{
		case SW_LEFT:
			if(index == 0) index = 3; else index--;
			GUI_wait(200);
			break;
		case SW_RIGHT:
			index = (index + 1) % 4;
			GUI_wait(200);
			break;
		case SW_UP:
			display_update_locked = true;
			while(!spi_BMI088_rx_done && !spi_ssd1351_tx_done);
			if(index > 0)
			{
				if(contrast[index] < 255) contrast[index]++;
				SSD1351setContrastRGB(contrast[1], contrast[2], contrast[3]);
			}
			else
			{
				if(contrast[index] < 15) contrast[index]++;
				SSD1351setContrastMaster(contrast[0]);
			}
			display_update_locked = false;
			GUI_wait(120);
			break;
		case SW_DOWN:
			display_update_locked = true;
			while(!spi_BMI088_rx_done && !spi_ssd1351_tx_done);
			if(index > 0)
			{
				if(contrast[index] > 0) contrast[index]--;
				SSD1351setContrastRGB(contrast[1], contrast[2], contrast[3]);
			}
			else
			{
				if(contrast[index] > 0) contrast[index]--;
				SSD1351setContrastMaster(contrast[0]);
			}
			display_update_locked = false;
			GUI_wait(120);
			break;
		case SW_PUSH:
			GUI_wait(200); // 200ms待機
			SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
											SSD1351_HEIGHT - 1, SSD1351_BLACK);
			bmi088_read_locked = false;
			init = false;
			return true;
		default:
			break;
	}

	return false;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_DisplayInverse
// 処理概要     色反転のON/OFF
// 引数         なし
// 戻り値       true:編集終了
/////////////////////////////////////////////////////////////////////
bool GUI_DisplayInverse(void)
{
	static bool inverse = false;

	inverse = !inverse;
	SSD1351InvertColors(inverse);
	GUI_wait(200);

	return true;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowQRcode
// 処理概要     QRコードの表示
// 引数         なし
// 戻り値       true:編集終了
/////////////////////////////////////////////////////////////////////
bool GUI_ShowQRcode(void)
{
	const uint8_t *menu_items[] = {
		  "X       "
		, "GitHub  "
		, "Back    "
	};

	static uint8_t sel = 0xff;
	static bool show = false;
	static bool init = false;

	if(!init)
	{
		SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
								SSD1351_HEIGHT - 1, SSD1351_BLACK);
		GUI_wait(200);
		sel = 0xff;
		init = true;
	}

	switch (sel) {
		case 0:
			if(!show)
			{
				SSD1351drawImage(0,0,SSD1351_WIDTH, SSD1351_HEIGHT,imgQRx128x128);
				GUI_wait(300);
				show = true;
			}
			break;
		case 1:
			if(!show)
			{
				SSD1351drawImage(0,0,SSD1351_WIDTH, SSD1351_HEIGHT,imgQRgithub128x128);
				GUI_wait(300);
				show = true;
			}
			break;
		case 2:
			GUI_wait(150);
			init = false;
			return true;
		default:
			if(!show)
			{
				sel = GUI_MenuSelect(menu_items,3);
			}
			break;
	}

	if(sel != 0xff)
	{
		if(swValTact == SW_PUSH)
		{
			SSD1351fill(SSD1351_BLACK);
			GUI_ShowStatusBar(swValRotary);
			GUI_wait(200);
			sel = 0xff;
			show = false;
		}
	}
	

        return false;
}

/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowSensors
// 処理概要     センサ値をメニューで選択して個別表示する
// 引数         なし
// 戻り値       true:ページ終了
/////////////////////////////////////////////////////////////////////
bool GUI_ShowSensors(void)
{

        const uint8_t *sensor_items[] = {
                  "Battery ",
                  "IMU     ",
                  "Encoder ",
                  "Line    "
        };

        if(sensor_state == SENSOR_MENU && sensor_sel == 0xff)
        {
                sensor_sel = GUI_MenuSelect(sensor_items, 4);
        }

        switch(sensor_state)
        {
        case SENSOR_MENU:
                if(sensor_sel == 0xff)
                {
                        sensor_sel = GUI_MenuSelect(sensor_items, 4);
                }
                else
                {
                        switch(sensor_sel)
                        {
                        case 0:
                                sensor_state = SENSOR_BAT;
                                break;
                        case 1:
                                sensor_state = SENSOR_IMU;
                                break;
                        case 2:
                                sensor_state = SENSOR_ENC;
                                break;
                        case 3:
                                sensor_state = SENSOR_LINE;
                                break;
                        default:
                                break;
                        }
                        if(sensor_state != SENSOR_MENU)
                        {
                                sensor_sel = 0xff;
                                SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                                        SSD1351_HEIGHT - 1, SSD1351_BLACK);
                        }
                }
                break;

        case SENSOR_BAT: // バッテリー電圧
                GetBatteryVoltage();
                SSD1351setCursor(2, MENU_START_Y);
                SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"BAT:%4.1fV", batteryVoltage);
                if(swValTact == SW_PUSH)
                {
                        GUI_wait(150);
                        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                                        SSD1351_HEIGHT - 1, SSD1351_BLACK);
                        sensor_state = SENSOR_MENU;
                        sensor_sel   = 0xff;
                }
                break;

        case SENSOR_IMU: // IMU各値を表示
                // 角度のヘッダー
                SSD1351setCursor(2, MENU_START_Y);
                SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"ANG");
                // X/Y/Z の角度
                SSD1351setCursor(2, MENU_START_Y + 12);
                SSD1351printf(Font_7x10, SSD1351_WHITE,
                              (uint8_t*)"X:%4dY:%4dZ:%4d",
                              (int16_t)BMI088val.angle.x,
                              (int16_t)BMI088val.angle.y,
                              (int16_t)BMI088val.angle.z);
                // 温度
                SSD1351setCursor(2, MENU_START_Y + 24);
                SSD1351printf(Font_7x10, SSD1351_WHITE,
                              (uint8_t*)"TEMP:%3d",
                              (int16_t)BMI088val.temp);
                // 加速度のヘッダー
                SSD1351setCursor(2, MENU_START_Y + 36);
                SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"ACC");
                // X/Y/Z の加速度
                SSD1351setCursor(2, MENU_START_Y + 48);
                SSD1351printf(Font_7x10, SSD1351_WHITE,
                              (uint8_t*)"X:%4dY:%4dZ:%4d",
                              (int16_t)BMI088val.accele.x,
                              (int16_t)BMI088val.accele.y,
                              (int16_t)BMI088val.accele.z);
                if(swValTact == SW_PUSH)
                {
                        GUI_wait(150);
                        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                                SSD1351_HEIGHT - 1, SSD1351_BLACK);
                        sensor_state = SENSOR_MENU;
                        sensor_sel   = 0xff;
                }
                break;

        case SENSOR_ENC: // エンコーダ表示
                SSD1351setCursor(2, MENU_START_Y);
                SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"ENC:%ld", encTotal);
                if(swValTact == SW_PUSH)
                {
                        GUI_wait(150);
                        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                                        SSD1351_HEIGHT - 1, SSD1351_BLACK);
                        sensor_state = SENSOR_MENU;
                        sensor_sel   = 0xff;
                }
                break;

        case SENSOR_LINE: // ラインセンサ表示
                SSD1351setCursor(2, MENU_START_Y);
                SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"LS:%4d %4d %4d",
                                lineSenVal[0], lineSenVal[1], lineSenVal[2]);
                SSD1351setCursor(2, MENU_START_Y + 12);
                SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"%4d %4d %4d",
                                lineSenVal[3], lineSenVal[4], lineSenVal[5]);
                SSD1351setCursor(2, MENU_START_Y + 24);
                SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"%4d", lineSenVal[6]);
                if(swValTact == SW_PUSH)
                {
                        GUI_wait(150);
                        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                                        SSD1351_HEIGHT - 1, SSD1351_BLACK);
                        sensor_state = SENSOR_MENU;
                        sensor_sel   = 0xff;
                }
                break;
        }

        return false;
}
