/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : lowsrc.h
* Description  : Functions to support stream I/O
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2019 2.00     Merged processing of all devices.
*                               Added support for GNUC and ICCRX.
*                               Fixed coding style.
*         : 31.07.2020 2.01     Fixed an issue that caused build errors when the _REENTRANT option was specified in 
*                               the CCRX compiler.
*         : 29.01.2021 3.01     Added tha __write function and __read function for ICCRX.
*         : 26.02.2025 3.02     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef LOWSRC_H
#define LOWSRC_H

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
#if defined(__CCRX__)
void init_iolib(void);
void close_all(void);
long open(const char *name, long  mode, long  flg);
long close(long fileno);
long write(long  fileno, const unsigned char *buf, long  count);
long read(long fileno, unsigned char *buf, long count);
long lseek(long fileno, long offset, long base);
#ifdef _REENTRANT
long *errno_addr(void);
long wait_sem(long semnum);
long signal_sem(long semnum);
#endif
#endif /* defined(__CCRX__) */

#if defined(__GNUC__)
int write(int fileno, char *buf, int count);
int read(int fileno, char *buf, int count);
int _write(int fileno, char *buf, int count);
int _read(int fileno, char *buf, int count);
void close(void);
void fstat(void);
void isatty(void);
void lseek(void);
#endif /* defined(__GNUC__) */

#if defined(__ICCRX__)
size_t __write(int handle, const unsigned char *buf, size_t bufSize);
size_t __read(int handle, unsigned char *buf, size_t bufSize);
#endif /* defined(__ICCRX__) */

#endif  /* End of multiple inclusion prevention macro */

