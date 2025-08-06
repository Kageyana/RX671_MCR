/*
* Copyright (c) 2016-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_sys_time_rx_private.h
* Description  : Functions for using SYSTEM TIME on RX devices.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           30.11.2016 1.00    Initial Release.
*           20.03.2025 1.02    Changed the disclaimer.
***********************************************************************************************************************/

#ifndef SYS_TIME_PRIVATE_H
#define SYS_TIME_PRIVATE_H

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define SYS_TIME_SECOND_OF_MIN  (60UL)
#define SYS_TIME_SECOND_OF_HOUR (60UL*60UL)
#define SYS_TIME_SECOND_OF_DAY  (60UL*60UL*24UL)
#define SYS_TIME_SECOND_OF_YEAR (60UL*60UL*24UL*365UL)
#define SYS_TIME_SECOND_OF_LEAP_YEAR (60UL*60UL*24UL*366UL)

#define SYS_TIME_INTERVAL_LIMIT (600UL)
#define SYS_TIME_REGISTERED_PROCESS_LIMIT (30UL)

typedef struct time_offset_tbl
{
    uint8_t time_offset_code[16];
    int32_t offset;
} time_offset_tbl_t;

typedef struct proccess_list
{
    callback_from_sys_time_t function_pointer;
    uint8_t registered_flag;
    uint32_t interval;
    uint32_t interval_initial;
} process_list_t;

#if defined (_UNIT_TEST)
#define SYS_TIME_STATIC
#else
#define SYS_TIME_STATIC static
#endif

#endif /* SYS_TIME_PRIVATE_H */
