/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI2.c
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
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_sci2_tx_address;               /* SCI2 transmit buffer address */
volatile uint16_t  g_sci2_tx_count;                  /* SCI2 transmit data number */
volatile uint8_t * gp_sci2_rx_address;               /* SCI2 receive buffer address */
volatile uint16_t  g_sci2_rx_count;                  /* SCI2 receive data number */
volatile uint16_t  g_sci2_rx_length;                 /* SCI2 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI2_Create
* Description  : This function initializes SCI2
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI2_Create(void)
{
    /* Cancel SCI2 module stop state */
    MSTP(SCI2) = 0U;

    /* Set interrupt priority */
    IPR(SCI2,TXI2) = _0F_SCI_PRIORITY_LEVEL15;
    IPR(SCI2,RXI2) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI2.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI2.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI2.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI2.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI2.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE | _00_SCI_CLOCK_PCLK;
    SCI2.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI2.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bit rate */
    SCI2.BRR = 0x01U;

    /* Set SMISO2 pin */
    MPC.P52PFS.BYTE = 0x0AU;
    PORT5.PMR.BYTE |= 0x04U;

    /* Set SMOSI2 pin */
    MPC.P50PFS.BYTE = 0x0AU;
    PORT5.PMR.BYTE |= 0x01U;

    /* Set SCK2 pin */
    MPC.P51PFS.BYTE = 0x0AU;
    PORT5.PMR.BYTE |= 0x02U;

    R_Config_SCI2_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI2_Start
* Description  : This function starts SCI2
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI2_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI2,TXI2) = 0U;
    IEN(SCI2,TXI2) = 1U;
    ICU.GENBL0.BIT.EN4 = 1U;

    /* Enable RXI interrupt */
    IR(SCI2,RXI2) = 0U;
    IEN(SCI2,RXI2) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI2_Stop
* Description  : This function stops SCI2
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI2_Stop(void)
{
    /* Disable serial transmit and receive */
    SCI2.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI2,TXI2) = 0U;
    ICU.GENBL0.BIT.EN4 = 0U;

    /* Disable RXI interrupt */
    IEN(SCI2,RXI2) = 0U;

    /* Clear interrupt flags */
    IR(SCI2,TXI2) = 0U;
    IR(SCI2,RXI2) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI2_SPI_Master_Send_Receive
* Description  : This function sends and receives SCI2 data to and from slave device
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

MD_STATUS R_Config_SCI2_SPI_Master_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci2_tx_count = tx_num;
        gp_sci2_tx_address = tx_buf;
        gp_sci2_rx_address = rx_buf;
        g_sci2_rx_count = 0U;
        g_sci2_rx_length = rx_num;

        /* Set TE, TIE, RE, RIE bits simultaneously */
        SCI2.SCR.BYTE |= 0xF0U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
