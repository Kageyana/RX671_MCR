#ifndef SSD1351_H__
#define SSD1351_H__
//=====================================//
// インクルード
//=====================================//
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include "r_smc_entry.h"
#include "fonts.h"
//=====================================//
// シンボル定義
//=====================================//
//=============自動生成関数=============//
#define SSD1351_RES_PORT	PORT5.PODR.BIT.B3
#define SSD1351_CS_PORT		PORT6.PODR.BIT.B0
#define SSD1351_DC_PORT		PORT6.PODR.BIT.B4
#define SSD1351_SPI_FUNC	R_Config_SCI2_SPI_Master_Send_Receive
//=====================================//

// default orientation
#define SSD1351_WIDTH  128
#define SSD1351_HEIGHT 128

// Color definitions
#define	SSD1351_BLACK   0x0000
#define	SSD1351_BLUE    0x001F
#define	SSD1351_RED     0xF800
#define	SSD1351_GREEN   0x07E0
#define SSD1351_CYAN    0x07FF
#define SSD1351_MAGENTA 0xF81F
#define SSD1351_YELLOW  0xFFE0
#define SSD1351_WHITE   0xFFFF
#define SSD1351_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

//====================================//
// グローバル変数の宣言
//====================================//
extern volatile bool spi_ssd1351_tx_done;
extern const uint16_t test_img_128x128[][128];

//====================================//
// プロトタイプ宣言
//====================================//
void SSD1351_Unselect();

void SSD1351_Init(void);
void SSD1351_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void SSD1351_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void SSD1351_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void SSD1351_FillScreen(uint16_t color);
void SSD1351_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void SSD1351_InvertColors(bool invert);
void ssd1351_printf(uint16_t x, uint16_t y, FontDef Font, uint16_t color, uint16_t bgcolor, uint8_t *format, ...);

#endif // SSD1351_H__