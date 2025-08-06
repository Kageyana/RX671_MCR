/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_S12AD1.c
* Component Version: 1.13.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_S12AD1.
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
#include "Config_S12AD1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Create
* Description  : This function initializes the S12AD1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Create(void)
{
    /* Cancel S12AD1 module stop state */
    MSTP(S12AD1) = 0U;

    /* Disable and clear interrupt flags of S12AD1 module */
    S12AD1.ADCSR.BIT.ADIE = 0U;
    S12AD1.ADCMPCR.BIT.CMPAIE = 0U;
    S12AD1.ADCMPCR.BIT.CMPBIE = 0U;
    IEN(PERIB, INTB186) = 0U;

    /* Set S12AD1 control registers */
    S12AD1.ADCSR.WORD = _0000_AD_SYNCASYNCTRG_DISABLE | _4000_AD_CONTINUOUS_SCAN_MODE;
    S12AD1.ADDISCR.BYTE = _00_AD_DISCONECT_UNUSED;
    S12AD1.ADADC.BYTE = _03_AD_4_TIME_CONVERSION | _80_AD_AVERAGE_MODE;

    /* Set channels and sampling time */
    S12AD1.ADSSTR0 = _3C_AD1_SAMPLING_STATE_0;
    S12AD1.ADSSTR1 = _3C_AD1_SAMPLING_STATE_1;
    S12AD1.ADSSTR3 = _3C_AD1_SAMPLING_STATE_3;
    S12AD1.ADSSTR5 = _3C_AD1_SAMPLING_STATE_5;
    S12AD1.ADSSTR6 = _3C_AD1_SAMPLING_STATE_6;
    S12AD1.ADSSTR7 = _3C_AD1_SAMPLING_STATE_7;
    S12AD1.ADSSTR9 = _3C_AD1_SAMPLING_STATE_9;
    S12AD1.ADSSTR10 = _3C_AD1_SAMPLING_STATE_10;
    S12AD1.ADSSTR11 = _3C_AD1_SAMPLING_STATE_11;
    S12AD1.ADANSA0.WORD = _0001_AD_ANx00_USED | _0002_AD_ANx01_USED | _0008_AD_ANx03_USED | _0020_AD_ANx05_USED | 
                          _0040_AD_ANx06_USED | _0080_AD_ANx07_USED | _0200_AD_AN109_USED | _0400_AD_AN110_USED | 
                          _0800_AD_AN111_USED;
    S12AD1.ADADS0.WORD = _0001_AD_ANx00_ADD_USED | _0002_AD_ANx01_ADD_USED | _0008_AD_ANx03_ADD_USED | 
                         _0020_AD_ANx05_ADD_USED | _0040_AD_ANx06_ADD_USED | _0080_AD_ANx07_ADD_USED | 
                         _0200_AD_AN109_ADD_USED | _0400_AD_AN110_ADD_USED | _0800_AD_AN111_ADD_USED;
    S12AD1.ADCER.WORD = _0000_AD_RESOLUTION_12BIT | _0000_AD_AUTO_CLEARING_DISABLE | _0000_AD_SELFTDIAGST_DISABLE | 
                        _0000_AD_RIGHT_ALIGNMENT;
    S12AD1.ADCSR.WORD |= _1000_AD_SCAN_END_INTERRUPT_ENABLE;

    /* Set compare control register */
    S12AD1.ADCMPCR.WORD = _0000_AD_WINDOWB_DISABLE | _0000_AD_WINDOWA_DISABLE | _0000_AD_WINDOWFUNCTION_DISABLE;

    /* Set interrupt and priority level */
    ICU.SLIBR186.BYTE = 0x44U;
    IPR(PERIB, INTB186) = _01_AD_PRIORITY_LEVEL1;

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

    R_Config_S12AD1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Start
* Description  : This function starts the AD1 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Start(void)
{
    IR(PERIB, INTB186) = 0U;
    IEN(PERIB, INTB186) = 1U;
    S12AD1.ADCSR.BIT.ADST = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Stop
* Description  : This function stops the AD1 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Stop(void)
{
    S12AD1.ADCSR.BIT.ADST = 0U;
    IEN(PERIB, INTB186) = 0U;
    IR(PERIB, INTB186) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD1_Get_ValueResult
* Description  : This function gets result from the AD1 converter
* Arguments    : channel -
*                    channel of data register to be read
*                buffer -
*                    buffer pointer
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD1_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer)
{
    switch (channel)
    {
        case ADSELFDIAGNOSIS:
        {
            *buffer = (uint16_t)(S12AD1.ADRD.WORD);
            break;
        }
        case ADCHANNEL0:
        {
            *buffer = (uint16_t)(S12AD1.ADDR0);
            break;
        }
        case ADCHANNEL1:
        {
            *buffer = (uint16_t)(S12AD1.ADDR1);
            break;
        }
        case ADCHANNEL2:
        {
            *buffer = (uint16_t)(S12AD1.ADDR2);
            break;
        }
        case ADCHANNEL3:
        {
            *buffer = (uint16_t)(S12AD1.ADDR3);
            break;
        }
        case ADCHANNEL4:
        {
            *buffer = (uint16_t)(S12AD1.ADDR4);
            break;
        }
        case ADCHANNEL5:
        {
            *buffer = (uint16_t)(S12AD1.ADDR5);
            break;
        }
        case ADCHANNEL6:
        {
            *buffer = (uint16_t)(S12AD1.ADDR6);
            break;
        }
        case ADCHANNEL7:
        {
            *buffer = (uint16_t)(S12AD1.ADDR7);
            break;
        }
        case ADCHANNEL8:
        {
            *buffer = (uint16_t)(S12AD1.ADDR8);
            break;
        }
        case ADCHANNEL9:
        {
            *buffer = (uint16_t)(S12AD1.ADDR9);
            break;
        }
        case ADCHANNEL10:
        {
            *buffer = (uint16_t)(S12AD1.ADDR10);
            break;
        }
        case ADCHANNEL11:
        {
            *buffer = (uint16_t)(S12AD1.ADDR11);
            break;
        }
        case ADTEMPSENSOR:
        {
            *buffer = (uint16_t)(S12AD1.ADTSDR);
            break;
        }
        case ADINTERREFVOLT:
        {
            *buffer = (uint16_t)(S12AD1.ADOCDR);
            break;
        }
        default:
        {
            break;
        }
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
