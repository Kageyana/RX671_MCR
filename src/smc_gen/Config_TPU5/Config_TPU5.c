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
* File Name        : Config_TPU5.c
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_TPU5.
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
#include "Config_TPU5.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU5_Create
* Description  : This function initializes the TPU5 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU5_Create(void)
{
    /* Release TPU channel 5 from stop state */
    MSTP(TPU5) = 0U;

    /* Stop TPU channel 5 counter */
    TPUA.TSTR.BIT.CST5 = 0U;

    /* TPU channel 5 is used as PWM mode 1 */
    TPUA.TSYR.BIT.SYNC5 = 0U;
    TPU5.TCR.BYTE = _00_TPU_PCLK_1 | _20_TPU_CKCL_A;
    TPU5.TIER.BYTE |= (_00_TPU_TGIEA_DISABLE | _00_TPU_TGIEB_DISABLE | _00_TPU_TCIEV_DISABLE);
    TPU5.TIOR.BYTE = _01_TPU_IOA_LL | _70_TPU_IOB_HT;
    TPU5.TGRA = _176F_TGRA5_VALUE;
    TPU5.TGRB = _0064_TGRB5_VALUE;
    TPU5.TMDR.BYTE = _02_TPU_PWM1;

    /* Set TIOCA5 pin */
    MPC.PB6PFS.BYTE = 0x03U;
    PORTB.PMR.BYTE |= 0x40U;

    R_Config_TPU5_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU5_Start
* Description  : This function starts the TPU5 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU5_Start(void)
{
    /* Start TPU channel 5 counter */
    TPUA.TSTR.BIT.CST5 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU5_Stop
* Description  : This function stops the TPU5 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU5_Stop(void)
{
    /* Stop TPU channel 5 counter */
    TPUA.TSTR.BIT.CST5 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
