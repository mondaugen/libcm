#ifndef INTERPUTILS_H
#define INTERPUTILS_H 
#include <stddef.h> 

/* Linear interpolation macros. No error checking. */
#define cm_flinterp_(x1,y1,x2,y2,x) (((((double)(y2)) - ((double)(y1)))\
	    /(((double)(x2)) - ((double)(x1)))) * (((double)(x))\
	    - ((double)(x1))) + ((double)(y1)))
#define  cm_flinterp_norm_(y1,y2,x) cm_flinterp_(0,y1,1,y2,x)

/* Make a line that fits the points (x1,y1), (x2,y2) and evaluate the point x on
 * this line */
double
cm_flinterp(double x1, double y1, double x2, double y2, double x);

/* Sometimes you just wanna simply linearly interpolate between two values using
 * 0-1 as your range */
double
cm_flinterp_norm(double y1, double y2, double x);

/* Linearly interpolate between all values in 4 arrays, putting the result in
 * a fifth */
int
cm_flinterp_a(double *x1, double *y1, double *x2, double *y2, double *y,
	double x, size_t len);

/* Linearly interpolate between two arrays using 0-1 as the domain */
int
cm_flinterp_a_norm(double *y1, double *y2, double *y, double x, size_t len);

#endif /* INTERPUTILS_H */
