//====================================//
// インクルード
//====================================//
#include "setup.h"
#include "PIDcontrol.h"
#include "linesensor.h"
#include <stdint.h>

// 他ファイルで定義されている現在のページ番号
uint8_t currentPage = 127; // 初期値は127(未設定)

//====================================//
// グローバル変数の宣言
//====================================//
volatile uint32_t	cntGUI; // GUI用カウンタ
uint8_t 			start = 0; // 0:セットアップ中	1:セットアップ完了
uint8_t				modePushcart = 0; // 手押しモードフラグ

typedef enum {
    SENSOR_MENU,	// トップメニュー
    SENSOR_BAT,		// バッテリー電圧表示
    SENSOR_IMU,		// IMUの角度・温度・加速度表示
    SENSOR_ENC,		// エンコーダ値表示
	SENSOR_POT, 	// ポテンショメータ値表示
    SENSOR_LINE,	// ラインセンサ値表示
    SENSOR_MOTOR	// モーター動作確認
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

typedef enum {
    PID_MENU,  // PID調整メニュー
    PID_EDIT   // PIDパラメータ編集
} PIDState;

static PIDState pid_state = PID_MENU;
static uint8_t  pid_sel   = 0xff;

typedef struct {
    const uint8_t **items; // メニュー項目配列へのポインタ
    uint8_t         top;   // 表示オフセット
} MenuState;

static MenuState menu_states[MAX_MENU_STATE];
static uint8_t menu_state_count = 0;
// Start ページのメニュー項目
static const uint8_t *menu1_items[] = {
    "Start   ",
    "Pushcart"
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
// モジュール名 GUI_EditPIDk
// 処理概要     PID係数(KP/KI/KD)を描画し選択中は強調表示する
// 引数         label   : 表示文字列
//              value   : 表示する数値
//              x       : 表示位置X座標
//              selected: 選択中フラグ
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
static void GUI_EditPIDk(const uint8_t *label, int16_t value, uint8_t x, bool selected)
{
    uint16_t color = selected ? SSD1351_YELLOW : SSD1351_WHITE;
    SSD1351setCursor(x, MENU_START_Y + 12);
    SSD1351printf(Font_7x10, color, (uint8_t*)"%s", label);
	SSD1351setCursor(x, MENU_START_Y + 24);
    SSD1351printf(Font_7x10, color, (uint8_t*)"%3d",value);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_EditPID
// 処理概要     KP/KI/KDを選択し値を変更、押下で制御の開始/停止を切り替える
// 引数         pid: 調整対象のPIDパラメータ
// 戻り値       true:編集終了
///////////////////////////////////////////////////////////////////////////
static bool GUI_EditPID(pidParam *pid)
{
    static uint8_t param_index = 0;
    static bool init = false;
    static bool running = false;

    if(!init)
    {
        // 編集開始時に表示エリアを初期化
        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                             SSD1351_HEIGHT - 1, SSD1351_BLACK);
        init = true;
    }

    // パラメータ名を表示
    SSD1351setCursor(2, MENU_START_Y);
    SSD1351printf(Font_7x10, SSD1351_WHITE, pid->name);

    // KP/KI/KD を描画し、選択中は強調表示
    GUI_EditPIDk((uint8_t*)"KP", pid->kp, 2,  param_index == 0);
    GUI_EditPIDk((uint8_t*)"KI", pid->ki, 42, param_index == 1);
    GUI_EditPIDk((uint8_t*)"KD", pid->kd, 82, param_index == 2);

    // PID出力値を表示（制御中は色付け）
    SSD1351setCursor(2, MENU_START_Y + 36);
    SSD1351printf(Font_7x10, running ? SSD1351_GREEN : SSD1351_WHITE,
                  (uint8_t*)"OUT:%4d", pid->pwm);

	// 選択されたPID制御の対象を表示
	if(pid == &lineTraceCtrl)
	{
		// ライントレース制御対象
		SSD1351setCursor(2, MENU_START_Y + 48);
		SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"target:%5d", getAnalogSensor());
	}
	else if(pid == &veloCtrl)
	{
		// 億度制御対象
		SSD1351setCursor(2, MENU_START_Y + 48);
		SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"target:%5d", encCurrent);
	}
	else if(pid == &angleCtrl)
	{
		// サーボ角度制御対象
		SSD1351setCursor(2, MENU_START_Y + 48);
		SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"target:%5d", getServoAngle());
	}
	

    switch(swValTact)
    {
        case SW_LEFT:
            // 左: 項目移動または終了
            if(param_index == 0)
            {
                // 先頭で左の場合はメニューに戻る
                init = false;
                running = false;
                MotorPwmOut(0,0,0,0);
                ServoPwmOut1(0);
                ServoPwmOut2(0);
                GUI_wait(150);
                return true;
            }
            else
            {
                // 左へ項目を移動
                param_index--;
                GUI_wait(150);
            }
            break;
        case SW_RIGHT:
            // 右: 項目を右へ移動
            if(param_index < 2) param_index++;
            GUI_wait(150);
            break;
        case SW_UP:
            // 上: 選択中の係数を増加
            if(param_index == 0) pid->kp++;
            else if(param_index == 1) pid->ki++;
            else pid->kd++;
            GUI_wait(120);
            break;
        case SW_DOWN:
            // 下: 選択中の係数を減少
            if(param_index == 0 && pid->kp > 0) pid->kp--;
            else if(param_index == 1 && pid->ki > 0) pid->ki--;
            else if(param_index == 2 && pid->kd > 0) pid->kd--;
            GUI_wait(120);
            break;
        case SW_PUSH:
            // 押下: 制御の開始/停止を切り替え
            running = !running;
            if(!running)
            {
                // 停止時は出力をゼロにする
				PowerLineSensors(0);
                MotorPwmOut(0,0,0,0);
                ServoPwmOut1(0);
                ServoPwmOut2(0);

				// PIDパラメータをSDカードに保存
				writePIDparameters(&lineTraceCtrl);
				writePIDparameters(&veloCtrl);
				writePIDparameters(&angleCtrl);

				lineTraceCtrl.Int = 0; // ライントレース制御の積分値リセット
				veloCtrl.Int = 0;      // 速度制御の積分値リセット
				angleCtrl.Int = 0;     // 角度制御の積分リセット
            }
            GUI_wait(200);
            break;
        default:
            break;
    }

    if(running)
    {
        // 選択されたPID制御を実行
        if(pid == &lineTraceCtrl)
        {
            // ライントレース制御
			PowerLineSensors(1);
            ServoPwmOut1(lineTraceCtrl.pwm);
        }
        else if(pid == &veloCtrl)
        {
            // 速度制御
			setTargetSpeed(0.0);
            MotorPwmOut(veloCtrl.pwm, veloCtrl.pwm, veloCtrl.pwm, veloCtrl.pwm);
        }
        else if(pid == &angleCtrl)
        {
            // サーボ角度制御
			setTargetAngle(-1000);
            ServoPwmOut1(angleCtrl.pwm);
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////
// モジュール名 GUI_ShowPidTuning
// 処理概要     PID調整メニューを表示し、選択した項目の編集画面を管理する
// 引数         なし
// 戻り値       true:編集終了
///////////////////////////////////////////////////////////////////////////
bool GUI_ShowPidTuning(void)
{
    static bool pid_menu_init = true;
    const uint8_t *pid_items[] = {
        "Trace",
        "Speed",
        "Servo"
    };

    if(pid_state == PID_MENU && pid_menu_init)
    {
        // メニュー表示時に画面をクリア
        SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                             SSD1351_HEIGHT - 1, SSD1351_BLACK);
        pid_menu_init = false;
    }

    if(pid_state == PID_MENU && pid_sel == 0xff)
    {
        // ロータリースイッチで項目を選択
        pid_sel = GUI_MenuSelect((const char **)pid_items,
                                 sizeof(pid_items)/sizeof(pid_items[0]));
    }

    switch(pid_state)
    {
        case PID_MENU:
            if(pid_sel == 0xff)
            {
                // メニュー選択待ち
                pid_sel = GUI_MenuSelect(pid_items,3);
            }
            else
            {
                // 項目が選択されたので編集モードへ遷移
                pid_state = PID_EDIT;
                swValTact = SW_NONE;
                // 編集画面を初期化
                SSD1351fillRectangle(0, MENU_START_Y, SSD1351_WIDTH - 1,
                                     SSD1351_HEIGHT - 1, SSD1351_BLACK);
            }
            break;
        case PID_EDIT:
        {
            // 選択されたPIDの編集を実行
            pidParam *pids[] = {&lineTraceCtrl, &veloCtrl, &angleCtrl};
            if(GUI_EditPID(pids[pid_sel]))
            {
                // 編集終了でメニューに戻る
                pid_state = PID_MENU;
                pid_sel = 0xff;
                pid_menu_init = true;
            }
            break;
        }
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
        display_sel = GUI_MenuSelect((const char **)disp_items,
									 sizeof(disp_items)/sizeof(disp_items[0]));
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
		"Potentiometer",
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
        sensor_sel = GUI_MenuSelect((const char **)sensor_items,
									sizeof(sensor_items)/sizeof(sensor_items[0]));
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
					case 3: sensor_state = SENSOR_POT;   break;
                    case 4: sensor_state = SENSOR_LINE;  break;
                    case 5: sensor_state = SENSOR_MOTOR; break;
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

		case SENSOR_POT: // ポテンショメータ値の表示
            SSD1351setCursor(2, MENU_START_Y);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"Front:%4d", potFrontVal);
			SSD1351setCursor(2, MENU_START_Y + 12);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"Rear :%4d", potRearVal);
			SSD1351setCursor(2, MENU_START_Y + 24);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"ServoAngle:%5d", getServoAngle());
			SSD1351setCursor(2, MENU_START_Y + 36);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"Angle0:%4d", Angle0);
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
			SSD1351setCursor(32, MENU_START_Y);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"%4d %4d",lineSenVal[1], lineSenVal[4]);
            SSD1351setCursor(14, MENU_START_Y + 12);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"%4d %4d %4d",lineSenVal[0], lineSenVal[2], lineSenVal[5]);
            SSD1351setCursor(2, MENU_START_Y + 24);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"Gate : %4d", lineSenVal[6]);
			SSD1351setCursor(2, MENU_START_Y + 36);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"Spare: %4d", lineSenVal[3]);
			SSD1351setCursor(2, MENU_START_Y + 48);
            SSD1351printf(Font_7x10, SSD1351_WHITE, (uint8_t*)"digital: %4d", sensor_inp());

            if(swValTact == SW_PUSH)
            {
                // PUSHでメニューへ戻る
				PowerLineSensors(0);
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
                ServoPwmOut1(lineTraceCtrl.pwm);
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
		GetBatteryVoltage();			// バッテリー電圧を取得
        GUI_ShowStatusBar(currentPage);	// ステータスバーを更新
        // ページ切替時は表示設定ページの状態を初期化
        display_state = DISP_MENU;
        display_sel   = 0xff;
    }

    // ページ表示
    switch (currentPage) {
        case 0x0:
            // Startページ
            {
                uint8_t sel = GUI_MenuSelect((const char **)menu1_items,
                                             sizeof(menu1_items)/sizeof(menu1_items[0]));
                if (sel == 0) {
                    start = 1;
                    modePushcart = 0;
                } else if (sel == 1) {
                    start = 1;
                    modePushcart = 1;
                }
            }
            break;
        case 0x1:
            // Display settingページ
            GUI_ShowDisplaySetting();
            break;
        case 0x2:
            // Sensorページ
            GUI_ShowSensors();
            break;
        case 0x3:
            // PID調整ページ
            GUI_ShowPidTuning();
            break;
        default:
            break;
    }
}