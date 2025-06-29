/***********************************************************************************************************************
* Copyright (c) 2014 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_tfat_drv_if_sdmem.c
* Description  : TFAT driver Interface for SD memory card.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 01.12.2014 1.00    First Release
*              : 22.06.2015 1.02    Added support MCU RX231.
*              : 01.04.2016 1.03    Updated the xml file.
*              : 29.06.2018 1.04    Modified SD card API.
*              : 08.08.2019 2.00    Added support for FreeRTOS and Renesas uITRON (RI600V4).
*                                   Added support for GNUC and ICCRX.
*              : 10.09.2020 2.20    Added support for the format function.
*              : 30.09.2024 2.60    Changed the comment of API functions to the Doxygen style.
*              : 15.03.2025 2.61    Updated disclaimer.
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <string.h>
#include "r_tfat_drv_if_dev.h"
#include "r_tfat_driver_rx_config.h"

#if (TFAT_SDMEM_DRIVE_NUM > 0)
#include "ff.h"              /* TFAT define */
#include "diskio.h"          /* TFAT define */

#include "r_sdc_sd_rx_if.h"
#include "src/r_sdc_sd_rx_private.h"

#include "r_dmaca_rx_if.h"
#include "SDcard.h"
/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SDMEM_PRV_CSD_SHIT_7                  (7)
#define SDMEM_PRV_CSD_ERASE_BLOCK_MASK        (0x0000007f)

#define USE_DMA
#define SDC_SD_WRITE_OP	(0)
#define SDC_SD_READ_OP	(1)
/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void SDcardDMAinterrupt(void);
static void r_dmaca_open(void);
static void r_dmaca_enable(sdc_sd_access_t * p_sdc_sd_access, uint8_t op_mode, uint32_t reg_buff);
static void r_dmaca_set_recv(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,dmaca_activation_source_t act_source);
static void r_dmaca_set_send(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,dmaca_activation_source_t act_source);

/******************************************************************************
Private global variables and functions
*******************************************************************************/

