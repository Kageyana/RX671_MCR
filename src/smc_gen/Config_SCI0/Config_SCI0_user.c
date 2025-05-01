/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI0_user.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_SCI0.
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
#include "Config_SCI0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci0_tx_address;               /* SCI0 transmit buffer address */
extern volatile uint16_t  g_sci0_tx_count;                  /* SCI0 transmit data number */
extern volatile uint8_t * gp_sci0_rx_address;               /* SCI0 receive buffer address */
extern volatile uint16_t  g_sci0_rx_count;                  /* SCI0 receive data number */
extern volatile uint16_t  g_sci0_rx_length;                 /* SCI0 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI0_Create_UserInit
* Description  : This function adds user code after initializing the SCI0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI0_transmit_interrupt
* Description  : This function is TXI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI0_transmit_interrupt(void)
{
    if (0U < g_sci0_tx_count)
    {
        SCI0.TDR = *gp_sci0_tx_address;
        gp_sci0_tx_address++;
        g_sci0_tx_count--;
    }
    else
    {
        SCI0.SCR.BIT.TIE = 0U;
        SCI0.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI0_transmitend_interrupt
* Description  : This function is TEI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI0_transmitend_interrupt(void)
{
    SCI0.SCR.BIT.TIE = 0U;
    SCI0.SCR.BIT.TEIE = 0U;

    /* Clear TE and RE bits */
    if(0U == SCI0.SCR.BIT.RIE)
    {
        SCI0.SCR.BYTE &= 0xCFU;
    }

    r_Config_SCI0_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI0_receive_interrupt
* Description  : This function is RXI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI0_receive_interrupt(void)
{
    if (g_sci0_rx_length > g_sci0_rx_count)
    {
        *gp_sci0_rx_address = SCI0.RDR;
        gp_sci0_rx_address++;
        g_sci0_rx_count++;

        if (g_sci0_rx_length == g_sci0_rx_count)
        {
            SCI0.SCR.BIT.RIE = 0;

            /* Clear TE and RE bits */
            if((0U == SCI0.SCR.BIT.TIE) && (0U == SCI0.SCR.BIT.TEIE))
            {
                SCI0.SCR.BYTE &= 0xCFU;
            }

            r_Config_SCI0_callback_receiveend();
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI0_receiveerror_interrupt
* Description  : This function is ERI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI0_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_Config_SCI0_callback_receiveerror();

    /* Clear overrun error flag */
    err_type = SCI0.SSR.BYTE;
    err_type &= 0xDFU;
    err_type |= 0xC0U;
    SCI0.SSR.BYTE = err_type;
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI0_callback_transmitend
* Description  : This function is a callback function when SCI0 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI0_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI0_callback_transmitend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI0_callback_receiveend
* Description  : This function is a callback function when SCI0 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI0_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI0_callback_receiveend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI0_callback_receiveerror
* Description  : This function is a callback function when SCI0 reception encounters error
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI0_callback_receiveerror(void)
{
    /* Start user code for r_Config_SCI0_callback_receiveerror. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
