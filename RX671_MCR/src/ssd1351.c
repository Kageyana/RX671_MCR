//====================================//
// インクルード
//====================================//
#include "ssd1351.h"
//====================================//
// グローバル変数の宣言
//====================================//
volatile bool spi_ssd1351_tx_done = false;
volatile bool g_dma_transfer_done = false;

static U16ToU8_Union SSD1351_Buffer;	// Screenbuffer
static U16ToU8_Union SSD1351_BufferBefore;	// Screenbuffer
static SSD1351_t SSD1351;	/// Screen object
static uint16_t draw_line_index = 0;

/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351reset
// 処理概要     SSD1351をリセットする
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
static void SSD1351reset(void) {
    SSD1351_RES_PORT = 1;
	R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
    SSD1351_RES_PORT = 0;
    R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
    SSD1351_RES_PORT = 1;
    R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351writeCommand
// 処理概要     コマンド送信
// 引数         cmd:コマンドデータ
// 戻り値       なし
////////////////////////////////////////////////////////////////////
static void SSD1351writeCommand(uint8_t cmd) {
	uint8_t rxData[1];

	SSD1351_CS_PORT = 0;
	SSD1351_DC_PORT = 0;
	SSD1351_SPI_FUNC(&cmd, sizeof(cmd), rxData, sizeof(cmd));
	spi_ssd1351_tx_done = false;
	while(!spi_ssd1351_tx_done);

	SSD1351_CS_PORT = 1;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351writeData
// 処理概要     データ送信
// 引数         buff:データ配列 buff_size:データ数
// 戻り値       なし
////////////////////////////////////////////////////////////////////
static void SSD1351writeData(uint8_t* buff, size_t buff_size) {
	uint8_t rxData[1024];

	SSD1351_CS_PORT = 0;
	SSD1351_DC_PORT = 1;

    // split data in small chunks because SMC can't send more then 1K at once
    while(buff_size > 0) {
        uint16_t chunk_size = buff_size > 1024 ? 1024 : buff_size;
		
		SSD1351_SPI_FUNC(buff, chunk_size, rxData, chunk_size);
		spi_ssd1351_tx_done = false;
		while(!spi_ssd1351_tx_done);
        
		buff += chunk_size;
        buff_size -= chunk_size;
    }

	SSD1351_CS_PORT = 1;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351setAddressWindow
// 処理概要     表示開始位置の設定
// 引数         x0 y0:開始座標(左上) x1 y1:終了座標(右下)
// 戻り値       なし
////////////////////////////////////////////////////////////////////
static void SSD1351setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    SSD1351writeCommand(0x15); // SETCOLUMN
    {
        uint8_t data[] = { x0 & 0xFF, x1 & 0xFF };
        SSD1351writeData(data, sizeof(data));
    }

    // row address set
    SSD1351writeCommand(0x75); // SETROW
    {
        uint8_t data[] = { y0 & 0xFF, y1 & 0xFF };
        SSD1351writeData(data, sizeof(data));
    }

    // write to RAM
    SSD1351writeCommand(0x5C); // WRITERAM
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351init
// 処理概要     初期化
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351init(void) {
    SSD1351reset();

    // command list is based on https://github.com/adafruit/Adafruit-SSD1351-library

    SSD1351writeCommand(0xFD); // COMMANDLOCK
    {
        uint8_t data[] = { 0x12 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xFD); // COMMANDLOCK
    {
        uint8_t data[] = { 0xB1 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xAE); // DISPLAYOFF
    SSD1351writeCommand(0xB3); // CLOCKDIV
    SSD1351writeCommand(0xF1); // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    SSD1351writeCommand(0xCA); // MUXRATIO
    {
        uint8_t data[] = { 0x7F }; // 127
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xA0); // SETREMAP
    {
        uint8_t data[] = { 0x74 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0x15); // SETCOLUMN
    {
        uint8_t data[] = { 0x00, 0x7F };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0x75); // SETROW
    {
        uint8_t data[] = { 0x00, 0x7F };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xA1); // STARTLINE
    {
        uint8_t data[] = { 0x00 }; // 96 if display height == 96
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xA2); // DISPLAYOFFSET
    {
        uint8_t data[] = { 0x00 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xB5); // SETGPIO
    {
        uint8_t data[] = { 0x00 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xAB); // FUNCTIONSELECT
    {
        uint8_t data[] = { 0x01 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xB1); // PRECHARGE
    {
        uint8_t data[] = { 0x32 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xBE); // VCOMH
    {
        uint8_t data[] = { 0x05 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xA6); // NORMALDISPLAY (don't invert)
    SSD1351writeCommand(0xC1); // CONTRASTABC
    {
        uint8_t data[] = { 0xC8, 0x80, 0xC8 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xC7); // CONTRASTMASTER
    {
        uint8_t data[] = { 0x0F };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xB4); // SETVSL
    {
        uint8_t data[] = { 0xA0, 0xB5, 0x55 };
        SSD1351writeData(data, sizeof(data));
    }
    SSD1351writeCommand(0xB6); // PRECHARGE2
    {
        uint8_t data[] = { 0x01 };
        SSD1351writeData(data, sizeof(data));
    }

    SSD1351setDisplayOn(1);	//--turn on SSD1315 panel

	// Clear screen
	SSD1351fill(SSD1351_BLACK);

	// Flush buffer to screen
	SSD1351updateScreen();

	// Set default values for screen object
	SSD1351.CurrentX = 0;
	SSD1351.CurrentY = 0;

	SSD1351.Initialized = 1;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351fill
// 処理概要     バッファを単一の色で埋める
// 引数         color:16bit色データ
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351fill(uint16_t color)
{
	uint32_t i;

	for (i = 0; i < sizeof(SSD1351_Buffer)/sizeof(uint16_t); i++)
	{
		SSD1351_Buffer.u16[i] = color;
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351updateScreen
// 処理概要     バッファを送信する
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351updateScreen(void)
{
	SSD1351setAddressWindow(0,0,SSD1351_WIDTH-1,SSD1351_HEIGHT-1);

	uint16_t buff_size = SSD1351_BUFFER_SIZE*2;
	uint8_t dummy_rx[SSD1351_DUMMYRX_SIZE];
	uint8_t *buff = SSD1351_Buffer.u8;
	
	SSD1351_CS_PORT = 0;	// select OLED
	SSD1351_DC_PORT = 1;	// data

	// split data in small chunks because SMC can't send more then 1K at once
	while(buff_size > 0) {
        uint16_t chunk_size = buff_size > SSD1351_DUMMYRX_SIZE? SSD1351_DUMMYRX_SIZE : buff_size;
		
		SSD1351_SPI_FUNC(buff, chunk_size, dummy_rx, chunk_size);
		spi_ssd1351_tx_done = false;
		while(!spi_ssd1351_tx_done);
        
		buff += chunk_size;
        buff_size -= chunk_size;
    }

	SSD1351_CS_PORT = 1;	// Unselect OLED

	// 前回値を保存
	memcpy(SSD1351_BufferBefore.u16,SSD1351_Buffer.u16,SSD1351_BUFFER_SIZE*sizeof(uint16_t));

}

void SSD1351updateScreenChunked(void)
{
    // uint8_t line_buffer[SSD1351_WIDTH * 2];
	union {
		uint16_t u16[SSD1351_WIDTH];
		uint8_t u8[SSD1351_WIDTH * 2];
	} line_buffer;
    uint8_t dummy_rx[SSD1351_WIDTH * 2];

    for (uint16_t l = 0; l < LINES_PER_FRAME; l++) {
        if (draw_line_index >= SSD1351_HEIGHT) {
            draw_line_index = 0;
            break;
        }

        uint16_t y = draw_line_index;
		uint16_t offset = y * SSD1351_WIDTH;

		bool changed = false;
		// cntsameがSSD1351_WIDTH-1と同値であるか＝その列に前回と違うピクセルがあるか判定
        for (uint16_t x = 0; x < SSD1351_WIDTH; x++) {
            if (SSD1351_Buffer.u16[offset + x] != SSD1351_BufferBefore.u16[offset + x]) {
                changed = true;
                break;
            }
        }
        
		
		if(changed)
		{
			memcpy(line_buffer.u16,SSD1351_Buffer.u16+offset,SSD1351_WIDTH*2);
			memcpy(&SSD1351_BufferBefore.u16[offset],&SSD1351_Buffer.u16[offset],SSD1351_WIDTH * sizeof(uint16_t));
		
			SSD1351setAddressWindow(0, y, SSD1351_WIDTH - 1, y);
			SSD1351_CS_PORT = 0;
			SSD1351_DC_PORT = 1;

			SSD1351_SPI_FUNC(line_buffer.u8, SSD1351_WIDTH * sizeof(uint16_t), dummy_rx, SSD1351_WIDTH * sizeof(uint16_t));
			spi_ssd1351_tx_done = false;
			while(!spi_ssd1351_tx_done);

			SSD1351_CS_PORT = 1;
		}
        
        draw_line_index++;
    }
}
/////////////////////////////////////////////////////////////////////
// モジュール名 ssd1306_DrawPixel
// 処理概要     指定座標に指定カラーを表示する
// 引数         x y:指定座標 color:16bitカラー
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351drawPixel(uint8_t x, uint8_t y, uint16_t color)
{
	if (x >= SSD1351_WIDTH || y >= SSD1351_HEIGHT)
	{
		// Don't write outside the buffer
		return;
	}

	// Draw in the right color
	#ifdef SSD1351_LITTLEENDIAN
	// MSB LSBを逆転させる
	SSD1351_Buffer.u8[(x + y * SSD1351_WIDTH)*2] = (color >> 8) & 0xFF;	//MSBを代入
	SSD1351_Buffer.u8[(x + y * SSD1351_WIDTH)*2+1] = color & 0xFF;		//LSBを代入
	#else
	SSD1351_Buffer.u16[x + y * SSD1351_WIDTH] = color;
	#endif
	
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351writeChar
// 処理概要     指定座標に文字を表示する
// 引数         ch:文字(ascii) Font:フォントサイズ color:16bitカラーコード
// 戻り値       文字データ
////////////////////////////////////////////////////////////////////
char SSD1351writeChar(char ch, FontDef Font, uint16_t color)
{
	uint32_t i, b, j;

	// Check if character is valid
	if (ch < 32 || ch > 126)
		return 0;

	// Check remaining space on current line
	if (SSD1351_WIDTH < (SSD1351.CurrentX + Font.FontWidth) ||
		SSD1351_HEIGHT < (SSD1351.CurrentY + Font.FontHeight))
	{
		// Not enough space on current line
		return 0;
	}

	// Use the font to write
	for (i = 0; i < Font.FontHeight; i++)
	{
		b = Font.data[(ch - 32) * Font.FontHeight + i];
		for (j = 0; j < Font.FontWidth; j++)
		{
			if ((b << j) & 0x8000)
			{
				SSD1351drawPixel(SSD1351.CurrentX + j, (SSD1351.CurrentY + i), color);
			}
			else
			{
				SSD1351drawPixel(SSD1351.CurrentX + j, (SSD1351.CurrentY + i), 0x0000);
			}
		}
	}

	// The current space is now taken
	SSD1351.CurrentX += Font.FontWidth;

	// Return written char for validation
	return ch;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351writeString
// 処理概要     CurrentX,Yに文字列を表示する 画面端から飛び出ると改行する
// 引数         str:文字列配列 Font:フォントサイズ color:16bitカラーコード
// 戻り値       文字列のアドレス
////////////////////////////////////////////////////////////////////
char SSD1351writeString(char *str, FontDef Font, uint16_t color)
{
	while(*str) {
        if(SSD1351.CurrentX + Font.FontWidth >= SSD1351_WIDTH) {
            SSD1351.CurrentX = 0;
            SSD1351.CurrentY += Font.FontHeight;
            if(SSD1351.CurrentY + Font.FontHeight >= SSD1351_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        SSD1351writeChar(*str, Font, color);
        str++;
    }

	// Everything ok
	return *str;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351setCursor
// 処理概要     CurrentX,Yを設定
// 引数         x y:指定座標(文字の左上の座標)
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351setCursor(uint8_t x, uint8_t y)
{
	SSD1351.CurrentX = x;
	SSD1351.CurrentY = y;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351printf
// 処理概要     SSD1351用のprintf
// 引数         Font:フォントサイズ color:16bitカラーコード
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351printf(FontDef Font, uint16_t color, uint8_t *format, ...)
{
	va_list argptr;
	uint8_t str[SSD1351_WIDTH / 6]; // 最小フォント幅での最大文字数

	va_start(argptr, format);
	vsprintf(str, format, argptr);
	va_end(argptr);

	SSD1351writeString(str, Font, color);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351setDisplayOn
// 処理概要     ディスプレイ表示のON/OFF
// 引数         0:非表示 1:表示
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351setDisplayOn(const uint8_t on)
{
	uint8_t value;
	if (on)
	{
		value = 0xAF; // Display on
		SSD1351.DisplayOn = 1;
	}
	else
	{
		value = 0xAE; // Display off
		SSD1351.DisplayOn = 0;
	}
	SSD1351writeCommand(value);
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351getDisplayOn
// 処理概要     ディスプレイ表示状態を取得
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
uint8_t SSD1351getDisplayOn(void)
{
	return SSD1351.DisplayOn;
}