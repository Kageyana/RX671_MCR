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
* File Name        : r_cg_hardware_setup.c
* Version          : 1.0.50
* Device(s)        : R5F5671EHxLE
* Description      : Initialization file for code generation configurations.
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
#include "Config_SCI1.h"
#include "Config_MTU0.h"
#include "Config_MTU3.h"
#include "Config_MTU4.h"
#include "Config_MTU7.h"
#include "Config_MTU2.h"
#include "Config_S12AD0.h"
#include "Config_S12AD1.h"
#include "Config_DMAC0.h"
#include "Config_PORT.h"
#include "Config_ICU.h"
#include "Config_MTU1.h"
#include "Config_SCI0.h"
#include "Config_TPU5.h"
#include "Config_SCI10.h"
#include "Config_SCI2.h"
#include "Config_CMT0.h"
#include "r_smc_cgc.h"
#include "r_smc_interrupt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_undefined_exception
* Description  : This function is undefined interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if BSP_CFG_BOOTLOADER_PROJECT == 0
/* Disable the following function in the bootloader project. */
void r_undefined_exception(void)
{
    /* Start user code for r_undefined_exception. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

/***********************************************************************************************************************
* Function Name: R_Systeminit
* Description  : This function initializes every configuration
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Systeminit(void)
{
    /* Enable writing to registers related to operating modes, LPC, CGC and software reset */
    SYSTEM.PRCR.WORD = 0xA50BU;

    /* Enable writing to MPC pin function control registers */
    MPC.PWPR.BIT.B0WI = 0U;
    MPC.PWPR.BIT.PFSWE = 1U;

#if BSP_CFG_BOOTLOADER_PROJECT == 0
    /* Disable the following codes in the bootloader project. */
    /* Write 0 to the target bits in the POECR2 registers */
    POE3.POECR2.WORD = 0x0000U;
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

    /* Initialize clocks settings */
    R_CGC_Create();

    /* Set peripheral settings */
    R_Config_PORT_Create();
    R_Config_SCI1_Create();
    R_Config_MTU0_Create();
    R_Config_MTU3_Create();
    R_Config_MTU4_Create();
    R_Config_MTU7_Create();
    R_Config_MTU2_Create();
    R_Config_S12AD0_Create();
    R_Config_S12AD1_Create();
    R_Config_DMAC0_Create();
    R_Config_ICU_Create();
    R_Config_MTU1_Create();
    R_Config_SCI0_Create();
    R_Config_TPU5_Create();
    R_Config_SCI10_Create();
    R_Config_SCI2_Create();
    R_Config_CMT0_Create();

    /* Set interrupt settings */
    R_Interrupt_Create();

#if BSP_CFG_BOOTLOADER_PROJECT == 0
    /* Disable the following codes in the bootloader project. */
    /* Register undefined interrupt */
    R_BSP_InterruptWrite(BSP_INT_SRC_UNDEFINED_INTERRUPT,(bsp_int_cb_t)r_undefined_exception);
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

    /* Register group BL0 interrupt TEI0 (SCI0) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL0_SCI0_TEI0,(bsp_int_cb_t)r_Config_SCI0_transmitend_interrupt);

    /* Register group BL0 interrupt ERI0 (SCI0) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL0_SCI0_ERI0,(bsp_int_cb_t)r_Config_SCI0_receiveerror_interrupt);

    /* Register group BL0 interrupt TEI1 (SCI1) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL0_SCI1_TEI1,(bsp_int_cb_t)r_Config_SCI1_transmitend_interrupt);

    /* Register group BL0 interrupt ERI1 (SCI1) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL0_SCI1_ERI1,(bsp_int_cb_t)r_Config_SCI1_receiveerror_interrupt);

    /* Register group BL0 interrupt TEI2 (SCI2) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL0_SCI2_TEI2,(bsp_int_cb_t)r_Config_SCI2_transmitend_interrupt);

    /* Register group BL0 interrupt ERI2 (SCI2) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL0_SCI2_ERI2,(bsp_int_cb_t)r_Config_SCI2_receiveerror_interrupt);

    /* Register group AL0 interrupt TEI10 (SCI10) */
    R_BSP_InterruptWrite(BSP_INT_SRC_AL0_SCI10_TEI10,(bsp_int_cb_t)r_Config_SCI10_transmitend_interrupt);

    /* Register group AL0 interrupt ERI10 (SCI10) */
    R_BSP_InterruptWrite(BSP_INT_SRC_AL0_SCI10_ERI10,(bsp_int_cb_t)r_Config_SCI10_receiveerror_interrupt);

    /* Disable writing to MPC pin function control registers */
    MPC.PWPR.BIT.PFSWE = 0U;
    MPC.PWPR.BIT.B0WI = 1U;

    /* Enable protection */
    SYSTEM.PRCR.WORD = 0xA500U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

