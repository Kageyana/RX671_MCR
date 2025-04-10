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
* File Name        : Config_MTU7.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_MTU7.
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
#include "Config_MTU7.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_MTU7_Create
* Description  : This function initializes the MTU7 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU7_Create(void)
{
    /* Release MTU channel 7 from stop state */
    MSTP(MTU7) = 0U;

    /* Enable read/write to MTU7 registers */
    MTU.TRWERB.BIT.RWE = 1U;

    /* Stop MTU channel 7 counter */
    MTU.TSTRB.BIT.CST7 = 0U;

    /* Set timer synchronous clear */
    MTU6.TSYCR.BYTE = _00_MTU6_CL0A_OFF | _00_MTU6_CL0B_OFF | _00_MTU6_CL0C_OFF | _00_MTU6_CL0D_OFF | 
                      _00_MTU6_CL1A_OFF | _00_MTU6_CL1B_OFF | _00_MTU6_CL2A_OFF | _00_MTU6_CL2B_OFF;

    /* MTU channel 7 is used as PWM mode 1 */
    MTU.TSYRB.BIT.SYNC7 = 0U;
    MTU.TOERB.BYTE |= (_D0_MTU_OE7C_ENABLE);
    MTU7.TCR.BYTE = _00_MTU_PCLK_1 | _20_MTU_CKCL_A;
    MTU7.TCR2.BYTE = _00_MTU_PCLK_1;
    MTU7.TIER.BYTE = _00_MTU_TGIEA_DISABLE | _00_MTU_TGIEB_DISABLE | _00_MTU_TGIEC_DISABLE | _00_MTU_TGIED_DISABLE | 
                     _00_MTU_TCIEV_DISABLE | _00_MTU_TTGE_DISABLE;
    MTU7.TMDR1.BYTE = _02_MTU_PWM1;
    MTU7.TIORH.BYTE = _00_MTU_IOA_DISABLE;
    MTU7.TIORL.BYTE = _01_MTU_IOC_LL | _70_MTU_IOD_HT;
    MTU7.TGRA = _0E0F_TGRA7_VALUE;
    MTU7.TGRB = _0064_TGRB7_VALUE;
    MTU7.TGRC = _0064_TGRC7_VALUE;
    MTU7.TGRD = _0064_TGRD7_VALUE;

    /* Disable read/write to MTU7 registers */
    MTU.TRWERB.BIT.RWE = 0U;

    /* Set MTIOC7C pin */
    MPC.P67PFS.BYTE = 0x08U;
    PORT6.PMR.BYTE |= 0x80U;

    R_Config_MTU7_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU7_Start
* Description  : This function starts the MTU7 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU7_Start(void)
{
    /* Start MTU channel 7 counter */
    MTU.TSTRB.BIT.CST7 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_MTU7_Stop
* Description  : This function stops the MTU7 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_MTU7_Stop(void)
{
    /* Stop MTU channel 7 counter */
    MTU.TSTRB.BIT.CST7 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
