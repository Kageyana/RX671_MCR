/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI4.c
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
volatile uint8_t * gp_sci4_tx_address;               /* SCI4 transmit buffer address */
volatile uint16_t  g_sci4_tx_count;                  /* SCI4 transmit data number */
volatile uint8_t * gp_sci4_rx_address;               /* SCI4 receive buffer address */
volatile uint16_t  g_sci4_rx_count;                  /* SCI4 receive data number */
volatile uint16_t  g_sci4_rx_length;                 /* SCI4 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI4_Create
* Description  : This function initializes SCI4
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI4_Create(void)
{
    /* Cancel SCI4 module stop state */
    MSTP(SCI4) = 0U;

    /* Set interrupt priority */
    IPR(SCI4,TXI4) = _0F_SCI_PRIORITY_LEVEL15;
    IPR(SCI4,RXI4) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI4.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI4.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI4.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI4.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI4.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE | _00_SCI_CLOCK_PCLK;
    SCI4.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _00_SCI_DATA_LSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI4.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bit rate */
    SCI4.BRR = 0x0EU;

    /* Set SMISO4 pin */
    MPC.PB0PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x01U;

    /* Set SMOSI4 pin */
    MPC.PB1PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x02U;

    /* Set SCK4 pin */
    MPC.PB3PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x08U;

    R_Config_SCI4_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI4_Start
* Description  : This function starts SCI4
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI4_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI4,TXI4) = 0U;
    IEN(SCI4,TXI4) = 1U;
    ICU.GENBL0.BIT.EN8 = 1U;

    /* Enable RXI interrupt */
    IR(SCI4,RXI4) = 0U;
    IEN(SCI4,RXI4) = 1U;

    /* Enable ERI interrupt */
    ICU.GENBL0.BIT.EN9 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI4_Stop
* Description  : This function stops SCI4
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI4_Stop(void)
{
    /* Disable serial transmit and receive */
    SCI4.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI4,TXI4) = 0U;
    ICU.GENBL0.BIT.EN8 = 0U;

    /* Disable RXI interrupt */
    IEN(SCI4,RXI4) = 0U;

    /* Disable ERI interrupt */
    ICU.GENBL0.BIT.EN9 = 0U;

    /* Clear interrupt flags */
    IR(SCI4,TXI4) = 0U;
    IR(SCI4,RXI4) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI4_SPI_Master_Send_Receive
* Description  : This function sends and receives SCI4 data to and from slave device
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

MD_STATUS R_Config_SCI4_SPI_Master_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf, uint16_t rx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_sci4_tx_count = tx_num;
        gp_sci4_tx_address = tx_buf;
        gp_sci4_rx_address = rx_buf;
        g_sci4_rx_count = 0U;
        g_sci4_rx_length = rx_num;

        /* Set TE, TIE, RE, RIE bits simultaneously */
        SCI4.SCR.BYTE |= 0xF0U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
