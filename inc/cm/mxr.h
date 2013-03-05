#ifndef MXR_H
#define MXR_H 

/* Takes an array of signals and makes their sum available for output */

#include <cm/signal.h> 
#include <vector> 
using namespace std;

class Mxr: public Signal {
    public:
	vector<Signal*> sigs;
	Mxr(vector<Signal*>);
	Mxr();
	double tick();
	void tick(double *output, int numchnls, int buflen);
};

#endif /* MXR_H */
