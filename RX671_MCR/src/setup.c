//====================================//
// インクルード
//====================================//
#include "setup.h"

// 他ファイルで定義されている現在のページ番号
extern uint8_t currentPage;

//====================================//
// グローバル変数の宣言
//====================================//
volatile uint32_t cntGUI; // GUI用カウンタ

typedef enum {
    SENSOR_MENU, // トップメニュー
    SENSOR_BAT,  // バッテリー電圧表示
    SENSOR_IMU,  // IMUの角度・温度・加速度表示
    SENSOR_ENC,  // エンコーダ値表示
    SENSOR_LINE, // ラインセンサ値表示
    SENSOR_MOTOR // モーター動作確認
} SensorState;

static SensorState sensor_state = SENSOR_MENU;
static uint8_t     sensor_sel   = 0xff;

typedef enum {
    DISP_MENU,     // トップメニュー
    DISP_CONTRAST, // コントラスト設定
    DISP_INVERSE,  // 反転表示設定
    DISP_QR        // QRコード表示
} DisplayState;

static DisplayState display_state = DISP_MENU; // ディスプレイ設定ページの状態
static uint8_t      display_sel   = 0xff;      // メニュー選択結果

typedef struct {
    const uint8_t **items; // メニュー項目配列へのポインタ
    uint8_t         top;   // 表示オフセット
} MenuState;

static MenuState menu_states[MAX_MENU_STATE];
static uint8_t menu_state_count = 0;
// Start ページのメニュー項目
static const uint8_t *menu1_items[] = {
    "START   ",
    "SETTINGS"
};