/**********************************************************************************************************************
* Function Name: sdmem_disk_initialize
*******************************************************************************************************************//**
* @brief This function initializes the disk drive.
*
* @param[in] drive  Specifies the initialize drive number.
*
* @retval TFAT_RES_OK  Normal termination.
*
* @retval Others       DSTATUS status of the disk after function execution.
*
* @details This function does not execute the SD memory card driver initialize. Please implement SD memory card
* initialize code in user code.
* @note None
*/
DSTATUS sdmem_disk_initialize(uint8_t drive)
{
#ifdef USE_DMA
	// Initialize DMACA
	r_dmaca_open();
#endif

    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: sdmem_disk_read
*******************************************************************************************************************//**
* @brief This function reads the data from disk.
*
* @param[in]  drive  Specifies the physical drive number.
*
* @param[out] buffer Pointer to the read buffer to store the read data. A buffer of the size equal to the
* number of bytes to be read is required.
*
* @param[in]  sector_number  Specifies the start sector number in logical block address (LBA).
*
* @param[in]  sector_count   Specifies number of sectors to read. The value can be 1 to 255.
*
* @retval DRESULT  Result of the function execution.
*
* @details Read data from SD memory by block.
* @note None
*/
DRESULT sdmem_disk_read (
    uint8_t drive,               /* Physical drive number            */
    uint8_t* buffer,             /* Pointer to the read data buffer  */
    uint32_t sector_number,      /* Start sector number              */
    uint32_t sector_count        /* Number of sectors to read        */
)
{
    sdc_sd_access_t   sdmem_Access;
    sdc_sd_status_t   res = SDC_SD_SUCCESS;

    /* parameter check */
    if ((NULL == buffer) || (0 == sector_count))
    {
        return RES_ERROR;
    }

    sdmem_Access.p_buff = (uint8_t *)buffer;
    sdmem_Access.lbn    = sector_number;
    sdmem_Access.cnt    = sector_count;

#ifdef USE_DMA
	uint32_t reg_buff = 0;
	if (R_SDC_SD_GetBuffRegAddress(SD_CARD_NO, &reg_buff) != SDC_SD_SUCCESS)
    {
        return RES_ERROR;
    }
	r_dmaca_enable(&sdmem_Access, SDC_SD_READ_OP, reg_buff);
	if(R_SDC_SDMEM_Read(SD_CARD_NO, &sdmem_Access) != SDC_SD_SUCCESS)
	{
		R_SDC_SD_SetDmacDtcTransFlg(SD_CARD_NO, SDC_SD_SET_TRANS_STOP);
		return RES_ERROR;
	}
#else
    /* TRANS mode : Software */
    res = R_SDC_SDMEM_ReadSoftwareTrans(drive, &sdmem_Access);
    if ( SDC_SD_SUCCESS != res )
    {
        return RES_ERROR;
    }
#endif

    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: sdmem_disk_write
*******************************************************************************************************************//**
* @brief This function writes the data to the disk.
*
* @param[in] drive  Specifies the physical drive number.
*
* @param[in] buffer Pointer to the data to be written.
*
* @param[in] sector_number  Specifies the start sector number in logical block address (LBA).
*
* @param[in] sector_count   Specifies number of sectors to read. The value can be 1 to 255.
*
* @retval DRESULT  Result of the function execution.
*
* @details Write the data to the SD memory by block.
* @note None
*/
DRESULT sdmem_disk_write (
    uint8_t drive,                /* Physical drive number           */
    const uint8_t* buffer,        /* Pointer to the write data       */
    uint32_t sector_number,       /* Sector number to write          */
    uint32_t sector_count         /* Number of sectors to write      */
)
{

    sdc_sd_access_t   sdmem_Access;
    sdc_sd_status_t   res;

    /* parameter check */
    if ((NULL == buffer) || (0 == sector_count))
    {
        return RES_ERROR;
    }

    sdmem_Access.p_buff     = (uint8_t *)buffer;
    sdmem_Access.lbn        = sector_number;
    sdmem_Access.cnt        = sector_count;
    sdmem_Access.write_mode = SDC_SD_WRITE_OVERWRITE;

#ifdef USE_DMA
	uint32_t reg_buff = 0;
	if (R_SDC_SD_GetBuffRegAddress(SD_CARD_NO, &reg_buff) != SDC_SD_SUCCESS)
    {
        return RES_ERROR;
    }

	r_dmaca_enable(&sdmem_Access, SDC_SD_WRITE_OP, reg_buff);
	if(R_SDC_SDMEM_Write(SD_CARD_NO, &sdmem_Access) != SDC_SD_SUCCESS)
	{
		R_SDC_SD_SetDmacDtcTransFlg(SD_CARD_NO, SDC_SD_SET_TRANS_STOP);
		return RES_ERROR;
	}
#else
    /* TRANS mode : Software */
    res = R_SDC_SDMEM_WriteSoftwareTrans(drive, &sdmem_Access);
    if ( SDC_SD_SUCCESS != res )
    {
        return RES_ERROR;
    }
#endif

    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: sdmem_disk_ioctl
*******************************************************************************************************************//**
* @brief This function controls the drive.
*
* @param[in] drive   Specifies the physical drive number.
*
* @param[in] command Specifies the command code. The command code will always be 0.
*
* @param[in] buffer  Pointer should always be a NULL pointer.
*
* @retval DRESULT  Result of the function execution.
*
* @details The sdmem_disk_ioctl function is used only by the f_sync function amongst all the TFAT FIT functions.
* Users who do not plan to use f_sync function in their applications can skip the implementation for this
* particular driver interface function.\n
* For users who wish to use f_sync function in their applications, the command CTRL_SYNC has to be implemented.\n
* For users who wish to use f_sync function in their applications, this particular driver interface function
* will have to be implemented. This driver function should consist of the code to finish off any pending write
* process. If the disk i/o module has a write back cache, the dirty sector must be flushed immediately.The f_sync
* function will perform a save operation to the unsaved data related to the file object passed as argument.
* @note None
*/
DRESULT sdmem_disk_ioctl (
    uint8_t drive,                /* Drive number             */
    uint8_t command,              /* Control command code     */
    void* buffer                  /* Data transfer buffer     */
)
{
    uint8_t                cmd = command;
#if FF_USE_MKFS == 1
    sdc_sd_card_status_t   sdmem_cardstatus;
    sdc_sd_card_reg_t      sdmem_cardinfo;
    sdc_sd_status_t        ret;
#endif

    if ((NULL == buffer) && (CTRL_SYNC != cmd))
    {
        return RES_PARERR;
    }

    switch (cmd)
    {
        case CTRL_SYNC:

        break;

        case GET_SECTOR_COUNT:
#if FF_USE_MKFS
            /* Get SD card sector count */
            ret = R_SDC_SD_GetCardStatus(drive, &sdmem_cardstatus);
            if (SDC_SD_SUCCESS != ret)
            {
                return RES_ERROR;
            }

            ((uint32_t *)buffer)[0] = sdmem_cardstatus.card_sector_size;
#else
            return RES_PARERR;
#endif
        break;

        case GET_SECTOR_SIZE:
#if FF_MAX_SS == FF_MIN_SS
            return RES_PARERR;
#else
            ((uint32_t *)buffer)[0] = (uint32_t)SDC_SD_TRANS_BLOCK_SIZE;
#endif
        break;

        case GET_BLOCK_SIZE:
#if FF_USE_MKFS
            /* Get SD card block size */
            ret = R_SDC_SD_GetCardInfo(drive, &sdmem_cardinfo);
            if (SDC_SD_SUCCESS != ret)
            {
                return RES_ERROR;
            }

            /* Erase sector size are [45:39] bits of CSD register */
            ((uint32_t *)buffer)[0]  = ((sdmem_cardinfo.csd[1]>>SDMEM_PRV_CSD_SHIT_7) & SDMEM_PRV_CSD_ERASE_BLOCK_MASK) + 1;
#else
            return RES_PARERR;
#endif
        break;

        case CTRL_TRIM:

        break;

        default:
            return RES_PARERR;
        break;
    }
    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: sdmem_disk_status
*******************************************************************************************************************//**
* @brief This function gets the disk drive status.
*
* @param[in] drive  Specifies the physical drive number.
*
* @retval TFAT_RES_OK  Normal termination.
*
* @retval Others       DSTATUS status of the disk after function execution.
*
* @details This function should consist of the code that checks the disk and returns the current disk status.
* The disk status can have any of the three values, see Section 2.10 in the application note for details.
* The disk status can be returned by updating the return value with the macros related to disk status.
* @note None
*/
DSTATUS sdmem_disk_status (
    uint8_t drive                  /* Physical drive number    */
)
{

    /*  Please put the code for disk_status driver interface
         function over here.  */
    /*  Please refer the application note for details.  */
    return RES_OK;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SDcardDMAinterrupt
// 処理概要     DMA割り込みハンドラ
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SDcardDMAinterrupt(void)
{
	volatile dmaca_return_t    ret_dmaca = DMACA_SUCCESS;
    dmaca_stat_t               p_stat_dmaca;

    // DMACAのステータスを取得
    ret_dmaca = R_DMACA_Control(DMACA_CH1, DMACA_CMD_STATUS_GET, &p_stat_dmaca);
    if (DMACA_SUCCESS != ret_dmaca)
    {
		// DMACAのステータス取得に失敗
        return;
    }

    if (true == (p_stat_dmaca.dtif_stat))
    {
		// DMAC1の転送完了フラグが立っている場合
        ret_dmaca = R_DMACA_Control(DMACA_CH1, DMACA_CMD_DTIF_STATUS_CLR, &p_stat_dmaca);
        R_SDC_SD_SetDmacDtcTransFlg(SD_CARD_NO, SDC_SD_SET_TRANS_STOP);
    }

    if (true == (p_stat_dmaca.esif_stat))
    {
		// DMAC1のエスケープ完了フラグが立っている場合
        ret_dmaca = R_DMACA_Control(DMACA_CH1, DMACA_CMD_ESIF_STATUS_CLR, &p_stat_dmaca);
    }

    return;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 r_dmaca_open
// 処理概要     DMACAの初期化処理
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
static void r_dmaca_open(void)
{
    volatile dmaca_return_t  ret_dmaca = DMACA_SUCCESS;
    
    ret_dmaca = R_DMACA_Open(DMACA_CH1);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        return; // DMACAのオープンに失敗
    }

    ret_dmaca = R_DMACA_Int_Callback(DMACA_CH1, SDcardDMAinterrupt);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        return; // DMACAの割り込みコールバック関数設定に失敗
    }

    ret_dmaca = R_DMACA_Int_Enable(DMACA_CH1, 10);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        return; // DMACAの割り込み有効化に失敗
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 r_dmaca_enable
// 処理概要     DMACAの転送有効化処理
// 引数         p_sdc_sd_access: 	SDカードドライバの転送設定 
// 				op_mode: 			書き込みor読み込み 
// 				reg_buff: 			データバッファ
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
static void r_dmaca_enable(sdc_sd_access_t * p_sdc_sd_access, uint8_t op_mode, uint32_t reg_buff)
{
	volatile dmaca_return_t  ret_dmaca = DMACA_SUCCESS;
    dmaca_transfer_data_cfg_t   p_data_cfg_dmac;
    dmaca_stat_t                p_stat_dmaca;

    if (SDC_SD_WRITE_OP == op_mode)
    {
        r_dmaca_set_send(&p_data_cfg_dmac, p_sdc_sd_access, reg_buff, IR_SDHI_SBFAI);
    }
    else
    {
        r_dmaca_set_recv(&p_data_cfg_dmac, p_sdc_sd_access, reg_buff, IR_SDHI_SBFAI);
    }

    ret_dmaca = R_DMACA_Create(DMACA_CH1, (dmaca_transfer_data_cfg_t *)&p_data_cfg_dmac);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        return; 
    }

    ret_dmaca = R_DMACA_Control(DMACA_CH1, DMACA_CMD_ENABLE, (dmaca_stat_t*)&p_stat_dmaca);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        return;
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 r_dmaca_set_recv
// 処理概要     DMACAの受信転送設定処理
// 引数         p_data_cfg: 		DMACA転送データ設定構造体 
// 				p_sdc_sd_access: 	SDカードドライバの転送設定
//				reg_buff: 			データバッファ 
// 				act_source: 		DMACのアクティベーションソース
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
static void r_dmaca_set_recv(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,
                             dmaca_activation_source_t act_source)
{
    p_data_cfg->transfer_mode          = DMACA_TRANSFER_MODE_BLOCK;         /* DMAC transfer mode                   */
    p_data_cfg->repeat_block_side      = DMACA_REPEAT_BLOCK_SOURCE;
    p_data_cfg->data_size              = DMACA_DATA_SIZE_LWORD;             /* The size of data                     */
    p_data_cfg->act_source             = act_source;
    p_data_cfg->request_source         = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    p_data_cfg->dtie_request           = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    p_data_cfg->esie_request           = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    p_data_cfg->rptie_request          = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    p_data_cfg->sarie_request          = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->darie_request          = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->src_addr_mode          = DMACA_SRC_ADDR_FIXED;              /* Addresss mode of source              */
    p_data_cfg->des_addr_mode          = DMACA_DES_ADDR_INCR;
    p_data_cfg->src_addr_repeat_area   = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->des_addr_repeat_area   = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->interrupt_sel          = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    /* The side being repeat or block in repeat / block transfer mode */
    p_data_cfg->p_src_addr             = (void *)reg_buff;                  /* Start address of source              */
    p_data_cfg->p_des_addr             = (void *)sdc_sd_access->p_buff;     /* Start address of destination         */
    p_data_cfg->transfer_count         = sdc_sd_access->cnt;                /* Transfer count                       */
    /* Size of a block in block transfer mode */
    p_data_cfg->block_size             = (512 / sizeof(uint32_t));
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 r_dmaca_set_send
// 処理概要     DMACAの送信転送設定処理
// 引数         p_data_cfg: 		DMACA転送データ設定構造体
// 				p_sdc_sd_access: 	SDカードドライバの転送設定
//				reg_buff: 			データバッファ
// 				act_source: 		DMACのアクティベーションソース
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
static void r_dmaca_set_send(dmaca_transfer_data_cfg_t *p_data_cfg, sdc_sd_access_t *sdc_sd_access, uint32_t reg_buff,
                             dmaca_activation_source_t act_source)
{
    p_data_cfg->transfer_mode          = DMACA_TRANSFER_MODE_BLOCK;         /* DMAC transfer mode                   */
    p_data_cfg->repeat_block_side      = DMACA_REPEAT_BLOCK_DESTINATION;
    p_data_cfg->data_size              = DMACA_DATA_SIZE_LWORD;             /* The size of data                     */
    p_data_cfg->act_source             = act_source;
    p_data_cfg->request_source         = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    p_data_cfg->dtie_request           = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    p_data_cfg->esie_request           = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    p_data_cfg->rptie_request          = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    p_data_cfg->sarie_request          = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->darie_request          = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_data_cfg->src_addr_mode          = DMACA_SRC_ADDR_INCR;               /* Addresss mode of source              */
    p_data_cfg->des_addr_mode          = DMACA_DES_ADDR_FIXED;              /* Addresss mode of destination         */
    p_data_cfg->src_addr_repeat_area   = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->des_addr_repeat_area   = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    p_data_cfg->interrupt_sel          = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    /* The side being repeat or block in repeat / block transfer mode */
    p_data_cfg->p_src_addr             = (void *)sdc_sd_access->p_buff;     /* Start address of source              */
    p_data_cfg->p_des_addr             = (void *)reg_buff;                  /* Start address of destination         */
    p_data_cfg->transfer_count         = sdc_sd_access->cnt;                /* Transfer count                       */
    /* Size of a block in block transfer mode */
    p_data_cfg->block_size             = (512 / sizeof(uint32_t));
}
#endif /* (TFAT_SDMEM_DRIVE_NUM > 0) */
/******************************************************************************
End  Of File
******************************************************************************/
