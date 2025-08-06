/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name    : r_sdhi_rx_pinset.c
* Version      : 1.0.2
* Device(s)    : R5F5671EHxLE
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_sdhi_rx_pinset.h"
#include "platform.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetInit
* Description  : This function initializes pins for r_sdhi_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetInit()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    PORT7.PMR.BIT.B7 = 0U;
    PORT7.DSCR.BIT.B7 = 1U;
    PORT7.PCR.BIT.B7 = 0U;
    PORT7.PODR.BIT.B7 = 0U;
    PORT7.PDR.BIT.B7 = 1U;
    MPC.P77PFS.BYTE = 0x00U;

    /* Set SDHI_CMD pin */
    PORT7.PMR.BIT.B6 = 0U;
    PORT7.DSCR.BIT.B6 = 1U;
    PORT7.PCR.BIT.B6 = 0U;
    PORT7.PODR.BIT.B6 = 0U;
    PORT7.PDR.BIT.B6 = 1U;
    MPC.P76PFS.BYTE = 0x00U;

    /* Set SDHI_CD pin */
    PORT8.PMR.BIT.B1 = 0U;
    PORT8.PCR.BIT.B1 = 0U;
    PORT8.PDR.BIT.B1 = 0U;
    MPC.P81PFS.BYTE = 0x1AU;
    PORT8.PMR.BIT.B1 = 1U;

    /* Set SDHI_D0 pin */
    PORTC.PMR.BIT.B3 = 0U;
    PORTC.DSCR.BIT.B3 = 1U;
    PORTC.PCR.BIT.B3 = 0U;
    PORTC.PODR.BIT.B3 = 0U;
    PORTC.PDR.BIT.B3 = 1U;
    MPC.PC3PFS.BYTE = 0x00U;

    /* Set SDHI_D1 pin */
    PORTC.PMR.BIT.B4 = 0U;
    PORTC.DSCR.BIT.B4 = 1U;
    PORTC.PCR.BIT.B4 = 0U;
    PORTC.PODR.BIT.B4 = 0U;
    PORTC.PDR.BIT.B4 = 1U;
    MPC.PC4PFS.BYTE = 0x00U;

    /* Set SDHI_D2 pin */
    PORT7.PMR.BIT.B5 = 0U;
    PORT7.DSCR.BIT.B5 = 1U;
    PORT7.PCR.BIT.B5 = 0U;
    PORT7.PODR.BIT.B5 = 0U;
    PORT7.PDR.BIT.B5 = 1U;
    MPC.P75PFS.BYTE = 0x00U;

    /* Set SDHI_D3 pin */
    PORTC.PMR.BIT.B2 = 0U;
    PORTC.DSCR.BIT.B2 = 1U;
    PORTC.PCR.BIT.B2 = 0U;
    PORTC.PODR.BIT.B2 = 0U;
    PORTC.PDR.BIT.B2 = 1U;
    MPC.PC2PFS.BYTE = 0x00U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}


/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetTransfer
* Description  : This function enables SD communication
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetTransfer()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    MPC.P77PFS.BYTE = 0x1AU;
    PORT7.PMR.BIT.B7 = 1U;

    /* Set SDHI_CMD pin */
    MPC.P76PFS.BYTE = 0x1AU;
    PORT7.PMR.BIT.B6 = 1U;

    /* Set SDHI_CD pin */
    MPC.P81PFS.BYTE = 0x1AU;
    PORT8.PMR.BIT.B1 = 1U;

    /* Set SDHI_D0 pin */
    MPC.PC3PFS.BYTE = 0x1AU;
    PORTC.PMR.BIT.B3 = 1U;

    /* Set SDHI_D1 pin */
    MPC.PC4PFS.BYTE = 0x1AU;
    PORTC.PMR.BIT.B4 = 1U;

    /* Set SDHI_D2 pin */
    MPC.P75PFS.BYTE = 0x1AU;
    PORT7.PMR.BIT.B5 = 1U;

    /* Set SDHI_D3 pin */
    MPC.PC2PFS.BYTE = 0x1AU;
    PORTC.PMR.BIT.B2 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetDetection
* Description  : This function disables SD communication and enables card detection and write protection
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetDetection()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    PORT7.PMR.BIT.B7 = 0U;
    MPC.P77PFS.BYTE = 0x00U;

    /* Set SDHI_CMD pin */
    PORT7.PMR.BIT.B6 = 0U;
    MPC.P76PFS.BYTE = 0x00U;

    /* Set SDHI_CD pin */
    MPC.P81PFS.BYTE = 0x1AU;
    PORT8.PMR.BIT.B1 = 1U;

    /* Set SDHI_D0 pin */
    PORTC.PMR.BIT.B3 = 0U;
    MPC.PC3PFS.BYTE = 0x00U;

    /* Set SDHI_D1 pin */
    PORTC.PMR.BIT.B4 = 0U;
    MPC.PC4PFS.BYTE = 0x00U;

    /* Set SDHI_D2 pin */
    PORT7.PMR.BIT.B5 = 0U;
    MPC.P75PFS.BYTE = 0x00U;

    /* Set SDHI_D3 pin */
    PORTC.PMR.BIT.B2 = 0U;
    MPC.PC2PFS.BYTE = 0x00U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

/***********************************************************************************************************************
* Function Name: R_SDHI_PinSetEnd
* Description  : This function ends pins for r_sdhi_rx module
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_SDHI_PinSetEnd()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set SDHI_CLK pin */
    PORT7.PMR.BIT.B7 = 0U;
    MPC.P77PFS.BYTE = 0x00U;

    /* Set SDHI_CMD pin */
    PORT7.PMR.BIT.B6 = 0U;
    MPC.P76PFS.BYTE = 0x00U;

    /* Set SDHI_CD pin */
    PORT8.PMR.BIT.B1 = 0U;
    MPC.P81PFS.BYTE = 0x00U;

    /* Set SDHI_D0 pin */
    PORTC.PMR.BIT.B3 = 0U;
    MPC.PC3PFS.BYTE = 0x00U;

    /* Set SDHI_D1 pin */
    PORTC.PMR.BIT.B4 = 0U;
    MPC.PC4PFS.BYTE = 0x00U;

    /* Set SDHI_D2 pin */
    PORT7.PMR.BIT.B5 = 0U;
    MPC.P75PFS.BYTE = 0x00U;

    /* Set SDHI_D3 pin */
    PORTC.PMR.BIT.B2 = 0U;
    MPC.PC2PFS.BYTE = 0x00U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

