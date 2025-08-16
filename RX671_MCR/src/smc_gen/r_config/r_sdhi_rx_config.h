/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
* Copyright (c) 2017 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdhi_rx_config.h
* Device       : RX Family MCU
* Tool-Chain   : RX Family C Compiler
* H/W Platform : 
* Description  : Interface file for SDHI API for RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 31.07.2017 2.00    First Release
*              : 15.03.2025 2.12    Updated disclaimer.
**********************************************************************************************************************/

#ifndef SDHI_CONFIG_H
#define SDHI_CONFIG_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/* #define for SDHI channel to be valid */
#define SDHI_CFG_CH0_INCLUDED           (1)         /* SDHI Channel 0 Active */
#define SDHI_CFG_CH1_INCLUDED           (0)         /* SDHI Channel 1 Active */

#if (SDHI_CFG_CH0_INCLUDED == 1)
#define SDHI_CFG_CH0_CD_ACTIVE          (1)         /* CD PORT ACTIVE */
#define SDHI_CFG_CH0_WP_ACTIVE          (0)         /* WP PORT ACTIVE */
#define SDHI_CFG_CH0_INT_LEVEL          (15)        /* SDHI channel 0 interrupt level */
#define SDHI_CFG_CH0_INT_LEVEL_DMADTC   (10)        /* SDHI channel 0 DMA/DTC interrupt level */
#endif  /* #if (SDHI_CFG_CH0_INCLUDED == 1) */

#if (SDHI_CFG_CH1_INCLUDED == 1)
#define SDHI_CFG_CH1_CD_ACTIVE          (1)         /* CD PORT ACTIVE */
#define SDHI_CFG_CH1_WP_ACTIVE          (1)         /* WP PORT ACTIVE */
#define SDHI_CFG_CH1_INT_LEVEL          (10)        /* SDHI channel 1 interrupt level */
#define SDHI_CFG_CH1_INT_LEVEL_DMADTC   (10)        /* SDHI channel 1 DMA/DTC interrupt level */
#endif  /* #if (SDHI_CFG_CH1_INCLUDED == 1) */

/* #define for SDHI clock DIV */
#define SDHI_CFG_DIV_HIGH_SPEED         (SDHI_DIV_1)
#define SDHI_CFG_DIV_DEFAULT_SPEED      (SDHI_DIV_2)
#define SDHI_CFG_DIV_INIT_SPEED         (SDHI_DIV_256)

/* #define for SDHI time out count */
#define SDHI_CFG_SDOPT_CTOP             (0x000eu)   /* CD time out count       */
#define SDHI_CFG_SDOPT_TOP              (0x00e0u)   /* response time out count */

/* #define for parameter checking */
#define SDHI_CFG_PARAM_CHECKING_ENABLE  (1)         /* Parameter Checking Active */

/* #define SDHI_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* SDHI_CONFIG_H */

