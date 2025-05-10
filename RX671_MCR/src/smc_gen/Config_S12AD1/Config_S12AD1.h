/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_S12AD1.h
* Component Version: 1.13.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_S12AD1.
***********************************************************************************************************************/

#ifndef CFG_Config_S12AD1_H
#define CFG_Config_S12AD1_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_s12ad.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _0B_AD1_SAMPLING_STATE_0           (0x0BU)   /* ANx00 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_1           (0x0BU)   /* ANx01 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_3           (0x0BU)   /* ANx03 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_5           (0x0BU)   /* ANx05 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_6           (0x0BU)   /* ANx06 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_7           (0x0BU)   /* ANx07 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_9           (0x0BU)   /* ANx09 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_10          (0x0BU)   /* ANx10 sampling time setting */
#define _0B_AD1_SAMPLING_STATE_11          (0x0BU)   /* ANx11 sampling time setting */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_S12AD1_Create(void);
void R_Config_S12AD1_Create_UserInit(void);
void R_Config_S12AD1_Start(void);
void R_Config_S12AD1_Stop(void);
void R_Config_S12AD1_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
