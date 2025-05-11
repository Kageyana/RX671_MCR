/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_DMAC0_user.c
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
#include "WS2812C.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_DMAC0_Create_UserInit
* Description  : This function adds user code after initializing the DMAC0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_DMAC0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_DMAC0_dmac0i_interrupt
* Description  : This function is dmac0i interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_DMAC0_dmac0i_interrupt(void)
{
    if (DMAC0.DMSTS.BIT.DTIF == 1U)
    {
        DMAC0.DMSTS.BIT.DTIF = 0U;
        r_dmac0_callback_transfer_end();
    }
}

/***********************************************************************************************************************
* Function Name: r_dmac0_callback_transfer_end
* Description  : This function is dmac0 transfer end callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_dmac0_callback_transfer_end(void)
{
    /* Start user code for r_dmac0_callback_transfer_end. Do not edit comment generated here */
    dma_done_flag = true;
	/* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
