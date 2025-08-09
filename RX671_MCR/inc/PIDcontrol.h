#ifndef LINETRACE_H_
#define LINETRACE_H_
//====================================//
// インクルード
//====================================//
#include "control.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "linesensor.h"

#include "SDcard.h"
//====================================//
// シンボル定義
//====================================//
#define KP1		22
#define KI1		0
#define KD1		100

#define KP2		15
#define KI2		100
#define KD2		0

#define KP3		10
#define KI3		0
#define KD3		6

typedef struct {
    uint8_t *name;
    int16_t kp;
    int16_t ki;
    int16_t kd;
    float   Int;
    int16_t pwm;
} pidParam;

//====================================//
// グローバル変数の宣言
//====================================//
extern uint8_t	targetSpeed;	// 目標速度
extern int16_t 	targetAngle;    // 目標角度

extern pidParam lineTraceCtrl;
extern pidParam veloCtrl;
extern pidParam angleCtrl;
//====================================//
// プロトタイプ宣言
//====================================//
int16_t getAnalogSensor(void);
void setTargetSpeed (float speed);
void setTargetAngle (int16_t angle);
void writePIDparameters(pidParam *pid);
void readPIDparameters(pidParam *pid);
void motorControlTrace(void);
void motorControlSpeed(void);
void motorControlAngle(void);

#endif // LINETRACE_H_
