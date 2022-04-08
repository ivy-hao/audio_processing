#include<stdio.h>
#include <math.h>
#include "fft.h"
#define SR_NUM 16000 //设置采样率16000
#define pi    3.1415926

int i;

complex complexAdd(complex a, complex b) 
{ //复数加  两个复数相加，实部虚部分别相加
	complex rt;
	rt.re = a.re + b.re;
	rt.im = a.im + b.im;
	return rt;
}
 
complex complexMult(complex a, complex b) { //复数乘,综合律
	complex rt;
	rt.re = a.re*b.re - a.im*b.im;
	rt.im = a.im*b.re + a.re*b.im;
	return rt;
}
 
void complexSet(complex *a, short *b, int Num)//复数设置,将原始信号复数化
{
    int i;
	for (i = 0; i < Num; i++)
	{
		a[i].re = b[i];
		a[i].im = 0;
	}
	return;
}
 
//离散傅里叶变换
//X[]标识变换后频域(in_out)，x[]为时域采样信号(in)，len为fft点数，frqValue为分频幅值。
void dft(complex Xf[], complex xt[], int len,short frqValue[])
{ 
    int n,i;
	complex temp;
	for (i = 0; i < len; i++)
	{
		Xf[i].re = 0;  //初始化复频率值
		Xf[i].im = 0;
		for (n = 0; n < len; n++)
		{
			temp.re = (float)cos(2 * pi*i*n/len);
			temp.im = -(float)sin(2 * pi*i*n/ len);
			Xf[i] = complexAdd(Xf[i], complexMult(xt[n], temp));
		}
 
		if (i >= 6000)//去除高频信号>6K
		{
			Xf[i].re = 0.0;
			Xf[i].im = 0.0;
		}
	    frqValue[i] = (int)(2.0/SR_NUM*sqrt(Xf[i].re*Xf[i].re + Xf[i].im*Xf[i].im));//转换后的分频幅
 
	}
}

//离散傅里叶逆变换
//Xin[]标识变换后频域，xOut[]为时域采样信号
void idft(complex Xin[], complex xOut[], int len,short xOut_re[]) 
{
	complex temp;
	int k, n;
	for (k = 0; k < len; k++)
	{
		xOut[k].re = 0;
		xOut[k].im = 0;
		for (n = 0; n < len; n++)
		{
			temp.re = (float)cos(2 * pi*k*n / len );
			temp.im = (float)sin(2 * pi*k*n / len );
			xOut[k] = complexAdd(xOut[k], complexMult(Xin[n], temp));
		}
		xOut[k].re /= len;
		xOut[k].im /= len;
        xOut_re[k] = xOut[k].re;

	}
}
