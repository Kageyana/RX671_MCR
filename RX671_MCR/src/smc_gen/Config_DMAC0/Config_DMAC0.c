/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_DMAC0.c
* Component Version: 1.8.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_DMAC0.
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
#include "Config_DMAC0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_DMAC0_Create
* Description  : This function initializes the DMAC0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DMAC0_Create(void)
{
    /* Cancel DMAC/DTC module stop state in LPC */
    MSTP(DMAC) = 0U;

    /* Disable DMAC interrupts */
    IEN(DMAC,DMAC0I) = 0U;

    /* Disable DMAC0 transfer */
    DMAC0.DMCNT.BIT.DTE = 0U;

    /* Set DMAC0 activation source */
    ICU.DMRSR0 = _A4_DMAC0_ACTIVATION_SOURCE;

    /* Set DMAC0 transfer address update and extended repeat setting */
    DMAC0.DMAMD.WORD = _8000_DMAC_SRC_ADDR_UPDATE_INCREMENT | _0000_DMAC_DST_ADDR_UPDATE_FIXED | 
                       _0000_DMAC0_SRC_EXT_RPT_AREA | _0000_DMAC0_DST_EXT_RPT_AREA;

    /* Set DMAC0 transfer mode, data size and repeat area */
    DMAC0.DMTMD.WORD = _0000_DMAC_TRANS_MODE_NORMAL | _2000_DMAC_REPEAT_AREA_NONE | _0100_DMAC_TRANS_DATA_SIZE_16 | 
                       _0001_DMAC_TRANS_REQ_SOURCE_INT;

    /* Set DMAC0 interrupt flag control */
    DMAC0.DMCSL.BYTE = _00_DMAC_INT_TRIGGER_FLAG_CLEAR;

    /* Set DMAC0 source address */
    DMAC0.DMSAR = (void *)_00000FCC_DMAC0_SRC_ADDR;

    /* Set DMAC0 destination address */
    DMAC0.DMDAR = (void *)_0008816A_DMAC0_DST_ADDR;

    /* Set DMAC0 transfer count */
    DMAC0.DMCRA = _00000146_DMAC0_DMCRA_COUNT;

    /* Set DMAC0 interrupt settings */
    DMAC0.DMINT.BIT.DTIE = 1U;

    /* Set DMAC0 priority level */
    IPR(DMAC,DMAC0I) = _0F_DMAC_PRIORITY_LEVEL15;

    R_Config_DMAC0_Create_UserInit();

    /* Enable DMAC activation */
    DMAC.DMAST.BIT.DMST = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_DMAC0_Start
* Description  : This function enable the DMAC0 activation
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DMAC0_Start(void)
{
    /* Enable DMAC0 interrupt in ICU */
    IR(DMAC,DMAC0I) = 0U;
    IEN(DMAC,DMAC0I) = 1U;
    DMAC0.DMCNT.BIT.DTE = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_DMAC0_Stop
* Description  : This function disable the DMAC0 activation
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DMAC0_Stop(void)
{
    /* Disable CMI0 interrupt in ICU */
    IR(DMAC,DMAC0I) = 0U;
    IEN(DMAC,DMAC0I) = 0U;
    DMAC0.DMCNT.BIT.DTE = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
