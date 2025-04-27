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
//#include "ssd1351.h"
//#include "fonts.h"
#include "BMI088.h"
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
	
	R_Config_SCI2_Start();
	

	BMI088_init();
	SSD1351_Init();

	for(int x = 0; x < SSD1351_WIDTH; x++) {
        SSD1351_DrawPixel(x, 0, SSD1351_RED);
        SSD1351_DrawPixel(x, SSD1351_HEIGHT-1, SSD1351_RED);
    }

    for(int y = 0; y < SSD1351_HEIGHT; y++) {
        SSD1351_DrawPixel(0, y, SSD1351_RED);
        SSD1351_DrawPixel(SSD1351_WIDTH-1, y, SSD1351_RED);
    }
	// SSD1351_SetCursor(0,0);
	// SSD1351_printf(Font_7x10,SSD1351_BLUE,"addr = 0x%08lX\n", (uint32_t)SSD1351_Buffer.u8,10);
	// SSD1351_Fill(0xffff);
	// SSD1351_UpdateScreen_DiffLineFast();

	R_Config_CMT0_Start();
	while (1)
	{
		SSD1351_SetCursor(2,2);
		SSD1351_printf(Font_7x10,SSD1351_BLUE,"x:%6.1f",BMI088val.angle.x);
		SSD1351_SetCursor(2,11);
		SSD1351_printf(Font_7x10,SSD1351_BLUE,"y:%6.1f",BMI088val.angle.y);
		SSD1351_SetCursor(2,19);
		SSD1351_printf(Font_7x10,SSD1351_BLUE,"z:%6.1f",BMI088val.angle.z);

		// if(cnt0 > 500) {
		// 	cnt0 = 0;
		// }
		// if(cnt0 > 250) {
		// 	PORT8.PODR.BIT.B0 = 1;
		// 	SSD1351_SetCursor(0,0);
		// 	// SSD1351_printf(Font_7x10,SSD1351_BLUE,"PORT8.PODR.B0=%d", PORT8.PODR.BIT.B0);

			
		// 	// SSD1351_Fill(0x00ff);
		// 	// SSD1351_UpdateScreen();
		// 	// SSD1351_SetCursor(0,0);
		// 	SSD1351_printf(Font_7x10,SSD1351_BLUE,"%6.1f",BMI088val.angle.z);
		// } else {
		// 	PORT8.PODR.BIT.B0 = 0;
		// 	// SSD1351_SetCursor(0,0);
		// 	// SSD1351_printf(Font_7x10,SSD1351_BLUE,"PORT8.PODR.B0=%d", PORT8.PODR.BIT.B0);

		// 	// SSD1351_Fill(0x0ff0);
		// 	// SSD1351_UpdateScreen();
		// }
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif[]
