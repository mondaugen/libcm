#ifndef WINDOWFUN_H
#define WINDOWFUN_H 

#include <stddef.h>

/* Fills a with a raised cosine. If frequency 1 then the array will have one
 * period of the raised cosine, if 2 then 2 periods etc. Phase is the phase
 * offset which is a number theta s.t. theta*2*pi = phase in radians. 0 gives
 * you a cosine function starting at x = 0 and y = 1. */
int
cm_fill_raised_cos(double *a, size_t N, double amp, double freq, double phase);
#define cm_hann_(a,N) cm_fill_raised_cos((a),(N),1.0,1.0,0.5)

/* Multiplies two arrays entrywise, effectively windowing one with the other.
 * Assumes both have the same length. The result is put in y and w is unchanged. 
 * This works even if x == y (as pointers)*/
static inline void
cm_window(double *y, double *x, double *w, size_t len)
{
    while(len-- > 0)
	y[len] = x[len]*w[len];
};
#define cm_window_inplace_(x,w,len) cm_window((x),(x),(w),(len))

#endif /* WINDOWFUN_H */
