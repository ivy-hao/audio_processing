#include<stdio.h>
#include <math.h>
#include "fft.h"
#define SR_NUM 16000 //���ò�����16000
#define pi    3.1415926

complex complexAdd(complex a, complex b) { //������  ����������ӣ�ʵ���鲿�ֱ����
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
	for (int i = 0; i < Num; i++)
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
	complex temp;
	for (int i = 0; i < len; i++)
	{
		Xf[i].re = 0;  //��ʼ����Ƶ��ֵ
		Xf[i].im = 0;
		for (int n = 0; n < len; n++)
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
	    frqValue[i] = int(2.0/SR_NUM*sqrt(Xf[i].re*Xf[i].re + Xf[i].im*Xf[i].im));//ת����ķ�Ƶ��
 
	}
}
//��ɢ����Ҷ��任
//Xin[]��ʶ�任��Ƶ��xOut[]Ϊʱ������ź�
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

// ð�ݷ�������
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

//�����˷�
struct compx EE(struct compx b1, struct compx b2)
{
	struct compx b3;
	b3.real = b1.real*b2.real - b1.imag*b2.imag;
	b3.imag = b1.real*b2.imag + b1.imag*b2.real;
	return (b3);
}

//���ٸ���Ҷ�任-FFT
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
		for (L=m; L>=1; L--)          // �����ʱ���Ҳ�в��
		{ 
			le = pow(2.0, L);
			B  = le / 2;                // ÿһ�������������ĵ���
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
	// ��ַ����
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