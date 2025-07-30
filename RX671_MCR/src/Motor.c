//====================================//
// インクルード
//====================================//
#include "Motor.h"
//====================================//
// グローバル変数の宣言
//====================================//
// エンコーダ関連
static unsigned short 	cnt_Encoder;	// エンコーダ値の格納先
static unsigned short	encbuff;		// 前回のエンコーダ値
short					Encoder;			// 1msごとのパルス
unsigned int			EncoderTotal;		// 総走行距離
unsigned int			enc1;				// 走行用距離カウント
unsigned int			enc_slope;			// 坂上距離カウント
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
uint16_t servoCurrentF;	// 前サーボ電流値
uint16_t servoCurrentR;	// 後ろサーボ電流値

// ポテンショメータ値
uint16_t potFrontVal;	// 前ポテンショメータの値
uint16_t potRearVal;		// 後ポテンショメータの値
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
	GET_MOTORCURRENT_FR_VAL;        // 右前モーター電流値の取得
	GET_MOTORCURRENT_RL_VAL;	// 左後モーター電流値の取得
	GET_MOTORCURRENT_RR_VAL;	// 右後モーター電流値の取得

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
	uint16_t pwmfl, pwmfr, pwmrl, pwmrr;
	
	accele_fL = accelefL;
	accele_fR = accelefR;
	accele_rL = accelerL;
	accele_rR = accelerR;
	
	pwmfl = TGR_MOTOR * abs(accelefL) / 1000;
	pwmfr = TGR_MOTOR * abs(accelefR) / 1000;
	pwmrl = TGR_MOTOR * abs(accelerL) / 1000;
	pwmrr = TGR_MOTOR * abs(accelerR) / 1000;
	
	// 左前輪
	if( accelefL >= 0) DIR_FL = 1;
	else DIR_FL = 0;
	if ( accelefL == 1000 || accelefL == -1000 )pwmfl = TGR_MOTOR + 2;
	PWM_FL_OUT = pwmfl;
	
	// 右前輪
	if( accelefR >= 0) DIR_FR = 1;
	else DIR_FR = 0;
	if ( accelefR == 1000 || accelefR == -1000 ) pwmfr = TGR_MOTOR + 2;
	PWM_FR_OUT = pwmfr;

	// 左後輪
	if( accelerL >= 0 ) DIR_RL = 1;
	else DIR_RL = 0;
	if ( accelerL == 1000 || accelerL == -1000 ) pwmrl = TGR_MOTOR + 2;
	PWM_RL_OUT = pwmrl;
	
	// 右後輪
	if( accelerR >= 0 ) DIR_RR = 1;
	else DIR_RR = 0;
	if ( accelerR == 1000 || accelerR == -1000 ) pwmrr = TGR_MOTOR + 2;
	PWM_RR_OUT = pwmrr;
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
		DIR_SERVO = 1;
	} else {
		// 逆転
		DIR_SERVO = 0;
	}
	PWM_SERVO_OUT = pwm;
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
	PWM_LANCER_OUT = pwm;
}