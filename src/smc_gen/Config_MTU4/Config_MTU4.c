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
* File Name        : Config_MTU4.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_MTU4.
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
#include "Config_MTU4.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_MTU4_Create
* Description  : This function initializes the MTU4 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU4_Create(void)
{
    /* Release MTU channel 4 from stop state */
    MSTP(MTU4) = 0U;

    /* Enable read/write to MTU4 registers */
    MTU.TRWERA.BIT.RWE = 1U;

    /* Stop MTU channel 4 counter */
    MTU.TSTRA.BIT.CST4 = 0U;

    /* MTU channel 4 is used as PWM mode 1 */
    MTU.TSYRA.BIT.SYNC4 = 0U;
    MTU.TOERA.BYTE |= (_C2_MTU_OE4A_ENABLE | _D0_MTU_OE4C_ENABLE);
    MTU4.TCR.BYTE = _00_MTU_PCLK_1 | _20_MTU_CKCL_A;
    MTU4.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU4.TIER.BYTE = _00_MTU_TGIEA_DISABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU4.TMDR1.BYTE = _02_MTU_PWM1;
    MTU4.TIORH.BYTE = _01_MTU_IOA_LL | _70_MTU_IOB_HT;
    MTU4.TIORL.BYTE = _01_MTU_IOC_LL | _70_MTU_IOD_HT;
    MTU4.TGRA = _0E0F_TGRA4_VALUE;
    MTU4.TGRB = _0064_TGRB4_VALUE;
    MTU4.TGRC = _0064_TGRC4_VALUE;
    MTU4.TGRD = _0064_TGRD4_VALUE;

    /* Disable read/write to MTU4 registers */
    MTU.TRWERA.BIT.RWE = 0U;

    /* Set MTIOC4A pin */
    MPC.PA0PFS.BYTE = 0x01U;
    PORTA.PMR.BYTE |= 0x01U;

    /* Set MTIOC4C pin */
    MPC.PB1PFS.BYTE = 0x02U;
    PORTB.PMR.BYTE |= 0x02U;

    R_Config_MTU4_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU4_Start
* Description  : This function starts the MTU4 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU4_Start(void)
{
    /* Start MTU channel 4 counter */
    MTU.TSTRA.BIT.CST4 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU4_Stop
* Description  : This function stops the MTU4 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU4_Stop(void)
{
    /* Stop MTU channel 4 counter */
    MTU.TSTRA.BIT.CST4 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
