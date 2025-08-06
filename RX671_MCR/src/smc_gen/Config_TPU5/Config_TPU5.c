/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_TPU5.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_TPU5.
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
#include "Config_TPU5.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU5_Create
* Description  : This function initializes the TPU5 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU5_Create(void)
{
    /* Release TPU channel 5 from stop state */
    MSTP(TPU5) = 0U;

    /* Stop TPU channel 5 counter */
    TPUA.TSTR.BIT.CST5 = 0U;

    /* Set TGI5A interrupt priority level */
    ICU.SLIBR164.BYTE = 0x25U;
    IPR(PERIB, INTB164) = _0F_TPU_PRIORITY_LEVEL15;

    /* TPU channel 5 is used as PWM mode 1 */
    TPUA.TSYR.BIT.SYNC5 = 0U;
    TPU5.TCR.BYTE = _00_TPU_PCLK_1 | _20_TPU_CKCL_A;
    TPU5.TIER.BYTE |= (_01_TPU_TGIEA_ENABLE | _00_TPU_TGIEB_DISABLE | _00_TPU_TCIEV_DISABLE);
    TPU5.TIOR.BYTE = _06_TPU_IOA_HH | _50_TPU_IOB_HL;
    TPU5.TGRA = _004A_TGRA5_VALUE;
    TPU5.TGRB = _0000_TGRB5_VALUE;
    TPU5.TMDR.BYTE = _02_TPU_PWM1;

    /* Set TIOCA5 pin */
    MPC.PB6PFS.BYTE = 0x03U;
    PORTB.PMR.BYTE |= 0x40U;

    R_Config_TPU5_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU5_Start
* Description  : This function starts the TPU5 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU5_Start(void)
{
    /* Enable TGI5A interrupt in ICU */
    IEN(PERIB, INTB164) = 1U;
    
    /* Start TPU channel 5 counter */
    TPUA.TSTR.BIT.CST5 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU5_Stop
* Description  : This function stops the TPU5 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU5_Stop(void)
{
    /* Disable TGI5A interrupt in ICU */
    IEN(PERIB, INTB164) = 0U;
    
    /* Stop TPU channel 5 counter */
    TPUA.TSTR.BIT.CST5 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
