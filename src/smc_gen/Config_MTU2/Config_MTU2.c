/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_MTU2.c
* Component Version: 2.4.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_MTU2.
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
#include "Config_MTU2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_MTU2_Create
* Description  : This function initializes the MTU2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU2_Create(void)
{
    /* Release MTU channel 2 from stop state */
    MSTP(MTU2) = 0U;

    /* Stop MTU channel 2 counter */
    MTU.TSTRA.BIT.CST2 = 0U;

    /* Set external clock noise filter */
    MTU0.NFCRC.BIT.NFCEN = 0U;
    MTU0.NFCRC.BIT.NFDEN = 0U;

    /* MTU channel 2 is used as phase counting mode */
    MTU2.TMDR1.BYTE = _04_MTU_COT1;
    MTU1.TMDR3.BIT.PHCKSEL = 1U;
    MTU.TSYRA.BIT.SYNC2 = 0U;
    MTU2.TCR.BYTE = _00_MTU_CKCL_DIS;
    MTU2.TIER.BYTE = _00_MTU_TGIEA_DISABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TCIEV_DISABLE | _00_MTU_TCIEU_DISABLE | 
                     _00_MTU_TTGE_DISABLE;
    MTU2.TIOR.BYTE = _07_MTU_IOA_HT | _00_MTU_IOB_DISABLE;
    MTU2.TGRA = _0063_TGRA2_VALUE;
    MTU2.TGRB = _0063_TGRB2_VALUE;

    /* Set MTCLKC pin */
    MPC.PA1PFS.BYTE = 0x02U;
    PORTA.PMR.BYTE |= 0x02U;

    /* Set MTCLKD pin */
    MPC.PA3PFS.BYTE = 0x02U;
    PORTA.PMR.BYTE |= 0x08U;

    /* Set MTIOC2A pin */
    MPC.PB5PFS.BYTE = 0x01U;
    PORTB.PMR.BYTE |= 0x20U;

    R_Config_MTU2_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU2_Start
* Description  : This function starts the MTU2 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU2_Start(void)
{
    /* Start MTU channel 2 counter */
    MTU.TSTRA.BIT.CST2 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU2_Stop
* Description  : This function stops the MTU2 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU2_Stop(void)
{
    /* Stop MTU channel 2 counter */
    MTU.TSTRA.BIT.CST2 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
