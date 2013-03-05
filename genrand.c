/* generate tables of values using various random distributions */

#include <stdlib.h> 
#include <time.h> 
#include <cm/gen_tab.h> 

/* fill a table with uniformly distributed random numbers constrained to the
 * range [a,b) */
int
cm_fill_rand_tab_uni(double *tab, long int len, double a, double b)
{
    if(a>b){
	/* swap a and b */
	double tmp = a;
	a = b;
	b = tmp;
    }else if(a==b){
	return a;
    }
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_ranlxs0);
    if(rng == NULL)
	return -1;
    gsl_rng_set(rng, time(NULL));
    while(len-- > 0)
	tab[len] = gsl_rng_uniform(rng) * (b - a) + a;
    gsl_rng_free(rng);
    return 0;
}

/* fill a table with uniformly distributed random numbers constrained to the
 * range [a,b). Accepts an external random number generator. */
int
cm_rng_fill_rand_tab_uni(gsl_rng *rng, double *tab, long int len,
	double a, double b)
{
    if(a>b){
	/* swap a and b */
	double tmp = a;
	a = b;
	b = tmp;
    }else if(a==b){
	return a;
    }
    if(rng == NULL)
	return -1;
    while(len-- > 0)
	tab[len] = gsl_rng_uniform(rng) * (b - a) + a;
    return 0;
}

/* allocate a table filled with uniformly distributed random numbers constrained
 * to the range [a,b) */
double *
cm_gen_rand_tab_uni(long int len, double a, double b)
{
    double *tab = (double*)malloc(len*sizeof(double));
    int err;
    if(err = cm_fill_rand_tab_uni(tab, len, a, b))
	return NULL;
    else
	return tab;
}
