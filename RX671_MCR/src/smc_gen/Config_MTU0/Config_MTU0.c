/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_MTU0.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_MTU0.
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
#include "Config_MTU0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_MTU0_Create
* Description  : This function initializes the MTU0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU0_Create(void)
{
    /* Release MTU channel 0 from stop state */
    MSTP(MTU0) = 0U;

    /* Stop MTU channel 0 counter */
    MTU.TSTRA.BIT.CST0 = 0U;

    /* MTU channel 0 is used as PWM mode 1 */
    MTU.TSYRA.BIT.SYNC0 = 0U;
    MTU0.TCR.BYTE = _00_MTU_PCLK_1 | _20_MTU_CKCL_A;
    MTU0.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU0.TIER.BYTE = _00_MTU_TGIEA_DISABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU0.TIER2.BYTE = _00_MTU_TGIEE_DISABLE | _00_MTU_TGIEF_DISABLE | _00_MTU_TTGE2_DISABLE;
    MTU0.TMDR1.BYTE = _02_MTU_PWM1;
    MTU0.TIORH.BYTE = _02_MTU_IOA_LH | _50_MTU_IOB_HL;
    MTU0.TIORL.BYTE = _02_MTU_IOC_LH | _50_MTU_IOD_HL;
    MTU0.TGRA = _0E0F_TGRA0_VALUE;
    MTU0.TGRB = _0000_TGRB0_VALUE;
    MTU0.TGRC = _0E0F_TGRC0_VALUE;
    MTU0.TGRD = _0000_TGRD0_VALUE;
    MTU0.TGRE = _0064_TGRE0_VALUE;
    MTU0.TGRF = _0E0F_TGRF0_VALUE;

    /* Set MTIOC0A pin */
    MPC.P34PFS.BYTE = 0x01U;
    PORT3.PMR.BYTE |= 0x10U;

    /* Set MTIOC0C pin */
    MPC.P32PFS.BYTE = 0x01U;
    PORT3.PMR.BYTE |= 0x04U;

    R_Config_MTU0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU0_Start
* Description  : This function starts the MTU0 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU0_Start(void)
{
    /* Start MTU channel 0 counter */
    MTU.TSTRA.BIT.CST0 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU0_Stop
* Description  : This function stops the MTU0 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU0_Stop(void)
{
    /* Stop MTU channel 0 counter */
    MTU.TSTRA.BIT.CST0 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
