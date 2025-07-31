#ifndef MOTOR_H_
#define MOTOR_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"

#include <stdint.h>
#include <stdlib.h>
//====================================//
// シンボル定義
//====================================//
#define SERVO_CENTER	2045	// サーボセンターのAD値
#define SERVO_LIMIT		1500	// サーボリミットAD値±
#define TGR_MOTOR		3598	// ジェネラルレジスタ初期値(駆動モータ) TGRA初期値-1
#define TGR_SERVO		3598	// ジェネラルレジスタ初期値(サーボ)	 TGRA初期値-1
/*************************** 自動生成関数 **************************/
// MTUスタート
#define SET_MTU_FRONT	R_Config_MTU4_Start()
#define SET_MTU_REAR	R_Config_MTU0_Start()
#define SET_MTU_SERVO	R_Config_MTU3_Start()

// 左前輪
#define DIR_FL		PORT6.PODR.BIT.B6	// モータ回転方向端子
#define PWM_FL_OUT	MTU4.TGRB			// PWM出力
#define GET_MOTORCURRENT_FL_VAL	R_Config_S12AD1_Get_ValueResult(ADCHANNEL7, &motorCurrentFL)
// 右前輪
#define DIR_FR		PORT7.PODR.BIT.B3
#define PWM_FR_OUT	MTU4.TGRD
#define GET_MOTORCURRENT_FR_VAL	R_Config_S12AD1_Get_ValueResult(ADCHANNEL0, &motorCurrentFR)
// 左後輪
#define DIR_RL		PORT2.PODR.BIT.B7
#define PWM_RL_OUT	MTU0.TGRB
#define GET_MOTORCURRENT_RL_VAL	R_Config_S12AD1_Get_ValueResult(ADCHANNEL10, &motorCurrentRL)
// 右後輪
#define DIR_RR		PORT2.PODR.BIT.B5
#define PWM_RR_OUT	MTU0.TGRD
#define GET_MOTORCURRENT_RR_VAL	R_Config_S12AD1_Get_ValueResult(ADCHANNEL11, &motorCurrentRR)
// サーボ1
#define DIR_SERVO	PORTB.PODR.BIT.B0
#define PWM_SERVO_OUT	MTU3.TGRB
#define GET_SERVO_CURRENT_F_VAL	R_Config_S12AD1_Get_ValueResult(ADCHANNEL3, &servoCurrentF)
// サーボ2
#define DIR_LANCER	PORT3.PODR.BIT.B3
#define PWM_LANCER_OUT	MTU3.TGRD
#define GET_SERVO_CURRENT_R_VAL	R_Config_S12AD1_Get_ValueResult(ADCHANNEL9, &servoCurrentR)

// ポテンショメータ
#define GET_POT_FRONT_VAL R_Config_S12AD1_Get_ValueResult(ADCHANNEL1, &potFrontVal)
#define GET_POT_REAR_VAL R_Config_S12AD1_Get_ValueResult(ADCHANNEL6, &potRearVal)

/******************************************************************************************/

//====================================//
// グローバル変数の宣言
//====================================//
// モーター関連
extern int16_t	accele_fR;		// 右前モーターPWM値
extern int16_t	accele_fL;		// 左前モーターPWM値
extern int16_t	accele_rR;		// 右後モーターPWM値
extern int16_t	accele_rL;		// 左後モーターPWM値
extern int16_t	sPwm;			// サーボモーターPWM値

extern uint16_t motorCurrentFL;	// 左前モーター電流値
extern uint16_t motorCurrentFR;	// 右前モーター電流値
extern uint16_t motorCurrentRL;	// 左後モーター電流値
extern uint16_t motorCurrentRR;	// 右後モーター電流値
extern uint16_t servoCurrentF;	// 前サーボ電流値
extern uint16_t servoCurrentR;	// 後ろサーボ電流値

extern uint16_t potFrontVal;	// 前ポテンショメータの値
extern uint16_t potRearVal;		// 後ポテンショメータの値
//====================================//
// プロトタイプ宣言
//====================================//
void InitMotor(void);
void GetMotorADVal(void);
void MotorPwmOut( int16_t accelefL, int16_t accelefR
				, int16_t accelerL, int16_t accelerR );
void ServoPwmOut1( int16_t servopwm );
void ServoPwmOut2( int16_t servopwm );

#endif // MOTOR_H_