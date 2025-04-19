//====================================//
// �C���N���[�h
//====================================//
#include "BMI088.h"
//====================================//
// �O���[�o���ϐ��̐�
//====================================//
axis accele = {0.0F, 0.0F, 0.0F};
axis gyro = {0.0F, 0.0F, 0.0F};
axis gyroTotal = {0.0F, 0.0F, 0.0F};
axis angle = {0.0F, 0.0F, 0.0F};
IMUval BMI088val;
volatile bool spi_BMI088_tx_done = false;
volatile bool spi_BMI088_rx_done = false;

int16_t angleOffset[3] = {0, 0, 0};
bool calibratIMU = false;
/////////////////////////////////////////////////////////////////////
// ���W���[���� BMI088ReadByteG
// �����T�v     �w�背�W�X�^�̒l��ǂݏo��(�W���C���Z���T��)
// ����         reg: ���W�X�^�̃A�h���X
// �߂�l       �ǂݏo�����l
////////////////////////////////////////////////////////////////////
uint8_t BMI088_ReadByte(bool sensorType, uint8_t reg)
{
	uint8_t txData[2]={reg | 0x80, 0x0}, rxData[2] = {0x0};

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 0;
	} else {
		BMI088_CSB2 = 0;
	}
	
	BMI088_SPI_FUNC(txData, sizeof(txData), rxData, sizeof(rxData));
	spi_BMI088_rx_done = false;
	while(!spi_BMI088_rx_done);

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 1;
	} else {
		BMI088_CSB2 = 1;
	}

	return rxData[1];
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� BMI088WriteByteG
// �����T�v     �w�背�W�X�^�ɒl����������(�W���C���Z���T��)
// ����         reg: ���W�X�^�̃A�h���X val: �������ޒl
// �߂�l       �Ȃ�
////////////////////////////////////////////////////////////////////
void BMI088_WriteByte(bool sensorType, uint8_t reg, uint8_t val)
{
	uint8_t txData[2] = {reg, val}, rxData[2];

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 0;
	} else {
		BMI088_CSB2 = 0;
	}

	BMI088_SPI_FUNC(txData, 2, rxData, 2);
	spi_BMI088_tx_done = false;
	while(!spi_BMI088_tx_done);

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 1;
	} else {
		BMI088_CSB2 = 1;
	}
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� BMI088ReadAxisDataG
// �����T�v     �w�背�W�X�^�̓ǂݏo��(�W���C���Z���T��)
// ����         reg:���W�X�^�A�h���X
// �߂�l       �ǂݏo�����f�[�^
/////////////////////////////////////////////////////////////////////
void BMI088_ReadAxisData(bool sensorType, uint8_t reg, uint8_t *rxData, uint8_t rxNum)
{
	uint8_t txData[20] = {0}, rxDatabuff[20];

	txData[0] = reg | 0x80; // ���M�p�f�[�^�ɕϊ�

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 0;
	} else {
		BMI088_CSB2 = 0;
	}

	BMI088_SPI_FUNC(txData, rxNum+1, rxDatabuff, rxNum+1);
	spi_BMI088_tx_done = false;
	while(!spi_BMI088_tx_done);

	memcpy(rxData,rxDatabuff+1,rxNum); // ���W�X�^���M���̎�M�f�[�^�������ăR�s�[

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 1;
	} else {
		BMI088_CSB2 = 1;
	}
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� initBMI088
// �����T�v     �����ݒ�p�����[�^�̏�������
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
bool BMI088_init(void)
{
	BMI088_ReadByte(ACCELE, REG_ACC_CHIP_ID); // �����x�Z���TSPI���[�h�ɐ؂�ւ�(SPI�_�~�[���[�h)
	BMI088_WriteByte(ACCELE, REG_ACC_SOFTRESET, 0xB6); // �����x�Z���T �\�t�g�E�F�A���Z�b�g
	BMI088_ReadByte(ACCELE, REG_ACC_CHIP_ID); // �����x�Z���TSPI���[�h�ɐ؂�ւ�(SPI�_�~�[���[�h)
	BMI088_WriteByte(GYRO, REG_GYRO_SOFTRESET, 0xB6); // �W���C���Z���T �\�t�g�E�F�A���Z�b�g
	R_BSP_SoftwareDelay(20,BSP_DELAY_MILLISECS);
	
	BMI088val.Aid = BMI088_ReadByte(ACCELE, REG_ACC_CHIP_ID);
	BMI088val.Gid = BMI088_ReadByte(GYRO, REG_GYRO_CHIP_ID);
	
	if (BMI088val.Aid == 0x1e && BMI088val.Gid == 0x0f)
	{
		// �R���t�B�O�ݒ�

		// ����
		BMI088_ReadByte(ACCELE, REG_ACC_CHIP_ID); // �����x�Z���TSPI���[�h�ɐ؂�ւ�(SPI�_�~�[���[�h)
		BMI088_WriteByte(ACCELE, REG_ACC_SOFTRESET, 0xB6); // �\�t�g�E�F�A���Z�b�g

		BMI088_WriteByte(ACCELE, REG_ACC_PWR_CTRL, 0x04); // �����x�Z���T�m�[�}�����[�h�Ɉڍs
		R_BSP_SoftwareDelay(450,BSP_DELAY_MILLISECS);

		BMI088_WriteByte(ACCELE, REG_ACC_RANGE, 0x01); // �����W��6g�ɐݒ�
		BMI088_WriteByte(ACCELE, REG_ACC_CONF, 0xA9);  // ODR��200Hz�ɐݒ�
		
		// �W���C��
		BMI088_WriteByte(GYRO, REG_GYRO_SOFTRESET, 0xB6); // �\�t�g�E�F�A���Z�b�g
		BMI088_WriteByte(GYRO, REG_GYRO_BANDWISTH, 0x83); // ODR��200Hz�ɐݒ�

		// BMI088val.aaa = BMI088_ReadByte(GYRO, REG_GYRO_BANDWISTH);
		// BMI088val.bbb = BMI088_ReadByte(GYRO, 0x3e);
		// ���[�h�ύX

		return true;
	}
	else
	{
		return false;
	}
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� BMI088getGyro
// �����T�v     �p���x�̎擾
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void BMI088_getGyro(void)
{
	uint8_t rawData[6];
	int16_t gyroVal[3];

	// �p���x�̐��f�[�^���擾
	BMI088_ReadAxisData(GYRO, REG_RATE_X_LSB, rawData, 6);

	// LSB��MSB������
	gyroVal[0] = ((rawData[1] << 8) | rawData[0]) - angleOffset[0];
	gyroVal[1] = ((rawData[3] << 8) | rawData[2]) - angleOffset[1];
	gyroVal[2] = ((rawData[5] << 8) | rawData[4]) - angleOffset[2];

	BMI088val.gyro.x = (float)gyroVal[0] / GYROLSB * COEFF_DPD;
	BMI088val.gyro.y = (float)gyroVal[1] / GYROLSB * COEFF_DPD;
	BMI088val.gyro.z = (float)gyroVal[2] / GYROLSB * COEFF_DPD;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� BMI088getAccele
// �����T�v     �����x�̎擾
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void BMI088_getAccele(void)
{
	uint8_t rawData[8];
	int16_t accelVal[3];

	// �����x�̐��f�[�^���擾
	BMI088_ReadAxisData(ACCELE, REG_ACC_X_LSB, rawData, 6);
	// LSB��MSB������
	accelVal[0] = (rawData[1] << 8) | rawData[0];
	accelVal[1] = (rawData[3] << 8) | rawData[2];
	accelVal[2] = (rawData[5] << 8) | rawData[4];

	BMI088val.accele.x = (float)accelVal[0] / ACCELELSB;
	BMI088val.accele.y = (float)accelVal[1] / ACCELELSB;
	BMI088val.accele.z = (float)accelVal[2] / ACCELELSB;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� BMI088getTemp
// �����T�v     ���x�̎擾
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void BMI088_getTemp(void)
{
	uint8_t rawData[3];
	uint16_t tempValu;
	int16_t tempVal;

	// ���x�̐��f�[�^���擾
	BMI088_ReadAxisData(ACCELE, REG_TEMP_MSB, rawData, 2);
	// LSB��MSB������
	tempValu = (uint16_t)((rawData[0] << 3) | (rawData[1] >> 5));
	if (tempValu > 1023)
	{
		tempVal = ~tempValu + 0x8000;
	}
	else
	{
		tempVal = tempValu;
	}

	BMI088val.temp = ((float)tempVal * 0.125F) + 23.0F;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� calcDegrees
// �����T�v     �p�x�̌v�Z
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void calcDegrees(void)
{
	BMI088val.angle.x += BMI088val.gyro.x * DEFF_TIME;
	BMI088val.angle.y += BMI088val.gyro.y * DEFF_TIME;
	BMI088val.angle.z += BMI088val.gyro.z * DEFF_TIME;
}
/////////////////////////////////////////////////////////////////////
// ���W���[���� cariblationIMU
// �����T�v     �p���x�L�����u���[�V����
// ����         �Ȃ�
// �߂�l       �Ȃ�
/////////////////////////////////////////////////////////////////////
void calibrationIMU(void)
{
	static int32_t angleInt[3];
	static uint16_t i = 0;
	uint8_t rawData[6];
	int16_t gyroVal[3];

	if (i < (uint32_t)(1.0 / DEFF_TIME))
	{
		// �p���x�̐��f�[�^���擾
		BMI088_ReadAxisData(GYRO, REG_RATE_X_LSB, rawData, 6);
		// LSB��MSB������
		gyroVal[0] = (rawData[1] << 8) | rawData[0];
		gyroVal[1] = (rawData[3] << 8) | rawData[2];
		gyroVal[2] = (rawData[5] << 8) | rawData[4];

		angleInt[0] += gyroVal[0];
		angleInt[1] += gyroVal[1];
		angleInt[2] += gyroVal[2];
		i++;
	}
	else
	{
		angleOffset[0] = angleInt[0] / i;
		angleOffset[1] = angleInt[1] / i;
		angleOffset[2] = angleInt[2] / i;
		angleInt[0] = 0;
		angleInt[1] = 0;
		angleInt[2] = 0;
		i = 0;
		calibratIMU = false;
	}
}