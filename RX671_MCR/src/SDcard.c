//====================================//
// インクルード
//====================================//
#include "SDcard.h"
#include <stdint.h>
//====================================//
// グローバル変数の宣言
//====================================//
uint32_t sdWorkarea[200 / sizeof(uint32_t)];
uint8_t initSDcard = 0;
uint8_t loggingSDcard = 0;

FATFS *fs;        // ファイルシステムオブジェクト
FIL file;        // ファイルオブジェクト
/////////////////////////////////////////////////////////////////////
// モジュール名 SDcardinit
// 処理概要  	SDカードの初期化
// 引数     	なし
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
sdc_sd_status_t SDcardinit(void)
{
	sdc_sd_cfg_t sdc_sd_config;

	// ピン設定
	R_SDHI_PinSetInit();

	if(R_SDC_SD_Open(SD_CARD_NO, SDHI_CH0, sdWorkarea) == SDC_SD_SUCCESS)
	{
		// SDカードの初期化に成功

		cnt0 = 0;
		while (R_SDC_SD_GetCardDetection(SD_CARD_NO) != SDC_SD_SUCCESS)
		{
			if (cnt0 > 2000)
			{
				// SDカードが検出されない場合の処理
				return R_SDC_SD_GetErrCode(SD_CARD_NO);; // SDカードが検出されない
			}
		}

		// SDHI設定
		sdc_sd_config.mode = SDC_SD_MODE_POLL | 
							SDC_SD_MODE_DMA | 
							SDC_SD_MODE_MEM | 
							SDC_SD_MODE_4BIT;
		// SDカード電源設定
		sdc_sd_config.voltage = SDC_SD_VOLT_3_3;

		SDCARD_POWER = SDCARD_POWER_ON; // SDカードの電源ON
		R_BSP_SoftwareDelay(1000,BSP_DELAY_MILLISECS); // SDカード電源が安定するまで待機

		R_SDHI_PinSetTransfer();	// SDHIピンを転送モードに設定

		if(R_SDC_SD_Initialize(SD_CARD_NO, &sdc_sd_config, SDC_SD_MODE_MEM) == SDC_SD_SUCCESS)
		{
			initSDcard = 1;
			return SDC_SD_SUCCESS; // 初期化成功
		}
		else
		{
			// SDカードの初期化に失敗
			return R_SDC_SD_GetErrCode(SD_CARD_NO);
		}
	}
	else
	{
		// SDカードの初期化に失敗
		return R_SDC_SD_GetErrCode(SD_CARD_NO);
	}
	
	return SDC_SD_ERR; // 初期化失敗
}