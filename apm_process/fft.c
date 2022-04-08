#include<stdio.h>
#include <math.h>
#include "fft.h"
#define SR_NUM 16000 //���ò�����16000
#define pi    3.1415926

int i;

complex complexAdd(complex a, complex b) 
{ //������  ����������ӣ�ʵ���鲿�ֱ����
	complex rt;
	rt.re = a.re + b.re;
	rt.im = a.im + b.im;
	return rt;
}
 
complex complexMult(complex a, complex b) { //������,�ۺ���
	complex rt;
	rt.re = a.re*b.re - a.im*b.im;
	rt.im = a.im*b.re + a.re*b.im;
	return rt;
}
 
void complexSet(complex *a, short *b, int Num)//��������,��ԭʼ�źŸ�����
{
    int i;
	for (i = 0; i < Num; i++)
	{
		a[i].re = b[i];
		a[i].im = 0;
	}
	return;
}
 
//��ɢ����Ҷ�任
//X[]��ʶ�任��Ƶ��(in_out)��x[]Ϊʱ������ź�(in)��lenΪfft������frqValueΪ��Ƶ��ֵ��
void dft(complex Xf[], complex xt[], int len,short frqValue[])
{ 
    int n,i;
	complex temp;
	for (i = 0; i < len; i++)
	{
		Xf[i].re = 0;  //��ʼ����Ƶ��ֵ
		Xf[i].im = 0;
		for (n = 0; n < len; n++)
		{
			temp.re = (float)cos(2 * pi*i*n/len);
			temp.im = -(float)sin(2 * pi*i*n/ len);
			Xf[i] = complexAdd(Xf[i], complexMult(xt[n], temp));
		}
 
		if (i >= 6000)//ȥ����Ƶ�ź�>6K
		{
			Xf[i].re = 0.0;
			Xf[i].im = 0.0;
		}
	    frqValue[i] = (int)(2.0/SR_NUM*sqrt(Xf[i].re*Xf[i].re + Xf[i].im*Xf[i].im));//ת����ķ�Ƶ��
 
	}
}

//��ɢ����Ҷ��任
//Xin[]��ʶ�任��Ƶ��xOut[]Ϊʱ������ź�
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
