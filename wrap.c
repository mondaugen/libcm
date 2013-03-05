/* wrap numbers between a and b */

#include <cm/utils.h> 
#include <math.h>

int cm_wrap(int x, int a, int b)
{
    if(a>b){
	/* swap entries */
	int tmp = a;
	a = b;
	b = tmp;
    }
    if(a==b)
	return a;
    if(x >= a){
	return ((x-a)%(b-a)) + a;
    }else{
	return (b-a) + ((x-a)%(b-a)) + a;
    }
}

double cm_fwrap(double x, double a, double b)
{
    if(a>b){
	/* swap entries */
	double tmp = a;
	a = b;
	b = tmp;
    }
    if(a==b)
	return a;
    if(x >= a){
	return fmod(x-a, b-a) + a;
    }else{
	return (b-a) + fmod(x-a, b-a) + a;
    }
}
