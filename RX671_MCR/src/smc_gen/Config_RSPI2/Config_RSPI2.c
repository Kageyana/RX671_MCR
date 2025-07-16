/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_RSPI2.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_RSPI2.
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
#include "Config_RSPI2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint16_t * gp_rspi2_tx_address;              /* RSPI2 transmit buffer address */
volatile uint16_t g_rspi2_tx_count;                   /* RSPI2 transmit data number */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_RSPI2_Create
* Description  : This function initializes RSPI2
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RSPI2_Create(void)
{
    volatile uint8_t spcr_dummy;

    /* Disable RSPI interrupts */
    IEN(RSPI2,SPTI2) = 0U;
    EN(RSPI2,SPII2) = 0U;

    /* Cancel RSPI2 module stop state */
    MSTP(RSPI2) = 0U;

    /* Disable RSPI function */
    RSPI2.SPCR.BIT.SPE = 0U;

    /* Set control registers */
    RSPI2.SPPCR.BYTE = _00_RSPI_MOSI_FIXING_PREV_TRANSFER | _00_RSPI_LOOPBACK_DISABLED | _00_RSPI_LOOPBACK2_DISABLED;
    RSPI2.SPBR = _3B_RSPI2_DIVISOR;
    RSPI2.SPDCR.BYTE = _00_RSPI_ACCESS_WORD | _00_RSPI_FRAMES_1;
    RSPI2.SPCKD.BYTE = _00_RSPI_RSPCK_DELAY_1;
    RSPI2.SSLND.BYTE = _00_RSPI_SSL_NEGATION_DELAY_1;
    RSPI2.SPND.BYTE = _00_RSPI_NEXT_ACCESS_DELAY_1;
    RSPI2.SPCR2.BYTE = _00_RSPI_PARITY_DISABLE | _00_RSPI_AUTO_STOP_DISABLED;
    RSPI2.SPSCR.BYTE = _00_RSPI_SEQUENCE_LENGTH_1;
    RSPI2.SPCMD0.WORD = _0001_RSPI_RSPCK_SAMPLING_EVEN | _0000_RSPI_RSPCK_POLARITY_LOW | _000C_RSPI_BASE_BITRATE_8 | 
                        _0400_RSPI_DATA_LENGTH_BITS_8 | _0000_RSPI_MSB_FIRST | _0000_RSPI_NEXT_ACCESS_DELAY_DISABLE | 
                        _0000_RSPI_NEGATION_DELAY_DISABLE | _0000_RSPI_RSPCK_DELAY_DISABLE;
    RSPI2.SPDCR2.BYTE = _00_RSPI_BYTESWAP_DISABLED | _00_RSPI_DINV_DISABLED;

    /* Set SPTI2 priority level */
    IPR(RSPI2,SPTI2) = _0F_RSPI_PRIORITY_LEVEL15;

    /* Set SPCI2 interrupt priority level */
    IPR(RSPI2, SPCI2) = _0F_RSPI_PRIORITY_LEVEL15;

    /* Set RSPCKC pin */
    MPC.P56PFS.BYTE = 0x0DU;
    PORT5.ODR1.BYTE &= 0xEFU;
    PORT5.PMR.BYTE |= 0x40U;

    /* Set MOSIC pin */
    MPC.P54PFS.BYTE = 0x0DU;
    PORT5.ODR1.BYTE &= 0xFEU;
    PORT5.PMR.BYTE |= 0x10U;

    RSPI2.SPCR3.BYTE = _00_RSPI_RXMD_DISABLED | _00_RSPI_SCKDDIS_DISABLED | _10_RSPI_SPCIE_ENABLED;
    RSPI2.SPCR.BYTE = _01_RSPI_MODE_CLOCK_SYNCHRONOUS | _02_RSPI_TRANSMIT_ONLY | _08_RSPI_MASTER_MODE;
    spcr_dummy = RSPI2.SPCR.BYTE;

    R_Config_RSPI2_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_RSPI2_Start
* Description  : This function starts the RSPI2 module operation
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RSPI2_Start(void)
{
    volatile uint8_t dummy;

    /* Enable RSPI interrupts */
    IEN(RSPI2,SPTI2) = 1U;
    EN(RSPI2,SPII2) = 1U;
    IEN(RSPI2, SPCI2) = 1U;

    /* Clear error sources */
    dummy = RSPI2.SPSR.BYTE;
    RSPI2.SPSR.BYTE = 0xA0U;

    /* Disable idle interrupt */
    RSPI2.SPCR2.BIT.SPIIE = 0U;

    /* Disable communication end interrupt */
    RSPI2.SPCR3.BIT.SPCIE = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_RSPI2_Stop
* Description  : This function stops the RSPI2 module operation
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_RSPI2_Stop(void)
{
    /* Disable RSPI interrupts */
    IEN(RSPI2,SPTI2) = 0U;
    EN(RSPI2,SPII2) = 0U;
    IEN(RSPI2, SPCI2) = 0U;

    /* Disable RSPI function */
    RSPI2.SPCR.BIT.SPE = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_RSPI2_Send
* Description  : This function sends RSPI2 data
* Arguments    : tx_buf -
*                    transfer buffer pointer (not used when data is handled by DMAC/DTC)
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/

MD_STATUS R_Config_RSPI2_Send(uint16_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_rspi2_tx_address = tx_buf;
        g_rspi2_tx_count = tx_num;

        /* Enable transmit interrupt */
        RSPI2.SPCR.BIT.SPTIE = 1U;

        /* Enable RSPI function */
        RSPI2.SPCR.BIT.SPE = 1U;
    }

    return (status);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
