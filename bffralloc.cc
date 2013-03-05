#include <cm/bffralloc.h>

double *
BffrAlloc::make_dbl_buf(size_t size)
{
    double *ptr = new (nothrow) double[size];
    if(ptr == NULL)
	return ptr;
    cur_dbl_bufs.push_back(ptr);
    return ptr;
}

bool
BffrAlloc::free_buf(double *ptr)
{
    list<double*>::iterator it;
    bool found = false;
    for(it = cur_dbl_bufs.begin(); it != cur_dbl_bufs.end(); it++){
	if(*it == ptr){
	    delete[] *it;
	    cur_dbl_bufs.erase(it);
	    found = true;
	}
    }
    return found;
}

