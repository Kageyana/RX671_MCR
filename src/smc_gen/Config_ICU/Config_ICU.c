/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_ICU.c
* Component Version: 2.3.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_ICU.
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
#include "Config_ICU.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_ICU_Create
* Description  : This function initializes the ICU module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_Create(void)
{
    /* Disable IRQ interrupts */
    ICU.IER[0x08].BYTE = 0x00U;
    ICU.IER[0x09].BYTE = 0x00U;

    /* Disable software interrupt */
    IEN(ICU,SWINT) = 0U;
    IEN(ICU,SWINT2) = 0U;

    /* Disable IRQ digital filter */
    ICU.IRQFLTE0.BYTE &= ~(_02_ICU_IRQ1_FILTER_ENABLE);
    ICU.IRQFLTE1.BYTE &= ~(_04_ICU_IRQ10_FILTER_ENABLE | _40_ICU_IRQ14_FILTER_ENABLE);

    /* Set IRQ1 pin */
    MPC.P31PFS.BYTE = 0x40U;
    PORT3.PDR.BYTE &= 0xFDU;
    PORT3.PMR.BYTE &= 0xFDU;

    /* Set IRQ10 pin */
    MPC.P55PFS.BYTE = 0x40U;
    PORT5.PDR.BYTE &= 0xDFU;
    PORT5.PMR.BYTE &= 0xDFU;

    /* Set IRQ14 pin */
    MPC.PC0PFS.BYTE = 0x40U;
    PORTC.PDR.BYTE &= 0xFEU;
    PORTC.PMR.BYTE &= 0xFEU;

    /* Set IRQ detection type */
    ICU.IRQCR[1].BYTE = _00_ICU_IRQ_EDGE_LOW_LEVEL;
    ICU.IRQCR[10].BYTE = _04_ICU_IRQ_EDGE_FALLING;
    IR(ICU,IRQ10) = 0U;
    ICU.IRQCR[14].BYTE = _00_ICU_IRQ_EDGE_LOW_LEVEL;

    /* Set IRQ priority level */
    IPR(ICU,IRQ1) = _0F_ICU_PRIORITY_LEVEL15;
    IPR(ICU,IRQ10) = _0F_ICU_PRIORITY_LEVEL15;
    IPR(ICU,IRQ14) = _0F_ICU_PRIORITY_LEVEL15;

    R_Config_ICU_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ1_Start
* Description  : This function enables IRQ1 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ1_Start(void)
{
    /* Enable IRQ1 interrupt */
    IEN(ICU,IRQ1) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ1_Stop
* Description  : This function disables IRQ1 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ1_Stop(void)
{
    /* Disable IRQ1 interrupt */
    IEN(ICU,IRQ1) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ10_Start
* Description  : This function enables IRQ10 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ10_Start(void)
{
    /* Enable IRQ10 interrupt */
    IEN(ICU,IRQ10) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ10_Stop
* Description  : This function disables IRQ10 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ10_Stop(void)
{
    /* Disable IRQ10 interrupt */
    IEN(ICU,IRQ10) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ14_Start
* Description  : This function enables IRQ14 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ14_Start(void)
{
    /* Enable IRQ14 interrupt */
    IEN(ICU,IRQ14) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_ICU_IRQ14_Stop
* Description  : This function disables IRQ14 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_ICU_IRQ14_Stop(void)
{
    /* Disable IRQ14 interrupt */
    IEN(ICU,IRQ14) = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