///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_wait
// 処理概要     指定ミリ秒だけ待機する
// 引数         ms: 待機時間(ミリ秒)
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void GUI_wait(uint32_t ms)
{
    cntGUI = 0;
    while(cntGUI < ms);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowStartup
// 処理概要     起動画面を表示する
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void GUI_ShowStartup(void)
{
    SSD1351fill(SSD1351_BLACK);
    SSD1351setCursor(10, 54);
    SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"RX671_MCR");
}
///////////////////////////////////////////////////////////////////////////
// モジュール名  GUI_ShowMenu
// 処理概要     メニューを描画する
//              offset で指定した位置から項目を描画し
//              selected の項目を強調表示する
// 引数         items    : 文字列配列
//              count    : 要素数
//              selected : 選択中インデックス
//              offset   : 表示開始インデックス
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void GUI_ShowMenu(const char **items, uint8_t count, uint8_t selected, uint8_t offset)
{
    uint8_t visible = MAX_VISIBLE_ITEMS;
    uint8_t show = (count - offset < visible) ? (count - offset) : visible;

    for(uint8_t i = 0; i < show; i++)
    {
        uint8_t idx = offset + i;
        SSD1351setCursor(2, (uint8_t)(i * MENU_ITEM_HEIGHT + MENU_START_Y));
        uint16_t color = (idx == selected) ? SSD1351_YELLOW : SSD1351_WHITE;
        SSD1351printf(Font_7x10, color, (uint8_t*)items[idx]);
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名  GetMenuTop
// 処理概要     指定したメニュー項目のスクロール位置を取得する
//              未登録のメニューは新規に登録し初期値0を返す
// 引数         items : メニュー項目配列へのポインタ
// 戻り値       top値へのポインタ
///////////////////////////////////////////////////////////////////////////
static uint8_t *GetMenuTop(const uint8_t **items)
{
    // 既存エントリを検索し、見つかればそのtopへのポインタを返す
    for(uint8_t i = 0; i < menu_state_count; i++)
    {
        if(menu_states[i].items == items)
        {
            return &menu_states[i].top;
        }
    }

    // 新規メニューなら空きスロットに登録して初期値0を返す
    if(menu_state_count < MAX_MENU_STATE)
    {
        menu_states[menu_state_count].items = items;
        menu_states[menu_state_count].top   = 0;
        return &menu_states[menu_state_count++].top;
    }

    // これ以上登録できない場合は0番目を再利用する
    return &menu_states[0].top;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名  GUI_MenuSelect
// 処理概要     スイッチ入力に応じてメニューをスクロールしながら選択する
// 引数         items : メニュー項目(文字列配列)
//              count : 要素数
// 戻り値       選択されたインデックス
///////////////////////////////////////////////////////////////////////////
uint8_t GUI_MenuSelect(const char **items, uint8_t count)
{
    static uint8_t index = 0;
    uint8_t *top_ptr = GetMenuTop((const uint8_t **)items);
    uint8_t top = *top_ptr;

    GUI_ShowMenu(items, count, index, top);

    switch(swValTact)
    {
        case SW_UP:
            if(index == 0)
            {
                index = count - 1;
                if(count > MAX_VISIBLE_ITEMS)
                {
                    top = count - MAX_VISIBLE_ITEMS;
                }
            }
            else
            {
                index--;
                if(index < top)
                {
                    top--;
                }
            }
            GUI_ShowMenu(items, count, index, top);
            *top_ptr = top;
            R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
            break;
        case SW_DOWN:
            if(index + 1 >= count)
            {
                index = 0;
                top   = 0;
            }
            else
            {
                index++;
                if(index >= top + MAX_VISIBLE_ITEMS)
                {
                    top++;
                }
            }
            GUI_ShowMenu(items, count, index, top);
            *top_ptr = top;
            R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
            break;
        case SW_PUSH:
            R_BSP_SoftwareDelay(150, BSP_DELAY_MILLISECS);
            return index;
        default:
            break;
    }

    *top_ptr = top;

    return 0xFF;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowStatusBar
// 処理概要     ステータスバーにページ番号とバッテリー残量を表示する
// 引数         page : 表示するページ番号
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void GUI_ShowStatusBar(uint8_t page)
{
    uint8_t percent = (uint8_t)((batteryVoltage / 12.0F) * 100.0F + 0.5F);
    if(percent > 100) percent = 100;
    SSD1351fillRectangle(0, 0, SSD1351_WIDTH - 1, 9, SSD1351_BLACK);
    SSD1351setCursor(2, 0);
    SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"P:%x", page);
    SSD1351setCursor(71, 0);
    SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"BAT:%3d%%", percent);
    SSD1351line(0, MENU_START_Y - 2, SSD1351_WIDTH - 1, MENU_START_Y - 2, SSD1351_WHITE);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_DrawTestPattern
// 処理概要     テレビのカラーバーを画面下部に表示する
// 引数         y_start : パターンを描画する開始Y座標
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_EditContrast
// 処理概要     コントラストを編集する
// 引数         なし
// 戻り値       true:編集終了
///////////////////////////////////////////////////////////////////////////
bool GUI_EditContrast(void)
{
    static uint8_t contrast[4] = {0x08, 0x64, 0x64, 0x64};
    static uint8_t index = 0; // 0:R 1:G 2:B
    static bool init = false;

    if(!init)
    {
        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                             SSD1351_HEIGHT - 1, SSD1351_BLACK);
        SSD1351setCursor(2, MENU_START_Y);
        SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"CONTRAST");
        GUI_DrawTestPattern(SSD1351_HEIGHT - 40);
        bmi088_read_locked = true;
        GUI_wait(200);
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
            GUI_wait(200);
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
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_DisplayInverse
// 処理概要     色反転のON/OFF
// 引数         なし
// 戻り値       true:編集終了
///////////////////////////////////////////////////////////////////////////
bool GUI_DisplayInverse(void)
{
    static bool inverse = false;

    inverse = !inverse;
    SSD1351InvertColors(inverse);
    GUI_wait(200);

    return true;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowQRcode
// 処理概要     QRコードの表示
// 引数         なし
// 戻り値       true:編集終了
///////////////////////////////////////////////////////////////////////////
bool GUI_ShowQRcode(void)
{
    const uint8_t *menu_items[] = {
        "X       ",
        "GitHub  ",
        "Back    "
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

///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowDisplaySetting
// 処理概要     ディスプレイ設定ページの処理
// 引数         なし
// 戻り値       true:ページ終了
///////////////////////////////////////////////////////////////////////////
bool GUI_ShowDisplaySetting(void)
{
    static bool disp_menu_init = true; // 初期描画の有無
    const uint8_t *disp_items[] = {
        "Contrast",
        "Inverse ",
        "QR code "
    };

    // 初回のみメニュー領域をクリア
    if(display_state == DISP_MENU && disp_menu_init)
    {
        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                             SSD1351_HEIGHT - 1, SSD1351_BLACK);
        disp_menu_init = false;
    }

    // メニュー選択が未確定なら入力を待つ
    if(display_state == DISP_MENU && display_sel == 0xff)
    {
        display_sel = GUI_MenuSelect(disp_items, 3);
    }

    switch(display_state)
    {
        case DISP_MENU: // 設定項目の選択
            if(display_sel == 0xff)
            {
                display_sel = GUI_MenuSelect(disp_items, 3);
            }
            else
            {
                // 選択結果に応じてサブメニューへ遷移
                switch(display_sel)
                {
                    case 0: display_state = DISP_CONTRAST; break; // コントラスト設定
                    case 1: display_state = DISP_INVERSE;  break; // 色反転設定
                    case 2: display_state = DISP_QR;       break; // QRコード表示
                    default: break;
                }
                if(display_state != DISP_MENU)
                {
                    // サブメニュー遷移時は入力と選択状態をリセット
                    swValTact   = SW_NONE;
                    display_sel = 0xff;
                    SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                         SSD1351_HEIGHT - 1, SSD1351_BLACK);
                }
            }
            break;

        case DISP_CONTRAST: // コントラスト調整処理
            if(GUI_EditContrast())
            {
                display_state  = DISP_MENU;
                display_sel    = 0xff;
                disp_menu_init = true; // メニュー再描画
            }
            break;

        case DISP_INVERSE: // 画面反転のON/OFF処理
            if(GUI_DisplayInverse())
            {
                display_state  = DISP_MENU;
                display_sel    = 0xff;
                disp_menu_init = true;
            }
            break;

        case DISP_QR: // QRコード表示処理
            if(GUI_ShowQRcode())
            {
                display_state  = DISP_MENU;
                display_sel    = 0xff;
                disp_menu_init = true;
            }
            break;

        default:
            break;
    }

    return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowSensors
// 処理概要     センサ値をメニューで選択して個別表示する
// 引数         なし
// 戻り値       true:ページ終了
///////////////////////////////////////////////////////////////////////////
bool GUI_ShowSensors(void)
{
    // センサページのメニュー初期化フラグ
    static bool sensor_menu_init = true;
    // センサページで表示するメニュー項目
    const uint8_t *sensor_items[] = {
        "Battery ",
        "IMU     ",
        "Encoder ",
        "Line    ",
        "Motor   "
    };

    // メニュー表示状態で初回のみ表示領域をクリア
    if(sensor_state == SENSOR_MENU && sensor_menu_init)
    {
        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                             SSD1351_HEIGHT - 1, SSD1351_BLACK);
        sensor_menu_init = false;
    }

    // センサー項目未選択時はメニュー選択処理を実行
    if(sensor_state == SENSOR_MENU && sensor_sel == 0xff)
    {
        sensor_sel = GUI_MenuSelect(sensor_items, 5);
    }

    // 状態に応じて各センサページを制御
    switch(sensor_state)
    {
        case SENSOR_MENU: // センサー種別を選択するメニュー表示
            if(sensor_sel == 0xff)
            {
                // 選択値未決定時はメニュー操作を継続
                sensor_sel = GUI_MenuSelect(sensor_items, 5);
            }
            else
            {
                // 選択されたセンサーのページへ遷移
                switch(sensor_sel)
                {
                    case 0: sensor_state = SENSOR_BAT;   break;
                    case 1: sensor_state = SENSOR_IMU;   break;
                    case 2: sensor_state = SENSOR_ENC;   break;
                    case 3: sensor_state = SENSOR_LINE;  break;
                    case 4: sensor_state = SENSOR_MOTOR; break;
                    default: break;
                }
                if(sensor_state != SENSOR_MENU)
                {
                    // 押下状態を残さず各ページの描画領域をクリア
                    swValTact = SW_NONE;
                    sensor_sel = 0xff;
                    SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                         SSD1351_HEIGHT - 1, SSD1351_BLACK);
                }
            }
            break;

        case SENSOR_BAT: // バッテリー電圧の表示
            GetBatteryVoltage();
            SSD1351setCursor(2, MENU_START_Y);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"BAT:%4.1fV",batteryVoltage);
            if(swValTact == SW_PUSH)
            {
                // PUSHでメニューへ戻る
                GUI_wait(150);
                sensor_state = SENSOR_MENU;
                sensor_sel   = 0xff;
                sensor_menu_init = true;
            }
            break;

        case SENSOR_IMU: // IMU角度・加速度の表示
            SSD1351setCursor(2, MENU_START_Y);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"ANG");
            SSD1351setCursor(2, MENU_START_Y + 12);
            SSD1351printf(Font_7x10, SSD1351_WHITE,(uint8_t*)"X:%5.1f",BMI088val.angle.x);
            SSD1351setCursor(2, MENU_START_Y + 24);
            SSD1351printf(Font_7x10, SSD1351_WHITE,(uint8_t*)"Y:%5.1f",BMI088val.angle.y);
            SSD1351setCursor(2, MENU_START_Y + 36);
            SSD1351printf(Font_7x10, SSD1351_WHITE,(uint8_t*)"Z:%5.1f",BMI088val.angle.z);
            SSD1351setCursor(2, MENU_START_Y + 48);
            SSD1351printf(Font_7x10, SSD1351_WHITE,(uint8_t*)"TEMP:%5.1f",BMI088val.temp);
            SSD1351setCursor(2, MENU_START_Y + 60);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"ACC");
            SSD1351setCursor(2, MENU_START_Y + 72);
            SSD1351printf(Font_7x10, SSD1351_WHITE,(uint8_t*)"X:%5.1f",BMI088val.accele.x);
            SSD1351setCursor(2, MENU_START_Y + 84);
            SSD1351printf(Font_7x10, SSD1351_WHITE,(uint8_t*)"Y:%5.1f",BMI088val.accele.y);
            SSD1351setCursor(2, MENU_START_Y + 96);
            SSD1351printf(Font_7x10, SSD1351_WHITE,(uint8_t*)"Z:%5.1f",BMI088val.accele.z);
            if(swValTact == SW_PUSH)
            {
                // PUSHでメニューへ戻る
                GUI_wait(150);
                sensor_state = SENSOR_MENU;
                sensor_sel   = 0xff;
                sensor_menu_init = true;
            }
            break;

        case SENSOR_ENC: // エンコーダカウントの表示
            SSD1351setCursor(2, MENU_START_Y);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"ENC:%7d", encTotal);
            if(swValTact == SW_PUSH)
            {
                // PUSHでメニューへ戻る
                GUI_wait(150);
                sensor_state = SENSOR_MENU;
                sensor_sel   = 0xff;
                sensor_menu_init = true;
            }
            break;

        case SENSOR_LINE: // ラインセンサ値の表示
            PowerLineSensors(1);
            SSD1351setCursor(2, MENU_START_Y);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"%4d %4d %4d",lineSenVal[0], lineSenVal[1], lineSenVal[2]);
            SSD1351setCursor(2, MENU_START_Y + 12);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"%4d %4d %4d",lineSenVal[3], lineSenVal[4], lineSenVal[5]);
            SSD1351setCursor(2, MENU_START_Y + 24);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"%4d", lineSenVal[6]);
            if(swValTact == SW_PUSH)
            {
                // PUSHでメニューへ戻る
                GUI_wait(150);
                sensor_state = SENSOR_MENU;
                sensor_sel   = 0xff;
                sensor_menu_init = true;
            }
            break;

        case SENSOR_MOTOR: // モーター・サーボの出力テスト
		{
            static uint8_t  motor_sel  = 0;		// 選択中のモーター/サーボ
            static int16_t  motor_duty = 0;		// 出力デューティ（-1000〜1000）
            static bool     motor_run  = false;	// 実行中かどうか
            // モーター/サーボ名一覧
            const uint8_t *motor_items[] = {
                "MotorFrontLeft ",
                "MotorFrontRight",
                "MotorRearLeft  ",
                "MotorRearRight ",
                "ServoFront     ",
                "ServoRear      "
            };

            // スイッチ操作でデューティや対象を変更
            switch(swValTact)
            {
                case SW_UP: // デューティ増加
                    if(motor_duty < 1000) motor_duty += 100;
                    GUI_wait(250);
                    break;
                case SW_DOWN: // デューティ減少
                    if(motor_duty > -1000) motor_duty -= 100;
                    GUI_wait(250);
                    break;
                case SW_RIGHT: // モーター選択 次
                    motor_sel = (motor_sel + 1) % 6;
                    GUI_wait(250);
                    break;
                case SW_LEFT: // モーター選択 前
                    motor_sel = (motor_sel + 5) % 6;
                    GUI_wait(250);
                    break;
                case SW_PUSH:
                    if(motor_duty == 0)
                    {
                        // デューティ0時はメニューへ戻る
                        GUI_wait(150);
                        sensor_state = SENSOR_MENU;
                        sensor_sel   = 0xff;
                        sensor_menu_init = true;
                        motor_run    = false;
                        MotorPwmOut(0,0,0,0);
                        ServoPwmOut1(0);
                        ServoPwmOut2(0);
                    }
                    else
                    {
                        // デューティ設定時は回転/停止をトグル
                        motor_run = !motor_run;
                        if(!motor_run)
                        {
                            MotorPwmOut(0,0,0,0);
                            ServoPwmOut1(0);
                            ServoPwmOut2(0);
                        }
                        GUI_wait(250);
                    }
                    break;
                default:
                    break;
            }

            // メニューへ戻った場合は表示更新せず抜ける
            if(sensor_state != SENSOR_MOTOR)
            {
                break;
            }

            // 現在の選択情報と電流値を表示
            SSD1351setCursor(2, MENU_START_Y);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)motor_items[motor_sel]);
            SSD1351setCursor(2, MENU_START_Y + 12);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"PWM:%5d%%", motor_duty / 10);
            uint16_t current = 0;
            switch(motor_sel)
            {
                case 0: current = motorCurrentFL; break;
                case 1: current = motorCurrentFR; break;
                case 2: current = motorCurrentRL; break;
                case 3: current = motorCurrentRR; break;
                case 4: current = servoCurrentF;  break;
                case 5: current = servoCurrentR;  break;
                default: break;
            }
            SSD1351setCursor(2, MENU_START_Y + 24);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"CUR:%4d", current);

            SSD1351setCursor(2, MENU_START_Y + 36);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"RUN:%s", motor_run ? "ON " : "OFF");

            if(motor_run)
            {
                // 選択したモーター/サーボへデューティを出力
                switch(motor_sel)
                {
                    case 0: MotorPwmOut(motor_duty, 0, 0, 0); break;
                    case 1: MotorPwmOut(0, motor_duty, 0, 0); break;
                    case 2: MotorPwmOut(0, 0, motor_duty, 0); break;
                    case 3: MotorPwmOut(0, 0, 0, motor_duty); break;
                    case 4: ServoPwmOut1(motor_duty); break;
                    case 5: ServoPwmOut2(motor_duty); break;
                    default: break;
                }
            }
            else
            {
                // 停止時は全出力をオフ
                MotorPwmOut(0,0,0,0);
                ServoPwmOut1(0);
                ServoPwmOut2(0);
            }
            break;
        }
    }

    return false;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SetupUpdate
// 処理概要     メニュー画面の更新処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SetupUpdate(void)
{
    // ステータスバー表示
    if(swValRotary != currentPage)
    {
        SSD1351fill(SSD1351_BLACK);
        currentPage = swValRotary;
        GUI_ShowStatusBar(currentPage);
        // ページ切替時は表示設定ページの状態を初期化
        display_state = DISP_MENU;
        display_sel   = 0xff;
    }

    // ページ表示
    switch (currentPage) {
        case 0x0:
            // Startページ
            GUI_MenuSelect((const char **)menu1_items,
                           sizeof(menu1_items)/sizeof(menu1_items[0]));
            break;
        case 0x1:
            // Display settingページ
            GUI_ShowDisplaySetting();
            break;
        case 0x2:
            // Sensorページ
            GUI_ShowSensors();
            break;
        default:
            break;
    }
}