#ifndef GEN_TAB_H
#define GEN_TAB_H 

#include <gsl/gsl_rng.h>

/* Library for generating tables of values */

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

int
cm_fill_sin_sum(double *tab, long int len, double *amps, int num);

double *
cm_make_sin_sum(long int len, double *amps, int num);

int
cm_fill_rand_tab_uni(double *tab, long int len, double a, double b);

double *
cm_gen_rand_tab_uni(long int len, double a, double b);

int
cm_rng_fill_rand_tab_uni(gsl_rng *rng, double *tab, long int len,
	double a, double b);

__END_DECLS

#endif /* GEN_TAB_H */
