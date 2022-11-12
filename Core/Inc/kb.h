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

#define ROW1 0xFE
#define ROW2 0xFD
#define ROW3 0xFB
#define ROW4 0xF7


uint8_t check_row(uint8_t row_nr);
HAL_StatusTypeDef set_keyboard(void);

#endif /* INC_KB_H_ */
