/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef WAVTABSMPLR_H
#define WAVTABSMPLR_H 
/* Reads through an Indexable at a set rate. 
 * The rate can be set various ways.
 */
#include <cm/signal.h> 
#include <cm/indexable.h>

class WavTabSmplr: public Signal {
    private:
	Indexable   *tab;
	Signal	    *rate;
	double	    idx;
	Signal	    dflt_rate; /*default rate*/
    public:
	WavTabSmplr(Indexable*, Signal*);
	WavTabSmplr(Indexable*);
	WavTabSmplr();
//	~WavTabSmplr();
	double tick();
	void tick(double *output, int numchnls, int buflen);
	void set_rate(Signal*);
	void set_rate(double);
	Signal *get_rate();
	void set_index(double);
	double get_index();
	void set_phase(double);
	double get_phase();
	void set_freq(double);
	void set_pitch(double);
	double get_length();
};


#endif /* WAVTABSMPLR_H */
