#ifndef APM_PROC_H
#define APM_PROC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "frame.h"
#include "defines.h"
#include "fft.h"
#include "highpass_filter.h"

typedef struct {              
    FILE *input_ptr;
    FILE *output_ptr;
    FILE *output_file_ptr;
    short in_array[FRAMELEN];   
    short out_array[FRAMELEN];  
    short in_array_win[FRAMELEN];  
    short output_data; 
    short pre_out_array[FRAMELEN]; 
}pcm_data;

complex samples[FRAMELEN], XFreq[FRAMELEN], xTime[FRAMELEN];
short frqValue[FRAMELEN];
short after_hp_array[160];

int open_pcm_data(pcm_data *pcm_data_ptr);

int close_pcm_data(pcm_data *pcm_data_ptr);

int pcm_data_init(pcm_data *pcm_data_ptr);

int proc_pcm_data(pcm_data *pcm_data_ptr);


#endif