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
* File Name        : Config_SCI10.c
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
volatile uint8_t * gp_sci10_tx_address;                /* SCI10 transmit buffer address */
volatile uint16_t  g_sci10_tx_count;                   /* SCI10 transmit data number */
volatile uint8_t * gp_sci10_rx_address;                /* SCI10 receive buffer address */
volatile uint16_t  g_sci10_rx_count;                   /* SCI10 receive data number */
volatile uint16_t  g_sci10_rx_length;                  /* SCI10 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Create
* Description  : This function initializes the SCI10 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Create(void)
{
    /* Cancel SCI stop state */
    MSTP(SCI10) = 0U;

    /* Set interrupt priority */
    IPR(SCI10, RXI10) = _0F_SCI_PRIORITY_LEVEL15;
    IPR(SCI10, TXI10) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI10.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI10.SCR.BYTE = _00_SCI_INTERNAL_SCK_UNUSED;

    /* Clear the SIMR1.IICM, SPMR.CKPH, and CKPOL bit, and set SPMR */
    SCI10.SIMR1.BIT.IICM = 0U;
    SCI10.SPMR.BYTE = _00_SCI_RTS | _00_SCI_CLOCK_NOT_INVERTED | _00_SCI_CLOCK_NOT_DELAYED;

    /* Set control registers */
    SCI10.SMR.BYTE = _01_SCI_CLOCK_PCLK_4 | _00_SCI_MULTI_PROCESSOR_DISABLE | _00_SCI_STOP_1 | 
                     _00_SCI_PARITY_DISABLE | _00_SCI_DATA_LENGTH_8 | _00_SCI_ASYNCHRONOUS_OR_I2C_MODE;
    SCI10.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                      _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI10.SEMR.BYTE = _00_SCI_INSTANT_TRANSMIT_DISABLE | _00_SCI_BIT_MODULATION_DISABLE | _00_SCI_DEPEND_BGDM_ABCS | 
                      _00_SCI_16_BASE_CLOCK | _00_SCI_NOISE_FILTER_DISABLE | _40_SCI_BAUDRATE_DOUBLE | 
                      _00_SCI_LOW_LEVEL_START_BIT;
    SCI10.SPTR.BYTE = _00_SCI_IN_SIGNAL_NOT_INVERT | _00_SCI_OUT_SIGNAL_NOT_INVERT | _00_SCI_RECEIVE_TIME_NOT_ADJUST | 
                      _00_SCI_TRANSMIT_TIME_NOT_ADJUST | _03_SCI_SPTR_DEFAULT;

    /* Set bit rate */
    SCI10.BRR = 0xC2U;

    /* Set TXD10 pin */
    MPC.P82PFS.BYTE = 0x0AU;
    PORT8.PODR.BYTE |= 0x04U;
    PORT8.PDR.BYTE |= 0x04U;

    /* Set RXD10 pin */
    MPC.PC6PFS.BYTE = 0x24U;
    PORTC.PMR.BYTE |= 0x40U;

    R_Config_SCI10_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Start
* Description  : This function starts the SCI10 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Start(void)
{
    /* Clear interrupt flag */
    IR(SCI10, TXI10) = 0U;
    IR(SCI10, RXI10) = 0U;

    /* Enable SCI interrupt */
    IEN(SCI10, TXI10) = 1U;
    ICU.GENAL0.BIT.EN8 = 1U;
    IEN(SCI10, RXI10) = 1U;
    ICU.GENAL0.BIT.EN9 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Stop
* Description  : This function stop the SCI10 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Stop(void)
{
    /* Set TXD10 pin */
    PORT8.PMR.BYTE &= 0xFBU;

    /* Disable serial transmit */
    SCI10.SCR.BIT.TE = 0U;

    /* Disable serial receive */
    SCI10.SCR.BIT.RE = 0U;

    /* Disable SCI interrupt */
    SCI10.SCR.BIT.TIE = 0U;
    SCI10.SCR.BIT.RIE = 0U;
    IEN(SCI10, TXI10) = 0U;
    ICU.GENAL0.BIT.EN8 = 0U;
    IR(SCI10, TXI10) = 0U;
    IEN(SCI10, RXI10) = 0U;
    ICU.GENAL0.BIT.EN9 = 0U;
    IR(SCI10, RXI10) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Serial_Receive
* Description  : This function receive SCI10 data
* Arguments    : rx_buf -
*                    receive buffer pointer (Not used when receive data handled by DMAC/DTC)
*                rx_num -
*                    buffer size (Not used when receive data handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI10_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > rx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci10_rx_count = 0U;
        g_sci10_rx_length = rx_num;
        gp_sci10_rx_address = rx_buf;
        SCI10.SCR.BYTE |= 0x50U;
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Serial_Send
* Description  : This function transmits SCI10 data
* Arguments    : tx_buf -
*                    transfer buffer pointer (Not used when transmit data handled by DMAC/DTC)
*                tx_num -
*                    buffer size (Not used when transmit data handled by DMAC/DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR or MD_ERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI10_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else if (0U == IEN(SCI10, TXI10))
    {
        status = MD_ERROR;
    }
    else
    {
        gp_sci10_tx_address = tx_buf;
        g_sci10_tx_count = tx_num;
        IEN(SCI10, TXI10) = 0U;
        SCI10.SCR.BYTE |= 0xA0U;

        /* Set TXD10 pin */
        PORT8.PMR.BYTE |= 0x04U;
        IEN(SCI10, TXI10) = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
