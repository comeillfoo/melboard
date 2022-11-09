/*
 * kb.c
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */
#include "kb.h"
#include "pca9598.h"

#define KBRD_ADDR 0xE2


HAL_StatusTypeDef set_keyboard(void) {
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf = 0;

	ret = PCA9538_Write_Register(KBRD_ADDR, POLARITY_INVERSION, &buf);
	if(ret != HAL_OK) return ret;

	buf = 0;
	ret = PCA9538_Write_Register(KBRD_ADDR, OUTPUT_PORT, &buf);
	return ret;
}


HAL_StatusTypeDef check_row(uint8_t row_nr, uint8_t* keys_p) {
	*keys_p = 0x00; // set to no pressed
	HAL_StatusTypeDef ret = HAL_OK;
	uint8_t buf = 0;

	ret = set_keyboard();
	if(ret != HAL_OK) return ret;

	buf = row_nr;
	ret = PCA9538_Write_Register(KBRD_ADDR, CONFIG, &buf);
	if(ret != HAL_OK) return ret;

	ret = PCA9538_Read_Inputs(KBRD_ADDR, &buf);
	if(ret != HAL_OK) return ret;

	uint8_t kbd_in = buf & 0x70; // 0b01110000 the upper nibble is responsible for row

	*keys_p = kbd_in;
	if(kbd_in != 0x70) { // test if no one is pressed
		if(!(kbd_in & 0x10)) // test if left button pressed
			*keys_p = 0x04;

		if(!(kbd_in & 0x20)) // test if middle button pressed
			*keys_p = 0x02;

		if(!(kbd_in & 0x40)) // test if right button pressed
			*keys_p = 0x01;

	} else *keys_p = 0x00;

	return ret;
}

