//====================================//
// インクルード
//====================================//
#include "ssd1351.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
//====================================//
// グローバル変数の宣言
//====================================//
volatile bool spi_ssd1351_tx_done = false;
volatile bool g_dma_transfer_done = false;
bool display_update_locked = false;

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

	spi_ssd1351_tx_done = false;
	SSD1351_SPI_FUNC(&cmd, sizeof(cmd), rxData, sizeof(cmd));
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
		spi_ssd1351_tx_done = false;
		SSD1351_SPI_FUNC(buff, chunk_size, rxData, chunk_size);
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

	SSD1351setContrastRGB(0x64, 0x64, 0x64); // CONTRASTABC

	SSD1351setContrastMaster(0x08); // CONTRASTMASTER

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
	for(uint16_t i = 0; i < SSD1351_BUFFER_SIZE; i++)
	{
		SSD1351_BufferBefore.u16[i] = SSD1351_BLACK-1;
	}
	

	// Flush buffer to screen
	for(uint16_t i = 0; i < SSD1351_HEIGHT/LINES_PER_FRAME; i++)
	{
		SSD1351updateScreen();
	}

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
	for (uint32_t i = 0; i < sizeof(SSD1351_Buffer)/sizeof(uint16_t); i++)
	{
#ifdef SSD1351_LITTLEENDIAN
		// MSB LSBを逆転させる
		SSD1351_Buffer.u8[i*2] = (color >> 8) & 0xFF;	//MSBを代入
		SSD1351_Buffer.u8[(i*2)+1] = color & 0xFF;		//LSBを代入
#else
		SSD1351_Buffer.u16[i] = color;
#endif
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351updateScreen
// 処理概要     バッファを数行ずつ送信する(タイマ割り込みで処理する)
// 引数         なし
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351updateScreen(void)
{
    // uint8_t line_buffer[SSD1351_WIDTH * 2];
	union {
		uint16_t u16[SSD1351_WIDTH];
		uint8_t u8[SSD1351_WIDTH * 2];
	} line_buffer;
    uint8_t dummy_rx[SSD1351_WIDTH * 2];

	if(SSD1351.Initialized)
	{
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
uint8_t SSD1351writeChar(uint8_t ch, FontDef Font, uint16_t color)
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
uint8_t SSD1351writeString(uint8_t *str, FontDef Font, uint16_t color)
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
// モジュール名 SSD1351_ftoa
// 処理概要     簡易浮動小数点を文字列に変換する
// 引数         value:変換する値 buffer:出力先バッファ
//              buffer_size:バッファサイズ precision:小数点以下桁数
// 戻り値       なし
////////////////////////////////////////////////////////////////////
static void SSD1351_ftoa(double value, char *buffer, size_t buffer_size, uint8_t precision)
{
        if (precision > 6)
        {
                precision = 6;
        }
        if (buffer_size == 0)
        {
                return;
        }

        char *p = buffer;

        if (value < 0.0)
        {
                *p++ = '-';
                value = -value;
        }

        int int_part = (int)value;
        double frac = value - (double)int_part;
        size_t remain = buffer_size - (p - buffer);
        int n = snprintf(p, remain, "%d", int_part);
        if (n < 0)
        {
                buffer[0] = '\0';
                return;
        }
        p += n;
        if (precision > 0 && (size_t)(p - buffer) < buffer_size - 1)
        {
                *p++ = '.';
                for (uint8_t i = 0; i < precision && (size_t)(p - buffer) < buffer_size - 1; i++)
                {
                        frac *= 10.0;
                        int digit = (int)frac;
                        *p++ = (char)('0' + digit);
                        frac -= digit;
                }
        }

        *p = '\0';
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351printf
// 処理概要     SSD1351用の簡易printf
// 引数         Font:フォントサイズ color:16bitカラーコード
//              format:フォーマット文字列 ...:書式化する値
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351printf(FontDef Font, uint16_t color, uint8_t *format, ...)
{
        va_list argptr;
        va_start(argptr, format);

        char str[SSD1351_WIDTH + 1];
        char *dest = str;
        const char *fmt = (const char *)format;

        while (*fmt && (size_t)(dest - str) < SSD1351_WIDTH)
        {
                if (*fmt != '%')
                {
                        *dest++ = *fmt++;
                        continue;
                }

                fmt++;
                int precision = 2;
                if (*fmt == '.')
                {
                        fmt++;
                        precision = 0;
                        while (isdigit((unsigned char)*fmt))
                        {
                                precision = precision * 10 + (*fmt - '0');
                                fmt++;
                        }
                }

                switch (*fmt)
                {
                case 'd':
                {
                        int val = va_arg(argptr, int);
                        size_t remain = sizeof(str) - (size_t)(dest - str);
                        int n = snprintf(dest, remain, "%d", val);
                        if (n > 0)
                        {
                                dest += n;
                        }
                        break;
                }
                case 'x':
                {
                        int val = va_arg(argptr, int);
                        size_t remain = sizeof(str) - (size_t)(dest - str);
                        int n = snprintf(dest, remain, "%x", val);
                        if (n > 0)
                        {
                                dest += n;
                        }
                        break;
                }
                case 's':
                {
                        const char *s = va_arg(argptr, const char *);
                        while (*s && (size_t)(dest - str) < SSD1351_WIDTH)
                        {
                                *dest++ = *s++;
                        }
                        break;
                }
                case 'f':
                {
                        double val = va_arg(argptr, double);
                        char tmp[32];
                        SSD1351_ftoa(val, tmp, sizeof(tmp), (uint8_t)precision);
                        size_t n = strnlen(tmp, sizeof(tmp));
                        if ((size_t)(dest - str) + n > SSD1351_WIDTH)
                        {
                                n = SSD1351_WIDTH - (size_t)(dest - str);
                        }
                        memcpy(dest, tmp, n);
                        dest += n;
                        break;
                }
                case '%':
                        *dest++ = '%';
                        break;
                default:
                        *dest++ = *fmt;
                        break;
                }
                fmt++;
        }

        *dest = '\0';
        va_end(argptr);

        SSD1351writeString((uint8_t *)str, Font, color);
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
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351_DrawImage
// 処理概要     指定座標に画像を描画する
// 引数         x y:描画開始座標 w h:画像の幅と高さ data:画像データ
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351drawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data)
{
	if((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) return;
	if((x + w - 1) >= SSD1351_WIDTH) return;
	if((y + h - 1) >= SSD1351_HEIGHT) return;

    // SSD1351setAddressWindow(x, y, x+w-1, y+h-1);
    // SSD1351writeData((uint8_t*)data, sizeof(uint16_t)*w*h);

	for (uint32_t i = 0; i < w*h; i++)
	{
#ifdef SSD1351_LITTLEENDIAN
		SSD1351_Buffer.u16[i] = data[i];
#else
		// MSB LSBを逆転させる
		SSD1351_Buffer.u8[i*2] = (data[i] >> 8) & 0xFF;	//MSBを代入
		SSD1351_Buffer.u8[(i*2)+1] = data[i] & 0xFF;	//LSBを代入
#endif
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351setContrastRGB
// 処理概要     コントラストをRGBで設定する
// 引数         red green blue:各色のコントラスト値(0-255)
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351setContrastRGB(uint8_t red, uint8_t green, uint8_t blue)
{
	SSD1351writeCommand(0xC1); // CONTRASTABC
	{
		uint8_t data[3] = { red, green, blue };
		SSD1351writeData(data, sizeof(data));
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351setContrastMaster
// 処理概要     コントラストをマスターで設定する
// 引数         contrast:コントラスト値(0-255)
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void SSD1351setContrastMaster(uint8_t contrast)
{
    SSD1351writeCommand(0xC7); // CONTRASTMASTER
    {
        uint8_t data[1] = { contrast };
        SSD1351writeData(data, sizeof(data));
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351line
// 処理概要     指定座標を結ぶ直線を描画する
// 引数         x1 y1:開始座標 x2 y2:終了座標 color:描画色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    int16_t dx = abs((int16_t)x2 - (int16_t)x1);
    int16_t sx = x1 < x2 ? 1 : -1;
    int16_t dy = -abs((int16_t)y2 - (int16_t)y1);
    int16_t sy = y1 < y2 ? 1 : -1;
    int16_t err = dx + dy;
    while (1)
    {
        SSD1351drawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2)
            break;
        int16_t e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351drawArc
// 処理概要     円弧を描画する
// 引数         x y:中心座標 radius:半径 start_angle:開始角度 sweep:角度範囲 color:色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351drawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, uint16_t color)
{
    if (sweep > 360)
        sweep = 360;
    float start = start_angle * M_PI / 180.0f;
    float end = (start_angle + sweep) * M_PI / 180.0f;
    float step = 1.0f / radius;
    for (float a = start; a <= end; a += step)
    {
        uint8_t xa = x + (int16_t)(cosf(a) * radius);
        uint8_t ya = y + (int16_t)(sinf(a) * radius);
        SSD1351drawPixel(xa, ya, color);
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351drawArcWithRadiusLine
// 処理概要     半径線付きの円弧を描画する
// 引数         x y:中心座標 radius:半径 start_angle:開始角度 sweep:角度範囲 color:色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351drawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, uint16_t color)
{
    SSD1351drawArc(x, y, radius, start_angle, sweep, color);
    float a1 = start_angle * M_PI / 180.0f;
    float a2 = (start_angle + sweep) * M_PI / 180.0f;
    uint8_t x1 = x + (int16_t)(cosf(a1) * radius);
    uint8_t y1 = y + (int16_t)(sinf(a1) * radius);
    uint8_t x2 = x + (int16_t)(cosf(a2) * radius);
    uint8_t y2 = y + (int16_t)(sinf(a2) * radius);
    SSD1351line(x, y, x1, y1, color);
    SSD1351line(x, y, x2, y2, color);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351drawCircle
// 処理概要     円を描画する
// 引数         x y:中心座標 radius:半径 color:色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351drawCircle(uint8_t x, uint8_t y, uint8_t radius, uint16_t color)
{
    int16_t x0 = radius;
    int16_t y0 = 0;
    int16_t err = 0;

    while (x0 >= y0)
    {
        SSD1351drawPixel(x + x0, y + y0, color);
        SSD1351drawPixel(x + y0, y + x0, color);
        SSD1351drawPixel(x - y0, y + x0, color);
        SSD1351drawPixel(x - x0, y + y0, color);
        SSD1351drawPixel(x - x0, y - y0, color);
        SSD1351drawPixel(x - y0, y - x0, color);
        SSD1351drawPixel(x + y0, y - x0, color);
        SSD1351drawPixel(x + x0, y - y0, color);

        if (err <= 0)
        {
            y0++;
            err += 2 * y0 + 1;
        }
        if (err > 0)
        {
            x0--;
            err -= 2 * x0 + 1;
        }
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351fillCircle
// 処理概要     円を塗りつぶす
// 引数         x y:中心座標 radius:半径 color:色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351fillCircle(uint8_t x, uint8_t y, uint8_t radius, uint16_t color)
{
    for (int16_t y0 = -radius; y0 <= radius; y0++)
    {
        for (int16_t x0 = -radius; x0 <= radius; x0++)
        {
            if (x0 * x0 + y0 * y0 <= radius * radius)
            {
                SSD1351drawPixel(x + x0, y + y0, color);
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351polyline
// 処理概要     複数の頂点を結ぶ折れ線を描画する
// 引数         par_vertex:頂点配列 par_count:頂点数 color:色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351polyline(const SSD1351_VERTEX* par_vertex, uint16_t par_count, uint16_t color)
{
    if (par_count < 2)
        return;
    for (uint16_t i = 1; i < par_count; i++)
    {
        SSD1351line(par_vertex[i - 1].x, par_vertex[i - 1].y,
                    par_vertex[i].x, par_vertex[i].y, color);
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351drawRectangle
// 処理概要     四角形の枠を描画する
// 引数         x1 y1:左上座標 x2 y2:右下座標 color:色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    SSD1351line(x1, y1, x2, y1, color);
    SSD1351line(x2, y1, x2, y2, color);
    SSD1351line(x2, y2, x1, y2, color);
    SSD1351line(x1, y2, x1, y1, color);
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 SSD1351fillRectangle
// 処理概要     四角形を塗りつぶす
// 引数         x1 y1:左上座標 x2 y2:右下座標 color:色
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void SSD1351fillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    for (uint8_t yy = y1; yy <= y2; yy++)
    {
        SSD1351line(x1, yy, x2, yy, color);
    }
}
///////////////////////////////////////////////////////////////////////////
// モジュール名	SSD1351InvertColors
// 処理概要		ディスプレイの色反転
// 引数			invert: true 反転 false 反転無効
// 戻り値		なし
///////////////////////////////////////////////////////////////////////////
void SSD1351InvertColors(bool invert)
{
    SSD1351writeCommand(invert ? 0xA7 /* INVERTDISPLAY */ : 0xA6 /* NORMALDISPLAY */);
}
