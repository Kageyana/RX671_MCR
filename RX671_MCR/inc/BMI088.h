#ifndef BMI088_H_
#define BMI088_H_

//====================================//
// インクルード
//====================================//
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "r_smc_entry.h"
//====================================//
// シンボル定義
//====================================//
//=============自動生成関数=============//
#define BMI088_CSB1 	PORTB.PODR.BIT.B7
#define BMI088_CSB2 	PORTC.PODR.BIT.B5
#define BMI088_SPI_FUNC R_Config_SCI2_SPI_Master_Send_Receive
//=====================================//

// sensor Type
#define GYRO		true
#define ACCELE		false

//  unit settings
#define ACCELELSB 5460.0F
#define GYROLSB 16.384F
#define MAGPLSB 16.0F

#define DEFF_TIME 0.01F
#define COEFF_DPD -1.00F
#define COEFF_COMPFILTER 0.96F
#define IMU_TRANSMIT true
#define IMU_STOP false

/*レジスタアドレス*/
#define REG_GYRO_CHIP_ID 0x00
#define REG_RATE_X_LSB 0x02
#define REG_RATE_X_MSB 0x03
#define REG_RATE_Y_LSB 0x04
#define REG_RATE_Y_MSB 0x05
#define REG_RATE_Z_LSB 0x06
#define REG_RATE_Z_MSB 0x07
#define REG_GYRO_BANDWISTH 0x10
#define REG_GYRO_SOFTRESET 0x14

#define REG_ACC_CHIP_ID 0x00
#define REG_ACC_X_LSB 0x12
#define REG_ACC_CONF 0x40
#define REG_ACC_RANGE 0x41
#define REG_ACC_PWR_CTRL 0x7D
#define REG_ACC_SOFTRESET 0x7E

#define REG_TEMP_MSB 0x22

typedef struct
{
	float x;
	float y;
	float z;
} axis;
typedef struct
{
	axis accele;
	axis gyro;
	axis gyroTotal;
	axis angle;
	float temp;
	uint8_t Aid;
	uint8_t Gid;
	uint8_t Initialized;
} IMUval;
//====================================//
// グローバル変数の宣言
//====================================//
extern volatile bool spi_BMI088_tx_done;
extern volatile bool spi_BMI088_rx_done;
extern IMUval BMI088val;
extern volatile bool calibratIMU;
//====================================//
// プロトタイプ宣言
//====================================//
uint8_t BMI088readByte(bool sensorType, uint8_t reg);
void BMI088writeByte(bool sensorType, uint8_t reg, uint8_t val);
void BMI088readAxisData(bool sensorType, uint8_t reg, uint8_t *rxData, uint8_t rxNum);
bool BMI088init(void);
void BMI088getGyro(void);
void BMI088getAccele(void);
void BMI088getTemp(void);
void calcDegrees(void);
void calibrationIMU(void);

#endif // BMI088_H_
