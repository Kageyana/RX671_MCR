/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Pin.c
* Version          : 1.0.2
* Device(s)        : R5F5671EHxLE
* Description      : This file implements SMC pin code generation.
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
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Pins_Create
* Description  : This function initializes Smart Configurator pins
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Pins_Create(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set AN001 pin */
    PORT4.PMR.BYTE &= 0xFDU;
    PORT4.PDR.BYTE &= 0xFDU;
    MPC.P41PFS.BYTE = 0x80U;

    /* Set AN002 pin */
    PORT4.PMR.BYTE &= 0xFBU;
    PORT4.PDR.BYTE &= 0xFBU;
    MPC.P42PFS.BYTE = 0x80U;

    /* Set AN003 pin */
    PORT4.PMR.BYTE &= 0xF7U;
    PORT4.PDR.BYTE &= 0xF7U;
    MPC.P43PFS.BYTE = 0x80U;

    /* Set AN004 pin */
    PORT4.PMR.BYTE &= 0xEFU;
    PORT4.PDR.BYTE &= 0xEFU;
    MPC.P44PFS.BYTE = 0x80U;

    /* Set AN005 pin */
    PORT4.PMR.BYTE &= 0xDFU;
    PORT4.PDR.BYTE &= 0xDFU;
    MPC.P45PFS.BYTE = 0x80U;

    /* Set AN006 pin */
    PORT4.PMR.BYTE &= 0xBFU;
    PORT4.PDR.BYTE &= 0xBFU;
    MPC.P46PFS.BYTE = 0x80U;

    /* Set AN007 pin */
    PORT4.PMR.BYTE &= 0x7FU;
    PORT4.PDR.BYTE &= 0x7FU;
    MPC.P47PFS.BYTE = 0x80U;

    /* Set AN100 pin */
    PORTD.PMR.BYTE &= 0x7FU;
    PORTD.PDR.BYTE &= 0x7FU;
    MPC.PD7PFS.BYTE = 0x80U;

    /* Set AN101 pin */
    PORTD.PMR.BYTE &= 0xBFU;
    PORTD.PDR.BYTE &= 0xBFU;
    MPC.PD6PFS.BYTE = 0x80U;

    /* Set AN103 pin */
    PORTD.PMR.BYTE &= 0xEFU;
    PORTD.PDR.BYTE &= 0xEFU;
    MPC.PD4PFS.BYTE = 0x80U;

    /* Set AN105 pin */
    PORTD.PMR.BYTE &= 0xFBU;
    PORTD.PDR.BYTE &= 0xFBU;
    MPC.PD2PFS.BYTE = 0x80U;

    /* Set AN106 pin */
    PORTD.PMR.BYTE &= 0xFDU;
    PORTD.PDR.BYTE &= 0xFDU;
    MPC.PD1PFS.BYTE = 0x80U;

    /* Set AN107 pin */
    PORTD.PMR.BYTE &= 0xFEU;
    PORTD.PDR.BYTE &= 0xFEU;
    MPC.PD0PFS.BYTE = 0x80U;

    /* Set AN109 pin */
    PORT0.PMR.BYTE &= 0xFBU;
    PORT0.PDR.BYTE &= 0xFBU;
    MPC.P02PFS.BYTE = 0x80U;

    /* Set AN110 pin */
    PORT0.PMR.BYTE &= 0xFDU;
    PORT0.PDR.BYTE &= 0xFDU;
    MPC.P01PFS.BYTE = 0x80U;

    /* Set AN111 pin */
    PORT0.PMR.BYTE &= 0xFEU;
    PORT0.PDR.BYTE &= 0xFEU;
    MPC.P00PFS.BYTE = 0x80U;

    /* Set EXTAL pin */
    PORT3.PMR.BYTE &= 0xBFU;
    PORT3.PDR.BYTE &= 0xBFU;

    /* Set IRQ1 pin */
    MPC.P31PFS.BYTE = 0x40U;
    PORT3.PMR.BYTE &= 0xFDU;
    PORT3.PDR.BYTE &= 0xFDU;

    /* Set IRQ10 pin */
    MPC.P55PFS.BYTE = 0x40U;
    PORT5.PMR.BYTE &= 0xDFU;
    PORT5.PDR.BYTE &= 0xDFU;

    /* Set IRQ14 pin */
    MPC.PC0PFS.BYTE = 0x40U;
    PORTC.PMR.BYTE &= 0xFEU;
    PORTC.PDR.BYTE &= 0xFEU;

    /* Set MTCLKA pin */
    MPC.PA4PFS.BYTE = 0x02U;
    PORTA.PMR.BYTE |= 0x10U;

    /* Set MTCLKB pin */
    MPC.PA6PFS.BYTE = 0x02U;
    PORTA.PMR.BYTE |= 0x40U;

    /* Set MTCLKC pin */
    MPC.PA1PFS.BYTE = 0x02U;
    PORTA.PMR.BYTE |= 0x02U;

    /* Set MTCLKD pin */
    MPC.PA3PFS.BYTE = 0x02U;
    PORTA.PMR.BYTE |= 0x08U;

    /* Set MTIOC0A pin */
    MPC.P34PFS.BYTE = 0x01U;
    PORT3.PMR.BYTE |= 0x10U;

    /* Set MTIOC0C pin */
    MPC.P32PFS.BYTE = 0x01U;
    PORT3.PMR.BYTE |= 0x04U;

    /* Set MTIOC2A pin */
    MPC.PB5PFS.BYTE = 0x01U;
    PORTB.PMR.BYTE |= 0x20U;

    /* Set MTIOC3A pin */
    MPC.PC1PFS.BYTE = 0x01U;
    PORTC.PMR.BYTE |= 0x02U;

    /* Set MTIOC3C pin */
    MPC.PJ3PFS.BYTE = 0x01U;
    PORTJ.PMR.BYTE |= 0x08U;

    /* Set MTIOC4A pin */
    MPC.PA0PFS.BYTE = 0x01U;
    PORTA.PMR.BYTE |= 0x01U;

    /* Set MTIOC4C pin */
    MPC.PB1PFS.BYTE = 0x02U;
    PORTB.PMR.BYTE |= 0x02U;

    /* Set MTIOC7C pin */
    MPC.P67PFS.BYTE = 0x08U;
    PORT6.PMR.BYTE |= 0x80U;

    /* Set RXD1 pin */
    MPC.P30PFS.BYTE = 0x0AU;
    PORT3.PMR.BYTE |= 0x01U;

    /* Set RXD10 pin */
    MPC.PC6PFS.BYTE = 0x24U;
    PORTC.PMR.BYTE |= 0x40U;

    /* Set SCK0 pin */
    MPC.P22PFS.BYTE = 0x0AU;
    PORT2.PMR.BYTE |= 0x04U;

    /* Set SCK2 pin */
    MPC.P51PFS.BYTE = 0x0AU;
    PORT5.PMR.BYTE |= 0x02U;

    /* Set SDCLK pin */
    PORT7.PMR.BYTE |= 0x01U;

    /* Set SMISO0 pin */
    MPC.P21PFS.BYTE = 0x0AU;
    PORT2.PMR.BYTE |= 0x02U;

    /* Set SMISO2 pin */
    MPC.P52PFS.BYTE = 0x0AU;
    PORT5.PMR.BYTE |= 0x04U;

    /* Set SMOSI0 pin */
    MPC.P20PFS.BYTE = 0x0AU;
    PORT2.PMR.BYTE |= 0x01U;

    /* Set SMOSI2 pin */
    MPC.P50PFS.BYTE = 0x0AU;
    PORT5.PMR.BYTE |= 0x01U;

    /* Set TIOCA5 pin */
    MPC.PB6PFS.BYTE = 0x03U;
    PORTB.PMR.BYTE |= 0x40U;

    /* Set TXD1 pin */
    PORT2.PODR.BYTE |= 0x40U;
    MPC.P26PFS.BYTE = 0x0AU;
    PORT2.PDR.BYTE |= 0x40U;
    // PORT2.PMR.BIT.B6 = 1U; // Please set the PMR bit after TE bit is set to 1.

    /* Set TXD10 pin */
    PORT8.PODR.BYTE |= 0x04U;
    MPC.P82PFS.BYTE = 0x0AU;
    PORT8.PDR.BYTE |= 0x04U;
    // PORT8.PMR.BIT.B2 = 1U; // Please set the PMR bit after TE bit is set to 1.

    /* Set USB0_DM pin */
    PORTH.PMR.BYTE &= 0xFBU;
    PORTH.PDR.BYTE &= 0xFBU;

    /* Set USB0_DP pin */
    PORTH.PMR.BYTE &= 0xFDU;
    PORTH.PDR.BYTE &= 0xFDU;

    /* Set USB0_VBUS pin */
    MPC.P16PFS.BYTE = 0x11U;
    PORT1.PMR.BYTE |= 0x40U;

    /* Set XTAL pin */
    PORT3.PMR.BYTE &= 0x7FU;
    PORT3.PDR.BYTE &= 0x7FU;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}

