#ifndef WANDER_H
#define WANDER_H 
/*-
 * A random walk version that allows the setting of a goal that will be reached
 * in a specified number of steps.
 *
 * Note the random number generation uses the POSIX C standard library random()
 * seeded with the time of the first call to wander().
 *
 * (c) August 2012, nicholas esterer
 *
 */

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

/* no error */
#define ERR_WANDER_GOOD		    0 
/* unreachable goal */
#define ERR_WANDER_BAD_GOAL	    -1
/* length less than 1 */
#define ERR_WANDER_BAD_LEN	    -2 
/* kmin greater than kmax */
#define ERR_WANDER_BAD_K	    -3
/* upper bound lower than start or goal, or lower bound higher than start or
 * goal*/
#define ERR_WANDER_BAD_BOUND	    -4

/* seed the generator automatically */
void
swanderdev(void) ;

/* In "steps" interations, wander will fill "rslt" with consecutive values that
 * lie within [min,max] (inclusive) but will arrive at "goal" by the last
 * iteration. "rslt" must be large enough to hold "steps".*/
int
wander(int s, int g, int kmi, int kma, int lbnd, int ubnd, int *a, int len) ;

__END_DECLS
#endif /* WANDER_H */
