/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name    : r_sdhi_rx_pinset.h
* Version      : 1.0.2
* Device(s)    : R5F5671EHxLE
* Tool-Chain   : RXC toolchain
* Description  : Setting of port and mpc registers
***********************************************************************************************************************/

#ifndef R_SDHI_RX_H
#define R_SDHI_RX_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

void R_SDHI_PinSetInit();
void R_SDHI_PinSetTransfer();
void R_SDHI_PinSetDetection();
void R_SDHI_PinSetEnd();

#endif
