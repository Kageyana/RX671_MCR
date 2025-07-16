/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_MTU6.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_MTU6.
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
#include "Config_MTU6.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_MTU6_Create
* Description  : This function initializes the MTU6 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU6_Create(void)
{
    /* Release MTU channel 6 from stop state */
    MSTP(MTU6) = 0U;

    /* Enable read/write to MTU6 registers */
    MTU.TRWERB.BIT.RWE = 1U;

    /* Stop MTU channel 6 counter */
    MTU.TSTRB.BIT.CST6 = 0U;

    /* Set timer synchronous clear */
    MTU6.TSYCR.BYTE = _00_MTU6_CL0A_OFF | _00_MTU6_CL0B_OFF | _00_MTU6_CL0C_OFF | _00_MTU6_CL0D_OFF | 
                      _00_MTU6_CL1A_OFF | _00_MTU6_CL1B_OFF | _00_MTU6_CL2A_OFF | _00_MTU6_CL2B_OFF;

    /* MTU channel 6 is used as PWM mode 1 */
    MTU.TSYRB.BIT.SYNC6 = 0U;
    MTU6.TCR.BYTE = _00_MTU_PCLK_1 | _20_MTU_CKCL_A;
    MTU6.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU6.TIER.BYTE = _00_MTU_TGIEA_DISABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU6.TMDR1.BYTE = _02_MTU_PWM1;
    MTU6.TIORH.BYTE = _01_MTU_IOA_LL | _50_MTU_IOB_HL;
    MTU6.TIORL.BYTE = _01_MTU_IOC_LL | _50_MTU_IOD_HL;
    MTU6.TGRA = _0E0F_TGRA6_VALUE;
    MTU6.TGRB = _0064_TGRB6_VALUE;
    MTU6.TGRC = _0064_TGRC6_VALUE;
    MTU6.TGRD = _0064_TGRD6_VALUE;

    /* Disable read/write to MTU6 registers */
    MTU.TRWERB.BIT.RWE = 0U;

    /* Set MTIOC6A pin */
    MPC.PE7PFS.BYTE = 0x08U;
    PORTE.PMR.BYTE |= 0x80U;

    /* Set MTIOC6C pin */
    MPC.PE6PFS.BYTE = 0x08U;
    PORTE.PMR.BYTE |= 0x40U;

    R_Config_MTU6_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU6_Start
* Description  : This function starts the MTU6 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU6_Start(void)
{
    /* Start MTU channel 6 counter */
    MTU.TSTRB.BIT.CST6 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU6_Stop
* Description  : This function stops the MTU6 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU6_Stop(void)
{
    /* Stop MTU channel 6 counter */
    MTU.TSTRB.BIT.CST6 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
