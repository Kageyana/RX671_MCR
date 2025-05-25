/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
* Copyright (c) 2014 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/*******************************************************************************
* File Name     : r_dmaca_rx_config.h
* Description   : Configuration file for DMACA driver
********************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.06.2014 1.00    Initial revision
*         : 27.08.2014 1.01    Not change program due to updating application note only.
*         : 30.01.2015 1.02    Added RX71M.
*         : 13.04.2015 1.03    Added RX231 and RX230.
*         : 30.09.2016 1.04    Added RX65N.
*         : 31.07.2017 1.05    Supported RX65N-2MB.
*                              Fixed to correspond to Renesas coding rule.
*         : 28.09.2018 1.10    Supported RX66T.
*         : 01.02.2019 1.20    Supported RX72T, RX65N-64pin.
*         : 06.09.2024 3.40    Added support Nested Interrupt.
*         : 15.03.2025 3.41    Updated disclaimer.
*******************************************************************************/
#ifndef DMACA_RX_CONFIG_H
#define DMACA_RX_CONFIG_H

/*
 * SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
 *  0 : Compiles out parameter checking.
 *  1 : Includes parameter checking.
 * Default value is set to BSP_CFG_PARAM_CHECKING_ENABLE to re-use the system default setting
*/
#define DMACA_CFG_PARAM_CHECKING_ENABLE       (BSP_CFG_PARAM_CHECKING_ENABLE)

/*
 * SPECIFY WHETHER THE DTC DRIVER IS USED WITH DMACA DRIVER
 *  0 : DTC driver is not used with DMACA driver.
 *  1 : DTC driver is used with DMACA driver.
*/
#define DMACA_CFG_USE_DTC_FIT_MODULE         (0)

/*
 * Enable Nested Interrupt
 *  0 : Disable nested interrupt.
 *  1 : Enable nested interrupt.
*/
#define DMACA_CFG_EN_NESTED_INT     (0)

#endif /* DMACA_RX_CONFIG_H */
