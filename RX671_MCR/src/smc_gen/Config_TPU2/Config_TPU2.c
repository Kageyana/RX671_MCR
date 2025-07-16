/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_TPU2.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_TPU2.
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
#include "Config_TPU2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU2_Create
* Description  : This function initializes the TPU2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU2_Create(void)
{
    /* Release TPU channel 2 from stop state */
    MSTP(TPU2) = 0U;

    /* Stop TPU channel 2 counter */
    TPUA.TSTR.BIT.CST2 = 0U;

    /* TPU channel 2 is used as PWM mode 1 */
    TPUA.TSYR.BIT.SYNC2 = 0U;
    TPU2.TCR.BYTE = _00_TPU_PCLK_1 | _20_TPU_CKCL_A;
    TPU2.TIER.BYTE |= (_00_TPU_TGIEA_DISABLE | _00_TPU_TGIEB_DISABLE | _00_TPU_TCIEV_DISABLE | _00_TPU_TTGE_DISABLE);
    TPU2.TIOR.BYTE = _01_TPU_IOA_LL | _50_TPU_IOB_HL;
    TPU2.TGRA = _0707_TGRA2_VALUE;
    TPU2.TGRB = _0064_TGRB2_VALUE;
    TPU2.TMDR.BYTE = _02_TPU_PWM1;

    /* Set TIOCA2 pin */
    MPC.P87PFS.BYTE = 0x03U;
    PORT8.PMR.BYTE |= 0x80U;

    R_Config_TPU2_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU2_Start
* Description  : This function starts the TPU2 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU2_Start(void)
{
    /* Start TPU channel 2 counter */
    TPUA.TSTR.BIT.CST2 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU2_Stop
* Description  : This function stops the TPU2 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU2_Stop(void)
{
    /* Stop TPU channel 2 counter */
    TPUA.TSTR.BIT.CST2 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
