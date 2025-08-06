//====================================//
// インクルード
//====================================//
#include "SDcard.h"
#include "ff.h"
#include <stdint.h>
//====================================//
// グローバル変数の宣言
//====================================//
sdc_sd_cfg_t sdc_sd_config;
uint32_t sdWorkarea[200 / sizeof(uint32_t)];
uint8_t initSDcard = 0;		// SDカード初期化フラグ
uint8_t loggingSDcard = 0;	// ログ取得開始フラグ
uint8_t insertSDcard = 0;	// SDカード挿入フラグ

// ログヘッダー
uint8_t columnTitle[256] = "", formatLog[128] = "";

FATFS *fs;        // ファイルシステムオブジェクト
FIL file;        // ファイルオブジェクト
/////////////////////////////////////////////////////////////////////
// モジュール名 SDcardOpen
// 処理概要  	SDHIドライバ初期化
// 引数     	なし
// 戻り値    	sdc_sd_status_t
/////////////////////////////////////////////////////////////////////
sdc_sd_status_t SDcardOpen(void)
{
	// SDHIピン初期化
	R_SDHI_PinSetInit();

	// SDHIインターフェースの初期化
	if(R_SDC_SD_Open(SD_CARD_NO, SDHI_CH0, sdWorkarea) == SDC_SD_SUCCESS)
	{
		// SDカードの検出を待機
		cnt0 = 0;
		while (R_SDC_SD_GetCardDetection(SD_CARD_NO) != SDC_SD_SUCCESS)
		{
			if (cnt0 > 2000)
			{
				// SDカードが検出されない場合の処理
				insertSDcard = 0; // SDカード挿入フラグをリセット
				return SDC_SD_ERR_NO_CARD;
			}
		}
		insertSDcard = 1; // SDカード挿入フラグをセット

		// SDカードの検出コールバック設定
		R_SDC_SD_CdInt(SD_CARD_NO, SDC_SD_CD_INT_ENABLE, SDcardDetectCallback);
		// SDカードのコールバック設定
		R_SDC_SD_IntCallback(SD_CARD_NO, SDcardCallback);

		return SDC_SD_SUCCESS; // 初期化成功
	}
	else
	{
		// SDカードの初期化に失敗
		return SDC_SD_ERR;
	}
	
	return SDC_SD_ERR; // 初期化失敗
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SDcardinit
// 処理概要  	SDMEMドライバ初期化
// 引数     	なし
// 戻り値    	なし
/////////////////////////////////////////////////////////////////////
sdc_sd_status_t SDcardinit(void)
{
	volatile sdc_sd_status_t ret;
	FRESULT fresult;

	// SDMEM設定
	sdc_sd_config.mode = SDC_SD_MODE_HWINT | 
						SDC_SD_MODE_DMA | 
						SDC_SD_MODE_MEM | 
						SDC_SD_MODE_4BIT;
	// SDカード電源設定
	sdc_sd_config.voltage = SDC_SD_VOLT_3_3;
	// SDカードの電源ON
	SDCARD_POWER = SDCARD_POWER_ON; 
	// SDカード電源が安定するまで待機
	R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
	// SDHIピンを転送モードに設定
	R_SDHI_PinSetTransfer();	

	// SDカードの初期化
	ret = R_SDC_SD_Initialize(SD_CARD_NO, &sdc_sd_config, SDC_SD_MODE_MEM);
	if(ret == SDC_SD_SUCCESS)
	{
		initSDcard = 1;
		fs = malloc(sizeof (FATFS)); // ファイルシステムオブジェクトのメモリ確保
		fresult = f_mount(fs, "",0); // ファイルシステムのマウント
		createDir("images"); // ホームディレクトリにimagesディレクトリを作成
		return SDC_SD_SUCCESS; // 初期化成功
	}
	else
	{
		// SDカードの初期化に失敗
		return SDC_SD_ERR;
	}

	
	return SDC_SD_ERR; // 初期化失敗
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SDcardDetectCallback
// 処理概要     SDカードの検出コールバック関数
// 引数         cd:検出状態 0 : SDC_SD_CD_INSERT 1 : SDC_SD_CD_REMOVE
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
sdc_sd_status_t SDcardDetectCallback(int32_t cd)
{
	if(cd & SDC_SD_CD_INSERT)
	{
		/* sdcard in */
		insertSDcard = 1; // SDカード挿入フラグをセット
	}
	else
	{
		/* sdcard out */
		insertSDcard = 0; // SDカード挿入フラグをリセット
	}
	return SDC_SD_SUCCESS;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SDcardCallback
// 処理概要     SDカードのコールバック関数
// 引数         channel: 常に0
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
sdc_sd_status_t SDcardCallback(int32_t channel)
{
	/* User program */
	return SDC_SD_SUCCESS;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SDcardEnd
// 処理概要     SDカードの終了処理 抜去可能にする
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void SDcardEnd(void)
{
	// r_dmaca_disable(); // DMACAの転送無効化
	R_SDC_SD_End(SD_CARD_NO,SDC_SD_MODE_MEM); // SDカードの終了処理

	R_SDHI_PinSetDetection(); // SDHIピンを検出モードのみに設定
	SDCARD_POWER = SDCARD_POWER_OFF; // SDカードの電源OFF
	R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS); // SDカード電源が安定するまで待機

	initSDcard = 0; // SDカード初期化フラグをリセット
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SDcardClose
// 処理概要     SDカードのクローズ処理(ドライバ未初期化状態にする)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void SDcardClose(void)
{
	if(R_SDC_SD_GetCardDetection(SD_CARD_NO) != SDC_SD_SUCCESS)
	{
		R_SDC_SD_Close(SD_CARD_NO); // SDカードのクローズ処理
		r_dmaca_close(); // DMACAのクローズ処理
		R_SDHI_PinSetEnd(); // SDHIピンの終了処理
	}
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
	FILINFO fno; // File Info
	uint8_t *tp, fileName[10];
	uint16_t fileNumber = 0;

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

		sprintf(fileName, "%d", fileNumber);	// 数値を文字列に変換
		strcat(fileName, ".csv");				// 拡張子を追加

		// ログヘッダー、フォーマット文字列の生成
		strcpy(columnTitle, "");
		strcpy(formatLog, "");
		setLogStr("time", "%d");
		setLogStr("ax", "%f");
		setLogStr("ay", "%f");
		setLogStr("az", "%f");
		setLogStr("gx", "%f");
		setLogStr("gy", "%f");
		setLogStr("gz", "%f");
		setLogStr("anglex", "%f");
		setLogStr("angley", "%f");
		setLogStr("anglez", "%f");
		setLogStr("temp", "%f");
		strcat((char*)columnTitle, "\n");
		strcat((char*)formatLog, "\n");

		// 書き込みテスト：ファイル新規作成
		fresult = f_open(&file, fileName, FA_OPEN_ALWAYS | FA_WRITE);
		if (fresult == FR_OK)
		{
			// ヘッダー書き込み
			fresult = f_printf(&file, columnTitle);
			return FR_OK;
		}
		return FR_DISK_ERR; // ファイルオープン失敗
	}
	return FR_DISK_ERR; // ディレクトリオープン失敗
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

    ret_dmaca = R_DMACA_Control(DMACA_CH1, DMACA_CMD_DISABLE, (dmaca_stat_t*)&p_stat_dmaca);
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

    ret_dmaca = R_DMACA_Close(DMACA_CH1);
    if (DMACA_SUCCESS < ret_dmaca)
    {
        return; // DMACAのクローズに失敗
    }
}
/////////////////////////////////////////////////////////////////////
// モジュール名 setLogStr
// 処理概要     ログCSVファイルのヘッダーとprintfのフォーマット文字列を生成
// 引数         column: ヘッダー文字列 format: フォーマット文字列
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void setLogStr(uint8_t *column, uint8_t *format)
{
	strcat(columnTitle, column);
	strcat(formatLog, format);
	strcat(columnTitle, ",");
	strcat(formatLog, ",");
}
/////////////////////////////////////////////////////////////////////
// モジュール名 createDir
// 処理概要     ホームディレクトリに指定されたディレクトリが存在しなければ作成する
// 引数         dirName: 作成するディレクトリ名
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void createDir(uint8_t *dirName)
{
	FRESULT fresult;
	DIR dir;	 // Directory
	FILINFO fno; // File Info
	uint8_t exist = 0;

	fresult = f_opendir(&dir, "/"); // directory open
	if (fresult == FR_OK)
	{
		do
		{
			// ディレクトリの情報を読み込む
			f_readdir(&dir, &fno);
			// ディレクトリ名とdirNameを比較
			if (strcmp(fno.fname, dirName) == 0) 
			{
				exist = 1; // dirNameディレクトリが存在する
				break;
			}
		} while (fno.fname[0] != 0); // ファイルの有無を確認

		if (!exist)
		{
			// dirNameディレクトリが存在しない場合は作成する
			f_mkdir(dirName);
		}

	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 swapEndian32
// 処理概要     32ビット整数のエンディアンを逆転する
// 引数         val: 逆転する32ビット整数
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
uint32_t swapEndian32(uint32_t val)
{
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 swapEndian16
// 処理概要     16ビット整数のエンディアンを逆転する
// 引数         val: 逆転する16ビット整数
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
uint16_t swapEndian16(uint16_t val)
{
    return ((val >> 8) & 0x00FF) |
           ((val << 8) & 0xFF00);
}
