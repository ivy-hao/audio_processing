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
    return 0;
}




int proc_pcm_data(pcm_data *pcm_data_ptr) 
{
    short recover_array[160];
    short after_hp_array[160];


    if (NULL == pcm_data_ptr)
	{
		return -1;
	}

    pcm_data_init(pcm_data_ptr);
    memset(recover_array,0,sizeof(short)*(FRAMELEN-STEP)); 
    memset( after_hp_array,0,sizeof(short)*(FRAMELEN-STEP)); 
     while (!feof(pcm_data_ptr->input_ptr))
    {
       frame(pcm_data_ptr);

       complexSet(samples, pcm_data_ptr->in_array_win, FRAMELEN);
       dft(XFreq, samples, FRAMELEN,pcm_data_ptr->out_array);
       idft(XFreq, xTime,FRAMELEN,pcm_data_ptr->in_array_win);

       recover(pcm_data_ptr,recover_array);     

     }  

   return 0;    
}