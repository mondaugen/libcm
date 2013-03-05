/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef FILTER_H
#define FILTER_H 

#include <stddef.h> 

typedef struct __cm_filter_line {
    double *data;
    size_t length;
    int offset;
} cm_filter_line_t;

typedef struct __cm_allpole_filter {
    cm_filter_line_t *y;
    double *a; /* order a coefficients */
    double g;  /* gain */
    size_t order;
    int owna;  /* does the struct own the coefficients in a ? if so they will be deallocated with the struct */
} cm_allpole_filter_t;

/* shift the offset by n */
int
cm_fl_shift_n(cm_filter_line_t *fl, int n);

/* get ith value, returns 0 if out of bounds and errno is set */
double
cm_fl_get_ith(cm_filter_line_t *fl, int i);

int
cm_fl_set_ith(cm_filter_line_t *fl, int i, double val);

cm_filter_line_t *
cm_fl_alloc(size_t length);

void
cm_fl_free(cm_filter_line_t *fl);

int
cm_allpole_filter_init(cm_allpole_filter_t **ap, size_t order);

cm_allpole_filter_t *
cm_allpole_filter_alloc(size_t order);

/* creates an all pole filter that does not own the coefficients in a and so
 * will not try and deallocate them when freed. Assumes a has length equal to
 * order. */
cm_allpole_filter_t *
cm_allpole_filter_view_alloc(size_t order, double *a);

void
cm_allpole_filter_free(cm_allpole_filter_t *ap);

/* filter a signal x and return the result, updating the internal buffers */
double
cm_allpole_filter(cm_allpole_filter_t *A, double *x);

#endif /* FILTER_H */
