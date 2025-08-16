/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_RSPI2.h
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_RSPI2.
***********************************************************************************************************************/

#ifndef CFG_Config_RSPI2_H
#define CFG_Config_RSPI2_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_rspi.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _00_RSPI2_DIVISOR               (0x00U) /* SPBR(RSPI bit rate) register value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_RSPI2_Create(void);
void R_Config_RSPI2_Create_UserInit(void);
void R_Config_RSPI2_Start(void);
void R_Config_RSPI2_Stop(void);
MD_STATUS R_Config_RSPI2_Send(uint16_t * const tx_buf, uint16_t tx_num);
void r_Config_RSPI2_idle_interrupt(void);
static void r_Config_RSPI2_callback_transmitend(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
