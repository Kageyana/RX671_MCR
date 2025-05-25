//====================================//
// インクルード
//====================================//
#include "SDcard.h"
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
/////////////////////////////////////////////////////////////////////
// モジュール名 logCreate
// 処理概要     ログファイルを作成する
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
FRESULT logCreate(void)
{
	FRESULT fresult;
	DIR dir;	 // Directory
	FIL fil_W;
	FILINFO fno; // File Info
	uint8_t *tp, fileName[10];
	uint16_t fileNumber = 0;
	static uint8_t read_buf[64];
	UINT bw, br;     // 書き込み／読み込みバイト数
	char write_data[] = "Hello, SD card world for DMAC!\r\n";

	fresult = f_opendir(&dir, "/"); // directory open
	if(fresult == FR_OK)
	{
		do
		{
			f_readdir(&dir, &fno);
			tp = strtok(fno.fname, "."); // 拡張子削除
			if (atoi(tp) > fileNumber)
			{						   // 番号比較
				fileNumber = atoi(tp); // 文字列を数値に変換
			}
		} while (fno.fname[0] != 0); // ファイルの有無を確認

		f_closedir(&dir); // directory close

		// ファイルナンバー作成
		if (fileNumber == 0)
		{
			// ファイルが無いとき
			fileNumber = 1;
		}
		else
		{
			// ファイルが有るとき
			fileNumber++; // index pulus
		}

		sprintf(fileName, "%d", fileNumber);						   // 数値を文字列に変換
		strcat(fileName, ".txt");									   // 拡張子を追加
		
		// 書き込みテスト：ファイル新規作成
		fresult = f_open(&file, fileName, FA_OPEN_ALWAYS | FA_WRITE);
		if (fresult == FR_OK)
		{
			// f_write(&file, write_data, strlen(write_data), &bw);
			f_printf(&file, "time,gz,temp\n");
			// fresult = f_close(&file); // create file
			return FR_OK;
		}

		// // 読み込みテスト
		// fresult = f_open(&file, fileName, FA_READ);
		// if (fresult == FR_OK)
		// {
		// 	// f_read(&file, read_buf, sizeof(read_buf) - 1, &br);
		// 	f_gets(read_buf, sizeof(read_buf), &file);
		// 	f_close(&file);
		// }
	}
	
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 r_dmaca_disable
// 処理概要     DMACAの転送無効化処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
static void r_dmaca_disable(void)
{
    dmaca_return_t  ret_dmaca = DMACA_SUCCESS;
    dmaca_stat_t                p_stat_dmaca;

    ret_dmaca = R_DMACA_Control(DMACA_CH0, DMACA_CMD_DISABLE, (dmaca_stat_t*)&p_stat_dmaca);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        return; // DMACAの転送無効化に失敗
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 r_dmaca_close
// 処理概要     DMACAのクローズ処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
static void r_dmaca_close(void)
{
    dmaca_return_t  ret_dmaca = DMACA_SUCCESS;

    ret_dmaca = R_DMACA_Close(DMACA_CH0);
    if (DMACA_SUCCESS < ret_dmaca)
    {
        return; // DMACAのクローズに失敗
    }
}
