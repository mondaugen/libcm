/* Utilities for reading and writing data from/to text files.
 * (c) 2012 Nicholas Esterer */

#include <cm/txtfio.h> 

static size_t
get_len_to_eol(FILE *f)
{
    fpos_t start;
    fgetpos(f,&start);
    size_t len = 0;
    char c;
    do{
	c = fgetc(f);
	len++;
    }while( (c != EOF) && (c != '\n'));
    fsetpos(f,&start);
    return len;
}

static size_t
find_num_words(char *s)
{
    /* there are two states: just read a non-whitespace(1) and just read a
     * whitespace(0)*/
    int state = 0;
    size_t count = 0;
    while(*s != '\0'){
	if(state == 0){
	    if(!isspace(*s)){
		count++;
		state = 1;
	    }
	}else{
	    if(isspace(*s))
		state = 0;
	}
	s++;
    }
    return count;
}

/* parse doubles from a string and put them in a double_ary_t */
int
fill_dbl_ary_from_str(double_ary_t *a, char *str)
{
    int i = 0;
    while((i < a->len) && (*str != '\0'))
	a->data[i++] = strtod(str,&str);
    if(i < (a->len))
	return -1; /* array is too big for all the numbers in the string */
    return 0;
}

/* parse 32-bit integers from a string and put them in an int32_ary_t. If base
 * is 0, base is set to 10. */
int
fill_int32_ary_from_str(int32_ary_t *a, char *str, int base)
{
    if(base == 0)
	base = 10;
    int i = 0;
    while((i < a->len) && (*str != '\0'))
	a->data[i++] = strtol(str, &str, base);
    if(i < (a->len))
	return -1; /* array is too big for all the numbers in the string */
    return 0;
}

/* return an ary_t_list made by reading the rows of a file, the last
 * element will be NULL*/
ary_t_list *
dbl_ary_t_list_from_rows(FILE *f)
{
    size_t len;
    size_t numwrds;
    ary_t_list *result = NULL;
    while((len = get_len_to_eol(f)) > 1){
	char str [len+1];
	if(fgets(str, len+1, f) == NULL)
	    break;
	if((numwrds = find_num_words(str)) == 0)
	    break;
	double_ary_t *da = new_double_ary_t(numwrds);
	int err;
	if(err = fill_dbl_ary_from_str(da, str))
	    ;
	ary_t_list *t = new_ary_t_list((ary_t*)da);
	if(err = append_ary_t_list(&result, t))
	    ;
    }
    return result;
}

ary_t_list *
dbl_ary_t_list_from_rows_path(const char* path)
{
    FILE *f;
    if(!(f=fopen(path, "r"))){
	return NULL;
    }
    return dbl_ary_t_list_from_rows(f);
}

/* return an ary_t_list made by reading the rows of a file, the last
 * element will be NULL*/
ary_t_list *
int32_ary_t_list_from_rows(FILE *f, int base)
{
    size_t len;
    size_t numwrds;
    ary_t_list *result = NULL;
    while((len = get_len_to_eol(f)) > 1){
	char str [len+1];
	if(fgets(str, len+1, f) == NULL)
	    break;
	if((numwrds = find_num_words(str)) == 0)
	    break;
	int32_ary_t *da = new_int32_ary_t(numwrds);
	int err;
	if(err = fill_int32_ary_from_str(da, str, base))
	    ;
	ary_t_list *t = new_ary_t_list((ary_t*)da);
	if(err = append_ary_t_list(&result, t))
	    ;
    }
    return result;
}

ary_t_list *
int32_ary_t_list_from_rows_path(const char* path, int base)
{
    FILE *f;
    if(!(f=fopen(path, "r"))){
	return NULL;
    }
    return int32_ary_t_list_from_rows(f, base);
}


