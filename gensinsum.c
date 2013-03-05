/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* generate tables of one period of a wave of harmonically related sinusoids */
#include <math.h>
#include <stdlib.h> 
#include <cm/gen_tab.h> 

/* tab is a table to fill with one period of values, len is the length of the
 * table, amps is an array of the amplitudes of the waves, num is the number of
 * sines to sum */
int
cm_fill_sin_sum(double *tab, long int len, double *amps, int num)
{
    int i, j;
    for(i=0; i<len; i++){
	tab[i] = 0.0;
	for(j=0; j<num; j++)
	    tab[i] += sin(2.0*M_PI*((double)i/(double)len)
			*((double)j+1))*amps[j];
    }
    return 0;
}

/* returns a newly allocated array of doubles, like cm_fill_sin_sum but no array
 * need be preallocated */
double *
cm_make_sin_sum(long int len, double *amps, int num)
{
    double *tab = (double*)malloc(len*sizeof(double));
    int err;
    if(err = cm_fill_sin_sum(tab, len, amps, num))
	return NULL;
    return tab;
}
