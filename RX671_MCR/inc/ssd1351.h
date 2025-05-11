#ifndef SSD1351_H__
#define SSD1351_H__
//=====================================//
// インクルード
//=====================================//
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "r_smc_entry.h"
#include "fonts.h"
//=====================================//
// シンボル定義
//=====================================//
//=============自動生成関数=============//
#define SSD1351_RES_PORT	PORT5.PODR.BIT.B3
#define SSD1351_CS_PORT		PORT6.PODR.BIT.B0
#define SSD1351_DC_PORT		PORT6.PODR.BIT.B4
#define SSD1351_SPI_CHANNEL SCI2
#define SSD1351_SPI_FUNC	R_Config_SCI2_SPI_Master_Send_Receive
//=====================================//

// マイコンのエンディアンを選択
#define SSD1351_LITTLEENDIAN
// #define SSD1351_BIGEENDIAN

#define LINES_PER_FRAME 4


// default orientation
#define SSD1351_WIDTH  128
#define SSD1351_HEIGHT 128
#define SSD1351_BUFFER_SIZE SSD1351_WIDTH * SSD1351_HEIGHT

#define SSD1351_DUMMYRX_SIZE 1024

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
// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} SSD1351_t;

typedef union {
    uint16_t u16[SSD1351_BUFFER_SIZE];
    uint8_t u8[SSD1351_BUFFER_SIZE*2];
} U16ToU8_Union;

extern volatile bool spi_ssd1351_tx_done;
extern volatile bool g_dma_transfer_done;
extern U16ToU8_Union SSD1351_Buffer;	// Screenbuffer
//====================================//
// プロトタイプ宣言
//====================================//
void SSD1351_Unselect();

void SSD1351init(void);
void SSD1351fill(uint16_t color);
void SSD1351updateScreen(void);
void SSD1351updateScreenChunked(void);
void SSD1351drawPixel(uint8_t x, uint8_t y, uint16_t color);
char SSD1351writeChar(char ch, FontDef Font, uint16_t color);
char SSD1351writeString(char *str, FontDef Font, uint16_t color);
void SSD1351printf(FontDef Font, uint16_t color, uint8_t *format, ...);
void SSD1351setCursor(uint8_t x, uint8_t y);
void SSD1351setDisplayOn(const uint8_t on);
uint8_t SSD1351getDisplayOn(void);


#endif // SSD1351_H__