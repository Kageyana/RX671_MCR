/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI2_user.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_SCI2.
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
#include "Config_SCI2.h"
/* Start user code for include. Do not edit comment generated here */
#include "BMI088.h"
#include "ssd1351.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci2_tx_address;               /* SCI2 transmit buffer address */
extern volatile uint16_t  g_sci2_tx_count;                  /* SCI2 transmit data number */
extern volatile uint8_t * gp_sci2_rx_address;               /* SCI2 receive buffer address */
extern volatile uint16_t  g_sci2_rx_count;                  /* SCI2 receive data number */
extern volatile uint16_t  g_sci2_rx_length;                 /* SCI2 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI2_Create_UserInit
* Description  : This function adds user code after initializing the SCI2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI2_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI2_transmit_interrupt
* Description  : This function is TXI2 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI2_transmit_interrupt(void)
{
    if (0U < g_sci2_tx_count)
    {
        SCI2.TDR = *gp_sci2_tx_address;
        gp_sci2_tx_address++;
        g_sci2_tx_count--;
    }
    else
    {
        SCI2.SCR.BIT.TIE = 0U;
        SCI2.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI2_transmitend_interrupt
* Description  : This function is TEI2 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI2_transmitend_interrupt(void)
{
    SCI2.SCR.BIT.TIE = 0U;
    SCI2.SCR.BIT.TEIE = 0U;

    /* Clear TE and RE bits */
    if(0U == SCI2.SCR.BIT.RIE)
    {
        SCI2.SCR.BYTE &= 0xCFU;
    }

    r_Config_SCI2_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI2_receive_interrupt
* Description  : This function is RXI2 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI2_receive_interrupt(void)
{
    if (g_sci2_rx_length > g_sci2_rx_count)
    {
        *gp_sci2_rx_address = SCI2.RDR;
        gp_sci2_rx_address++;
        g_sci2_rx_count++;

        if (g_sci2_rx_length == g_sci2_rx_count)
        {
            SCI2.SCR.BIT.RIE = 0;

            /* Clear TE and RE bits */
            if((0U == SCI2.SCR.BIT.TIE) && (0U == SCI2.SCR.BIT.TEIE))
            {
                SCI2.SCR.BYTE &= 0xCFU;
            }

            r_Config_SCI2_callback_receiveend();
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI2_callback_transmitend
* Description  : This function is a callback function when SCI2 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI2_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI2_callback_transmitend. Do not edit comment generated here */
	spi_ssd1351_tx_done = true;
	spi_BMI088_tx_done = true;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI2_callback_receiveend
* Description  : This function is a callback function when SCI2 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI2_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI2_callback_receiveend. Do not edit comment generated here */
	spi_BMI088_rx_done = true;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
