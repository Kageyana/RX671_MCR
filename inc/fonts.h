#ifndef FONTS_H__
#define FONTS_H__
//=====================================//
// インクルード
//=====================================//
#include <stdint.h>
//=====================================//
// シンボル定義
//=====================================//
typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   		/*!< Font height in pixels */
	const uint16_t *data; 		/*!< Pointer to data font data array */
} FontDef;
//====================================//
// グローバル変数の宣言
//====================================//
extern FontDef Font_6x8;
extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;
//====================================//
// プロトタイプ宣言
//====================================//

#endif // FONTS_H__