#include <cm/mxr.h>

Mxr::Mxr(vector<Signal*> sigs)
{
    this->sigs = sigs;
}

Mxr::Mxr()
{
    ;
}

double Mxr::tick()
{
    double rslt = 0;
    vector<Signal*>::iterator it;
    for(it=sigs.begin(); it < sigs.end(); it++)
	rslt += (*it)->tick();
    last_tick = rslt;
    return rslt;
}
	
void 
Mxr::tick(double *output, int numchnls, int buflen)
{
    Signal::tick(output,numchnls,buflen);
}
