/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI10_user.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_SCI10.
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
#include "Config_SCI10.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci10_tx_address;                /* SCI10 transmit buffer address */
extern volatile uint16_t  g_sci10_tx_count;                   /* SCI10 transmit data number */
extern volatile uint8_t * gp_sci10_rx_address;                /* SCI10 receive buffer address */
extern volatile uint16_t  g_sci10_rx_count;                   /* SCI10 receive data number */
extern volatile uint16_t  g_sci10_rx_length;                  /* SCI10 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Create_UserInit
* Description  : This function adds user code after initializing the SCI10 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_transmit_interrupt
* Description  : This function is TXI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI10_transmit_interrupt(void)
{
    if (0U < g_sci10_tx_count)
    {
        SCI10.TDR = *gp_sci10_tx_address;
        gp_sci10_tx_address++;
        g_sci10_tx_count--;
    }
    else
    {
        SCI10.SCR.BIT.TIE = 0U;
        SCI10.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_transmitend_interrupt
* Description  : This function is TEI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI10_transmitend_interrupt(void)
{
    /* Set TXD10 pin */
    PORT8.PMR.BYTE &= 0xFBU;

    SCI10.SCR.BIT.TIE = 0U;
    SCI10.SCR.BIT.TE = 0U;
    SCI10.SCR.BIT.TEIE = 0U;

    r_Config_SCI10_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_receive_interrupt
* Description  : This function is RXI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI10_receive_interrupt(void)
{
    if (g_sci10_rx_length > g_sci10_rx_count)
    {
        *gp_sci10_rx_address = SCI10.RDR;
        gp_sci10_rx_address++;
        g_sci10_rx_count++;
    }

    if (g_sci10_rx_length <= g_sci10_rx_count)
    {
        /* All data received */
        SCI10.SCR.BIT.RIE = 0U;
        SCI10.SCR.BIT.RE = 0U;
        r_Config_SCI10_callback_receiveend();
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_receiveerror_interrupt
* Description  : This function is ERI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI10_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_Config_SCI10_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI10.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI10.SSR.BYTE = err_type;
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_callback_transmitend
* Description  : This function is a callback function when SCI10 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI10_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI10_callback_transmitend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_callback_receiveend
* Description  : This function is a callback function when SCI10 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI10_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI10_callback_receiveend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_callback_receiveerror
* Description  : This function is a callback function when SCI10 reception encounters error
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI10_callback_receiveerror(void)
{
    /* Start user code for r_Config_SCI10_callback_receiveerror. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
