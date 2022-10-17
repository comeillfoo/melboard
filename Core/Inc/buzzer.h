/*
 * buzzer.h
 *
 *  Created on: Oct 16, 2022
 *      Author: come_ill_foo
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "main.h"
#include "queue.h"

struct buzzer {
	uint32_t duration;
	size_t octave;
	int done;
	uint32_t passed_time;
};


void raise_octave(struct fifo_queue*, enum request_type);

void lower_octave(struct fifo_queue*, enum request_type);

void raise_duration(struct fifo_queue*, enum request_type);

void lower_duration(struct fifo_queue*, enum request_type);

void play_note(struct fifo_queue*, enum request_type);

void mute_buzzer();

void unmute_buzzer();

int is_buzzer_done();

void pass_time(uint32_t);

#endif /* INC_BUZZER_H_ */
