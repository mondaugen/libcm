/* A simple delay line. Does not support multi-taps. */

#include <cm/smpldel.h>
#include <cstdlib> 
#include <cstring>

#define max(a,b) a>b ? a : b
#define min(a,b) a<b ? a : b

void
SmplDel::init_tab(int len)
{
    this->tab = new double[len];
    memset(this->tab, 0, len*sizeof(double));
    this->len = len;
}

void
SmplDel::__init()
{
    in	    = NULL;
    tab	    = NULL;
    len	    = 0;
    d_time  = 0;
    ptr	    = 0;
}

SmplDel::SmplDel()
{
    __init();
}

SmplDel::SmplDel(int len, int d_time)
{
    __init();

    init_tab(len);
    this->d_time = d_time;
}

SmplDel::SmplDel(int len, int d_time, Signal *s)
{
    __init();
    init_tab(len);
    this->d_time = d_time;
    this->in = s;
}

SmplDel::~SmplDel()
{
//    delete[] this->tab;
}

double
SmplDel::tick()
{
    last_tick = tab[ptr];
    tab[ptr]	= in->tick();
    ptr		= (ptr+1)%d_time;
    return last_tick;
}

void
SmplDel::set_d_time_ms(double ms)
{
    if(ms < 0)
	return;
    /* if ms requested is greater than delay line length, make d_time the line
     * length */
    d_time = min((int)(ms/1000.0*Signal::get_samplerate()),len);
}

double
SmplDel::get_d_time_ms()
{
    return ((double)d_time)/((double)Signal::get_samplerate())*1000.0;
}

void
SmplDel::set_d_time_samples(int samps)
{
    d_time = min(samps,len);
}

int
SmplDel::get_d_time_samples()
{
    return d_time;
}

void
SmplDel::set_input(Signal*s)
{
    in = s;
}
