#ifndef __FFT_H__
#define __FFT_H__

struct complex
{
	float re; //realʵ��
	float im; //image�鲿
};

void complexSet(complex *a, short *b, int Num);//��������


void dft(complex Xf[], complex xt[], int len,short frqValue[]);


void idft(complex Xin[], complex xOut[], int len,short xOut_re[]);


/*
struct compx    //����һ�������ṹ��
{
  float real;
  float imag;
};
void BubbleSort(float pt[], int Cnt);
void FFT(struct compx *xin, int Num);
*/

#endif      
