/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#include <cm/cmfiler.h> 

/* Make a cm_file_formatter_t that holds the header information. Length is the
 * number of entries in the header. Returns NULL on error.
 */
cm_file_formatter_t *
cm_make_cm_file_formatter(size_t length)
{
    cm_file_formatter_t *result = (cm_file_formatter_t*)
	malloc(sizeof(cm_file_formatter_t) + sizeof(cm_file_entry_t)*length);
    if(result == NULL)
	return NULL;
    result->length = length;
    return result;
}

/* Set the entry of the file formatter at index. Note this does not check if it
 * is a valid type. */
int
cm_set_cm_ff_entry(cm_file_formatter_t *cf, size_t i, cm_file_entry_type_e t,
		    cm_file_entry_u data)
{
    if((i < 0) || (i >= cf->length))
	return CM_ERR_BOUNDS;
    cf->entries[i].type = t;
    cf->entries[i].entry = data;
    return 0;
}


/* write an entry to an open file */
int
cm_write_cm_file_entry(FILE *f, cm_file_entry_t *e)
{
    size_t size = cm_file_entry_size(e);
    if(size == 0)
	return CM_ERR_UNKNOWN_TYPE;
    if(fwrite(&(e->entry), size, 1, f) < 1)
	return CM_ERR_WRITING_FILE; /* error writing */
    return 0;
}

/* Returns 0 on unknown type */
size_t
cm_file_entry_size(cm_file_entry_t *e)
{
    size_t size;
    switch(e->type){
	case INT_T:
	    size = sizeof(int);
	    break;
	case DOUBLE_T:
	    size = sizeof(double);
	    break;
	case SIZE_T:
	    size = sizeof(size_t);
	    break;
	default:
	    return 0; /* unkown type */
    }
    return size;
}

/* print a cm file entry using its printf formatting */
int
cm_fprintf_cm_file_entry(FILE *f, cm_file_entry_t *e)
{
    switch(e->type){
	case INT_T:
	    fprintf(f, "%d", (e->entry).i);
	    break;
	case DOUBLE_T:
	    fprintf(f, "%g", (e->entry).d);
	    break;
	case SIZE_T:
	    fprintf(f, "%lu", (e->entry).s);
	    break;
	default:
	    return -1; /* unkown type */
    }
    return 0;
}

/* print a cm file entry using its sprintf formatting. Returns like sprintf*/
int
cm_sprintf_cm_file_entry(char *c, cm_file_entry_t *e)
{
    switch(e->type){
	case INT_T:
	    return sprintf(c, "%d", (e->entry).i);
	case DOUBLE_T:
	    return sprintf(c, "%g", (e->entry).d);
	case SIZE_T:
	    return sprintf(c, "%lu", (e->entry).s);
	default:
	    return CM_ERR_UNKNOWN_TYPE; /* unkown type */
    }
    return -2;
}

/* print a cm file formatter using its printf formatting and the separator*/
int
cm_fprintf_cm_file_formatter(FILE *f, cm_file_formatter_t *cf, char *sep)
{
    int i;
    char buf[CM_BUF_SIZE];
    for(i = 0; i < cf->length; i++)
	if(cm_sprintf_cm_file_entry(buf, cf->entries + i) > 0)
	    if(fprintf(f, "%s%s", buf, sep) < 0)
		return CM_ERR_WRITING_FILE;
    return 0;
    
}

/* Write a file formatter to an open file. Returns non-zero on error. */
int
cm_write_cm_file_formatter(FILE *f, cm_file_formatter_t *h)
{
    size_t i;
    for(i = 0; i < h->length; i++)
	if(cm_write_cm_file_entry(f, h->entries + i))
	    return CM_ERR_WRITING_FILE;
    return 0;
}

/* Reads data from the file into the entry according to its format. Returns
 * error on unknown type or if there was a problem reading. Use feof to check if
 * this was the end of the file. */
int
cm_read_cm_file_entry(cm_file_entry_t *e, FILE *f)
{
    size_t size = cm_file_entry_size(e);
    if(size == 0)
	return CM_ERR_UNKNOWN_TYPE;
    if(fread(&(e->entry), size, 1, f) != 1)
	return CM_ERR_READING_FILE;
}

/* Fills a preallocated cm_file_formatter_t with entries from a file, using the
 * type information stored within. A way to get one of these is to call
 * cm_make_file_formatter_t with the first arg length and then the next args
 * with the types and values of 0 (or whatever, they will get refilled). The
 * important things are the types so that these can be properly read from the
 * file. */
int
cm_read_cm_file_formatter(cm_file_formatter_t *cf, FILE *f)
{
    int err, i;
    for(i = 0; i < cf->length; i++){
	err = cm_read_cm_file_entry(cf->entries + i, f);
	if((err == CM_ERR_READING_FILE) || (err == CM_ERR_UNKNOWN_TYPE))
	    return err;
    }
    return 0;
}

