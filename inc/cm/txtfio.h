/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef TXTFIO_H
#define TXTFIO_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <comptyp.h> 
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

/* parse doubles from a string and put them in a double_ary_t */
int
fill_dbl_ary_from_str(double_ary_t *a, char *str);

/* parse 32-bit integers from a string and put them in an int32_ary_t. If base
 * is 0, base is set to 10. */
int
fill_int32_ary_from_str(int32_ary_t *a, char *str, int base);

/* return an ary_t_list made by reading the rows of a file, the last
 * element will be NULL*/
ary_t_list *
dbl_ary_t_list_from_rows(FILE *f);

/* return an ary_t_list made by reading the rows of a file, the last
 * element will be NULL*/
ary_t_list *
int32_ary_t_list_from_rows(FILE *f, int base);

/* use a path argument instead of a file, returns NULL on error */
ary_t_list *
dbl_ary_t_list_from_rows_path(const char* path);

/* use a path argument instead of a file, returns NULL on error */
ary_t_list *
int32_ary_t_list_from_rows_path(const char* path, int base);

__END_DECLS
#endif /* TXTFIO_H */
