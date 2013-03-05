#include <signal.h> 
#include <cm/interputils.h> 
/* Tools for interpolation */

/* Make a line that fits the points (x1,y1), (x2,y2) and evaluate the point x on
 * this line */
double
cm_flinterp(double x1, double y1, double x2, double y2, double x)
{
    /* y = mx + b */
    if(x2 == x1)
	raise(SIGFPE);
    return ((y2 - y1)/(x2 - x1)) * (x - x1) + y1;
}

/* Sometimes you just wanna simply linearly interpolate between two values using
 * 0-1 as the domain */
double
cm_flinterp_norm(double y1, double y2, double x)
{
    return cm_flinterp(0,y1,1,y2,x);
}

/* Linearly interpolate between all values in 4 arrays, putting the result in
 * a fifth */
int
cm_flinterp_a(double *x1, double *y1, double *x2, double *y2, double *y, double x, size_t len)
{

    int i;
    for( i = 0; i < len; i++)
	y[i] = cm_flinterp_(x1[i],y1[i],x2[i],y2[i],x);
    return 0;
}

/* Linearly interpolate between two arrays using 0-1 as the domain */
int
cm_flinterp_a_norm(double *y1, double *y2, double *y, double x, size_t len)
{
    int i;
    for( i = 0; i < len; i++)
	y[i] = cm_flinterp_norm_(y1[i],y2[i],x);
    return 0;
}
