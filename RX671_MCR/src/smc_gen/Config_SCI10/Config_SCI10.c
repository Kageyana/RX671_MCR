/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

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
volatile uint8_t * gp_sci10_tx_address;               /* SCI10 transmit buffer address */
volatile uint16_t  g_sci10_tx_count;                  /* SCI10 transmit data number */
volatile uint8_t * gp_sci10_rx_address;               /* SCI10 receive buffer address */
volatile uint16_t  g_sci10_rx_count;                  /* SCI10 receive data number */
volatile uint16_t  g_sci10_rx_length;                 /* SCI10 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Create
* Description  : This function initializes SCI10
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Create(void)
{
    /* Cancel SCI10 module stop state */
    MSTP(SCI10) = 0U;

    /* Set interrupt priority */
    IPR(SCI10,TXI10) = _0F_SCI_PRIORITY_LEVEL15;
    IPR(SCI10,RXI10) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI10.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI10.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI10.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI10.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                      _00_SCI_CLOCK_NOT_DELAYED;
    SCI10.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE | _00_SCI_CLOCK_PCLK;
    SCI10.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | 
                      _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI10.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bit rate */
    SCI10.BRR = 0x00U;

    /* Set SMISO10 pin */
    MPC.PC6PFS.BYTE = 0x24U;
    PORTC.PMR.BYTE |= 0x40U;

    /* Set SMOSI10 pin */
    MPC.P82PFS.BYTE = 0x0AU;
    PORT8.PMR.BYTE |= 0x04U;

    /* Set SCK10 pin */
    MPC.P83PFS.BYTE = 0x0AU;
    PORT8.PMR.BYTE |= 0x08U;

    R_Config_SCI10_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Start
* Description  : This function starts SCI10
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI10,TXI10) = 0U;
    IEN(SCI10,TXI10) = 1U;
    ICU.GENAL0.BIT.EN8 = 1U;

    /* Enable RXI interrupt */
    IR(SCI10,RXI10) = 0U;
    IEN(SCI10,RXI10) = 1U;

    /* Enable ERI interrupt */
    ICU.GENAL0.BIT.EN9 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Stop
* Description  : This function stops SCI10
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Stop(void)
{
    /* Disable serial transmit and receive */
    SCI10.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI10,TXI10) = 0U;
    ICU.GENAL0.BIT.EN8 = 0U;

    /* Disable RXI interrupt */
    IEN(SCI10,RXI10) = 0U;

    /* Disable ERI interrupt */
    ICU.GENAL0.BIT.EN9 = 0U;

    /* Clear interrupt flags */
    IR(SCI10,TXI10) = 0U;
    IR(SCI10,RXI10) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_SPI_Master_Send_Receive
* Description  : This function sends and receives SCI10 data to and from slave device
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC/DTC)
*                tx_num -
*                    transfer buffer size
*                rx_buf -
*                    receive buffer pointer (not used when data is handled by DMAC/DTC)
*                rx_num -
*                    receive buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI10_SPI_Master_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci10_tx_count = tx_num;
        gp_sci10_tx_address = tx_buf;
        gp_sci10_rx_address = rx_buf;
        g_sci10_rx_count = 0U;
        g_sci10_rx_length = rx_num;

        /* Set TE, TIE, RE, RIE bits simultaneously */
        SCI10.SCR.BYTE |= 0xF0U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
