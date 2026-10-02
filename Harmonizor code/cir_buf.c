/*
 * cir_buf.c
 *
 *  Created on: Sep 3, 2026
 *      Author: phlwi
 */
#include "cir_buf.h"

#define cir_N  8192

extern uint16_t cir_buf[cir_N];

uint16_t cir_index = 0;

void cir_index_plus(uint16_t plus_index){
	cir_index = (cir_index + plus_index) % cir_N;
}
//write from the adc input, change cir index
void write_cir_buf(uint32_t buf[], uint16_t size, uint8_t status){
	int start_idx;
	int end_idx;
	if (status == 0){start_idx = 0;      end_idx = size/2;}
	else            {start_idx = size/2; end_idx = size;}
	for(int i=start_idx; i<end_idx; i++){
	    cir_buf[cir_index] = (uint16_t)buf[i];
	    cir_index_plus(1);
	}
}
//read to the pitch detecting algorithm without changing cir index
void read_newest_cir_buf(uint16_t buf[], uint16_t size){ //size should be 2048
	for(int i=0; i<size; i++){
		int idx = (cir_index + (cir_N-size) + i) % cir_N;
		buf[i] = cir_buf[idx];
	}
}


//DEBUG read cir buf for dac to know whether cir buf content is trusable
void read_newest_dac_cir_buffer(uint32_t buf[], uint8_t status){
	int plus_idx;
	if(status == 0){plus_idx = 0;}
	else           {plus_idx = 512;}
	for(int i=0; i<512; i++){
		int idx = (cir_index + cir_N - 512 + i) % cir_N;
		buf[i+plus_idx] = cir_buf[idx];
	}
}


