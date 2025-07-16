/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI4_user.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_SCI4.
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
#include "Config_SCI4.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci4_tx_address;               /* SCI4 transmit buffer address */
extern volatile uint16_t  g_sci4_tx_count;                  /* SCI4 transmit data number */
extern volatile uint8_t * gp_sci4_rx_address;               /* SCI4 receive buffer address */
extern volatile uint16_t  g_sci4_rx_count;                  /* SCI4 receive data number */
extern volatile uint16_t  g_sci4_rx_length;                 /* SCI4 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI4_Create_UserInit
* Description  : This function adds user code after initializing the SCI4 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI4_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI4_transmit_interrupt
* Description  : This function is TXI4 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI4_transmit_interrupt(void)
{
    if (0U < g_sci4_tx_count)
    {
        SCI4.TDR = *gp_sci4_tx_address;
        gp_sci4_tx_address++;
        g_sci4_tx_count--;
    }
    else
    {
        SCI4.SCR.BIT.TIE = 0U;
        SCI4.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI4_transmitend_interrupt
* Description  : This function is TEI4 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI4_transmitend_interrupt(void)
{
    SCI4.SCR.BIT.TIE = 0U;
    SCI4.SCR.BIT.TEIE = 0U;

    /* Clear TE and RE bits */
    if(0U == SCI4.SCR.BIT.RIE)
    {
        SCI4.SCR.BYTE &= 0xCFU;
    }

    r_Config_SCI4_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI4_receive_interrupt
* Description  : This function is RXI4 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI4_receive_interrupt(void)
{
    if (g_sci4_rx_length > g_sci4_rx_count)
    {
        *gp_sci4_rx_address = SCI4.RDR;
        gp_sci4_rx_address++;
        g_sci4_rx_count++;

        if (g_sci4_rx_length == g_sci4_rx_count)
        {
            SCI4.SCR.BIT.RIE = 0;

            /* Clear TE and RE bits */
            if((0U == SCI4.SCR.BIT.TIE) && (0U == SCI4.SCR.BIT.TEIE))
            {
                SCI4.SCR.BYTE &= 0xCFU;
            }

            r_Config_SCI4_callback_receiveend();
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI4_receiveerror_interrupt
* Description  : This function is ERI4 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI4_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_Config_SCI4_callback_receiveerror();

    /* Clear overrun error flag */
    err_type = SCI4.SSR.BYTE;
    err_type &= 0xDFU;
    err_type |= 0xC0U;
    SCI4.SSR.BYTE = err_type;
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI4_callback_transmitend
* Description  : This function is a callback function when SCI4 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI4_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI4_callback_transmitend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI4_callback_receiveend
* Description  : This function is a callback function when SCI4 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI4_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI4_callback_receiveend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI4_callback_receiveerror
* Description  : This function is a callback function when SCI4 reception encounters error
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI4_callback_receiveerror(void)
{
    /* Start user code for r_Config_SCI4_callback_receiveerror. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
