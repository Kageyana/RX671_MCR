#ifndef SDCARD_H_
#define SDCARD_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"
#include "r_sdc_sd_rx_if.h"
#include "r_sdhi_rx_if.h"
#include "r_pinset.h"

#include "timer.h"
//====================================//
// シンボル定義
//====================================//
#define SD_CARD_NO 0

/************* 自動生成関数 **************/
// CD pin
#define SDCARD_POWER PORT7.PODR.BIT.B4 // SDカードの電源制御ピン
#define SDCARD_POWER_ON 0
#define SDCARD_POWER_OFF 1
/****************************************/

//====================================//
// グローバル変数の宣言
//====================================//

//====================================//
// プロトタイプ宣言
//====================================//
sdc_sd_status_t SDcardinit(void);


#endif // SDCARD_H_
