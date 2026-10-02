/*
 * filter.c
 *
 *  Created on: Sep 7, 2026
 *      Author: phlwi
 */
#include "filter.h"

arm_biquad_casd_df1_inst_f32 lpf_iir;
float32_t lpf_state [4 * NUM_STAGES];  // 每個 stage 需要 4 個 state
float32_t lpf_coeffs[5 * NUM_STAGES] = {
	    0.0066582f,
	    0.0133164f,
	    0.0066582f,
	    1.8172379f,
	   -0.8438707f,
};

void filter_init(){
	arm_biquad_cascade_df1_init_f32(
      &lpf_iir,
      NUM_STAGES,
      lpf_coeffs,
      lpf_state
    );
	arm_fill_f32(0.0f, lpf_state, 4*NUM_STAGES);
}

void apply_filter(uint16_t *in_buf, uint16_t *out_buf, uint16_t size){
	float32_t  fin_buf[size];
	float32_t fout_buf[size];
	for (int i=0;i<size;i++){
		 fin_buf[i]=(float32_t) in_buf[i];
		fout_buf[i]=(float32_t)out_buf[i];
	}
	//No need ba, should be coefficient problem
//	for (int i=0;i<size;i++){
//		 fin_buf[i]=(((float32_t) in_buf[i]) /2048.0f )-1.0f;
//		fout_buf[i]=(((float32_t)out_buf[i]) /2048.0f )-1.0f;
//	}
//    for (int i=0;i<size;i++){
//    	out_buf[i] = (uint16_t)(fout_buf[i] * 4096);
//    }

    arm_biquad_cascade_df1_f32(&lpf_iir, fin_buf, fout_buf, size);

    for (int i=0;i<size;i++){
    	out_buf[i] = (uint16_t)(fout_buf[i]);
    }
}

void decimate(uint16_t *in_buf, uint16_t *out_buf, uint16_t out_size, uint8_t factor){
	for (int i=0; i<out_size; i++){
		out_buf[i] = in_buf[i*factor];
	}
}

//self filter function
void smooth(float32_t target, float32_t *current, float smooth_factor){
    *current = *current + smooth_factor * (target - *current);
    if(fabsf(*current) < 1e-3f){
    	*current = 0.0f;
    }
}

void disortion(uint32_t *buf, uint16_t size, uint32_t cut_strength, uint8_t status){  //cut altitude = 0-2048
	int start_idx;
	int end_idx;
	if (status == 0){start_idx = 0;      end_idx = size/2;}
	else            {start_idx = size/2; end_idx = size;}

	for(int i=start_idx; i<end_idx; i++){
		if(buf[i] < cut_strength){
			buf[i] = cut_strength;
		}
		if(buf[i] > 4096 - cut_strength){
			buf[i] = 4096 - cut_strength;
		}
	}
}
