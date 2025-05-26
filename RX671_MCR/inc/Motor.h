#ifndef MOTOR_H_
#define MOTOR_H_
//====================================//
// インクルード
//====================================//
#include "r_smc_entry.h"

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
// 右前輪
#define DIR_FR		PORT7.PODR.BIT.B3
#define PWM_FR_OUT	MTU4.TGRD
// 左後輪
#define DIR_RL		PORT2.PODR.BIT.B7
#define PWM_RL_OUT	MTU0.TGRB
// 右後輪
#define DIR_RR		PORT2.PODR.BIT.B5
#define PWM_RR_OUT	MTU0.TGRD
// サーボ1
#define DIR_SERVO		PORTB.PODR.BIT.B7
#define PWM_SERVO_OUT	MTU3.TGRB
// サーボ2
#define DIR_LANCER		PORTB.PODR.BIT.B7
#define PWM_LANCER_OUT	MTU3.TGRD
/******************************************************************************************/

//====================================//
// グローバル変数の宣言
//====================================//
// モーター関連
extern int16_t	accele_fR;		// 右前モーターPWM値
extern int16_t	accele_fL;		// 左前モーターPWM値
extern int16_t	accele_rR;		// 右後モーターPWM値
extern int16_t	accele_rL;		// 左後モーターPWM値
extern int16_t	sPwm;		// サーボモーターPWM値

//====================================//
// プロトタイプ宣言
//====================================//
void motorInit(void);
void motorPwmOut( int16_t accelefL, int16_t accelefR
				, int16_t accelerL, int16_t accelerR );
void servoPwmOut( signed char pwm );

#endif // MOTOR_H_