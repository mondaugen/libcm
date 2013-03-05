#include <cm/wavtabsmplr.h>
#include <cm/utils.h>
#include <iostream> 

WavTabSmplr::WavTabSmplr()
{
}


/* Only initializes the table from where to look-up, does not have a rate
 * initialized yet! Use set_rate() to set it. */
WavTabSmplr::WavTabSmplr(Indexable *i)
{
    rate = NULL;
    tab = i;
    idx = tab->first_x();
}

WavTabSmplr::WavTabSmplr(Indexable *i, Signal *s)
{
    tab = i;
    rate = s;
    idx = tab->first_x();
}

double WavTabSmplr::tick()
{
    try{
	if( rate == NULL )
	    throw -1;
	double tmp = (*tab)[idx];
	last_tick = tmp;
	idx = cm_fwrap(idx + rate->tick(), 0, tab->length());
	return tmp;
    }catch(int e){
	if(e == -1)
	    std::cout << "Rate field is NULL" << std::endl;
	else
	    std::cout << "Unknown exception." << std::endl;
    }
}

void WavTabSmplr::tick(double *output, int numchnls, int buflen)
{
    /* wavtabsmplr is only in mono so it writes a signal to the first channel
     * only */
    int i;
    for(i=0; i<buflen; i++){
	output[i*numchnls /* + 0 */] = tick();
	int j;
	for(j = 1; j < numchnls; j++)
	    output[i*numchnls + j] = 0;
    }
}

void WavTabSmplr::set_rate(Signal *s)
{
    rate = s;
}

void WavTabSmplr::set_rate(double s)
{
    dflt_rate = Signal(s);
    rate = &dflt_rate;
}

Signal *WavTabSmplr::get_rate()
{
    return rate;
}

void WavTabSmplr::set_index(double d)
{
    idx = d;
}

double WavTabSmplr::get_index()
{
    return idx;
}

/* phase is [0-1.0) and wrapped if beyond these bounds */
void WavTabSmplr::set_phase(double ph)
{
    idx = tab->first_x() + ((double)tab->length())*cm_fwrap(ph, 0, 1);
}

double WavTabSmplr::get_phase()
{
    return (idx - tab->first_x())/((double)tab->length());
}

void WavTabSmplr::set_freq(double f)
{
    set_rate(cm_ftowtr(f, Signal::get_samplerate(), tab->length()));
}

void WavTabSmplr::set_pitch(double p)
{
    set_freq(cm_mtof(p));
}

double
WavTabSmplr::get_length()
{
    return tab->length();
}




