/*
 * kb.h
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */

#ifndef INC_KB_H_
#define INC_KB_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

enum kb_fms {
	KB_FMS_SELECT_ROW = 0,
	KB_FMS_POLL_ROW
};


enum kb_event {
	KBE_NONE       =  0,
	KBE_PRESSED_1  =  1,
	KBE_PRESSED_2  =  2,
	KBE_PRESSED_3  =  3,
	KBE_PRESSED_4  =  4,
	KBE_PRESSED_5  =  5,
	KBE_PRESSED_6  =  6,
	KBE_PRESSED_7  =  7,
	KBE_PRESSED_8  =  8,
	KBE_PRESSED_9  =  9,
	KBE_PRESSED_10 = 10,
	KBE_PRESSED_11 = 11,
	KBE_PRESSED_12 = 12
};

HAL_StatusTypeDef KB_Init();
HAL_StatusTypeDef KB_Poll_Start();

enum kb_event KB_Poll_Finish();

#endif /* INC_KB_H_ */
