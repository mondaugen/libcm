/* a wave table which allows the reading of values from some indexable class
 * with an arbitrary signal class */
#include <cm/wavtab.h>

WavTab::WavTab(Indexable *i, Signal *s)
{
    this->tab = i;
    this->sig = s;
}

double WavTab::tick()
{
    last_tick = (*tab)[sig->tick()];
    return last_tick;
}

void WavTab::tick(double *output, int numchnls, int buflen)
{
    /* wavtab is mono only so it only writes to first channel */
    int i;
    for(i=0; i<buflen; i++){
	output[i*numchnls /* + 0 */] = tick();
	int j;
	for(j = 1; j < numchnls; j++)
	    output[i*numchnls + j] = 0;
    }
}
