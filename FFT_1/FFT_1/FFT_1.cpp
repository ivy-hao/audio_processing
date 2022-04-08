//��ȡpcm�ļ��е���Ƶ���ݺ󣬷�֡��֡��20ms��320���㣩���ص�10ms��160���㣩��֡��10ms��160���㣩�Ӵ���������������������ص���ӻָ���ԭ���ݡ�
/*��������w��n��=0.54-0.46cos[2*pi*n/(N-1)]  0��n��N, 0,����*/

/*�˴δ������⣺1������̫�࣬һ������������������������in_array��out_array��
2�������֡�Ӵ������ݵ�ʱ��Ҫ��֡�����д��������ٰ�֡��Ԫ�ؽ��д���
3���Ӵ�����Ϊһ���̶����ߣ���˽����������ŵ������С�
4���ָ�����ʱ��Ӧ��ֻ�õ������ĵ�ǰ֡��ǰ��֡�����ݣ����˴��㷨���õ�������͵�ǰ֡���������㷨����ȷ��
�����������⣬�����Ż����Ż�ǰ���ݣ����ں���ע�͵���

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>  
#include<math.h>
#include"fft.h"


const int FRAMELEN = 320; //
const int STEP = 160;  //
const double Hamming_Win[FRAMELEN]={0.080000,0.080089,0.080357,0.080803,0.081427,0.082229,0.083209,0.084365,0.085699,0.087209,0.088894,0.090755,0.092789,0.094997,0.097378,0.099931,0.102654,0.105547,0.108609,0.111837,0.115232,0.118792,0.122515,0.126400,0.130446,0.134650,0.139012,0.143529,0.148200,0.153023,0.157997,0.163118,0.168385,0.173797,0.179351,0.185045,0.190876,0.196843,0.202943,0.209174,0.215533,0.222018,0.228626,0.235355,0.242202,0.249165,0.256241,0.263426,0.270719,0.278117,0.285616,0.293214,0.300907,0.308693,0.316569,0.324532,0.332578,0.340705,0.348909,0.357187,0.365536,0.373953,0.382434,0.390977,0.399577,0.408231,0.416937,0.425690,0.434488,0.443327,0.452203,0.461113,0.470054,0.479022,0.488014,0.497026,0.506054,0.515096,0.524147,0.533205,0.542265,0.551324,0.560379,0.569426,0.578462,0.587482,0.596485,0.605465,0.614420,0.623346,0.632239,0.641097,0.649916,0.658692,0.667422,0.676103,0.684730,0.693302,0.701814,0.710264,0.718647,0.726961,0.735202,0.743368,0.751455,0.759460,0.767380,0.775211,0.782951,0.790597,0.798146,0.805595,0.812940,0.820180,0.827311,0.834331,0.841236,0.848024,0.854693,0.861240,0.867663,0.873958,0.880124,0.886157,0.892057,0.897820,0.903444,0.908927,0.914266,0.919461,0.924509,0.929407,0.934154,0.938749,0.943188,0.947472,0.951597,0.955562,0.959367,0.963008,0.966486,0.969798,0.972943,0.975920,0.978729,0.981367,0.983834,0.986128,0.988250,0.990198,0.991971,0.993568,0.994990,0.996235,0.997303,0.998194,0.998907,0.999442,0.999799,0.999978,0.999978,0.999799,0.999442,0.998907,0.998194,0.997303,0.996235,0.994990,0.993568,0.991971,0.990198,0.988250,0.986128,0.983834,0.981367,0.978729,0.975920,0.972943,0.969798,0.966486,0.963008,0.959367,0.955562,0.951597,0.947472,0.943188,0.938749,0.934154,0.929407,0.924509,0.919461,0.914266,0.908927,0.903444,0.897820,0.892057,0.886157,0.880124,0.873958,0.867663,0.861240,0.854693,0.848024,0.841236,0.834331,0.827311,0.820180,0.812940,0.805595,0.798146,0.790597,0.782951,0.775211,0.767380,0.759460,0.751455,0.743368,0.735202,0.726961,0.718647,0.710264,0.701814,0.693302,0.684730,0.676103,0.667422,0.658692,0.649916,0.641097,0.632239,0.623346,0.614420,0.605465,0.596485,0.587482,0.578462,0.569426,0.560379,0.551324,0.542265,0.533205,0.524147,0.515096,0.506054,0.497026,0.488014,0.479022,0.470054,0.461113,0.452203,0.443327,0.434488,0.425690,0.416937,0.408231,0.399577,0.390977,0.382434,0.373953,0.365536,0.357187,0.348909,0.340705,0.332578,0.324532,0.316569,0.308694,0.300907,0.293214,0.285616,0.278117,0.270719,0.263426,0.256241,0.249165,0.242202,0.235355,0.228626,0.222018,0.215533,0.209174,0.202943,0.196843,0.190876,0.185045,0.179351,0.173797,0.168385,0.163118,0.157997,0.153023,0.148200,0.143529,0.139012,0.134650,0.130446,0.126400,0.122515,0.118792,0.115232,0.111838,0.108609,0.105547,0.102654,0.099931,0.097378,0.094998,0.092789,0.090755,0.088894,0.087209,0.085699,0.084365,0.083209,0.082229,0.081427,0.080803,0.080357,0.080089,0.080000};
int i;

//����fft��ز���
//ԭʼ���ݣ��任��Ƶ�����ݣ����任��ʱ������
complex samples[FRAMELEN], XFreq[FRAMELEN], xTime[FRAMELEN];
//FFT���Ƶ����
short frqValue[FRAMELEN];


typedef struct {              
    FILE *input_ptr;
    FILE *output_ptr;
    FILE *output_file_ptr;
    short in_array[FRAMELEN];   //��ȡ���ķ�֡����
    short out_array[FRAMELEN];  //FFT���Ƶ������
    short tmp_array[FRAMELEN];  //�������飬�Ӵ��󡢻ָ�ǰ���飬��FFT֮ǰ��IFFT֮������
    short output_data; 
    short pre_out_array[FRAMELEN]; //�ָ�����ʱ�õ���ǰһ֡������

 
}pcm_data;




int open_pcm_data(pcm_data *pcm_data_ptr)
{
    if(NULL == pcm_data_ptr) 
    {
        return -1;
    }
    int err = 0;
 
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
    if(NULL == pcm_data_ptr)
    {
        return -1;
    }
    fclose(pcm_data_ptr->input_ptr);
    fclose(pcm_data_ptr->output_ptr);
    fclose(pcm_data_ptr->output_file_ptr);

    return 0;
}

 
        


int proc_pcm_data(pcm_data *pcm_data_ptr) 
{
    if (NULL == pcm_data_ptr)
	{
		return -1;
	}
    short recover_array[160];
    memset(pcm_data_ptr->in_array,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->out_array,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->tmp_array,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->pre_out_array,0,sizeof(short)*FRAMELEN); 
    memset(recover_array,0,sizeof(short)*(FRAMELEN-STEP)); 

     while (!feof(pcm_data_ptr->input_ptr))   
    {
       
        memcpy(pcm_data_ptr->in_array,&pcm_data_ptr->in_array[STEP],sizeof(short)*(FRAMELEN-STEP));  
        fread(&pcm_data_ptr->in_array[STEP],sizeof(short),FRAMELEN-STEP,pcm_data_ptr->input_ptr); 

        for(i=0;i<FRAMELEN;i++)
        {
            fprintf(pcm_data_ptr->output_ptr,"%d,",pcm_data_ptr->in_array[i]); //��֡������
            printf("%d", pcm_data_ptr->in_array[i]);

            pcm_data_ptr->tmp_array[i]=(short)(pcm_data_ptr->in_array[i]*Hamming_Win[i]); //�Ӵ������ݽ�����short�ض�
         }
                fprintf(pcm_data_ptr->output_ptr,"\n"); 

        //�м������������,FFT��IFFT
        //ԭʼ���ݵĸ�����ʽ��ԭʼ���ݡ�FFT����
       complexSet(samples, pcm_data_ptr->tmp_array, FRAMELEN);
       //FFT�任:FFT��Ƶ�ʡ�ԭʼ���ݵĸ�����ʽ��FFT������FFT��ķ�Ƶ����
       dft(XFreq, samples, FRAMELEN,pcm_data_ptr->out_array);


        for(i=0;i<FRAMELEN;i++)
        {

             if(pcm_data_ptr->out_array[i]>0)
            {
          
                 printf("%d   %d\n",i,pcm_data_ptr->out_array[i] );
            }

        }

       //IFFT�任��FFT��ĸ�Ƶ�ʡ�IFFT���ʱ���źŵĸ�����ʽ��FFT������IFFT���ʱ���źŵ�ʵ������
       idft(XFreq, xTime,FRAMELEN,pcm_data_ptr->tmp_array);


       
        for(i=0;i<FRAMELEN-STEP;i++)
        {
            
            recover_array[i] = pcm_data_ptr->pre_out_array[i+160]+pcm_data_ptr->tmp_array[i];
            
        }
        for(i=0;i<FRAMELEN;i++)
        {
            pcm_data_ptr->pre_out_array[i] = pcm_data_ptr->tmp_array[i];
        }

        fwrite(recover_array, sizeof(short),FRAMELEN-STEP ,pcm_data_ptr->output_file_ptr);
       

     }  

   return 0;    
}

int main()
{
    pcm_data pcm_data;

	open_pcm_data(&pcm_data);

	proc_pcm_data(&pcm_data);

	close_pcm_data(&pcm_data);

	return 0;

}



/*
#include<stdio.h>
#include<stdlib.h>
#include<string.h>  
#include<math.h>
const float PI = 3.1415926;

const int FRAMELEN = 320; //
const int STEP = 160;  //
const float MAX_AMPLITUDE_16S_P = 32767;  
const float MAX_AMPLITUDE_16S_N = 32768;
int i;

typedef struct {              
    FILE *input_ptr;
    FILE *output_ptr;
    FILE *output_file_ptr;
    short array1[FRAMELEN]; //��֡������
    float array1_f[FRAMELEN]; //��֡��ת��Ϊfloat
    float array2[FRAMELEN]; //�Ӵ�������
    float array3[FRAMELEN]; //�ص���Ӻ�����
    short array3_s[FRAMELEN]; //�ص���Ӻ�ת��Ϊshort
    short output_data; 
 
}pcm_data;



int open_pcm_data(pcm_data *pcm_data_ptr)
{
    if(NULL == pcm_data_ptr) 
    {
        return -1;
    }
    int err = 0;
 
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
    if(NULL == pcm_data_ptr)
    {
        return -1;
    }
    fclose(pcm_data_ptr->input_ptr);
    fclose(pcm_data_ptr->output_ptr);
    fclose(pcm_data_ptr->output_file_ptr);

    return 0;
}


int proc_pcm_data(pcm_data *pcm_data_ptr) 
{
    if (NULL == pcm_data_ptr)
	{
		return -1;
	}
    memset(pcm_data_ptr->array1,0,sizeof(short)*FRAMELEN); 
    memset(pcm_data_ptr->array1_f,0,sizeof(float)*FRAMELEN); 
    memset(pcm_data_ptr->array2,0,sizeof(float)*FRAMELEN); 
    memset(pcm_data_ptr->array3,0,sizeof(float)*FRAMELEN); 
    memset(pcm_data_ptr->array3_s,0,sizeof(short)*FRAMELEN); 
     while (!feof(pcm_data_ptr->input_ptr))   
    {
       
        memcpy(pcm_data_ptr->array1,&pcm_data_ptr->array1[STEP],sizeof(short)*(FRAMELEN-STEP));  
        fread(&pcm_data_ptr->array1[STEP],sizeof(short),FRAMELEN-STEP,pcm_data_ptr->input_ptr); 
        for(i=0;i<FRAMELEN;i++)  
        {
            //printf("%d,",pcm_data_ptr->array1[i]); 
            //fprintf(pcm_data_ptr->output_ptr,"%d,",pcm_data_ptr->array1[i]); //��֤֡��320֡��160ʱ�����ȡ�Ƿ���ȷ
            if(pcm_data_ptr->array1[i]>=0)
            {
                pcm_data_ptr->array1_f[i]=(float)pcm_data_ptr->array1[i]/MAX_AMPLITUDE_16S_P ;

            }
            else
            {
                pcm_data_ptr->array1_f[i]=(float)pcm_data_ptr->array1[i]/MAX_AMPLITUDE_16S_N ;
            }

           // printf("%f,",pcm_data_ptr->array1_f[i]); 

          // ��Ӻ�����
            pcm_data_ptr->array2[i] = pcm_data_ptr->array1_f[i]*(0.54-0.46*cos(2*PI*i/(FRAMELEN-1)));

            //printf("%f\n",pcm_data_ptr->array2[i]);//��֤�Ӵ��������
            //fprintf(pcm_data_ptr->output_ptr,"%d,%f,",pcm_data_ptr->array1[i],pcm_data_ptr->array2[i]); 

            //��֡�Ӵ����ص���ӣ��鿴������ԭʼ�����Ƿ���ͬ
            if(i<=STEP)
            {
               pcm_data_ptr->array3[i]=pcm_data_ptr->array2[i]+pcm_data_ptr->array1_f[i]*(0.54-0.46*cos(2*PI*(i+STEP)/(FRAMELEN-1)));
            }
            else
            {
                pcm_data_ptr->array3[i]=pcm_data_ptr->array2[i]+pcm_data_ptr->array1_f[i]*(0.54-0.46*cos(2*PI*(i-STEP)/(FRAMELEN-1)));
            }
            //printf("%f,",pcm_data_ptr->array3[i]);
            //fprintf(pcm_data_ptr->output_ptr,"%f,",pcm_data_ptr->array3[i]); 
            if(pcm_data_ptr->array3[i]>=0)
            {
               pcm_data_ptr->array3_s[i]=(short)(pcm_data_ptr->array3[i]*MAX_AMPLITUDE_16S_P);
            }
            else
            {
                pcm_data_ptr->array3_s[i] =(short)(pcm_data_ptr->array3[i]*MAX_AMPLITUDE_16S_N);

            }

           // printf("%d\n",pcm_data_ptr->array3_s[i]);
             fprintf(pcm_data_ptr->output_ptr,"%d,",pcm_data_ptr->array3_s[i]); 

            
            //��FFT��N=320

            //��IFFT

        }
        fwrite(&pcm_data_ptr->array3_s, sizeof(short),FRAMELEN-STEP ,pcm_data_ptr->output_file_ptr);

        fprintf(pcm_data_ptr->output_ptr,"\n");
     }  
       
}

int main()
{
    pcm_data pcm_data;

	open_pcm_data(&pcm_data);

	proc_pcm_data(&pcm_data);

	close_pcm_data(&pcm_data);

	return 0;

}
*/



/*��ú�����ϵ���ĳ���
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
const double PI =3.1415926;
FILE *output_ptr;
int main()
{
    int i;
    double Hamming_Win[320];
    int err = 0;
 
    err = fopen_s(&output_ptr,"out.txt","w+");  

    if(0!=err)
    {
        return -2;
    }

  for(i=0;i<320;i++)
        {
            Hamming_Win[i] = 0.54-0.46*cos(2*PI*i/(320-1));
            printf("%f\n",Hamming_Win[i]);
            fprintf(output_ptr,"%f,",Hamming_Win[i]);
         }
  fclose(output_ptr);
  return 0;

}
*/
