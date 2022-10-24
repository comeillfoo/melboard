/*
 * buzzer.c
 *
 *  Created on: Oct 16, 2022
 *      Author: come_ill_foo
 */
#include "buzzer.h"
#include <stdio.h>
#include <string.h>

#include "tim.h"

#define DEFAULT_OCTAVE (5)
#define DEFAULT_DURATION (10) // in deciseconds; 1 decisecond == 0.1 second

static struct buzzer buzz = { DEFAULT_DURATION, DEFAULT_OCTAVE, 1 };

enum note_frequency { // in milliherz for first octave (5)
//	NF_DO  = 261630,
//	NF_RE  = 293670,
//	NF_MI  = 329630,
//	NF_FA  = 349230,
//	NF_SOL = 392000,
//	NF_LA  = 440000,
//	NF_TI  = 493880
	NF_DO  = 16352,
	NF_RE  = 18354,
	NF_MI  = 20602,
	NF_FA  = 21827,
	NF_SOL = 24500,
	NF_LA  = 27500,
	NF_TI  = 30868
};

static enum note_frequency millifreq_map[] = {
		[RQT_DO]  = NF_DO,
		[RQT_RE]  = NF_RE,
		[RQT_MI]  = NF_MI,
		[RQT_FA]  = NF_FA,
		[RQT_SOL] = NF_SOL,
		[RQT_LA]  = NF_LA,
		[RQT_TI]  = NF_TI
};


#define DEFAULT_RESPONSE_LENGTH (256)

static void response_number(struct fifo_queue* response_q, uint32_t number) {
	char response[DEFAULT_RESPONSE_LENGTH];
	snprintf(response, DEFAULT_RESPONSE_LENGTH, "%lu\r\n", number);
	const size_t length = strlen(response);
	queue_write(response_q, (uint8_t*) response, length);
}

void raise_octave(struct fifo_queue* response_q, enum request_type req) {
	if (buzz.octave < 5) buzz.octave += 1;
	response_number(response_q, buzz.octave);
}

void lower_octave(struct fifo_queue* response_q, enum request_type req) {
	if (buzz.octave > 1) buzz.octave -= 1;
	response_number(response_q, buzz.octave);
}

void raise_duration(struct fifo_queue* response_q, enum request_type req) {
	if (buzz.duration < 50) buzz.duration += 1;
	response_number(response_q, buzz.duration);
}

void lower_duration(struct fifo_queue* response_q, enum request_type req) {
	if (buzz.duration > 1) buzz.duration -= 1;
	response_number(response_q, buzz.duration);
}


void mute_buzzer() { TIM1->CCR1 = 0; }

void unmute_buzzer() { TIM1->CCR1 = TIM1->ARR >> 1; }


static void set_frequency(uint32_t millifreq) {
	TIM1->PSC = ((2 * HAL_RCC_GetPCLK2Freq() * 10) / (2 * (TIM1->ARR) * millifreq)) * 100 - 1;
}


static char notes[] = {
	[RQT_DO]  = 'C',
	[RQT_RE]  = 'D',
	[RQT_MI]  = 'E',
	[RQT_FA]  = 'F',
	[RQT_SOL] = 'G',
	[RQT_LA]  = 'A',
	[RQT_TI]  = 'B',
};

void play_note(struct fifo_queue* response_q, enum request_type req) {
	const uint32_t millifreq_of_octave = millifreq_map[req] << (buzz.octave - 1);
	set_frequency(millifreq_of_octave);
	unmute_buzzer();
	buzz.done = 0;

	// send response
	char response[DEFAULT_RESPONSE_LENGTH];
	snprintf(response, DEFAULT_RESPONSE_LENGTH, "%c%u %lu,%lu\r\n", notes[req], buzz.octave, buzz.duration / 10, buzz.duration % 10);
	const size_t length = strlen(response);
	queue_write(response_q, (uint8_t*) response, length);
}

int is_buzzer_done() { return buzz.done; }

void pass_time(uint32_t milliseconds_passed) {
	buzz.passed_time += milliseconds_passed;
	if (buzz.passed_time >= buzz.duration * 100) {
		buzz.done = 1;
		buzz.passed_time = 0;
	}
}

#undef DEFAULT_RESPONSE_LENGTH

