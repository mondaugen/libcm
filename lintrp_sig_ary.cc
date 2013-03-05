/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* A class that allows the linear inpolatation of a vector of signals. The index
 * is between which signal indices to interpolate, so 2.3 will interpolate
 * between 2 and 3 */
#include <cm/lintrp_sig_ary.h> 

LintrpSigAry::LintrpSigAry(Signal **tab, long int len)
{
    this->tab = tab;
    this->len = len;
    this->idx = 0;
}

void LintrpSigAry::set_index(double i)
{
    idx = i;
}

double LintrpSigAry::tick()
{
    double ary [len];
    for(int i=0; i<len; i++)
	ary[i] = tab[i]->tick();
    LintrpDblTab ldt = LintrpDblTab(ary, len);
    last_tick = ldt[idx];
    return last_tick;
}

