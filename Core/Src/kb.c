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


uint8_t check_row(uint8_t row_nr) {
	uint8_t keys = 0x00; // set to no pressed
	uint8_t buf = 0;

	set_keyboard();
//	if(ret != HAL_OK) return ret;

	buf = row_nr;
	PCA9538_Write_Register(KBRD_ADDR, CONFIG, &buf);
//	if(ret != HAL_OK) return ret;

	PCA9538_Read_Inputs(KBRD_ADDR, &buf);
//	if(ret != HAL_OK) return ret;

	uint8_t kbd_in = buf & 0x70;
	keys = kbd_in;
//	if( kbd_in != 0x70) {
//		if( !(kbd_in & 0x10) ) keys = 0x04;
//
//		if( !(kbd_in & 0x20) ) keys = 0x02;
//
//		if( !(kbd_in & 0x40) ) keys = 0x01;
//	}
//	else keys = 0x00;
	keys = keys >> 4;
	keys = (~keys) & 0x7;

	return keys;
}

