//====================================//
// インクルード
//====================================//
#include "BMI088.h"
//====================================//
// グローバル変数の宣
//====================================//
axis accele = {0.0F, 0.0F, 0.0F};
axis gyro = {0.0F, 0.0F, 0.0F};
axis gyroTotal = {0.0F, 0.0F, 0.0F};
axis angle = {0.0F, 0.0F, 0.0F};
IMUval BMI088val;
volatile bool spi_BMI088_tx_done = false;
volatile bool spi_BMI088_rx_done = false;
bool bmi088_read_locked = false;

int16_t angleOffset[3] = {0, 0, 0};
volatile bool calibratIMU = false;
/////////////////////////////////////////////////////////////////////
// モジュール名 BMI088ReadByteG
// 処理概要     指定レジスタの値を読み出す(ジャイロセンサ部)
// 引数         reg: レジスタのアドレス
// 戻り値       読み出した値
////////////////////////////////////////////////////////////////////
uint8_t BMI088readByte(bool sensorType, uint8_t reg)
{
	uint8_t txData[2]={reg | 0x80, 0x0}, rxData[2] = {0x0};

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 0;
	} else {
		BMI088_CSB2 = 0;
	}
	
	spi_BMI088_rx_done = false;
	BMI088_SPI_FUNC(txData, sizeof(txData), rxData, sizeof(rxData));

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
// モジュール名 BMI088WriteByteG
// 処理概要     指定レジスタに値を書き込む(ジャイロセンサ部)
// 引数         reg: レジスタのアドレス val: 書き込む値
// 戻り値       なし
////////////////////////////////////////////////////////////////////
void BMI088writeByte(bool sensorType, uint8_t reg, uint8_t val)
{
	uint8_t txData[2] = {reg, val}, rxData[2];

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 0;
	} else {
		BMI088_CSB2 = 0;
	}

	spi_BMI088_tx_done = false;
	BMI088_SPI_FUNC(txData, 2, rxData, 2);

	while(!spi_BMI088_tx_done);

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 1;
	} else {
		BMI088_CSB2 = 1;
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 BMI088ReadAxisDataG
// 処理概要     指定レジスタの読み出し(ジャイロセンサ部)
// 引数         reg:レジスタアドレス
// 戻り値       読み出したデータ
/////////////////////////////////////////////////////////////////////
void BMI088readAxisData(bool sensorType, uint8_t reg, uint8_t *rxData, uint8_t rxNum)
{
	uint8_t txData[20] = {0}, rxDatabuff[20];

	txData[0] = reg | 0x80; // 送信用データに変換

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 0;
	} else {
		BMI088_CSB2 = 0;
	}

	spi_BMI088_tx_done = false;
	BMI088_SPI_FUNC(txData, rxNum+1, rxDatabuff, rxNum+1);

	while(!spi_BMI088_tx_done);

	memcpy(rxData,rxDatabuff+1,rxNum); // レジスタ送信時の受信データを除いてコピー

	if(sensorType == ACCELE)
	{
		BMI088_CSB1 = 1;
	} else {
		BMI088_CSB2 = 1;
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 initBMI088
// 処理概要     初期設定パラメータの書き込み
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
bool BMI088init(void)
{
	R_BSP_SoftwareDelay(10,BSP_DELAY_MILLISECS);
	BMI088readByte(ACCELE, REG_ACC_CHIP_ID); // 加速度センサSPIモードに切り替え(SPIダミーリード)
	R_BSP_SoftwareDelay(100,BSP_DELAY_MILLISECS);
	BMI088val.Aid = BMI088readByte(ACCELE, REG_ACC_CHIP_ID); // ノーマルモード移行前にチップIDを読む
	BMI088val.Aid = BMI088readByte(ACCELE, REG_ACC_CHIP_ID); // ノーマルモード移行前にチップIDを読む

	BMI088writeByte(GYRO, REG_GYRO_SOFTRESET, 0xB6); // ジャイロセンサ ソフトウェアリセット
	R_BSP_SoftwareDelay(30,BSP_DELAY_MILLISECS);
	BMI088val.Gid = BMI088readByte(GYRO, REG_GYRO_CHIP_ID);
	
	if (BMI088val.Gid == 0x0f && BMI088val.Aid == 0x1e)
	{
		// コンフィグ設定

		// 加速度
		BMI088writeByte(ACCELE, REG_ACC_PWR_CTRL, 0x04); // 加速度センサノーマルモードに移行
		R_BSP_SoftwareDelay(450,BSP_DELAY_MILLISECS);
		BMI088writeByte(ACCELE, REG_ACC_RANGE, 0x01); // レンジを6gに設定
		BMI088writeByte(ACCELE, REG_ACC_CONF, 0xA9);  // ODRを200Hzに設定
		
		// ジャイロ
		BMI088writeByte(GYRO, REG_GYRO_SOFTRESET, 0xB6); // ソフトウェアリセット
		R_BSP_SoftwareDelay(100,BSP_DELAY_MILLISECS);
		BMI088writeByte(GYRO, REG_GYRO_BANDWISTH, 0x03); // ODRを400Hz バンドフィルタ47Hzに設定

		BMI088val.Initialized = 1;

		return true;
	}
	else
	{
		return false;
	}
}
/////////////////////////////////////////////////////////////////////
// モジュール名 BMI088getGyro
// 処理概要     角速度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void BMI088getGyro(void)
{
	if(!BMI088val.Initialized)
	{
		return;
	}
	uint8_t rawData[6];
	int16_t gyroVal[3];

	// 角速度の生データを取得
	BMI088readAxisData(GYRO, REG_RATE_X_LSB, rawData, 6);

	// LSBとMSBを結合
	gyroVal[0] = ((rawData[1] << 8) | rawData[0]) - angleOffset[0];
	gyroVal[1] = ((rawData[3] << 8) | rawData[2]) - angleOffset[1];
	gyroVal[2] = ((rawData[5] << 8) | rawData[4]) - angleOffset[2];

	BMI088val.gyro.x = (float)gyroVal[0] / GYROLSB * COEFF_DPD;
	BMI088val.gyro.y = (float)gyroVal[1] / GYROLSB * COEFF_DPD;
	BMI088val.gyro.z = (float)gyroVal[2] / GYROLSB * COEFF_DPD;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 BMI088getAccele
// 処理概要     加速度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void BMI088getAccele(void)
{
	if(!BMI088val.Initialized)
	{
		return;
	}
	uint8_t rawData[8];
	int16_t accelVal[3];

	// 加速度の生データを取得
	BMI088readAxisData(ACCELE, REG_ACC_X_LSB, rawData, 7);
	// LSBとMSBを結合
	// 最初のデータは破棄する 
	accelVal[0] = (rawData[2] << 8) | rawData[1];
	accelVal[1] = (rawData[4] << 8) | rawData[3];
	accelVal[2] = (rawData[6] << 8) | rawData[5];

	BMI088val.accele.x = (float)accelVal[0] / ACCELELSB;
	BMI088val.accele.y = (float)accelVal[1] / ACCELELSB;
	BMI088val.accele.z = (float)accelVal[2] / ACCELELSB;
}
/////////////////////////////////////////////////////////////////////
// モジュール名 BMI088getTemp
// 処理概要     温度の取得
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void BMI088getTemp(void)
{
	if(!BMI088val.Initialized)
	{
		return;
	}
	uint8_t rawData[3];
	uint16_t tempValu;
	int16_t tempVal;

	// 温度の生データを取得
	BMI088readAxisData(ACCELE, REG_TEMP_MSB, rawData, 3);
	// LSBとMSBを結合
	// 最初のデータは破棄する 
	tempValu = (uint16_t)((rawData[1] << 3) | (rawData[2] >> 5));
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
// モジュール名 calcDegrees
// 処理概要     角度の計算(相補フィルタ)
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void calcDegrees(void)
{
	if(!BMI088val.Initialized)
	{
		return;
	}
	// ジャイロ積分による角度更新 度数法
    BMI088val.angle.x += BMI088val.gyro.x * DEFF_TIME;  // pitch
    BMI088val.angle.y += BMI088val.gyro.y * DEFF_TIME;  // roll
    BMI088val.angle.z += BMI088val.gyro.z * DEFF_TIME;  // yaw（補正しない）

    // 加速度からのピッチ・ロール角算出 度数法
    float pitchAccele = atan2f(BMI088val.accele.x,sqrtf(BMI088val.accele.y * BMI088val.accele.y +BMI088val.accele.z * BMI088val.accele.z)) * 180.0f / M_PI;

    float rollAccele = atan2f(BMI088val.accele.y, BMI088val.accele.z) * 180.0f / M_PI;

    // 相補フィルタでドリフト補正
    BMI088val.angle.x = COEFF_COMPFILTER * BMI088val.angle.x + (1.0f - COEFF_COMPFILTER) * pitchAccele;
    BMI088val.angle.y = COEFF_COMPFILTER * BMI088val.angle.y + (1.0f - COEFF_COMPFILTER) * rollAccele;
    // Z軸（ヨー角）は加速度では補正できないのでそのまま
}
/////////////////////////////////////////////////////////////////////
// モジュール名 cariblationIMU
// 処理概要     角速度キャリブレーション
// 引数         なし
// 戻り値       なし
/////////////////////////////////////////////////////////////////////
void calibrationIMU(void)
{
	static int32_t angleInt[3];
	static uint16_t i = 0;
	uint8_t rawData[6];
	int16_t gyroVal[3];

	if (i < (uint32_t)(1.0 / DEFF_TIME))
	{
		// 角速度の生データを取得
		BMI088readAxisData(GYRO, REG_RATE_X_LSB, rawData, 6);
		// LSBとMSBを結合
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
