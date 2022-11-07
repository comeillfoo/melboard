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

//enum note_frequency { // in milliherz for first octave (5)
////	NF_DO  = 261630,
////	NF_RE  = 293670,
////	NF_MI  = 329630,
////	NF_FA  = 349230,
////	NF_SOL = 392000,
////	NF_LA  = 440000,
////	NF_TI  = 493880
//	NF_DO  = 16351,
//	NF_RE  = 18354,
//	NF_MI  = 20602,
//	NF_FA  = 21827,
//	NF_SOL = 24500,
//	NF_LA  = 27500,
//	NF_TI  = 30868
//};

//static enum note_frequency millifreq_map[] = {
//		[RQT_DO]  = NF_DO,
//		[RQT_RE]  = NF_RE,
//		[RQT_MI]  = NF_MI,
//		[RQT_FA]  = NF_FA,
//		[RQT_SOL] = NF_SOL,
//		[RQT_LA]  = NF_LA,
//		[RQT_TI]  = NF_TI
//};

static uint32_t herz_map[] = {
	N_C0,
	N_D0,
	N_E0,
	N_F0,
	N_G0,
	N_A0,
	N_B0,
	N_C1,
	N_D1,
	N_E1,
	N_F1,
	N_G1,
	N_A1,
	N_B1,
	N_C2,
	N_D2,
	N_E2,
	N_F2,
	N_G2,
	N_A2,
	N_B2,
	N_C3,
	N_D3,
	N_E3,
	N_F3,
	N_G3,
	N_A3,
	N_B3,
	N_C4,
	N_D4,
	N_E4,
	N_F4,
	N_G4,
	N_A4,
	N_B4,
	N_C5,
	N_D5,
	N_E5,
	N_F5,
	N_G5,
	N_A5,
	N_B5,
	N_C6,
	N_D6,
	N_E6,
	N_F6,
	N_G6,
	N_A6,
	N_B6,
	N_C7,
	N_D7,
	N_E7,
	N_F7,
	N_G7,
	N_A7,
	N_B7,
	N_C8,
	N_D8,
	N_E8,
	N_F8,
	N_G8,
	N_A8,
	N_B8
};


#define DEFAULT_RESPONSE_LENGTH (256)

static void response_number(struct fifo_queue* response_q, uint32_t number) {
	char response[DEFAULT_RESPONSE_LENGTH];
	snprintf(response, DEFAULT_RESPONSE_LENGTH, "%lu\r\n", number);
	const size_t length = strlen(response);
	queue_write(response_q, (uint8_t*) response, length);
}

void raise_octave(struct fifo_queue* response_q, enum request_type req) {
	if (buzz.octave < 9) buzz.octave += 1;
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
	TIM1->PSC = ((2 * HAL_RCC_GetPCLK2Freq()) / (2 * (TIM1->ARR) * (millifreq / 1000))) - 1;
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
	const uint32_t freq_of_octave = herz_map[req + 7 * (buzz.octave - 1)];
	set_frequency(freq_of_octave);
	unmute_buzzer();
	buzz.done = 0;

	// send response
	char response[DEFAULT_RESPONSE_LENGTH];
	snprintf(response, DEFAULT_RESPONSE_LENGTH, "%c%u %lu,%lu\r\n", notes[req], buzz.octave - 1, buzz.duration / 10, buzz.duration % 10);
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

