/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI9_user.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_SCI9.
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
#include "Config_SCI9.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci9_tx_address;               /* SCI9 transmit buffer address */
extern volatile uint16_t  g_sci9_tx_count;                  /* SCI9 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI9_Create_UserInit
* Description  : This function adds user code after initializing the SCI9 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI9_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI9_transmit_interrupt
* Description  : This function is TXI9 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI9_transmit_interrupt(void)
{
    if (0U < g_sci9_tx_count)
    {
        SCI9.TDR = *gp_sci9_tx_address;
        gp_sci9_tx_address++;
        g_sci9_tx_count--;
    }
    else
    {
        SCI9.SCR.BIT.TIE = 0U;
        SCI9.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI9_transmitend_interrupt
* Description  : This function is TEI9 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI9_transmitend_interrupt(void)
{
    SCI9.SCR.BIT.TIE = 0U;
    SCI9.SCR.BIT.TEIE = 0U;

    /* Clear TE and RE bits */
    if(0U == SCI9.SCR.BIT.RIE)
    {
        SCI9.SCR.BYTE &= 0xCFU;
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
