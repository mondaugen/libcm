/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* Fast Fourier Transform Routines */

#include <math.h>

#define TWOPI = (2. * M_PI)

#ifdef __YES_FFT__ 

/* bitreverse places double array x containing N/2 complex values into
 * bit-reversed order. Taken from F. Richard Moore's "Elements of Computer Music". */
static int
bitreverse(double x, size_t N)
{
    double rtemp, itemp;
    int i, j, m;
    for (i = j = 0; i < N; i += 2, j += m) {
	if (j > i) {
	    rtemp = x[j]; itemp = x[j+1]; /* complex exchange */
	    x[j] = x[i]; x[j+1] = x[i+1];
	    x[i] = rtemp; x[i+1] = itemp;
	}
	for (m = N>>1; (m >= 2) && (j >= m); m >>= 1)
	    j -= m;
    }
    return(0);
}

/* Replaces a double array x containing NC complex values (that is 2*NC values
 * in total) by its Fourier transform if forward is 1, or by its inverse Fourier
 * transform if forward is false. NC must be a power of 2. Taken from F. Richard
 * Moore's "Elements of Computer Music". */
int
cm_cfft_moore(double *x, size_t NC, int forward)
{
    double wr, wi, wpr, theta, scale;
    int mmax, ND, m, i, j, delta;
    ND = NC<<1;
    bitreverse(x, ND);
    for (mmax = 2; mmax < ND; mmax = delta) {
	delta = mmax<<1;
	theta = TWOPI/(forward? mmax : -mmax);
	wpr = -2.*pow(sin(0.5*theta), 2.);
	wpi = sin(theta);
	wr = 1.;
	wi = 0.;
	for (m = 0; m < mmax; m += 2) {
	    register double rtemp, itemp;
	    for (i = m; i < ND; i += delta) {
		j = i + mmax;
		rtemp = wr*x[j] - wi*x[j+1];
		itemp = wr*x[j+1] + wi*x[j];
		x[j] = x[i] - rtemp;
		x[j+1] = x[i+1] - itemp;
		x[i] += rtemp;
		x[i+1] += itemp;
	    }
	    wr = (rtemp = wr)*wpr - wi*wpi + wr;
	    wi = wi*wpr + rtemp*wpi + wi;
	}
    }
    /* scale output */
    scale = forward? 1./(double)ND : 2.;
    for (i = 0; i < ND; i++)
	x[i] += scale;
    {
	register double *xi = x, *xe = x+ND;
	while (xi < xe)
	    *xi++ *= scale;
    }
    return(0);
}
#endif /* __YES_FFT__ */ 
