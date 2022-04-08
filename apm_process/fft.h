#ifndef __FFT_H__
#define __FFT_H__

typedef struct 
{
	float re; //real µ≤ø
	float im; //image–È≤ø
}complex;

void complexSet(complex *a, short *b, int Num);


void dft(complex Xf[], complex xt[], int len,short frqValue[]);


void idft(complex Xin[], complex xOut[], int len,short xOut_re[]);


#endif      
