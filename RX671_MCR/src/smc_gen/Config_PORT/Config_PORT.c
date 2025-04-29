/*
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
* File Name        : Config_PORT.c
* Component Version: 2.4.1
* Device(s)        : R5F5671EHxLE
* Description      : This file implements device driver for Config_PORT.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_PORT.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_PORT_Create
* Description  : This function initializes the PORT
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_PORT_Create(void)
{
    /* Set PORT0 registers */
    PORT0.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT;
    PORT0.PCR.BYTE = _08_Pm3_PULLUP_ON | _20_Pm5_PULLUP_ON | _80_Pm7_PULLUP_ON;
    PORT0.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF;
    PORT0.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF;
    PORT0.PMR.BYTE = _00_Pm3_PIN_GPIO | _00_Pm5_PIN_GPIO | _00_Pm7_PIN_GPIO;
    PORT0.PDR.BYTE = _00_Pm3_MODE_INPUT | _00_Pm5_MODE_INPUT | _00_Pm7_MODE_INPUT | _50_PDR0_DEFAULT;

    /* Set PORT1 registers */
    PORT1.PODR.BYTE = _00_Pm4_OUTPUT_0 | _00_Pm5_OUTPUT_0 | _00_Pm7_OUTPUT_0;
    PORT1.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT1.PCR.BYTE = _04_Pm2_PULLUP_ON | _08_Pm3_PULLUP_ON;
    PORT1.DSCR.BYTE = _00_Pm4_HIDRV_OFF | _80_Pm7_HIDRV_ON;
    PORT1.DSCR2.BYTE = _00_Pm4_HISPEED_OFF | _00_Pm7_HISPEED_OFF;
    PORT1.PMR.BYTE = _00_Pm2_PIN_GPIO | _00_Pm3_PIN_GPIO | _00_Pm4_PIN_GPIO | _00_Pm5_PIN_GPIO | _00_Pm7_PIN_GPIO;
    PORT1.PDR.BYTE = _00_Pm2_MODE_INPUT | _00_Pm3_MODE_INPUT | _10_Pm4_MODE_OUTPUT | _20_Pm5_MODE_OUTPUT | 
                     _80_Pm7_MODE_OUTPUT | _03_PDR1_DEFAULT;

    /* Set PORT2 registers */
    PORT2.PODR.BYTE = _00_Pm5_OUTPUT_0 | _00_Pm7_OUTPUT_0;
    PORT2.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT;
    PORT2.ODR1.BYTE = _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT2.PCR.BYTE = _08_Pm3_PULLUP_ON | _10_Pm4_PULLUP_ON;
    PORT2.DSCR.BYTE = _01_Pm0_HIDRV_ON | _02_Pm1_HIDRV_ON | _04_Pm2_HIDRV_ON | _00_Pm7_HIDRV_OFF;
    PORT2.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF | _00_Pm7_HISPEED_OFF;
    PORT2.PMR.BYTE = _00_Pm3_PIN_GPIO | _00_Pm4_PIN_GPIO | _00_Pm5_PIN_GPIO | _00_Pm7_PIN_GPIO;
    PORT2.PDR.BYTE = _00_Pm3_MODE_INPUT | _00_Pm4_MODE_INPUT | _20_Pm5_MODE_OUTPUT | _80_Pm7_MODE_OUTPUT;

    /* Set PORT3 registers */
    PORT3.PODR.BYTE = _00_Pm3_OUTPUT_0;
    PORT3.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT3.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT3.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF;
    PORT3.PMR.BYTE = _00_Pm3_PIN_GPIO;
    PORT3.PDR.BYTE = _08_Pm3_MODE_OUTPUT;

    /* Set PORT4 registers */
    PORT4.ODR0.BYTE = _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT4.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT4.PCR.BYTE = _01_Pm0_PULLUP_ON;
    PORT4.PMR.BYTE = _00_Pm0_PIN_GPIO;
    PORT4.PDR.BYTE = _00_Pm0_MODE_INPUT;

    /* Set PORT5 registers */
    PORT5.PODR.BYTE = _08_Pm3_OUTPUT_1;
    PORT5.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT5.ODR1.BYTE = _00_Pm5_CMOS_OUTPUT;
    PORT5.PCR.BYTE = _10_Pm4_PULLUP_ON | _40_Pm6_PULLUP_ON;
    PORT5.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm5_HIDRV_OFF;
    PORT5.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF | _08_Pm3_HISPEED_ON | 
                       _00_Pm5_HISPEED_OFF;
    PORT5.PMR.BYTE = _00_Pm3_PIN_GPIO | _00_Pm4_PIN_GPIO | _00_Pm6_PIN_GPIO;
    PORT5.PDR.BYTE = _08_Pm3_MODE_OUTPUT | _00_Pm4_MODE_INPUT | _00_Pm6_MODE_INPUT | _80_PDR5_DEFAULT;

    /* Set PORT6 registers */
    PORT6.PODR.BYTE = _01_Pm0_OUTPUT_1 | _00_Pm4_OUTPUT_0 | _00_Pm6_OUTPUT_0;
    PORT6.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT6.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT6.PCR.BYTE = _04_Pm2_PULLUP_ON;
    PORT6.PMR.BYTE = _00_Pm0_PIN_GPIO | _00_Pm2_PIN_GPIO | _00_Pm4_PIN_GPIO | _00_Pm6_PIN_GPIO;
    PORT6.PDR.BYTE = _01_Pm0_MODE_OUTPUT | _00_Pm2_MODE_INPUT | _10_Pm4_MODE_OUTPUT | _40_Pm6_MODE_OUTPUT;

    /* Set PORT7 registers */
    PORT7.PODR.BYTE = _00_Pm4_OUTPUT_0;
    PORT7.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT;
    PORT7.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT7.PCR.BYTE = _08_Pm3_PULLUP_ON;
    PORT7.DSCR.BYTE = _01_Pm0_HIDRV_ON | _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF | _00_Pm6_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORT7.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm4_HISPEED_OFF | _00_Pm5_HISPEED_OFF | _00_Pm6_HISPEED_OFF | 
                       _00_Pm7_HISPEED_OFF;
    PORT7.PMR.BYTE = _00_Pm3_PIN_GPIO | _00_Pm4_PIN_GPIO;
    PORT7.PDR.BYTE = _00_Pm3_MODE_INPUT | _10_Pm4_MODE_OUTPUT | _06_PDR7_DEFAULT;

    /* Set PORT8 registers */
    PORT8.PODR.BYTE = _00_Pm0_OUTPUT_0 | _00_Pm7_OUTPUT_0;
    PORT8.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT8.ODR1.BYTE = _00_Pm7_CMOS_OUTPUT;
    PORT8.PCR.BYTE = _40_Pm6_PULLUP_ON;
    PORT8.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm3_HIDRV_OFF | _80_Pm7_HIDRV_ON;
    PORT8.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF | _00_Pm3_HISPEED_OFF | 
                       _00_Pm7_HISPEED_OFF;
    PORT8.PMR.BYTE = _00_Pm0_PIN_GPIO | _00_Pm6_PIN_GPIO | _00_Pm7_PIN_GPIO;
    PORT8.PDR.BYTE = _01_Pm0_MODE_OUTPUT | _00_Pm6_MODE_INPUT | _80_Pm7_MODE_OUTPUT | _30_PDR8_DEFAULT;

    /* Set PORT9 registers */
    PORT9.PCR.BYTE = _01_Pm0_PULLUP_ON | _02_Pm1_PULLUP_ON | _04_Pm2_PULLUP_ON | _08_Pm3_PULLUP_ON;
    PORT9.PMR.BYTE = _00_Pm0_PIN_GPIO | _00_Pm1_PIN_GPIO | _00_Pm2_PIN_GPIO | _00_Pm3_PIN_GPIO;
    PORT9.PDR.BYTE = _00_Pm0_MODE_INPUT | _00_Pm1_MODE_INPUT | _00_Pm2_MODE_INPUT | _00_Pm3_MODE_INPUT | 
                     _F0_PDR9_DEFAULT;

    /* Set PORTA registers */
    PORTA.PODR.BYTE = _00_Pm5_OUTPUT_0 | _00_Pm7_OUTPUT_0;
    PORTA.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORTA.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORTA.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm3_HIDRV_OFF | 
                      _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF | _00_Pm6_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORTA.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF | _00_Pm3_HISPEED_OFF | 
                       _00_Pm4_HISPEED_OFF | _00_Pm5_HISPEED_OFF | _00_Pm6_HISPEED_OFF | _00_Pm7_HISPEED_OFF;
    PORTA.PMR.BYTE = _00_Pm5_PIN_GPIO | _00_Pm7_PIN_GPIO;
    PORTA.PDR.BYTE = _20_Pm5_MODE_OUTPUT | _80_Pm7_MODE_OUTPUT;

    /* Set PORTB registers */
    PORTB.PODR.BYTE = _00_Pm0_OUTPUT_0 | _00_Pm2_OUTPUT_0 | _00_Pm3_OUTPUT_0 | _00_Pm4_OUTPUT_0 | _80_Pm7_OUTPUT_1;
    PORTB.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORTB.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORTB.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm3_HIDRV_OFF | 
                      _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF | _00_Pm6_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORTB.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF | _00_Pm3_HISPEED_OFF | 
                       _00_Pm4_HISPEED_OFF | _00_Pm5_HISPEED_OFF | _00_Pm6_HISPEED_OFF | _00_Pm7_HISPEED_OFF;
    PORTB.PMR.BYTE = _00_Pm0_PIN_GPIO | _00_Pm2_PIN_GPIO | _00_Pm3_PIN_GPIO | _00_Pm4_PIN_GPIO | _00_Pm7_PIN_GPIO;
    PORTB.PDR.BYTE = _01_Pm0_MODE_OUTPUT | _04_Pm2_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT | _10_Pm4_MODE_OUTPUT | 
                     _80_Pm7_MODE_OUTPUT;

    /* Set PORTC registers */
    PORTC.PODR.BYTE = _20_Pm5_OUTPUT_1;
    PORTC.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORTC.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORTC.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm3_HIDRV_OFF | 
                      _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF | _00_Pm6_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORTC.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF | _00_Pm3_HISPEED_OFF | 
                       _00_Pm4_HISPEED_OFF | _00_Pm5_HISPEED_OFF | _00_Pm6_HISPEED_OFF | _00_Pm7_HISPEED_OFF;
    PORTC.PMR.BYTE = _00_Pm5_PIN_GPIO;
    PORTC.PDR.BYTE = _20_Pm5_MODE_OUTPUT;

    /* Set PORTD registers */
    PORTD.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORTD.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORTD.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm3_HIDRV_OFF | 
                      _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF | _00_Pm6_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORTD.DSCR2.BYTE = _00_Pm0_HISPEED_OFF | _00_Pm1_HISPEED_OFF | _00_Pm2_HISPEED_OFF | _00_Pm3_HISPEED_OFF | 
                       _00_Pm4_HISPEED_OFF | _00_Pm5_HISPEED_OFF | _00_Pm6_HISPEED_OFF | _00_Pm7_HISPEED_OFF;

    /* Set PORTE registers */
    PORTE.ODR1.BYTE = _00_Pm5_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORTE.PCR.BYTE = _01_Pm0_PULLUP_ON | _02_Pm1_PULLUP_ON | _04_Pm2_PULLUP_ON | _08_Pm3_PULLUP_ON | 
                     _10_Pm4_PULLUP_ON | _40_Pm6_PULLUP_ON;
    PORTE.DSCR.BYTE = _00_Pm5_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORTE.DSCR2.BYTE = _00_Pm5_HISPEED_OFF | _00_Pm7_HISPEED_OFF;
    PORTE.PMR.BYTE = _00_Pm0_PIN_GPIO | _00_Pm1_PIN_GPIO | _00_Pm2_PIN_GPIO | _00_Pm3_PIN_GPIO | _00_Pm4_PIN_GPIO | 
                     _00_Pm6_PIN_GPIO;
    PORTE.PDR.BYTE = _00_Pm0_MODE_INPUT | _00_Pm1_MODE_INPUT | _00_Pm2_MODE_INPUT | _00_Pm3_MODE_INPUT | 
                     _00_Pm4_MODE_INPUT | _00_Pm6_MODE_INPUT;

    /* Set PORTF registers */
    PORTF.PCR.BYTE = _20_Pm5_PULLUP_ON;
    PORTF.PMR.BYTE = _00_Pm5_PIN_GPIO;
    PORTF.PDR.BYTE = _00_Pm5_MODE_INPUT | _DF_PDRF_DEFAULT;

    /* Set PORTJ registers */
    PORTJ.ODR0.BYTE = _00_Pm3_CMOS_OUTPUT;
    PORTJ.PCR.BYTE = _20_Pm5_PULLUP_ON;
    PORTJ.PMR.BYTE = _00_Pm5_PIN_GPIO;
    PORTJ.PDR.BYTE = _00_Pm5_MODE_INPUT | _D7_PDRJ_DEFAULT;

    /* Set all unused pins to output 1 */
    PORT6.PDR.BYTE |= _02_Pm1_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT | _20_Pm5_MODE_OUTPUT;
    PORT6.PODR.BYTE |= _02_Pm1_OUTPUT_1 | _08_Pm3_OUTPUT_1 | _20_Pm5_OUTPUT_1;
    PORT7.PDR.BYTE |= _20_Pm5_MODE_OUTPUT | _40_Pm6_MODE_OUTPUT | _80_Pm7_MODE_OUTPUT;
    PORT7.PODR.BYTE |= _20_Pm5_OUTPUT_1 | _40_Pm6_OUTPUT_1 | _80_Pm7_OUTPUT_1;
    PORT8.PDR.BYTE |= _02_Pm1_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT;
    PORT8.PODR.BYTE |= _02_Pm1_OUTPUT_1 | _08_Pm3_OUTPUT_1;
    PORTA.PDR.BYTE |= _04_Pm2_MODE_OUTPUT;
    PORTA.PODR.BYTE |= _04_Pm2_OUTPUT_1;
    PORTC.PDR.BYTE |= _04_Pm2_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT | _10_Pm4_MODE_OUTPUT;
    PORTC.PODR.BYTE |= _04_Pm2_OUTPUT_1 | _08_Pm3_OUTPUT_1 | _10_Pm4_OUTPUT_1;
    PORTD.PDR.BYTE |= _08_Pm3_MODE_OUTPUT | _20_Pm5_MODE_OUTPUT;
    PORTD.PODR.BYTE |= _08_Pm3_OUTPUT_1 | _20_Pm5_OUTPUT_1;
    PORTE.PDR.BYTE |= _20_Pm5_MODE_OUTPUT | _80_Pm7_MODE_OUTPUT;
    PORTE.PODR.BYTE |= _20_Pm5_OUTPUT_1 | _80_Pm7_OUTPUT_1;

    R_Config_PORT_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
