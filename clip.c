/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* constrain values between a and b */

#include <cm/utils.h> 

int cm_clip(int x, int a, int b)
{
    if(a>b){
	/* swap a and b */
	int tmp = a;
	a = b;
	b = tmp;
    }else if(a==b){
	return a;
    }
    if(x>b)
	return b;
    else if(x<a)
	return a;
    else
	return x;
}

double cm_fclip(double x,  double a, double b)
{
    if(a>b){
	/* swap a and b */
	double tmp = a;
	a = b;
	b = tmp;
    }else if(a==b){
	return a;
    }
    if(x>b)
	return b;
    else if(x<a)
	return a;
    else
	return x;
}

