/**********************************************************************************************************************
* Copyright (c) 2014 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdc_sdmem_param.c
* Version      : 3.01
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : For RX e2_studio
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDC SD API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 05.09.2015 1.00    First Release
*              : 17.07.2015 1.10    Standardized by the combo.
*              : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*              :                    Changed prefix from SDHI to SDC_SD.
*              : 20.03.2025 3.01    Changed the disclaimer in program sources.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdc_sd_rx_if.h"
#include ".\src\r_sdc_sd_rx_private.h"

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/

/**********************************************************************************************************************
* Outline      : Check Parameter Of Extended Operation
* Function Name: r_sdc_sdmem_chk_param
* Description  : Checks parameters of memory operation.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : sdc_sd_access_t  *p_sdc_sd_access      ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_chk_param(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_PARAM, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_sdc_sd_access)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    if (0 == (uint32_t)p_sdc_sd_access->p_buff)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    /* ---- Check access area. ---- */
    if ((p_sdc_sd_access->lbn >= p_hndl->card_sector_size) ||
        ((p_sdc_sd_access->lbn + p_sdc_sd_access->cnt) > p_hndl->card_sector_size))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR; /* Out of area */
    }

    if ((p_sdc_sd_access->cnt > SDC_SDMEM_TRANS_SECTORS) || (0 >= p_sdc_sd_access->cnt))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR; /* Out of area */
    }

    return SDC_SD_SUCCESS;
#else
    return SDC_SD_SUCCESS;
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
} /* End of function r_sdc_sdmem_chk_param() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */

/* End of File */
