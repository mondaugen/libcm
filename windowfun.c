/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* Functions for making all kinds of windows */

#include <math.h>
#include <cm/windowfun.h> 

/* Fills a with a raised cosine. If frequency 1 then the array will have one
 * period of the raised cosine, if 2 then 2 periods etc. Phase is the phase
 * offset which is a number theta s.t. theta*2*pi = phase in radians. 0 gives
 * you a cosine function starting at x = 0 and y = 1. */
int
cm_fill_raised_cos(double *a, size_t N, double amp, double freq, double phase)
{
    /* We want the function as symmetrical as possible around the centre of the
     * table, therefore we add half an index. */
    size_t i;
    for(i = 0; i < N; i++)
	a[i] = amp*(1.0 + cos(M_PI*2.0*(((double)i) + 0.5)*freq/((double)N)
		    + M_PI*2.0*phase))/2.0;
    return 0;
}

