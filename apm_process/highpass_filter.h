#ifndef __HIGHPASS_FILTER_H__
#define __HIGHPASS_FILTER_H__

#define HPF_ORDER  4

typedef struct {              
    short pre_in_[HPF_ORDER - 1];
    short pre_out_[HPF_ORDER - 1];
}hp_filter;

void HP_Proc(short arr_x[], short arr_y[]);

void hpf(void *pcm_data_ptr);


#endif 
