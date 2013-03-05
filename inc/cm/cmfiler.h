#ifndef CMFILER_H
#define CMFILER_H 

#include <stdarg.h> 
#include <stdlib.h>
#include <stdio.h> 
#include <cm/cm_errno.h> 

#define CM_BUF_SIZE	1024 


typedef enum __cm_file_entry_type_e {
    SIZE_T,
    DOUBLE_T,
    INT_T
} cm_file_entry_type_e;

typedef union __cm_file_entry_u {
    size_t  s;
    double  d;
    int	    i;
} cm_file_entry_u;

typedef struct __cm_file_entry_t {
    cm_file_entry_type_e type;
    cm_file_entry_u      entry;
} cm_file_entry_t;

/* a collection of entries to be written to the lpc file header */
typedef struct __cm_file_formatter_t {
    size_t	    length;
    cm_file_entry_t entries[];
} cm_file_formatter_t;

/* Make a cm_file_formatter_t that holds the header information. Length is the
 * number of entries in the header. For each entry the arguments <type> and then
 * what the entry is are required. So if you want the header to be 3 entries
 * long the first an int 8, the second a size_t 200 and the third a double 3.4
 * then the arguments are (3, INT_T, 8, SIZE_T, 200, DOUBLE_T, 3.4). Free the
 * file_formatter when you are done with it (using free()). Returns NULL on error. */
cm_file_formatter_t *
cm_make_cm_file_formatter(size_t length);

/* Set the entry of the file formatter at index. Note this does not check if it
 * is a valid type. */
int
cm_set_cm_ff_entry(cm_file_formatter_t *cf, size_t i, cm_file_entry_type_e t,
		    cm_file_entry_u data);
#define cm_set_cm_ffe_(a,b,c,d) cm_set_cm_ff_entry(a,b,c,(cm_file_entry_u)d) 
#define cm_set_cm_ffed_(a,b,c)  cm_set_cm_ff_entry(a,b,c,(cm_file_entry_u)0) 

/* write an entry to an open file */
int
cm_write_cm_file_entry(FILE *f, cm_file_entry_t *e);

/* Write a file formatter to an open file. Returns non-zero on error. */
int
cm_write_cm_file_formatter(FILE *f, cm_file_formatter_t *h);

/* Reads data from the file into the entry according to its format. Returns
 * error on unknown type or if there was a problem reading. Use feof to check if
 * this was the end of the file. */
int
cm_read_cm_file_entry(cm_file_entry_t *e, FILE *f);

/* Fills a preallocated cm_file_formatter_t with entries from a file, using the
 * type information stored within. A way to get one of these is to call
 * cm_make_file_formatter_t with the first arg length and then the next args
 * with the types and values of 0 (or whatever, they will get refilled). The
 * important things are the types so that these can be properly read from the
 * file. */
int
cm_read_cm_file_formatter(cm_file_formatter_t *cf, FILE *f);

/* Returns 0 on unknown type */
size_t
cm_file_entry_size(cm_file_entry_t *e);

/* print a cm file entry using its printf formatting */
int
cm_fprintf_cm_file_entry(FILE *f, cm_file_entry_t *e);

/* print a cm file entry using its sprintf formatting. Returns like sprintf*/
int
cm_sprintf_cm_file_entry(char *c, cm_file_entry_t *e);

/* print a cm file formatter using its printf formatting and the separator*/
int
cm_fprintf_cm_file_formatter(FILE *f, cm_file_formatter_t *cf, char *sep);

#endif /* CMFILER_H */
