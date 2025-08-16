/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI0.c
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
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI0_Create
* Description  : This function initializes SCI0
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI0_Create(void)
{
    /* Cancel SCI0 module stop state */
    MSTP(SCI0) = 0U;

    /* Set interrupt priority */
    IPR(SCI0,TXI0) = _0F_SCI_PRIORITY_LEVEL15;

    /* Clear the control register */
    SCI0.SCR.BYTE = 0x00U;

    /* Set clock enable */
    SCI0.SCR.BYTE |= _01_SCI_INTERNAL_SCK_OUTPUT;

    /* Clear the SIMR1.IICM */
    SCI0.SIMR1.BIT.IICM = 0U;

    /* Set control registers */
    SCI0.SPMR.BYTE = _00_SCI_SS_PIN_DISABLE | _00_SCI_SPI_MASTER | _00_SCI_CLOCK_NOT_INVERTED | 
                     _00_SCI_CLOCK_NOT_DELAYED;
    SCI0.SMR.BYTE = _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE | _00_SCI_CLOCK_PCLK;
    SCI0.SCMR.BYTE = _00_SCI_SERIAL_MODE | _00_SCI_DATA_INVERT_NONE | _08_SCI_DATA_MSB_FIRST | 
                     _10_SCI_DATA_LENGTH_8_OR_7 | _62_SCI_SCMR_DEFAULT;
    SCI0.SEMR.BYTE = _00_SCI_BIT_MODULATION_DISABLE;

    /* Set bit rate */
    SCI0.BRR = 0x01U;

    /* Set SMOSI0 pin */
    MPC.P20PFS.BYTE = 0x0AU;
    PORT2.PMR.BYTE |= 0x01U;

    /* Set SCK0 pin */
    MPC.P22PFS.BYTE = 0x0AU;
    PORT2.PMR.BYTE |= 0x04U;

    R_Config_SCI0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI0_Start
* Description  : This function starts SCI0
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI0_Start(void)
{
    /* Enable TXI and TEI interrupt */
    IR(SCI0,TXI0) = 0U;
    IEN(SCI0,TXI0) = 1U;
    ICU.GENBL0.BIT.EN0 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI0_Stop
* Description  : This function stops SCI0
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI0_Stop(void)
{
    /* Disable serial transmit and receive */
    SCI0.SCR.BYTE &= 0xCFU;

    /* Disable TXI and TEI interrupt */
    IEN(SCI0,TXI0) = 0U;
    ICU.GENBL0.BIT.EN0 = 0U;

    /* Clear interrupt flags */
    IR(SCI0,TXI0) = 0U;
    IR(SCI0,RXI0) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_SCI0_SPI_Master_Send
* Description  : This function sends SCI0 data to slave device
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC/DTC
*                tx_num -
*                    transfer buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_SCI0_SPI_Master_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    SCI0.SCR.BIT.TIE = 1U;
    SCI0.SCR.BIT.TE = 1U;

    return MD_OK;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
