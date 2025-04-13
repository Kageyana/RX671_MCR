#ifndef FONTS_H__
#define FONTS_H__
//=====================================//
// �C���N���[�h
//=====================================//
#include <stdint.h>
//=====================================//
// �V���{����`
//=====================================//
typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;
//====================================//
// �O���[�o���ϐ��̐錾
//====================================//
extern FontDef Font_6x8;
extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;
//====================================//
// �v���g�^�C�v�錾
//====================================//

#endif // FONTS_H__