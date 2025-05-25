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
#define RESET_LEN  300  // > 287us分
#define PWM_0 21        // 0bit: High=0.4us
#define PWM_1 48        // 1bit: High=0.8us

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
void setLED(int LEDnum, int Red, int Green, int Blue);
void sendLED(void);
void fullColorLED(uint8_t brightness, uint8_t add);
void r2b(RGBLED *led, uint8_t brightness, uint8_t add);
void led_out(uint8_t data);
void clearLED(void);

#endif // WS2812C_H_
