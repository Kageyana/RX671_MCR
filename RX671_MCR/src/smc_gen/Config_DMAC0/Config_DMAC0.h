/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_DMAC0.h
* Component Version: 1.8.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_DMAC0.
***********************************************************************************************************************/

#ifndef CFG_Config_DMAC0_H
#define CFG_Config_DMAC0_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_dmac.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _A4_DMAC0_ACTIVATION_SOURCE                             (0xA4U) /* Activation source select */
#define _00000FCC_DMAC0_SRC_ADDR                                (0x00000FCCUL) /* Source address */
#define _0008816A_DMAC0_DST_ADDR                                (0x0008816AUL) /* Destination address */
#define _00000146_DMAC0_DMCRA_COUNT                             (0x00000146UL) /* Transfer count */
#define _0000_DMAC0_SRC_EXT_RPT_AREA                            (0x0000U) /* Source address extended repeat area */
#define _0000_DMAC0_DST_EXT_RPT_AREA                            (0x0000U) /* Destination address extended repeat area */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_DMAC0_Create(void);
void R_Config_DMAC0_Create_UserInit(void);
void R_Config_DMAC0_Start(void);
void R_Config_DMAC0_Stop(void);
static void r_dmac0_callback_transfer_end(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
