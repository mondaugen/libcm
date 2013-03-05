/* An abstract base class for signals */
#include <cm/signal.h> 

#define SIGNAL_DEFAULT_SR	44100

Signal::Signal(double k)
{
    last_tick = 0;
    this->k = k;
}

Signal::Signal() { last_tick = 0; }

double
Signal::tick()
{
    last_tick = k;
    return k;
}

void
Signal::tick(double *output, int numchnls, int buflen)
{
    /* the default is to write to the first channel of the buffer only, this can
     * be overridden in subclasses*/
    for(int i = 0; i < buflen ; i++){
	/* only channel 1 has any signal */
	output[i*numchnls /* + 0 */] = tick();
	for(int j = 1; j < numchnls; i++)
	    output[i*numchnls + j] = 0;
    }
}

void
Signal::tick(float *output, int numchnls, int buflen)
{
    /* the default is to write to the first channel of the buffer only, this can
     * be overridden in subclasses*/
    for(int i = 0; i < buflen ; i++){
	/* only channel 1 has any signal */
	output[i*numchnls /* + 0 */] = (double)tick();
	for(int j = 1; j < numchnls; i++)
	    output[i*numchnls + j] = 0;
    }
}

double
Signal::get_last_tick()
{
    return last_tick;
}

long int
Signal::samplerate = SIGNAL_DEFAULT_SR; /* default */

long int
Signal::get_samplerate()
{
    return Signal::samplerate;
}

void
Signal::set_samplerate(long int sr)
{
    Signal::samplerate = sr;
}
