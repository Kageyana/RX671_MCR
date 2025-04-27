//====================================//
// �C���N���[�h
//====================================//
#include "timer.h"
#include "BMI088.h"
#include "ssd1351.h"
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
uint16_t cnt10 = 0;
uint16_t cnt0 = 0;
extern uint8_t useIMU=0, useDisplay=0;
volatile bool update_display = false;
/////////////////////////////////////////////////////////////////////
// ���W���[���� Interrupt1ms
// �����T�v     �^�C�}�[���荞��(1ms)
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void Interrupt1ms(void)
{
	cnt0++;
	cnt10++;

	SSD1351_UpdateScreen_Chunked();

	switch (cnt10)
	{
	case 1:
		
		if(useDisplay == 0 && useIMU == 0)
		{
			useIMU = 1;
			BMI088_getGyro(); // �p���x�擾
			BMI088_getTemp();
			// BMI088_getAccele();
			calcDegrees();	 // �p�x�v�Z
			useIMU = 0;
		}
		
		break;
	case 2:
		// if(useIMU == 0 && useDisplay == 0)
		// {
		// 	useDisplay = 1;
			
		// 	useDisplay = 0;
		// }
		
		break;
	case 10:
		cnt10 = 0;
		break;
	}
}
