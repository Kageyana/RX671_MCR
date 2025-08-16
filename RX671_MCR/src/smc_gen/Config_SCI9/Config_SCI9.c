/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI9.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_SCI9.
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
#include "Config_SCI9.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_sci9_tx_address;               /* SCI9 transmit buffer address */
volatile uint16_t  g_sci9_tx_count;                  /* SCI9 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI9_Create
* Description  : This function initializes SCI9
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI9_Create(void)
{
    /* Cancel SCI9 module stop state */
    MSTP(SCI9) = 0U;

    /* Set interrupt priority */
    IPR(SCI9,TXI9) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI9.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI9.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI9.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI9.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI9.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE | _00_SCI_CLOCK_PCLK;
    SCI9.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI9.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bit rate */
    SCI9.BRR = 0x01U;

    /* Set SMOSI9 pin */
    MPC.PB7PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x80U;

    /* Set SCK9 pin */
    MPC.PB5PFS.BYTE = 0x0AU;
    PORTB.PMR.BYTE |= 0x20U;

    R_Config_SCI9_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI9_Start
* Description  : This function starts SCI9
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI9_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI9,TXI9) = 0U;
    IEN(SCI9,TXI9) = 1U;
    ICU.GENBL1.BIT.EN26 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI9_Stop
* Description  : This function stops SCI9
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI9_Stop(void)
{
    /* Disable serial transmit and receive */
    SCI9.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI9,TXI9) = 0U;
    ICU.GENBL1.BIT.EN26 = 0U;

    /* Clear interrupt flags */
    IR(SCI9,TXI9) = 0U;
    IR(SCI9,RXI9) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI9_SPI_Master_Send
* Description  : This function sends SCI9 data to slave device
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC/DTC
*                tx_num -
*                    transfer buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI9_SPI_Master_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (1U > tx_num)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_sci9_tx_address = tx_buf;
        g_sci9_tx_count = tx_num;

        SCI9.SCR.BIT.TIE = 1U;
        SCI9.SCR.BIT.TE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
