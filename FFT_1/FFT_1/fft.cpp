#include<stdio.h>
#include <math.h>
#include "fft.h"
#define SR_NUM 16000 //设置采样率16000
#define pi    3.1415926

complex complexAdd(complex a, complex b) { //复数加  两个复数相加，实部虚部分别相加
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
	for (int i = 0; i < Num; i++)
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
	complex temp;
	for (int i = 0; i < len; i++)
	{
		Xf[i].re = 0;  //初始化复频率值
		Xf[i].im = 0;
		for (int n = 0; n < len; n++)
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
	    frqValue[i] = int(2.0/SR_NUM*sqrt(Xf[i].re*Xf[i].re + Xf[i].im*Xf[i].im));//转换后的分频幅
 
	}
}
//离散傅里叶逆变换
//Xin[]标识变换后频域，xOut[]为时域采样信号
void idft(complex Xin[], complex xOut[], int len,short xOut_re[]) 
{
	complex temp;
	//int k, n;
	for (int k = 0; k < len; k++)
	{
		xOut[k].re = 0;
		xOut[k].im = 0;
		for (int n = 0; n < len; n++)
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










/*

// 冒泡法排序函数
void BubbleSort(float pt[], int Cnt)
{
	int     k      = 0;
        float temp = 0;
	while (Cnt > 0)
	{
		for (k=0; k<Cnt-1; k++)
		{
			if (pt[k] < pt[k+1])
			{
				temp    = pt[k];
				pt[k]   = pt[k+1];
				pt[k+1] = temp;
			}
		}
		Cnt--;
	}
}

//复数乘法
struct compx EE(struct compx b1, struct compx b2)
{
	struct compx b3;
	b3.real = b1.real*b2.real - b1.imag*b2.imag;
	b3.imag = b1.real*b2.imag + b1.imag*b2.real;
	return (b3);
}

//快速傅立叶变换-FFT
void FFT(struct compx *xin, int Num)
{
	int f, m, LH, nm, i, k, j, L;
	double p, ps ;
	int le, B, ip;
	float pi;
	struct compx w, t;
	
	LH = Num / 2; 
	f  = Num;
	for (m=1; (f=f/2)!=1; m++);     // 2^m=N
	{
		for (L=m; L>=1; L--)          // 这里和时域的也有差别
		{ 
			le = pow(2.0, L);
			B  = le / 2;                // 每一级碟形运算间隔的点数
			pi = 3.1415;
			for (j=0; j<=B-1; j++)
			{
				p  =pow(2.0, m-L) * j;
				ps = 2*pi/Num*p;
				w.real = cos(ps);
				w.imag = -sin(ps);
				for (i=j; i<=Num-1; i=i+le)
				{
					ip = i+B;  
					t  = xin[i];
					xin[i].real = xin[i].real + xin[ip].real;
					xin[i].imag = xin[i].imag + xin[ip].imag;  
					xin[ip].real = xin[ip].real - t.real;
					xin[ip].imag = xin[ip].imag - t.imag;     
					xin[ip] = EE(xin[ip], w);
				}
			}
		}
	}
	// 变址运算
	nm = Num - 2;   
	j  = Num / 2;
	for(i=1;i<=nm;i++)
	{
	if(i<j){t=xin[j];xin[j]=xin[i];xin[i]=t;}
	k=LH;
	while(j>=k){j=j-k;k=k/2;}
	j=j+k;
	}
}

*/