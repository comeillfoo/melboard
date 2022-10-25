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


#define N_C0  16352
#define N_D0  18354
#define N_E0  20602
#define N_F0  21827
#define N_G0  24500
#define N_A0  27500
#define N_B0  30868
#define N_C1  32703
#define N_D1  36708
#define N_E1  41203
#define N_F1  43654
#define N_G1  48999
#define N_A1  55000
#define N_B1  61735
#define N_C2  65406
#define N_D2  73416
#define N_E2  82407
#define N_F2  87307
#define N_G2  97999
#define N_A2  110000
#define N_B2  123471
#define N_C3  130813
#define N_D3  146832
#define N_E3  164814
#define N_F3  176614
#define N_G3  195998
#define N_A3  220000
#define N_B3  246942
#define N_C4  261626
#define N_D4  293665
#define N_E4  329628
#define N_F4  349228
#define N_G4  391995
#define N_A4  440000
#define N_B4  493883
//#define N_C5  523
//#define N_D5  587
//#define N_E5  659
//#define N_F5  698
//#define N_G5  784
//#define N_A5  880
//#define N_B5  988
//#define N_C6  1047
//#define N_D6  1175
//#define N_E6  1319
//#define N_F6  1397
//#define N_G6  1568
//#define N_A6  1760
//#define N_B6  1976
//#define N_C7  2093
//#define N_D7  2349
//#define N_E7  2637
//#define N_F7  2794
//#define N_G7  3136
//#define N_A7  3520
//#define N_B7  3951
//#define N_C8  4186
//#define N_D8  4699

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
