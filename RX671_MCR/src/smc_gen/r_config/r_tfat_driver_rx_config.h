/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
* Copyright (c) 2014 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_tfat_driver_rx_config.h
* Description   : Configures the TFAT driver interface
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 01.12.2014 1.00    First Release
*              : 21.01.2015 1.01    Added support USB Mini Firmware.
*              : 22.06.2015 1.02    Added support MCU RX231.
*              : 01.04.2016 1.03    Added support RX family.
*              : 29.06.2018 1.04    Modified SDHI to SDMEM.
*              : 20.12.2019 2.00    Added support for FreeRTOS and Renesas uITRON (RI600V4).
*                                   Added support for GNUC and ICCRX.
*              : 10.06.2020 2.10    Added support MMC Firmware and FLASH Firmware.
*              : 15.12.2023 2.40    Added support FLASH SPI sector size selectable.
*                                   Fixed to comply with GSCE Coding Standards Rev.6.5.0.
*              : 08.05.2024 2.50    Added support SPI mode SD card
*              : 15.03.2025 2.61    Updated disclaimer.
***********************************************************************************************************************/
#ifndef _R_TFAT_DRIVER_RX_CONFIG_H_
#define _R_TFAT_DRIVER_RX_CONFIG_H_

#include "r_tfat_driver_rx_if.h"

/*******************************************************************************
Configuration Options
*******************************************************************************/
/* Number of logical drives to be used.
   Setting to 0     : unused memory
              other : number of logical drives
   (USB and SD memory card can be used together.)
*/
#define TFAT_USB_DRIVE_NUM          (0)
#define TFAT_SDMEM_DRIVE_NUM        (1)
#define TFAT_SPI_SDMEM_DRIVE_NUM    (0)
#define TFAT_USB_MINI_DRIVE_NUM     (0)
#define TFAT_MMC_DRIVE_NUM          (0)
#define TFAT_SERIAL_FLASH_DRIVE_NUM (0)

/* Configure selectable sector size
   (available from 512, 1024, 2048 or 4096 bytes) */
#define TFAT_FLASH_SECTOR_SIZE           (4096)    /*Flash SPI */

/* allocate a drive number(initial setting)
  <valid define>
  TFAT_CTRL_USB             : for USB
  TFAT_CTRL_SDMEM           : for SD memory card
  TFAT_CTRL_SPI_SDMEM       : for SPI mode SD memory card
  TFAT_CTRL_USB_MINI        : for USB Mini
  TFAT_CTRL_MMC             : for MMC memory card or eMMC
  TFAT_CTRL_SERIAL_FLASH    : for Serial FLASH memory
  TFAT_CTRL_NONE            : unallocated drive

  MAX 10 drives(TFAT module spec)

  ex.)
      #define TFAT_DRIVE_ALLOC_NUM_0    TFAT_CTRL_USB
      #define TFAT_DRIVE_ALLOC_NUM_1    TFAT_CTRL_SDMEM
      #define TFAT_DRIVE_ALLOC_NUM_2    TFAT_CTRL_SDMEM
      #define TFAT_DRIVE_ALLOC_NUM_3    TFAT_CTRL_USB

  The device channel or The drive number of device side is ascending number.

  When you'd like to change the allocation.
  Please use drv_change_alloc().
*/
#define TFAT_DRIVE_ALLOC_NUM_0    (TFAT_CTRL_SDMEM)
#define TFAT_DRIVE_ALLOC_NUM_1    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_2    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_3    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_4    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_5    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_6    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_7    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_8    (TFAT_CTRL_NONE)
#define TFAT_DRIVE_ALLOC_NUM_9    (TFAT_CTRL_NONE)

/* Mutex ID number that used for the each TFAT drive (only RI600V4)
  When using RI600V4, define the mutex ID created by RI600V4 configuration or 0.
  This mutex is used by TFAT APIs to obtain the re-entrancy (thread safe) for 
  exclusive use of file/directory access on the each drive (logical volume).
  <valid define>
  0     : Define 0 when not using the target drive.
  1~255 : Used mutex ID when using the target drive.
          Duplication with ID for other using drives is not allowed.

    ex.)
      #define MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0    (1)    // mutex ID for the using drive0
      #define MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1    (2)    // mutex ID for the using drive1
      #define MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2    (0)    // not used mutex for the unusing drive2
  
  Note that you must define mutex settings first in RI600V4 configuration file (.cfg)
  because RI600V4 cannot dynamically create mutexes.
*/
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_0    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_1    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_2    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_3    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_4    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_5    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_6    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_7    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_8    (0)
#define RI600V4_MUTEX_ID_FOR_TFAT_DRIVE_ALLOC_NUM_9    (0)

/* Task priority when locking the mutex for the TFAT drive (only RI600PX)
  When using RI600PX, define the task priority that you want to increase when 
  locking mutex for the TFAT drive.
  This mutex is used by TFAT APIs to obtain the re-entrancy (thread safe) for 
  exclusive use of file/directory access on the each drive (logical volume).
  <valid define>
  1 ~ TMAX_TPRI: TMAX_TPRI is maximum task priority defined in kernel_id.h that outputed 
                 by the system.priority setting in RI600PX configuration file (.cfg).
*/
#define RI600PX_LOCKED_MUTEX_TASK_PRIORITY    (1)

#endif /* _R_TFAT_DRIVER_RX_CONFIG_H_ */
