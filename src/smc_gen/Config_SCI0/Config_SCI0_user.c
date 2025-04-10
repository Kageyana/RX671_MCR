/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

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

#if FAST_INTERRUPT_VECTOR == VECT_SCI0_TXI0
#pragma interrupt r_Config_SCI0_transmit_interrupt(vect=VECT(SCI0,TXI0),fint)
#else
#pragma interrupt r_Config_SCI0_transmit_interrupt(vect=VECT(SCI0,TXI0))
#endif
static void r_Config_SCI0_transmit_interrupt(void)
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

#if FAST_INTERRUPT_VECTOR == VECT_SCI0_RXI0
#pragma interrupt r_Config_SCI0_receive_interrupt(vect=VECT(SCI0,RXI0),fint)
#else
#pragma interrupt r_Config_SCI0_receive_interrupt(vect=VECT(SCI0,RXI0))
#endif
static void r_Config_SCI0_receive_interrupt(void)
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
