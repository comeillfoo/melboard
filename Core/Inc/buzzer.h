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
#define N_C5  523251
#define N_D5  587330
#define N_E5  659255
#define N_F5  698456
#define N_G5  783991
#define N_A5  880000
#define N_B5  987767
#define N_C6  1046502
#define N_D6  1174659
#define N_E6  1318510
#define N_F6  1396913
#define N_G6  1567982
#define N_A6  1760000
#define N_B6  1975533
#define N_C7  2093005
#define N_D7  2349318
#define N_E7  2637020
#define N_F7  2793826
#define N_G7  3135963
#define N_A7  3520000
#define N_B7  3951066
#define N_C8  4186009
#define N_D8  4698636
#define N_E8  5274041
#define N_F8  5587652
#define N_G8  6271927
#define N_A8  7040000
#define N_B8  7902133

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
