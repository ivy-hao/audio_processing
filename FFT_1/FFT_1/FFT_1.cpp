//读取pcm文件中的音频数据后，分帧：帧长20ms（320个点），重叠10ms（160个点），帧移10ms（160个点）加窗：汉明窗，并根据输出重叠相加恢复出原数据。
/*汉明窗：w（n）=0.54-0.46cos[2*pi*n/(N-1)]  0《n《N, 0,其他*/

/*此次代码问题：1、数组太多，一般如无特殊需求，数组有两个in_array和out_array。
2、处理分帧加窗后数据的时候，要按帧来进行处理，不能再按帧内元素进行处理。
3、加窗函数为一条固定曲线，因此将其计算出来放到数组中。
4、恢复数据时，应该只用到输出后的当前帧和前后帧的数据，而此次算法中用到了输入和当前帧输出，因此算法不正确。
根据以上问题，做出优化，优化前数据，放在后面注释掉了

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

//设置fft相关参数
//原始数据，变换后频域数据，反变换后时域数据
complex samples[FRAMELEN], XFreq[FRAMELEN], xTime[FRAMELEN];
//FFT后分频幅度
short frqValue[FRAMELEN];


typedef struct {              
    FILE *input_ptr;
    FILE *output_ptr;
    FILE *output_file_ptr;
    short in_array[FRAMELEN];   //读取到的分帧数据
    short out_array[FRAMELEN];  //FFT后的频域数据
    short tmp_array[FRAMELEN];  //过程数组，加窗后、恢复前数组，即FFT之前和IFFT之后数组
    short output_data; 
    short pre_out_array[FRAMELEN]; //恢复数据时用到的前一帧的数组

 
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
            fprintf(pcm_data_ptr->output_ptr,"%d,",pcm_data_ptr->in_array[i]); //分帧后数据
            printf("%d", pcm_data_ptr->in_array[i]);

            pcm_data_ptr->tmp_array[i]=(short)(pcm_data_ptr->in_array[i]*Hamming_Win[i]); //加窗后数据进行了short截断
         }
                fprintf(pcm_data_ptr->output_ptr,"\n"); 

        //中间进行其他处理,FFT，IFFT
        //原始数据的复数形式、原始数据、FFT点数
       complexSet(samples, pcm_data_ptr->tmp_array, FRAMELEN);
       //FFT变换:FFT后复频率、原始数据的复数形式、FFT点数、FFT后的分频幅度
       dft(XFreq, samples, FRAMELEN,pcm_data_ptr->out_array);


        for(i=0;i<FRAMELEN;i++)
        {

             if(pcm_data_ptr->out_array[i]>0)
            {
          
                 printf("%d   %d\n",i,pcm_data_ptr->out_array[i] );
            }

        }

       //IFFT变换：FFT后的复频率、IFFT后的时域信号的复数形式、FFT点数、IFFT后的时域信号的实数部分
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
    short array1[FRAMELEN]; //分帧后数据
    float array1_f[FRAMELEN]; //分帧后转换为float
    float array2[FRAMELEN]; //加窗后数据
    float array3[FRAMELEN]; //重叠相加后数据
    short array3_s[FRAMELEN]; //重叠相加后转换为short
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
            //fprintf(pcm_data_ptr->output_ptr,"%d,",pcm_data_ptr->array1[i]); //验证帧长320帧移160时数组读取是否正确
            if(pcm_data_ptr->array1[i]>=0)
            {
                pcm_data_ptr->array1_f[i]=(float)pcm_data_ptr->array1[i]/MAX_AMPLITUDE_16S_P ;

            }
            else
            {
                pcm_data_ptr->array1_f[i]=(float)pcm_data_ptr->array1[i]/MAX_AMPLITUDE_16S_N ;
            }

           // printf("%f,",pcm_data_ptr->array1_f[i]); 

          // 添加汉明窗
            pcm_data_ptr->array2[i] = pcm_data_ptr->array1_f[i]*(0.54-0.46*cos(2*PI*i/(FRAMELEN-1)));

            //printf("%f\n",pcm_data_ptr->array2[i]);//验证加窗后的数据
            //fprintf(pcm_data_ptr->output_ptr,"%d,%f,",pcm_data_ptr->array1[i],pcm_data_ptr->array2[i]); 

            //分帧加窗后重叠相加，查看数据与原始数据是否相同
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

            
            //作FFT，N=320

            //做IFFT

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



/*获得汉明窗系数的程序，
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
