/*
 * harmonic_gen.c
 *
 *  Created on: Sep 8, 2026
 *      Author: phlwi
 */
#include "harmonic_gen.h"

#define min3_factor 1.189207f
#define maj3_factor 1.259921f

#define SAMPLE_RATE 48000

float32_t wave_buf[600];
uint16_t current_idx = 0;
uint16_t max_idx = 0;     //<600

void add_wave_to_dac(uint32_t *out_buf, uint16_t size, float altitude_percent, uint8_t status){ //size shoud be 1024
	if(max_idx != 0){
	int start_idx;
	int end_idx;
	if (status == 0){start_idx = 0;      end_idx = size/2;}
	else            {start_idx = size/2; end_idx = size;}
	for(int i=start_idx; i<end_idx; i++){
    	int wave_idx = (current_idx + i - start_idx) % max_idx;  //prevent overflow
    	out_buf[i] += (uint32_t)(wave_buf[wave_idx] * altitude_percent);
    }
    current_idx = (current_idx + size/2) % max_idx;  //update current idx
	}
}


//void update_wave_buffer(float note_freq){
//	uint8_t is_maj3 = get_is_maj3();
//	float harm_freq;
//	if(is_maj3) {harm_freq = note_freq * maj3_factor;}
//	else        {harm_freq = note_freq * min3_factor;}
//	max_idx = 48000.0f/harm_freq + 0.5/*round off*/;
//}

void clear_wave_buffer(){
	for(int i=0; i<600; i++){
	    wave_buf[i] = 0;
	}
}

//WAVES WAVES WAVES
void gen_cool_as_fuck_saw_wave(float freq){
	int sample_per_period = 48000.0f/freq + 0.5/*round off*/;
	float increment_val = 4096.0f/(float)sample_per_period;
	float32_t cur_val=0;
	for(int i=0; i<sample_per_period; i++){
	    wave_buf[i] += cur_val;
	    cur_val += increment_val;
	}
}
void gen_cool_as_fuck_saw_wave_with_fking_octive(float freq){
	clear_wave_buffer();

	int sample_per_period = 48000.0f/freq + 0.5/*round off*/;
	float increment_val = 3072.0f/(float)sample_per_period;
	float32_t cur_val=0;
	for(int i=0; i<sample_per_period; i++){
	    wave_buf[i] += cur_val;
	    cur_val += increment_val;
	}

	//A octive higher
	increment_val = 1024.0f/(float)sample_per_period  * 2;
	cur_val=0;
	for(int i=0; i<sample_per_period/2; i++){
        wave_buf[i] += cur_val;
		cur_val += increment_val;
	}
	cur_val=0;
	for(int i=sample_per_period/2; i<sample_per_period; i++){
	    wave_buf[i] += cur_val;
		cur_val += increment_val;
	}
}
void gen_triangle_wave(float freq){
	int sample_per_period = 48000.0f/freq + 0.5/*round off*/;
	float increment_val = 4096.0f/(float)sample_per_period;
	float32_t cur_val=0;
	for(int i=0; i<sample_per_period/2; i++){
	    wave_buf[i] += cur_val;
	    cur_val += 2 * increment_val;
	}
	for(int i=sample_per_period/2; i<sample_per_period; i++){
		wave_buf[i] += cur_val;
		cur_val -= 2 * increment_val;
	}
}
void gen_sine_wave(float freq){
	int sample_per_period = 48000.0f/freq + 0.5/*round off*/;
	for (int i = 0; i < sample_per_period; i++){
	    float32_t angle = (freq * 2.0f * PI   *   i / (float32_t)48000);
	    float32_t sinVal = arm_sin_f32(angle);
//			  //new for makin sine wave disorrtion
//			  if (sinVal > 0.2){sinVal = 0.2;}
//			  if (sinVal < -0.2){sinVal = -0.2;}
	    uint32_t dacVal = ((4096/2) * (sinVal + 1)) + 0.5;
	    wave_buf[i] += dacVal;
	}
}
void add_wave_to_buffer(float note_freq, uint8_t is_maj3){
	if(note_freq != 0){
	float harm_freq;
	if(is_maj3) {harm_freq = note_freq * maj3_factor;}
	else        {harm_freq = note_freq * min3_factor;}

	gen_cool_as_fuck_saw_wave_with_fking_octive(harm_freq);

	max_idx = 48000.0f/harm_freq + 0.5/*round off*/;
	}
	else {clear_wave_buffer();}
}


//uint16_t frequency_to_midi(float freq){
//	if (freq <= 0){ return 0; }
//	return 12.0 * (log((float)freq/440.0)/log(2.0))+69.0 + 0.5;
//}
