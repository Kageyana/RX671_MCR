/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_MTU0.h
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_MTU0.
***********************************************************************************************************************/

#ifndef CFG_Config_MTU0_H
#define CFG_Config_MTU0_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_mtu3.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define MTU0_PCLK_COUNTER_DIVISION      (1)
#define _0E0F_TGRA0_VALUE               (0x0E0FU) /* TGRA0 value */
#define _0064_TGRB0_VALUE               (0x0064U) /* TGRB0 value */
#define _0064_TGRC0_VALUE               (0x0064U) /* TGRC0 value */
#define _0E0F_TGRD0_VALUE               (0x0E0FU) /* TGRD0 value */
#define _0064_TGRE0_VALUE               (0x0064U) /* TGRE0 value */
#define _0E0F_TGRF0_VALUE               (0x0E0FU) /* TGRF0 value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_MTU0_Create(void);
void R_Config_MTU0_Create_UserInit(void);
void R_Config_MTU0_Start(void);
void R_Config_MTU0_Stop(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
