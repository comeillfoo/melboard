/*
 * mb.h
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */

#ifndef INC_MB_H_
#define INC_MB_H_

#include <stdint.h>
#include <stdbool.h>

enum kb_btn {
	KB_BTN_NONE = 0,
	KB_BTN_1,
	KB_BTN_2,
	KB_BTN_3,
	KB_BTN_4,
	KB_BTN_5,
	KB_BTN_6,
	KB_BTN_7,
	KB_BTN_P,
	KB_BTN_M,
	KB_BTN_A,
	KB_BTN_a,
	KB_BTN_ENTER
};

#define BUTTONS_NR (13)

uint8_t poll_btn();

#endif /* INC_MB_H_ */
