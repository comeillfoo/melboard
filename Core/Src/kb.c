/*
 * kb.c
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */
#include "kb.h"
#include "i2c.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>


#define KB_ADDR 0xE2

enum kb_registers {
	KB_REG_INPUT  = 0x0,
	KB_REG_OUTPUT = 0x1,
	KB_REG_CONFIG = 0x3
};


enum kb_fms kb_state = KB_FMS_SELECT_ROW;
uint8_t row = 0;
uint8_t kb_buffer = 0;
static int32_t old_buttons_st[4][3] = {0};


static uint8_t request_row(uint8_t row) {
	return ~(1 << row);
}


static HAL_StatusTypeDef KB_Select_Row(uint8_t row) {
	kb_buffer = request_row(row);
	return HAL_I2C_Mem_Write(&hi2c1, KB_ADDR | 1, KB_REG_CONFIG, 1, &kb_buffer, 1, 100);
}

#define KB_PRESS_THRESHOLD 10
#define KB_RELEASE_THRESHOLD 0

int32_t cur_buttons_st[4][3] = {KB_RELEASE_THRESHOLD};

static void update_counters() {
	// RX Done .. Do Something!
	// updating counters for row
	// X001'XXXX -> XXXX'X001 -> XXXX'X110 -> pressed left and centre buttons
	const uint8_t buttons = ~(kb_buffer >> 4);
	for (size_t col = 0; col < 3; ++col) {
		const int32_t current = cur_buttons_st[row][col];

		old_buttons_st[row][col] = current;

		const int32_t diff = ((buttons >> col) & 0x1) ? ((current < KB_PRESS_THRESHOLD) ? +1 : 0) : ((current > KB_RELEASE_THRESHOLD) ? -1 : 0);
		cur_buttons_st[row][col] += diff;
	}
}


static enum kb_event kbev_state = KBE_NONE;

static void update_state() {
	switch (kbev_state) {
		case KBE_PRESSED_1:
		case KBE_PRESSED_2:
		case KBE_PRESSED_3:
		case KBE_PRESSED_4:
		case KBE_PRESSED_5:
		case KBE_PRESSED_6:
		case KBE_PRESSED_7:
		case KBE_PRESSED_8:
		case KBE_PRESSED_9:
		case KBE_PRESSED_10:
		case KBE_PRESSED_11:
		case KBE_PRESSED_12: {
			const size_t btn_row = kbev_state / 4;
			const size_t btn_col = (kbev_state - 1) % 3;
			if (cur_buttons_st[btn_row][btn_col] == KB_RELEASE_THRESHOLD) {
				kbev_state = KBE_NONE;
				return;
			}

			for (size_t i = 0; i < 4; ++i)
				for (size_t j = 0; j < 3; ++j)
					if ((i != btn_row) && (j != btn_col) && (cur_buttons_st[i][j] == KB_PRESS_THRESHOLD)) {
						kbev_state = KBE_NONE;
						return;
					}
			break;
		}
		case KBE_NONE: {
			size_t candidates = 0;
			size_t btn_row = 0;
			size_t btn_col = 0;
			for (size_t i = 0; i < 4; ++i)
				for (size_t j = 0; j < 3; ++j)
					if (cur_buttons_st[i][j] == KB_PRESS_THRESHOLD) {
						candidates++;
						btn_row = i;
						btn_col = j;
					}

			if (candidates == 1)
				kbev_state = btn_row * 3 + (btn_col + 1);
			break;
		}

	}
}

#undef KB_PRESS_THRESHOLD
#undef KB_RELEASE_THRESHOLD

HAL_StatusTypeDef KB_Poll_Start() {
	HAL_StatusTypeDef ret = HAL_OK;
	switch (kb_state) {
		case KB_FMS_SELECT_ROW: {
			ret = KB_Select_Row(row);
			kb_state = KB_FMS_POLL_ROW;
			break;
		}
		case KB_FMS_POLL_ROW: {
			ret = HAL_I2C_Mem_Read(&hi2c1, KB_ADDR & ~1, KB_REG_INPUT, 1, &kb_buffer, 1, 100);
			kb_state = KB_FMS_SELECT_ROW;
			update_counters();
			update_state();
			row = (row + 1) % 4;
			break;
		}
	}
	return ret;
}

HAL_StatusTypeDef KB_Init() {
	kb_buffer = 0x0;
	return HAL_I2C_Mem_Write(&hi2c1, KB_ADDR | 1, KB_REG_OUTPUT, 1, &kb_buffer, 1, 100);
}



enum kb_event KB_Poll_Finish() {
	return kbev_state;
}

