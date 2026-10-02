/*
 * map_key_midi.c
 *
 *  Created on: Sep 15, 2026
 *      Author: phlwi
 */
#include "map_key_midi.h"

static const uint64_t base = 0b000010100001;
uint64_t key_array;

//freq to midi to maj3 (when frequency is detect
uint8_t is_it_maj3(float freq){
	if (freq <= 0){ return 0; }

	uint32_t midi = 12.0 * (log((float)freq/440.0)/log(2.0)) + 69.0 + 0.5;
    uint32_t idx = midi - 40;
    return (key_array >> idx) & 1u ;
}

//update key array (when state is detected
uint64_t rotate_left(uint64_t origin, int steps){
    return ((origin << steps) | (origin >> (12-steps))) & 0x0FFF;
}
uint64_t rotate_right(uint64_t origin, int steps){
    return ((origin >> steps) | (origin << (12-steps))) & 0x0FFF;
}

void update_key(uint32_t state){
   key_array = 0x00;

   uint64_t new_base;
   if (state >=4){ new_base = rotate_left (base, state-4); }
   if (state < 4){ new_base = rotate_right(base, 4-state); }

   for(int i = 0; i < 4; i++){
       key_array = key_array | (new_base << i*12);
   }
}
