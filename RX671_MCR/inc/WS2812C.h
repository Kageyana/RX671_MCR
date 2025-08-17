#ifndef WS2812C_H_
#define WS2812C_H_
//====================================//
// インクルード
//====================================//
#include <stdint.h>
#include <stdbool.h>
#include "r_smc_entry.h"
#include "r_dmaca_rx_if.h"
//====================================//
// シンボル定義
//====================================//
#define MAX_LED 4
#define RESET_LEN  270	// > 287us分
#define PWM_0 	0b11100000		// 0bit: High=0.4us
#define PWM_1 	0b11111100		// 1bit: High=0.8us

typedef struct {
    uint8_t pattern;
    int8_t r;
    int8_t g;
    int8_t b;
} RGBLED;
//====================================//
// グローバル変数の宣言
//====================================//
extern volatile bool datasentflag;
extern volatile bool dma_done_flag;
//====================================//
// プロトタイプ宣言
//====================================//
void initLED(void);
void ledDMAinterrupt(void);
void setLED(uint8_t LEDnum, uint8_t Red, uint8_t Green, uint8_t Blue);
void sendLED(void);
void sequenceColorLED(uint8_t brightness, uint8_t add);
void led_out(uint8_t data);
void clearLED(void);

#endif // WS2812C_H_
