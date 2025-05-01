/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_SCI1.h
* Component Version: 1.12.0
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_SCI1.
***********************************************************************************************************************/

#ifndef CFG_Config_SCI1_H
#define CFG_Config_SCI1_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_sci.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_SCI1_Create(void);
void R_Config_SCI1_Create_UserInit(void);
void R_Config_SCI1_Start(void);
void R_Config_SCI1_Stop(void);
MD_STATUS R_Config_SCI1_Serial_Send(uint8_t * const tx_buf, uint16_t tx_num);
MD_STATUS R_Config_SCI1_Serial_Receive(uint8_t * const rx_buf, uint16_t rx_num);
void r_Config_SCI1_transmitend_interrupt(void);
static void r_Config_SCI1_callback_transmitend(void);
static void r_Config_SCI1_callback_receiveend(void);
void r_Config_SCI1_receiveerror_interrupt(void);
static void r_Config_SCI1_callback_receiveerror(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
