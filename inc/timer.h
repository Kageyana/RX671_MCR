#ifndef TIME_H_
#define TIME_H_
//=====================================//
// �C���N���[�h
//=====================================//
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//=====================================//
// �V���{����`
//=====================================//

//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
extern uint16_t cnt0;
extern uint16_t cnt1;
extern uint8_t useIMU, useDisplay;
extern volatile bool update_display;
//====================================//
// �v���g�^�C�v�錾
//====================================//
void Interrupt1ms(void);

#endif // TIME_H_