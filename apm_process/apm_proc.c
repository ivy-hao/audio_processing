#include "apm_proc.h"


int open_pcm_data(pcm_data *pcm_data_ptr)
{
    int err = 0;

    if(NULL == pcm_data_ptr) 
    {
        return -1;
    }
 
    err = fopen_s(&pcm_data_ptr->input_ptr,"origin_audio.pcm","rb");  

    if(0!=err)
    {
        return -2;
    }
 
    err = fopen_s(&pcm_data_ptr->output_ptr,"output.txt","w+");

    if(0!=err)
    {
        return -2;
    }
    err = fopen_s(&pcm_data_ptr->output_file_ptr,"output.pcm","wb");

    if(0!=err)
    {
        return -2;
    }

    return 0;
}




int close_pcm_data(pcm_data *pcm_data_ptr)
{
    if( NULL == pcm_data_ptr 
        || NULL == pcm_data_ptr->input_ptr
        ||NULL == pcm_data_ptr->output_ptr
        ||NULL == pcm_data_ptr->output_file_ptr)
    {
        return -1;
    }
    fclose(pcm_data_ptr->input_ptr);
    fclose(pcm_data_ptr->output_ptr);
    fclose(pcm_data_ptr->output_file_ptr);

    return 0;
}

int pcm_data_init(pcm_data *pcm_data_ptr)
{
    if (NULL == pcm_data_ptr)
	{
		return -1;
	}

    memset(pcm_data_ptr->in_array,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->out_array,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->in_array_win,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->pre_out_array,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->hp_filter.pre_in_,0,sizeof(short)*(HPF_ORDER-1)); 
    memset(pcm_data_ptr->hp_filter.pre_out_,0,sizeof(short)*(HPF_ORDER-1)); 
    return 0;
}




int proc_pcm_data(pcm_data *pcm_data_ptr) 
{
    short recover_array[FRAMELEN];
    short after_hp_array[FRAMELEN];


    if (NULL == pcm_data_ptr)
	{
		return -1;
	}

    pcm_data_init(pcm_data_ptr);
    memset(recover_array,0,sizeof(short)*FRAMELEN); 
    memset( after_hp_array,0,sizeof(short)*FRAMELEN); 
     while (!feof(pcm_data_ptr->input_ptr))
    {
        fread(&pcm_data_ptr->in_array,sizeof(short),FRAMELEN,pcm_data_ptr->input_ptr); 
        
        hpf(pcm_data_ptr);

        memcpy(pcm_data_ptr->out_array,pcm_data_ptr->in_array,sizeof(short)*FRAMELEN);
        fwrite(pcm_data_ptr->out_array, sizeof(short),FRAMELEN,pcm_data_ptr->output_file_ptr);

     }  

   return 0;    
}