/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#include <cm/wander.h> 
#include <stdlib.h>
#include <time.h>

#define MIN(x,y) x<y?x:y
#define MAX(x,y) x>y?x:y 

/* returns a positive random number between 0 and val - 1 inclusive */
static int
scaled_pos_rand( int val )
{
    if( val <= 0 )
	return 0 ;
    else
	return random() % val;
}

/* returns a random number in the range [min,max] (inclusive) */
static int 
rand_range( int min, int max )
{
    return min + scaled_pos_rand( max - min + 1 ) ;
}

/* constrains val between max and min inclusive */
static int
clip( int val, int min, int max )
{
    if( val > max )
	return max ;
    else if( val < min )
	return min ;
    else
	return val ;
}

/* seed the generator automatically */
void
swanderdev(void)
{
    srandomdev();
}

/* wander */
int
wander(int s, int g, int kmi, int kma, int lbnd, int ubnd, int *a, int len)
{
    if(len<1)
	return ERR_WANDER_BAD_LEN;  /* bad length */
    if(kmi>kma)
	return ERR_WANDER_BAD_K;    /* bad kmin and kmax */
    if(!(((g-s) >= kmi*(len-1)) && ((g-s) <= kma*(len-1))))
	return ERR_WANDER_BAD_GOAL; /* unreachable goal */
    if((lbnd>s)||(lbnd>g)||(ubnd<s)||(ubnd<g))
	return ERR_WANDER_BAD_BOUND; /* boundaries make start or goal impossible */
    int i = 0;
    a[i] = s;
    i++;
    while(i < len){
	int max = g - s - kmi*(len - 1 - i) ;
	int min = g - s - kma*(len - 1 - i) ;
	s += rand_range(MAX(clip(kmi,min,max),lbnd-s),
		MIN(clip(kma,min,max),ubnd-s));
	a[i] = s;
	i++;
    }
    return ERR_WANDER_GOOD;
}

/* this will hopefully allow you to go from any point to any goal, rather than
 * having the goal remain the same as the starting value */
/*
int
main(int argc, char *argv[])
{
    srand((int)time(NULL));
    int len = 20;
    int a [len];
    int s = 0;
    int g = 10;
    int i = 0;
    a[i] = s;
    i++;
    int kmi =  1;
    int kma =  2;
    while(i < len){
	int max = g - s - kmi*(len - 1 - i) ;
	int min = g - s - kma*(len - 1 - i) ;
	s += rand_range(clip(kmi,min,max), clip(kma,min,max));
	a[i] = s;
	i++;
    }

    printf("%d\n", a[0]);
    for(i=1; i<len; i++){
//	int j = a[i];
//	while(j-->0)
//	    printf(".");
	int delta = a[i] - a[i-1] ;
	printf("%d\n", a[i]);
	printf("Delta: %d\n", delta) ;
    }
    return 0;

}
*/
