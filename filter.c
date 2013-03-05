/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* stuff for filters */
#include <cm/filter.h>
#include <cm/utils.h>
#include <cm/cm_errno.h>
#include <stdlib.h> 

/* shift the offset by n */
int
cm_fl_shift_n(cm_filter_line_t *fl, int n)
{
    fl->offset = cm_wrap(fl->offset + n, 0, fl->length);
    return 0;
}

/* get ith value, returns 0 if out of bounds and errno is set */
double
cm_fl_get_ith(cm_filter_line_t *fl, int i)
{
#ifdef  CM_BOUNDS_CHECK
    if(cm_bounds_check(i, 0, fl->length)){
	/* handle error */
	return 0;
    }
#endif /* CM_BOUNDS_CHECK */ 
    return fl->data[cm_wrap(fl->offset + i, 0, fl->length)];
}

int
cm_fl_set_ith(cm_filter_line_t *fl, int i, double val)
{
#ifdef CM_BOUNDS_CHECK
    if(cm_bounds_check(i, 0, fl->length)){
	/* handle error */
	return CM_ERR_BOUNDS;
    }
#endif /* CM_BOUNDS_CHECK */ 
    fl->data[cm_wrap(fl->offset + i, 0, fl->length)] = val;
    return 0;
}

cm_filter_line_t *
cm_fl_alloc(size_t length)
{
    cm_filter_line_t *result;
    if((!(result = (cm_filter_line_t*)malloc(sizeof(cm_filter_line_t)))))
	NULL;
    if((!(result->data = (double*)calloc(length, sizeof(double))))){
	free(result);
	NULL;
    }
    result->length = length;
    return result;
}

void
cm_fl_free(cm_filter_line_t *fl)
{
    if(fl != NULL) {
	free(fl->data);
	free(fl);
    }
}

int
cm_allpole_filter_init(cm_allpole_filter_t **ap, size_t order)
{
    if(!((*ap) = (cm_allpole_filter_t*)malloc(sizeof(cm_allpole_filter_t))))
	return CM_ERR_MEM;
    /* order + 1 because it stores y[0], y[1], ... y[order] */
    if(!((*ap)->y = cm_fl_alloc(order+1))){
	free(*ap);
	return CM_ERR_MEM;
    }
    return 0;
}

cm_allpole_filter_t *
cm_allpole_filter_alloc(size_t order)
{
    cm_allpole_filter_t *result;
    int err;
    if(err = cm_allpole_filter_init(&result, order))
	return NULL;
    if(!(result->a = (double*)calloc(order, sizeof(double)))){
	cm_fl_free(result->y);
	free(result);
	return NULL;
    }
    result->g = 1;
    result->order = order;
    result->owna = 1;
    return result;
}

/* creates an all pole filter that does not own the coefficients in a and so
 * will not try and deallocate them when freed. Assumes a has length equal to
 * order. */
cm_allpole_filter_t *
cm_allpole_filter_view_alloc(size_t order, double *a)
{
    cm_allpole_filter_t *result;
    int err;
    if(err = cm_allpole_filter_init(&result, order))
	return NULL;
    result->g = 1;
    result->a = a;
    result->order = order;
    result->owna = 0;
    return result;
}

void
cm_allpole_filter_free(cm_allpole_filter_t *ap)
{
    if(ap != NULL){
	cm_fl_free(ap->y);
	if(ap->owna)
	    free(ap->a);
	free(ap);
    }
}

/* filter a signal x and return the result, updating the internal buffers */
double
cm_allpole_filter(cm_allpole_filter_t *A, double *x)
{
    double y0 = A->g * (*x);
    /* coefficient a0 is always 1.0 so a1 is a[0] a2 a[1] etc. */
    int i;
    for(i = 0; i < A->order; i++)
	y0 -= A->a[i] * cm_fl_get_ith(A->y, i+1);
    cm_fl_set_ith(A->y, 0, y0);
    cm_fl_shift_n(A->y, -1);
    return y0;
}
    
