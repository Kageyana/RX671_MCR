//====================================//
// �C���N���[�h
//====================================//
#include "ssd1351.h"
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
volatile bool spi_ssd1351_tx_done = false;

typedef union {
    uint16_t u16[SSD1351_BUFFER_SIZE];
    uint8_t u8[SSD1351_BUFFER_SIZE*2];
} U16ToU8_Union;

U16ToU8_Union SSD1351_Buffer;	// Screenbuffer

/////////////////////////////////////////////////////////////////////
// ���W���[���� SSD1351_Reset
// �����T�v     SSD1351�����Z�b�g����
// ����         �Ȃ�
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
static void SSD1351_Reset(void) {
    SSD1351_RES_PORT = 1;
	R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
    SSD1351_RES_PORT = 0;
    R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
    SSD1351_RES_PORT = 1;
    R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� SSD1351_WriteCommand
// �����T�v     �R�}���h���M
// ����         cmd:�R�}���h�f�[�^
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
static void SSD1351_WriteCommand(uint8_t cmd) {
	uint8_t rxData[1];

	SSD1351_CS_PORT = 0;
	SSD1351_DC_PORT = 0;
	SSD1351_SPI_FUNC(&cmd, sizeof(cmd), rxData, sizeof(cmd));
	spi_ssd1351_tx_done = false;
	while(!spi_ssd1351_tx_done);

	SSD1351_CS_PORT = 1;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� SSD1351_WriteData
// �����T�v     �f�[�^���M
// ����         buff:�f�[�^�z�� buff_size:�f�[�^��
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
static void SSD1351_WriteData(uint8_t* buff, size_t buff_size) {
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
// ���W���[���� SSD1351_SetAddressWindow
// �����T�v     �\���J�n�ʒu�̐ݒ�
// ����         x0 y0:�J�n���W(����) x1 y1:�I�����W(�E��)
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
static void SSD1351_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    SSD1351_WriteCommand(0x15); // SETCOLUMN
    {
        uint8_t data[] = { x0 & 0xFF, x1 & 0xFF };
        SSD1351_WriteData(data, sizeof(data));
    }

    // row address set
    SSD1351_WriteCommand(0x75); // SETROW
    {
        uint8_t data[] = { y0 & 0xFF, y1 & 0xFF };
        SSD1351_WriteData(data, sizeof(data));
    }

    // write to RAM
    SSD1351_WriteCommand(0x5C); // WRITERAM
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� SSD1351_Init
// �����T�v     ������
// ����         �Ȃ�
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
void SSD1351_Init(void) {
    SSD1351_Reset();

    // command list is based on https://github.com/adafruit/Adafruit-SSD1351-library

    SSD1351_WriteCommand(0xFD); // COMMANDLOCK
    {
        uint8_t data[] = { 0x12 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xFD); // COMMANDLOCK
    {
        uint8_t data[] = { 0xB1 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xAE); // DISPLAYOFF
    SSD1351_WriteCommand(0xB3); // CLOCKDIV
    SSD1351_WriteCommand(0xF1); // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    SSD1351_WriteCommand(0xCA); // MUXRATIO
    {
        uint8_t data[] = { 0x7F }; // 127
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xA0); // SETREMAP
    {
        uint8_t data[] = { 0x74 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0x15); // SETCOLUMN
    {
        uint8_t data[] = { 0x00, 0x7F };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0x75); // SETROW
    {
        uint8_t data[] = { 0x00, 0x7F };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xA1); // STARTLINE
    {
        uint8_t data[] = { 0x00 }; // 96 if display height == 96
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xA2); // DISPLAYOFFSET
    {
        uint8_t data[] = { 0x00 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xB5); // SETGPIO
    {
        uint8_t data[] = { 0x00 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xAB); // FUNCTIONSELECT
    {
        uint8_t data[] = { 0x01 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xB1); // PRECHARGE
    {
        uint8_t data[] = { 0x32 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xBE); // VCOMH
    {
        uint8_t data[] = { 0x05 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xA6); // NORMALDISPLAY (don't invert)
    SSD1351_WriteCommand(0xC1); // CONTRASTABC
    {
        uint8_t data[] = { 0xC8, 0x80, 0xC8 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xC7); // CONTRASTMASTER
    {
        uint8_t data[] = { 0x0F };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xB4); // SETVSL
    {
        uint8_t data[] = { 0xA0, 0xB5, 0x55 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xB6); // PRECHARGE2
    {
        uint8_t data[] = { 0x01 };
        SSD1351_WriteData(data, sizeof(data));
    }
    SSD1351_WriteCommand(0xAF); // DISPLAYON
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� SSD1351_Fill
// �����T�v     �o�b�t�@��P��̐F�Ŗ��߂�
// ����         color:16bit�F�f�[�^
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
void SSD1351_Fill(uint16_t color)
{
	uint32_t i;

	for (i = 0; i < sizeof(SSD1351_Buffer)/sizeof(uint16_t); i++)
	{
		SSD1351_Buffer.u16[i] = color;
	}
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� SSD1351_UpdateScreen
// �����T�v     �o�b�t�@�𑗐M����
// ����         �Ȃ�
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
/* Write the screenbuffer with changed to the screen */
void SSD1351_UpdateScreen(void)
{
	SSD1351_SetAddressWindow(0,0,SSD1351_WIDTH-1,SSD1351_HEIGHT-1);

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

	SSD1351_CS_PORT = 1;	// select OLED

}

