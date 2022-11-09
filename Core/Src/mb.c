/*
 * mb.c
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */
#include "mb.h"
#include "kb.h"

#define BTN_PRESSED_THRESHOLD (100)
#define BTN_RELEASED_THRESHOLD (20)

static int btn_counters[BUTTONS_NR] = {0};
static bool btn_hold[BUTTONS_NR] = {false};
static int map_row2btn[4][3] = {
		{KB_BTN_1, KB_BTN_2,     KB_BTN_3},
		{KB_BTN_4, KB_BTN_5,     KB_BTN_6},
		{KB_BTN_P, KB_BTN_7,     KB_BTN_M},
		{KB_BTN_A, KB_BTN_ENTER, KB_BTN_a}
};


static void update_counters() {
	// update counters
	const uint8_t rows[4] = {ROW1, ROW2, ROW3, ROW4};

	// loop through rows
	for (size_t i = 0; i < 4; ++i) {
		uint8_t row = 0;
		HAL_StatusTypeDef ret = check_row(rows[i], &row); // warning it takes 100ms
		if (ret == HAL_OK) {

			// detect pressed button candidate
			uint8_t pressed_btn = 0;
			switch (row) {
				case 0x1:
					pressed_btn = map_row2btn[i][0]; break;
				case 0x2:
					pressed_btn = map_row2btn[i][1]; break;
				case 0x4:
					pressed_btn = map_row2btn[i][2]; break;
				default:
					pressed_btn = KB_BTN_NONE; break;
			}

			// increment pressed
			if (pressed_btn != KB_BTN_NONE) {
				if (!btn_hold[pressed_btn])
					btn_counters[pressed_btn]++;

				// update for new cycle
				if (btn_counters[pressed_btn] > BTN_PRESSED_THRESHOLD)
					btn_hold[pressed_btn] = true;
			}

			// decrement released
			for (size_t j = 0; j < 3; ++j) {
				const uint8_t released_btn = map_row2btn[i][j];
				if (pressed_btn != released_btn) {
					if (btn_hold[released_btn])
						btn_counters[released_btn]--;

					// update for new cycle
					if (btn_counters[released_btn] < BTN_RELEASED_THRESHOLD)
						btn_hold[released_btn] = false;
				}
			}
		}
	}
}


uint8_t poll_btn() {

	update_counters();

	// detect only pressed number
	uint16_t buttons_bitarray = 0;
	for (size_t i = 1; i < BUTTONS_NR; ++i)
		if (btn_counters[i] > BTN_PRESSED_THRESHOLD)
			buttons_bitarray |= 1 << i;

	// if power of two then single button pressed
	if ((buttons_bitarray & (buttons_bitarray - 1)) != 0)
		return KB_BTN_NONE;

	// get the number of button
	int button_nr = 0;
	while ((buttons_bitarray & 0x1)== 0) {
		button_nr++;
		buttons_bitarray = buttons_bitarray >> 1;
	}

	return button_nr;
}

#undef BTN_PRESSED_THRESHOLD
#undef BTN_RELEASED_THRESHOLD
