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

static struct buzzer buzz = { DEFAULT_DURATION, DEFAULT_OCTAVE };

enum note_frequency { // in milliherz for first octave (5)
	NF_DO  = 261630,
	NF_RE  = 293670,
	NF_MI  = 329630,
	NF_FA  = 349230,
	NF_SOL = 392000,
	NF_LA  = 440000,
	NF_TI  = 493880
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


static void set_freq_and_duration(uint32_t millifreq, uint32_t deciseconds) {
	TIM1->PSC = ((2 * HAL_RCC_GetPCLK1Freq() * 100) / millifreq) * 10 - 1;

	const uint32_t one_tick_duration = 1000000000 / millifreq; // in megaseconds
	uint32_t tick_duration = (100000 * deciseconds) / one_tick_duration - 1;

	if (tick_duration == 0) tick_duration = 1;
	TIM1->ARR = tick_duration;
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
	const uint32_t millifreq_of_octave = millifreq_map[req] >> (DEFAULT_OCTAVE - buzz.octave);
	set_freq_and_duration(millifreq_of_octave, buzz.duration);
	unmute_buzzer();

	// send response
	char response[DEFAULT_RESPONSE_LENGTH];
	snprintf(response, DEFAULT_RESPONSE_LENGTH, "%c%zu %lu\r\n", notes[req], buzz.octave, buzz.duration);
	const size_t length = strlen(response);
	queue_write(response_q, (uint8_t*) response, length);
}


#undef DEFAULT_RESPONSE_LENGTH

