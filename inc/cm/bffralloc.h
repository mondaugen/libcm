/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef BFFRALLOC_H
#define BFFRALLOC_H 
/*
 * Handles the allocation and deallocation of arrays for storing data.
 */
#include <list> 
#include <new> 
using namespace std;

class BffrAlloc {
    private:
	static list<double*> cur_dbl_bufs;
    public:
        static double * make_dbl_buf(size_t size);
	static bool free_buf(double *ptr);
};

#endif /* BFFRALLOC_H */
