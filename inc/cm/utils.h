/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef CM_UTILS_H
#define CM_UTILS_H 

#include <stddef.h> 
#include <cm/conversions.h> 
#include <cm/sigutils.h> 

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

#define cm_vswp_(a,b) cm_vswap((void**)(a),(void**)(b)) 

/* wrap x in the range [a,b) */
int cm_wrap(int x, int a, int b);
double cm_fwrap(double x, double a, double b);
/* clip s to the range [a,b] */
int cm_clip(int x, int a, int b);
double cm_fclip(double x,  double a, double b);
/* returns -1 if less than a, 1 i >= b 0 otherwise */
static inline int
cm_check_bounds(int i, int a, int b)
{
    if(i < a)
	return -1;
    if(i >= b)
	return 1;
    return 0;
}

static inline void
cm_vswap(void **a, void **b)
{
    void *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

__END_DECLS

#endif /* CM_UTILS_H */
