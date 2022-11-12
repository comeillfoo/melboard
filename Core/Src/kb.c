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
	uint8_t Nkey = 0x00; // set to no pressed
	uint8_t buf = 0;

	set_keyboard();
//	if(ret != HAL_OK) return ret;

	buf = row_nr;
	PCA9538_Write_Register(KBRD_ADDR, CONFIG, &buf);
//	if(ret != HAL_OK) return ret;

	PCA9538_Read_Inputs(KBRD_ADDR, &buf);
//	if(ret != HAL_OK) return ret;

	uint8_t kbd_in = buf & 0x70;
	Nkey = kbd_in;
	if( kbd_in != 0x70) {
		if( !(kbd_in & 0x10) ) {
			switch (row_nr) {
				case ROW1:
					Nkey = 0x04;
					break;
				case ROW2:
					Nkey = 0x04;
					break;
				case ROW3:
					Nkey = 0x04;
					break;
				case ROW4:
					Nkey = 0x04;
					break;
			}
		}
		if( !(kbd_in & 0x20) ) {
			switch (row_nr) {
				case ROW1:
					Nkey = 0x02;
					break;
				case ROW2:
					Nkey = 0x02;
					break;
				case ROW3:
					Nkey = 0x02;
					break;
				case ROW4:
					Nkey = 0x02;
					break;
			}
		}
		if( !(kbd_in & 0x40) ) {
			switch (row_nr) {
				case ROW1:
					Nkey = 0x01;
					break;
				case ROW2:
					Nkey = 0x01;
					break;
				case ROW3:
					Nkey = 0x01;
					break;
				case ROW4:
					Nkey = 0x01;
					break;
			}
		}
	}
	else Nkey = 0x00;

	return Nkey;
}

