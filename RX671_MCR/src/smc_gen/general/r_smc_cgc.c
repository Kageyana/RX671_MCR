/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : r_smc_cgc.c
* Version          : 2.1.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements CGC setting.
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
#include "r_smc_cgc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CGC_Create
* Description  : This function generates CGC pin settings
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_CGC_Create(void)
{
    /* Set SDCLK pin */
    SYSTEM.SCKCR.BIT.PSTOP0 = 1U;
    MPC.PFBCR1.BIT.SDCLKE = 1U;
    SYSTEM.SCKCR.BIT.PSTOP0 = 0U;
    PORT7.PMR.BYTE |= 0x01U;

    R_CGC_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
