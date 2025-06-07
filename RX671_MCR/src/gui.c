//====================================//
// インクルード
//====================================//
#include "gui.h"

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
// モジュール名 GUI_ShowMenu
// 処理概要     メニューを描画する
// 引数         items: 文字列配列 count: 要素数 selected: 選択中インデックス
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void GUI_ShowMenu(const char **items, uint8_t count, uint8_t selected)
{
    SSD1351fill(SSD1351_BLACK);
    for(uint8_t i = 0; i < count; i++)
    {
        SSD1351setCursor(2, (uint8_t)(i * 12 + 2));
        uint16_t color = (i == selected) ? SSD1351_YELLOW : SSD1351_WHITE;
        SSD1351printf(Font_7x10, color, (uint8_t*)items[i]);
    }
}

/////////////////////////////////////////////////////////////////////
// モジュール名 GUI_MenuSelect
// 処理概要     ボタン操作でメニューを選択する
// 引数         items: 文字列配列 count: 要素数
// 戻り値       選択されたインデックス
/////////////////////////////////////////////////////////////////////
uint8_t GUI_MenuSelect(const char **items, uint8_t count)
{
    uint8_t index = 0;
    GUI_ShowMenu(items, count, index);

    while(1)
    {
        switch(swValTact)
        {
        case SW_UP:
            if(index == 0)
            {
                index = count - 1; // 先頭からUPで末尾へ循環
            }
            else
            {
                index--;
            }
            GUI_ShowMenu(items, count, index);
            R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
            break;
        case SW_DOWN:
            if(index + 1 >= count)
            {
                index = 0; // 末尾からDOWNで先頭へ循環
            }
            else
            {
                index++;
            }
            GUI_ShowMenu(items, count, index);
            R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
            break;
        case SW_PUSH:
            R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
            return index;
        default:
            break;
        }
    }
}

