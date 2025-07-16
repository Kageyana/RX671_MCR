/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_TPU3.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_TPU3.
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
#include "Config_TPU3.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU3_Create
* Description  : This function initializes the TPU3 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU3_Create(void)
{
    /* Release TPU channel 3 from stop state */
    MSTP(TPU3) = 0U;

    /* Stop TPU channel 3 counter */
    TPUA.TSTR.BIT.CST3 = 0U;

    /* TPU channel 3 is used as PWM mode 1 */
    TPUA.TSYR.BIT.SYNC3 = 0U;
    TPU3.TCR.BYTE = _00_TPU_PCLK_1 | _20_TPU_CKCL_A;
    TPU3.TIER.BYTE |= (_00_TPU_TGIEA_DISABLE | _00_TPU_TGIEB_DISABLE | _00_TPU_TGIEC_DISABLE | _00_TPU_TGIED_DISABLE | 
                      _00_TPU_TCIEV_DISABLE | _00_TPU_TTGE_DISABLE);
    TPU3.TIORH.BYTE = _01_TPU_IOA_LL | _50_TPU_IOB_HL;
    TPU3.TIORL.BYTE = _01_TPU_IOC_LL | _50_TPU_IOD_HL;
    TPU3.TGRA = _0707_TGRA3_VALUE;
    TPU3.TGRB = _0064_TGRB3_VALUE;
    TPU3.TGRC = _0064_TGRC3_VALUE;
    TPU3.TGRD = _0064_TGRD3_VALUE;
    TPU3.TMDR.BYTE = _02_TPU_PWM1;

    /* Set TIOCA3 pin */
    MPC.P21PFS.BYTE = 0x03U;
    PORT2.PMR.BYTE |= 0x02U;

    /* Set TIOCC3 pin */
    MPC.P22PFS.BYTE = 0x03U;
    PORT2.PMR.BYTE |= 0x04U;

    R_Config_TPU3_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU3_Start
* Description  : This function starts the TPU3 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU3_Start(void)
{
    /* Start TPU channel 3 counter */
    TPUA.TSTR.BIT.CST3 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU3_Stop
* Description  : This function stops the TPU3 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU3_Stop(void)
{
    /* Stop TPU channel 3 counter */
    TPUA.TSTR.BIT.CST3 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
