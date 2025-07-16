/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_RSPI2_user.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_RSPI2.
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
#include "Config_RSPI2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint16_t * gp_rspi2_tx_address;              /* RSPI2 transmit buffer address */
extern volatile uint16_t g_rspi2_tx_count;                   /* RSPI2 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RSPI2_Create_UserInit
* Description  : This function adds user code after initializing RSPI2
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RSPI2_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_RSPI2_transmit_interrupt
* Description  : This function is SPTI2 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_RSPI2_transmit_interrupt(void)
{
    uint16_t frame_cnt;

    /* WAIT_LOOP */
    for (frame_cnt = 0U; frame_cnt < (_00_RSPI_FRAMES_1 + 1U); frame_cnt++)
    {
        if (g_rspi2_tx_count > 0U)
        {
            /* Write data for transmission */
            RSPI2.SPDR.WORD.H = (*(uint16_t*)gp_rspi2_tx_address);
            gp_rspi2_tx_address++;
            g_rspi2_tx_count--;
        }
        else
        {
            /* Disable transmit interrupt */
            RSPI2.SPCR.BIT.SPTIE = 0U;

            /* Enable communication end interrupt */
            RSPI2.SPCR3.BIT.SPCIE = 1U;

            /* Enable idle interrupt */
            RSPI2.SPCR2.BIT.SPIIE = 1U;
            break;
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_RSPI2_idle_interrupt
* Description  : This function is SPII2 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_RSPI2_idle_interrupt(void)
{
    /* Disable RSPI function */
    RSPI2.SPCR.BIT.SPE = 0U;

    /* Disable idle interrupt */
    RSPI2.SPCR2.BIT.SPIIE = 0U;

    /* Disable communication end interrupt */
    RSPI2.SPCR3.BIT.SPCIE = 0U;

    r_Config_RSPI2_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_RSPI2_communication_end_interrupt
* Description  : This function is SPCI2 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_RSPI2_communication_end_interrupt(void)
{
    /* Disable RSPI function */
    RSPI2.SPCR.BIT.SPE = 0U;
    
    /* Disable idle interrupt */
    RSPI2.SPCR2.BIT.SPIIE = 0U;

    /* Disable communication end interrupt */
    RSPI2.SPCR3.BIT.SPCIE = 0U;

    r_Config_RSPI2_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_RSPI2_callback_transmitend
* Description  : This function is a callback function when RSPI2 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_RSPI2_callback_transmitend(void)
{
    /* Start user code for r_Config_RSPI2_callback_transmitend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
