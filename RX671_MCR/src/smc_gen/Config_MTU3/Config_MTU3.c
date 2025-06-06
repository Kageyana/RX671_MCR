/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_MTU3.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_MTU3.
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
#include "Config_MTU3.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_MTU3_Create
* Description  : This function initializes the MTU3 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU3_Create(void)
{
    /* Release MTU channel 3 from stop state */
    MSTP(MTU3) = 0U;

    /* Enable read/write to MTU3 registers */
    MTU.TRWERA.BIT.RWE = 1U;

    /* Stop MTU channel 3 counter */
    MTU.TSTRA.BIT.CST3 = 0U;

    /* MTU channel 3 is used as PWM mode 1 */
    MTU.TSYRA.BIT.SYNC3 = 0U;
    MTU3.TCR.BYTE = _00_MTU_PCLK_1 | _20_MTU_CKCL_A;
    MTU3.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU3.TIER.BYTE = _00_MTU_TGIEA_DISABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU3.TMDR1.BYTE = _02_MTU_PWM1;
    MTU3.TIORH.BYTE = _02_MTU_IOA_LH | _50_MTU_IOB_HL;
    MTU3.TIORL.BYTE = _02_MTU_IOC_LH | _50_MTU_IOD_HL;
    MTU3.TGRA = _0E0F_TGRA3_VALUE;
    MTU3.TGRB = _0000_TGRB3_VALUE;
    MTU3.TGRC = _0E0F_TGRC3_VALUE;
    MTU3.TGRD = _0000_TGRD3_VALUE;

    /* Disable read/write to MTU3 registers */
    MTU.TRWERA.BIT.RWE = 0U;

    /* Set MTIOC3A pin */
    MPC.PC1PFS.BYTE = 0x01U;
    PORTC.PMR.BYTE |= 0x02U;

    /* Set MTIOC3C pin */
    MPC.PJ3PFS.BYTE = 0x01U;
    PORTJ.PMR.BYTE |= 0x08U;

    R_Config_MTU3_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU3_Start
* Description  : This function starts the MTU3 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU3_Start(void)
{
    /* Start MTU channel 3 counter */
    MTU.TSTRA.BIT.CST3 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU3_Stop
* Description  : This function stops the MTU3 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU3_Stop(void)
{
    /* Stop MTU channel 3 counter */
    MTU.TSTRA.BIT.CST3 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
