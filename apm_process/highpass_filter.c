#include <math.h>
#include "apm_proc.h"
#include "defines.h"
#include "highpass_filter.h"

const float hpf_bb_[HPF_ORDER] = {0.8945735693,    -2.68268609,     2.68268609,  -0.8945735693};
const float hpf_ab_[HPF_ORDER] = {1,   -2.776320696,    2.577936888,  -0.8002618551};

void HP_Proc(short arr_x[], short arr_y[])
{ 
    int j;

    for(j=0;j<FRAMELEN-STEP;j++)
    {
        
        if(0==j)
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]);
        }
        else if(1==j)
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]-(1.75-1.732051)*arr_x[j-1]-3*0.8660255*arr_y[j-1]);
        }
        else if(2==j)
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]-(1.75-1.732051)*arr_x[j-1]-3*0.8660255*arr_y[j-1]-2.5*arr_y[j-2]);
        }
        else
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]-(1.75-1.732051)*arr_x[j-1]-1.5*1.732051*arr_y[j-1]-2.5*arr_y[j-2]-0.8660255*arr_y[j-3]);
        }

    }
 return;
}


void hpf(void *pcm_data_ptr)
{
    pcm_data *stt = (pcm_data *)pcm_data_ptr;
    int i,j;
    for (i = 0; i < FRAMELEN ; i ++)
    {
        short tmp_out = 0;
        tmp_out += stt->in_array[i] * hpf_bb_[0];
        for(j = 0; j < HPF_ORDER-1; j ++)
        {
            tmp_out +=   stt->hp_filter.pre_in_[j] * hpf_bb_[j+1];
        }
        
        for(j = 0; j < HPF_ORDER-1; j ++)
        {
            tmp_out -=   stt->hp_filter.pre_out_[j] * hpf_ab_[j+1];
        }

        memmove(&stt->hp_filter.pre_in_[1],stt->hp_filter.pre_in_,sizeof(short)*(HPF_ORDER - 2));
        memmove(&stt->hp_filter.pre_out_[1],stt->hp_filter.pre_out_,sizeof(short)*(HPF_ORDER - 2));
        stt->hp_filter.pre_in_[0] = stt->in_array[i];
        stt->hp_filter.pre_out_[0] = (short)tmp_out;
        
        stt->in_array[i] = tmp_out;
    }
    return;
}