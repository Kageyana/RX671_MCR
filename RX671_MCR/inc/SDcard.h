#ifndef SDCARD_H_
#define SDCARD_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
#include "r_sdc_sd_rx_if.h"
#include "r_sdhi_rx_if.h"
#include "r_pinset.h"
#include "r_dmaca_rx_if.h"
#include "ff.h"
#include "diskio.h"

#include "timer.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
//====================================//
// シンボル定義
//====================================//
#define SD_CARD_NO SDC_SD_CARD_NO0
#define USE_DMA

/************* 自動生成関数 **************/
// CD pin
#define SDCARD_POWER PORT7.PODR.BIT.B4 // SDカードの電源制御ピン
#define SDCARD_POWER_ON 0
#define SDCARD_POWER_OFF 1
/****************************************/

//====================================//
// グローバル変数の宣言
//====================================//
extern uint8_t initSDcard;
extern uint8_t loggingSDcard;

extern FATFS *fs;        // ファイルシステムオブジェクト
extern FIL file;        // ファイルオブジェクト

extern uint8_t columnTitle[512], formatLog[256];
//====================================//
// プロトタイプ宣言
//====================================//
sdc_sd_status_t SDcardinit(void);
sdc_sd_status_t SDcardend(void);
FRESULT logCreate(void);
static void r_dmaca_disable(void);
static void r_dmaca_close(void);
void setLogStr(uint8_t *column, uint8_t *format);
#endif // SDCARD_H_
