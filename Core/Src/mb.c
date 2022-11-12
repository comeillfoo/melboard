/*
 * mb.c
 *
 *  Created on: 9 нояб. 2022 г.
 *      Author: come_ill_foo
 */
#include "mb.h"
#include "kb.h"

#include <string.h>
#include <stdio.h>
#include "usart.h"

#define BTN_PRESSED_THRESHOLD  (500)
#define BTN_RELEASED_THRESHOLD (400)

static int counters[BUTTONS_NR] = {0};
static int kb_state = KB_BTN_NONE;

static int map_buttons[4][3] = {
		{KB_BTN_1, KB_BTN_2,     KB_BTN_3},
		{KB_BTN_4, KB_BTN_5,     KB_BTN_6},
		{KB_BTN_P, KB_BTN_7,     KB_BTN_M},
		{KB_BTN_A, KB_BTN_ENTER, KB_BTN_a}
};

enum btn_side {
	BTN_S_LEFT = 0,
	BTN_S_MIDDLE,
	BTN_S_RIGHT
};

static enum btn_side map_row2side[3] = { BTN_S_LEFT, BTN_S_MIDDLE, BTN_S_RIGHT };


void update_counters() {
	// update counters
	const uint8_t rows[4] = {ROW1, ROW2, ROW3, ROW4};

	// loop through rows
	for (size_t i = 0; i < 4; ++i) {
		uint8_t row = check_row(rows[i]); // warning it takes 100ms

		// update counters
		for (size_t j = 0; j < 3; ++j) {
			uint8_t is_pressed = (row >> j) & 0x1;
			if (is_pressed)
				counters[map_buttons[i][map_row2side[j]]]++;
			else counters[map_buttons[i][map_row2side[j]]]--;
		}
	}
}


uint8_t poll_keyboard() {
	for (size_t i = KB_BTN_1; i <= KB_BTN_ENTER; ++i) {
		switch (kb_state) {
			case KB_BTN_NONE: if (counters[i] == BTN_PRESSED_THRESHOLD) kb_state = i; break;
			case KB_BTN_1:
			case KB_BTN_2:
			case KB_BTN_3:
			case KB_BTN_5:
			case KB_BTN_6:
			case KB_BTN_7:
			case KB_BTN_A:
			case KB_BTN_a:
			case KB_BTN_P:
			case KB_BTN_M:
			case KB_BTN_ENTER:
				if (((i != kb_state) && (counters[i] == BTN_PRESSED_THRESHOLD))
					|| ((i == kb_state) && (counters[i] < BTN_RELEASED_THRESHOLD)))
						kb_state = KB_BTN_NONE;
				break;
			default: break;
		}
	}

	return kb_state;
}

#undef BTN_PRESSED_THRESHOLD
#undef BTN_RELEASED_THRESHOLD
