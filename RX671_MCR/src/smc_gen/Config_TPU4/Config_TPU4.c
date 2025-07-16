/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_TPU4.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_TPU4.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_TPU4.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU4_Create
* Description  : This function initializes the TPU4 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU4_Create(void)
{
    /* Release TPU channel 4 from stop state */
    MSTP(TPU4) = 0U;

    /* Stop TPU channel 4 counter */
    TPUA.TSTR.BIT.CST4 = 0U;

    /* TPU channel 4 is used as PWM mode 1 */
    TPUA.TSYR.BIT.SYNC4 = 0U;
    TPU4.TCR.BYTE = _00_TPU_PCLK_1 | _20_TPU_CKCL_A;
    TPU4.TIER.BYTE |= (_00_TPU_TGIEA_DISABLE | _00_TPU_TGIEB_DISABLE | _00_TPU_TCIEV_DISABLE | _00_TPU_TTGE_DISABLE);
    TPU4.TIOR.BYTE = _01_TPU_IOA_LL | _50_TPU_IOB_HL;
    TPU4.TGRA = _0707_TGRA4_VALUE;
    TPU4.TGRB = _0064_TGRB4_VALUE;
    TPU4.TMDR.BYTE = _02_TPU_PWM1;

    /* Set TIOCA4 pin */
    MPC.P25PFS.BYTE = 0x03U;
    PORT2.PMR.BYTE |= 0x20U;

    R_Config_TPU4_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU4_Start
* Description  : This function starts the TPU4 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU4_Start(void)
{
    /* Start TPU channel 4 counter */
    TPUA.TSTR.BIT.CST4 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU4_Stop
* Description  : This function stops the TPU4 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU4_Stop(void)
{
    /* Stop TPU channel 4 counter */
    TPUA.TSTR.BIT.CST4 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
