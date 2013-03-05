#ifndef SIGUTILS_H
#define SIGUTILS_H 

#include <stddef.h> 

#define CM_SIGU_DEBUG


#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define  __BEGIN_DECLS extern "C" {
# define  __END_DECLS }
#else  
# define  __BEGIN_DECLS /* empty */
# define  __END_DECLS /* empty */
#endif

__BEGIN_DECLS

#define CM_SIGU_HAVE_INLINE  

/* make an entire signal lie between s and -s */
void cm_normalize_signal(double *x, size_t len, double s);

/* Returns the autocorellation of signal x with itself offset by i indices. For
 * signal s, the autocorrelation R(i) is defined as:
 *	R(i) = s[0]s[i] + s[0]s[i+1] + ... + s[N-1]s[N-1-i].
 * If the indices are ever out of bounds, the conribution of that product to
 * the sum is 0.
 */
#ifdef CM_SIGU_HAVE_INLINE 
static inline double
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
#else
double
cm_autocorr(double *x, size_t N, int i);
#endif /* CM_SIGU_HAVE_INLINE */

static inline double
cm_find_max(double *a, size_t N)
{
    double max = a[N-1];
    while(N-- > 0){
	if(a[N] > max)
	    max = a[N];
    }
    return max;
}


__END_DECLS

#endif /* SIGUTILS_H */
