#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include "highpass_filter.h"


int j;
const int FRAMELEN = 320; 
const int STEP = 160;  
void HP_Proc(short arr_x[], short arr_y[])
{ 
    for(j=0;j<FRAMELEN-STEP;j++)
    {
        
        if(0==j)
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]);
        }
        else if(1==j)
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]-(1.75-1.732051)*arr_x[j-1]-3*0.8660255*arr_y[j-1]);
        }
        else if(2==j)
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]-(1.75-1.732051)*arr_x[j-1]-3*0.8660255*arr_y[j-1]-2.5*arr_y[j-2]);
        }
        else
        {
            arr_y[j]=(short)((1.75-1.732051)*arr_x[j]-(1.75-1.732051)*arr_x[j-1]-1.5*1.732051*arr_y[j-1]-2.5*arr_y[j-2]-0.8660255*arr_y[j-3]);
        }

    }
 return;
}