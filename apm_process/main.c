#include "apm_proc.h"


int main()
{
    pcm_data pcm_data;

	if(0 != open_pcm_data(&pcm_data))
    {
        return -2;
    }


	proc_pcm_data(&pcm_data);


	if(0 != close_pcm_data(&pcm_data))
    {
        return -2;
    }
	return 0;

}