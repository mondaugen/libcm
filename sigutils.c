/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#include <gsl/gsl_blas.h>
#include <cm/utils.h>

#ifdef CM_SIGU_DEBUG
 #include  <stdio.h> 
 #define  return(x) fprintf(stderr,"%g ", (x)); return x;
#endif  


void
cm_normalize_signal(double *x, size_t len, double s)
{
    gsl_vector_view X = gsl_vector_view_array(x,len);
    double max = gsl_vector_max(&X.vector);
    gsl_vector_scale(&X.vector, (1.0/max)*s);
}

#ifndef CM_SIGU_HAVE_INLINE 
/* Returns the autocorellation of signal x with itself offset by i indices. For
 * signal s, the autocorrelation R(i) is defined as:
 *	R(i) = s[0]s[i] + s[0]s[i+1] + ... + s[N-1]s[N-1-i].
 * If the indices are ever out of bounds, the conribution of that product to
 * the sum is 0.
 */
double
cm_autocorr(double *x, size_t N, int i)
{
    size_t j;
    double sum = 0;
    if(i < 0)
	i *= -1;
    for(j = i; j < N; j++)
	sum += (x[j-i])*(x[j]);
    return(sum);
}
#endif /* CM_SIGU_HAVE_INLINE */ 







