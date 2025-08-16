//====================================//
// インクルード
//====================================//
#include "Motor.h"
#include "setup.h"
#include <stdint.h>
//====================================//
// グローバル変数の宣言
//====================================//
// モーター関連
int16_t		accele_fR;		// 右前モーターPWM値
int16_t		accele_fL;		// 左前モーターPWM値
int16_t		accele_rR;		// 右後モーターPWM値
int16_t		accele_rL;		// 左後モーターPWM値
int16_t		sPwm;			// サーボモーターPWM値

// モーター電流値
uint16_t motorCurrentFL;	// 左前モーター電流値
uint16_t motorCurrentFR;	// 右前モーター電流値
uint16_t motorCurrentRL;	// 左後モーター電流値
uint16_t motorCurrentRR;	// 右後モーター電流値
uint16_t servoCurrentF;		// 前サーボ電流値
uint16_t servoCurrentR;		// 後ろサーボ電流値

// ポテンショメータ値
uint16_t potFrontVal;			// 前ポテンショメータの値
uint16_t potRearVal;			// 後ポテンショメータの値
uint16_t Angle0 = SERVO_CENTER;	// サーボセンター値
///////////////////////////////////////////////////////////////////////////
// モジュール名 InitMotor
// 処理概要     モーターPWMの初期化
// 引数         なし
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void InitMotor(void)
{
	SET_MTU_FRONT;
	SET_MTU_REAR;
	SET_MTU_SERVO;
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 GetPotensionADVal
// 処理概要     モーターのPWMの変更
// 引数         accelefL, accelefR(PWMを1～100%で指定)
// 戻り値       な
///////////////////////////////////////////////////////////////////////////
void GetMotorADVal(void)
{
	GET_MOTORCURRENT_FL_VAL;	// 左前モーター電流値の取得
	GET_MOTORCURRENT_FR_VAL;	// 右前モーター電流値の取得
	GET_MOTORCURRENT_RL_VAL;	// 左後モーター電流値の取得
	GET_MOTORCURRENT_RR_VAL;	// 右後モーター電流値の取得
    GET_SERVO_CURRENT_F_VAL;	// 前サーボ電流値の取得
    GET_SERVO_CURRENT_R_VAL;	// 後サーボ電流値の取得

	GET_POT_FRONT_VAL;	// 前ポテンションメーター値の取得
	GET_POT_REAR_VAL;	// 後ポテンションメーター値の取得
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 MotorPwmOut
// 処理概要     モーターのPWMの変更
// 引数         accelefL, accelefR, accelerL, accelerR(PWMを1～1000で指定)
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void MotorPwmOut( int16_t accelefL, int16_t accelefR
	, int16_t accelerL, int16_t accelerR )
{
	uint16_t pwmfl = 0, pwmfr = 0, pwmrl = 0, pwmrr = 0;
	
	accele_fL = accelefL;
	accele_fR = accelefR;
	accele_rL = accelerL;
	accele_rR = accelerR;
	
	if(!modePushcart) {
		pwmfl = TGR_MOTOR * abs(accelefL) / 1000;
		pwmfr = TGR_MOTOR * abs(accelefR) / 1000;
		pwmrl = TGR_MOTOR * abs(accelerL) / 1000;
		pwmrr = TGR_MOTOR * abs(accelerR) / 1000;
	}
	
	MTU.TRWERA.BIT.RWE = 1U; // MTUレジスタの読み書きを許可

	// 左前輪
	if( accelefL >= 0) DIR_FL = 1;
	else DIR_FL = 0;
	if ( accelefL == 1000 || accelefL == -1000 )pwmfl = TGR_MOTOR + 2;
	PWM_FL_OUT = pwmfl;
	
	
	// 右前輪
	if( accelefR >= 0) DIR_FR = 0;
	else DIR_FR = 1;
	if ( accelefR == 1000 || accelefR == -1000 ) pwmfr = TGR_MOTOR + 2;
	PWM_FR_OUT = pwmfr;

	// 左後輪
	if( accelerL >= 0 ) DIR_RL = 1;
	else DIR_RL = 0;
	if ( accelerL == 1000 || accelerL == -1000 ) pwmrl = TGR_MOTOR + 2;
	PWM_RL_OUT = pwmrl;
	
	// 右後輪
	// if( accelerR >= 0 ) DIR_RR = 1;
	// else DIR_RR = 0;
	// if ( accelerR == 1000 || accelerR == -1000 ) pwmrr = TGR_MOTOR + 2;
	// PWM_RR_OUT = pwmrr;
	if( accelerR >= 0 ) DIR_LANCER = 1;
	else DIR_LANCER = 0;
	if ( accelerR == 1000 || accelerR == -1000 ) pwmrr = TGR_MOTOR + 2;
	PWM_LANCER_OUT = pwmrr;


	MTU.TRWERA.BIT.RWE = 0U; // MTUレジスタの読み書きを禁止
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 ServoPwmOut1
// 処理概要     白線トレース時サーボのPWMの変更
// 引数         servopwm(PWMを-1000～1000で指定)
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void ServoPwmOut1( int16_t servopwm )
{
	uint16_t pwm;
	short angle;
	
	sPwm = servopwm;		// ログ用変数に代入
	//servopwm = -servopwm;		// 回転方向を変える
	
	// サーボリミット制御
	angle = getServoAngle();
	
	// 角度によるリミット制御
	if ( angle >= SERVO_LIMIT ) servopwm = -15;
	if ( angle <= -SERVO_LIMIT ) servopwm = 15;
	
	// ポテンションメーターが外れていたら制御しない
	if ( angle > SERVO_LIMIT + 100 ) servopwm = 0;
	if ( angle < -SERVO_LIMIT - 100 ) servopwm = 0;

	pwm = TGR_SERVO * abs(servopwm) / 1000;
	// サーボモータ制御
	if( servopwm > 0) {
		// 正転
		DIR_SERVO = 1;
	} else {
		// 逆転
		DIR_SERVO = 0;
	}
	MTU.TRWERA.BIT.RWE = 1U; // MTUレジスタの読み書きを許可
	PWM_SERVO_OUT = pwm;
	MTU.TRWERA.BIT.RWE = 0U; // MTUレジスタの読み書きを許可
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 ServoPwmOut2
// 処理概要     白線トレース時サーボのPWMの変更
// 引数         servopwm(PWMを-1000～1000で指定)
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void ServoPwmOut2( int16_t servopwm )
{
	uint16_t pwm;
	short angle;
	
	sPwm = servopwm;		// ログ用変数に代入
	//servopwm = -servopwm;		// 回転方向を変える
	
	// // サーボリミット制御
	// angle = getServoAngle();
	
	// // 角度によるリミット制御
	// if ( angle >= SERVO_LIMIT ) servopwm = -15;
	// if ( angle <= -SERVO_LIMIT ) servopwm = 15;
	
	// // ポテンションメーターが外れていたら制御しない
	// if ( angle > SERVO_LIMIT + 100 ) servopwm = 0;
	// if ( angle < -SERVO_LIMIT - 100 ) servopwm = 0;

	pwm = TGR_SERVO * abs(servopwm) / 1000;
	// サーボモータ制御
	if( servopwm > 0) {
		// 正転
		DIR_LANCER = 1;
	} else {
		// 逆転
		DIR_LANCER = 0;
	}
	MTU.TRWERA.BIT.RWE = 1U; // MTUレジスタの読み書きを許可
	PWM_LANCER_OUT = pwm;
	MTU.TRWERA.BIT.RWE = 0U; // MTUレジスタの読み書きを許可
}
/////////////////////////////////////////////////////////////////////
// モジュール名 getAnalogSensor
// 処理概要     アナログセンサのアナログ値で取得
// 引数         なし
// 戻り値       センサ値
/////////////////////////////////////////////////////////////////////
int16_t getServoAngle(void)
{
	return  ( Angle0 - potFrontVal );
}
///////////////////////////////////////////////////////////////////////////
// モジュール名 diff
// 処理概要   	R1,R2,R3及びR4の計算
// 引数         pwm(PWMを-1000～1000で指定)
// 戻り値       なし
///////////////////////////////////////////////////////////////////////////
void diff ( int16_t pwm )
{
	// 角度から内輪、外輪回転差計算(duty.mで計算)
	const int16_t rev_difference_D[] = {
		1000,1000,1000,993,1000,993,986,999,987,979,999,980,971,998,973,
		963,996,967,955,994,960,946,992,953,937,990,947,928,987,940,
		919,984,934,909,981,927,899,977,921,889,973,914,879,968,908,
		868,963,902,857,958,895,846,952,889,835,946,883,823,939,876,
		811,932,870,799,925,864,787,917,858,774,909,852,761,900,846,
		748,891,840,735,881,834,721,871,828,707,860,822,693,849,816,
		678,837,810,663,825,804,648,811,798,632,798,792,616,783,787,
		600,768,781,583,752,775,565,735,770,547,717,764,529,697,758,
		509,677,752,489,655,747,467,631,741,445,605,735,
		};

	int16_t R1, R2, R3, R4;
	int16_t angle2, r1, r2, r3;
	double angle;
	
	angle = (double)getServoAngle()*AD2DEG;		// AD値を角度に変換
	angle2 = abs((int16_t)angle* 3);			// 角度を3倍して配列のインデックスに変換
	
	if ( pwm >= 0 ) {
		r1 = rev_difference_D[ angle2 ];
		r2 = rev_difference_D[ angle2 + 1 ];
		r3 = rev_difference_D[ angle2 + 2 ];
		
		R1 = r1 * pwm / 100;
		R2 = r2 * pwm / 100;
		R3 = r3 * pwm / 100;
		R4 = pwm;
		
		if ( angle >= 0 ) {
			MotorPwmOut(R1, R3, R2, R4);
		} else {
			MotorPwmOut(R3, R1, R4, R2);
		}
	} else {
		r1 = rev_difference_D[ angle2 ];
		r2 = rev_difference_D[ angle2 + 1 ];
		r3 = rev_difference_D[ angle2 + 2 ];
		
		R1 = r1 * pwm / 100;
		R2 = r2 * pwm / 100;
		R3 = r3 * pwm / 100;
		R4 = pwm;
		
		if ( angle >= 0 ) {
			MotorPwmOut(R3, R4, R1, R2);
		} else {
			MotorPwmOut(R4, R3, R2, R1);
		}
	}
}
