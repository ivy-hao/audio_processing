#ifndef __FFT_H__
#define __FFT_H__

struct complex
{
	float re; //real实部
	float im; //image虚部
};

void complexSet(complex *a, short *b, int Num);//复数设置


void dft(complex Xf[], complex xt[], int len,short frqValue[]);


void idft(complex Xin[], complex xOut[], int len,short xOut_re[]);


/*
struct compx    //定义一个复数结构体
{
  float real;
  float imag;
};
void BubbleSort(float pt[], int Cnt);
void FFT(struct compx *xin, int Num);
*/

#endif      
