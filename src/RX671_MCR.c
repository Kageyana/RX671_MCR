/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#include <stdint.h>
#include "r_smc_entry.h"
#include "timer.h"
#include "ssd1351.h"
#include "fonts.h"

#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	// R_Config_CMT0_Start();
	R_Config_SCI2_Start();
	
	SSD1351_Unselect();
    SSD1351_Init();

	// Check border
	SSD1351_FillScreen(SSD1351_BLACK);

	for(int x = 0; x < SSD1351_WIDTH; x++) {
		SSD1351_DrawPixel(x, 0, SSD1351_RED);
		SSD1351_DrawPixel(x, SSD1351_HEIGHT-1, SSD1351_RED);
	}

	for(int y = 0; y < SSD1351_HEIGHT; y++) {
		SSD1351_DrawPixel(0, y, SSD1351_RED);
		SSD1351_DrawPixel(SSD1351_WIDTH-1, y, SSD1351_RED);
	}

	R_BSP_SoftwareDelay(3000,BSP_DELAY_MILLISECS);

	// Check fonts
	SSD1351_FillScreen(SSD1351_BLACK);
	SSD1351_WriteString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, SSD1351_RED, SSD1351_BLACK);
	SSD1351_WriteString(0, 3*10, "Font_11x18, green, lorem ipsum", Font_11x18, SSD1351_GREEN, SSD1351_BLACK);
	SSD1351_WriteString(0, 3*10+3*18, "Font_16x26, blue, lorem ipsum dolor sit amet", Font_16x26, SSD1351_BLUE, SSD1351_BLACK);

	R_BSP_SoftwareDelay(1000,BSP_DELAY_MILLISECS);
	SSD1351_InvertColors(true);
	R_BSP_SoftwareDelay(1000,BSP_DELAY_MILLISECS);
	SSD1351_InvertColors(false);

	R_BSP_SoftwareDelay(1000,BSP_DELAY_MILLISECS);

	SSD1351_FillScreen(SSD1351_BLACK);
	ssd1351_printf(0, 0, Font_6x8, SSD1351_RED, SSD1351_BLACK, "%dtest%d", 1,2);
	ssd1351_printf(0, 8, Font_7x10, SSD1351_RED, SSD1351_BLACK, "%dtest%d", 1,2);
	ssd1351_printf(0, 16, Font_11x18, SSD1351_RED, SSD1351_BLACK, "%dtest%d", 1,2);

	// Check colors
	// SSD1351_FillScreen(SSD1351_WHITE);
	// SSD1351_WriteString(0, 0, "WHITE", Font_11x18, SSD1351_BLACK, SSD1351_WHITE);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_FillScreen(SSD1351_BLUE);
	// SSD1351_WriteString(0, 0, "BLUE", Font_11x18, SSD1351_BLACK, SSD1351_BLUE);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_FillScreen(SSD1351_RED);
	// SSD1351_WriteString(0, 0, "RED", Font_11x18, SSD1351_BLACK, SSD1351_RED);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_FillScreen(SSD1351_GREEN);
	// SSD1351_WriteString(0, 0, "GREEN", Font_11x18, SSD1351_BLACK, SSD1351_GREEN);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_FillScreen(SSD1351_CYAN);
	// SSD1351_WriteString(0, 0, "CYAN", Font_11x18, SSD1351_BLACK, SSD1351_CYAN);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_FillScreen(SSD1351_MAGENTA);
	// SSD1351_WriteString(0, 0, "MAGENTA", Font_11x18, SSD1351_BLACK, SSD1351_MAGENTA);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_FillScreen(SSD1351_YELLOW);
	// SSD1351_WriteString(0, 0, "YELLOW", Font_11x18, SSD1351_BLACK, SSD1351_YELLOW);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_FillScreen(SSD1351_BLACK);
	// SSD1351_WriteString(0, 0, "BLACK", Font_11x18, SSD1351_WHITE, SSD1351_BLACK);
	// R_BSP_SoftwareDelay(500,BSP_DELAY_MILLISECS);

	// SSD1351_DrawImage(0, 0, 128, 128, (const uint16_t*)test_img_128x128);

	while (1)
	{
		// PORT8.PODR.BIT.B0 = 1;
		// R_BSP_SoftwareDelay(1000,BSP_DELAY_MILLISECS);
		// PORT8.PODR.BIT.B0 = 0;
		// R_BSP_SoftwareDelay(1000,BSP_DELAY_MILLISECS);
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
