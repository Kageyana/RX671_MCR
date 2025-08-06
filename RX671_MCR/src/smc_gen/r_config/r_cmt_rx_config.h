/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
* Copyright (c) 2013 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_cmt_rx_config.h
* Description  : Configures the r_cmt_rx code.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.11.2013 2.00    First GSCE Release.
*         : 25.11.2019 4.30    Changed for FIT+RTOS
*         : 31.08.2020 4.70    Added condition for _RI_TRACE_TIMER macro
*                              Fixed warning when using RI600V4 with device has 2 CMT channels
*         : 27.12.2022 5.40    Updated macro definition enable and disable nested interrupt for CMT.
*         : 15.03.2025 5.71    Updated disclaimer.
***********************************************************************************************************************/
#ifndef CMT_CONFIG_HEADER_FILE
#define CMT_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* The interrupt priority level to be used for CMT interrupts. */
#define CMT_RX_CFG_IPR         (5)

/* SPECIFY WHETHER TO INCLUDE CODE FOR NESTED INTERRUPT CMT CHANNEL 0 */
/* 1=included, 0=not */
#define CMT_CFG_CH0_EN_NESTED_INT    (1)

/* SPECIFY WHETHER TO INCLUDE CODE FOR NESTED INTERRUPT CMT CHANNEL 1 */
/* 1=included, 0=not */
#define CMT_CFG_CH1_EN_NESTED_INT    (0)

/* SPECIFY WHETHER TO INCLUDE CODE FOR NESTED INTERRUPT CMT CHANNEL 2 */
/* 1=included, 0=not */
#define CMT_CFG_CH2_EN_NESTED_INT    (0)

/* SPECIFY WHETHER TO INCLUDE CODE FOR NESTED INTERRUPT CMT CHANNEL 3 */
/* 1=included, 0=not */
#define CMT_CFG_CH3_EN_NESTED_INT    (0)

#if (BSP_CFG_RTOS_USED == 4) && (BSP_CFG_RENESAS_RTOS_USED == 0) /* RI600V4 */
#define _RI_TRACE_TIMER 1 /* RI600V4 uses CMT1 channel for the trace feature.*/
#elif (BSP_CFG_RTOS_USED == 4) && (BSP_CFG_RENESAS_RTOS_USED == 1) /* RI600PX */
#include "r_bsp_config.h"
#define _RI_TRACE_TIMER BSP_CFG_RTOS_SYSTEM_TIMER /* RI600PX does not actually have the trace feature.*/
#else
#define _RI_TRACE_TIMER 1 /* It’s guaranteed that definition _RI_TRACE_TIMER is never used unless BSP_CFG_RTOS_USED == 4.*/
#endif

#endif /* CMT_CONFIG_HEADER_FILE */


