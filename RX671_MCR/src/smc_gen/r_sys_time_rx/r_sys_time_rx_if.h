/*
* Copyright (c) 2013-2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_sys_time_rx_if.h
* Description  : Functions for using SYSTEM TIME on RX devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           25.09.2013 1.00    Initial Release
*           20.06.2019 1.01    Fixed bug: R_SYS_TIME_GetCurrentTime()
*                              Uppdated:  SYS_TIME_UTC table.
*                              Changed:   Error code macro name from SYS_TIME_BAD_xxx to SYS_TIME_ERR_BAD_xxx.
*                              Added support GCC RX compiler and IAR RX compiler .
*           20.03.2025 1.02    Changed the disclaimer.
***********************************************************************************************************************/

#ifndef SYS_TIME_IF_H
#define SYS_TIME_IF_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define SYS_TIME_VERSION_MAJOR  (1)
#define SYS_TIME_VERSION_MINOR  (02)

#define SYS_TIME_UTC_MINUS_1200 "UTC-12:00"
#define SYS_TIME_UTC_MINUS_1100 "UTC-11:00"
#define SYS_TIME_UTC_MINUS_1000 "UTC-10:00"
#define SYS_TIME_UTC_MINUS_0930 "UTC-09:30"
#define SYS_TIME_UTC_MINUS_0900 "UTC-09:00"
#define SYS_TIME_UTC_MINUS_0800 "UTC-08:00"
#define SYS_TIME_UTC_MINUS_0700 "UTC-07:00"
#define SYS_TIME_UTC_MINUS_0600 "UTC-06:00"
#define SYS_TIME_UTC_MINUS_0500 "UTC-05:00"
#define SYS_TIME_UTC_MINUS_0400 "UTC-04:00"
#define SYS_TIME_UTC_MINUS_0330 "UTC-03:30"
#define SYS_TIME_UTC_MINUS_0300 "UTC-03:00"
#define SYS_TIME_UTC_MINUS_0200 "UTC-02:00"
#define SYS_TIME_UTC_MINUS_0100 "UTC-01:00"
#define SYS_TIME_UTC "UTC"
#define SYS_TIME_UTC_PLUS_0100 "UTC+01:00"
#define SYS_TIME_UTC_PLUS_0200 "UTC+02:00"
#define SYS_TIME_UTC_PLUS_0300 "UTC+03:00"
#define SYS_TIME_UTC_PLUS_0330 "UTC+03:30"
#define SYS_TIME_UTC_PLUS_0400 "UTC+04:00"
#define SYS_TIME_UTC_PLUS_0430 "UTC+04:30"
#define SYS_TIME_UTC_PLUS_0500 "UTC+05:00"
#define SYS_TIME_UTC_PLUS_0530 "UTC+05:30"
#define SYS_TIME_UTC_PLUS_0545 "UTC+05:45"
#define SYS_TIME_UTC_PLUS_0600 "UTC+06:00"
#define SYS_TIME_UTC_PLUS_0630 "UTC+06:30"
#define SYS_TIME_UTC_PLUS_0700 "UTC+07:00"
#define SYS_TIME_UTC_PLUS_0800 "UTC+08:00"
#define SYS_TIME_UTC_PLUS_0845 "UTC+08:45"
#define SYS_TIME_UTC_PLUS_0900 "UTC+09:00"
#define SYS_TIME_UTC_PLUS_0930 "UTC+09:30"
#define SYS_TIME_UTC_PLUS_1000 "UTC+10:00"
#define SYS_TIME_UTC_PLUS_1030 "UTC+10:30"
#define SYS_TIME_UTC_PLUS_1100 "UTC+11:00"
#define SYS_TIME_UTC_PLUS_1200 "UTC+12:00"
#define SYS_TIME_UTC_PLUS_1245 "UTC+12:45"
#define SYS_TIME_UTC_PLUS_1300 "UTC+13:00"
#define SYS_TIME_UTC_PLUS_1400 "UTC+14:00"

/*****************************************************************************
Typedef definitions
******************************************************************************/
typedef enum e_sys_time_err
{
    SYS_TIME_SUCCESS = 0,               /* Normally terminated. */
    SYS_TIME_ERR_BAD_CHANNEL,           /* Non-existent channel number. */
    SYS_TIME_ERR_BAD_INTERVAL,          /* Bad interval parameter is specified. */
    SYS_TIME_ERR_BAD_TIME_OFFSET,       /* Bad time offset is set. */
    SYS_TIME_ERR_BAD_FUNCTION_POINTER,  /* Bad function pointer is set. */
    SYS_TIME_ERR_BAD_SYS_TIME,          /* Bad system timer value is input */
    SYS_TIME_ERR_ALREADY_STARTED,       /* System timer is already started. */
    SYS_TIME_ERR_NOT_STARTED,           /* System timer is not started. */
    SYS_TIME_ERR_FULL_REGISTERED,       /* All register table is used. */
    SYS_TIME_ERR_ALREADY_REGISTERED,    /* Specified function pointer has been already registered. */
} sys_time_err_t;

typedef struct sys_time_
{
    uint32_t sec;
    uint32_t min;
    uint32_t hour;
    uint32_t day;
    uint32_t month;
    uint32_t year;
    uint32_t unix_time;
    uint8_t time_zone[16];
} SYS_TIME;

typedef void (*callback_from_sys_time_t)(void);

/*****************************************************************************
Public Functions
******************************************************************************/
sys_time_err_t R_SYS_TIME_Open(void);
sys_time_err_t R_SYS_TIME_GetCurrentTime(SYS_TIME *sys_time);
sys_time_err_t R_SYS_TIME_SetCurrentTime(SYS_TIME *sys_time);
sys_time_err_t R_SYS_TIME_ConvertUnixTimeToSystemTime(uint32_t unix_time, SYS_TIME *sys_time, uint8_t *time_offset);
sys_time_err_t R_SYS_TIME_RegisterPeriodicCallback(callback_from_sys_time_t function_pointer, uint32_t interval);
sys_time_err_t R_SYS_TIME_UnregisterPeriodicCallback(callback_from_sys_time_t function_pointer);
bool R_SYS_TIME_IsPeriodicCallbackRegistered(callback_from_sys_time_t function_pointer);
sys_time_err_t R_SYS_TIME_Close(void);
uint32_t R_SYS_TIME_GetVersion(void);

#endif /* SYS_TIME_IF_H */

