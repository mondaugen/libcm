#ifndef WAVTAB_H
#define WAVTAB_H 
#include <cm/indexable.h> 
#include <cm/signal.h> 

class WavTab: public Signal {
    private:
	Indexable *tab;
	Signal    *sig;
    public:
	WavTab(Indexable*, Signal*);
	~WavTab ();
	double tick();
	void tick(double *output, int numchnls, int buflen);
};

#endif /* WAVTAB_H */
