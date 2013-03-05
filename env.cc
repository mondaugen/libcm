#include <cm/env.h>

static double
identity(double x)
{
    return x;
}

Env::Env() {}

Env::Env(Indexable *tab)
{
    static Signal s = Signal(1.0);
    this->tab = tab;
    this->rate = &s;
    this->ptr = tab->first_x();
    this->tfn = &identity;
    this->state = ENV_FREE;
}

Env::Env(Indexable *tab, Signal *rate)
{
    this->tab = tab;
    this->rate = rate;
    this->ptr = tab->first_x();
    this->tfn = &identity;
    this->state = ENV_FREE;
}

Env::Env(Indexable *tab, Signal *rate, double (*tfn)(double))
{
    this->tab = tab;
    this->rate = rate;
    this->ptr = tab->first_x();
    this->tfn = tfn;
    this->state = ENV_FREE;
}

/* get a value at the specified number of seconds */
double
Env::get_val(double sec)
{
    sec = cm_fclip(sec, tab->first_x(), tab->last_x());
    return (*tfn)((*tab)[sec]);
}

/* if the envelope has been triggered it reads through the table and returns an
 * interpolated value at the index (index is seconds). If the envelope has
 * finished playing, but has not been freed, it continuously returns the last
 * value in the table, if it has been freed, it continuously returns the first
 * value in the table. WARNING: if you do not want to hear a click when
 * resetting the env to FREE, make sure the first and last values in the table
 * are equal. */
double
Env::tick()
{
    if(state == ENV_BUSY){
	if(ptr < tab->length()){
	    double rslt = get_val(ptr);
	    ptr += (1.0/Signal::get_samplerate())*(rate->tick());
	    last_tick = rslt;
	    return rslt;
	}
	last_tick = get_val(tab->last_x());
	return last_tick;
    }
    last_tick = get_val(tab->first_x());
    return last_tick;
}

void 
Env::tick(double *output, int numchnls, int buflen)
{
    Signal::tick(output,numchnls,buflen);
}

void
Env::note_on()
{
    state = ENV_BUSY;
}

void
Env::note_off()
{
    state = ENV_FREE;
    reset();
}

bool
Env::is_busy()
{
    if(state == ENV_BUSY)
	return true;
    else
	return false;
}

void
Env::reset()
{
    ptr = tab->first_x();
}

void
Env::set_tfn(double (*tfn)(double))
{
    this->tfn = tfn;
}

/* NOTE: This action can prematurely push the rate signal if it is meant to be
 * changing */
double
Env::length()
{
    return tab->length()/rate->tick();
}

LinEnv::LinEnv(double *xtab, double *ytab, long int len)
    : Env(new LintrpDblTab(xtab, ytab, len))
{
}

LinEnv::LinEnv(double *xtab, double *ytab, long int len, Signal *rate)
    : Env(new LintrpDblTab(xtab, ytab, len), rate)
{
}

LinEnv::LinEnv(double *xtab, double *ytab, long int len, Signal *rate, 
	double (*tfn)(double) )
    : Env(new LintrpDblTab(xtab, ytab, len), rate, tfn)
{
}

LinEnv::~LinEnv()
{
    delete this->tab;
}
