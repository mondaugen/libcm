/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef SMPLRINST_H
#define SMPLRINST_H 
/*
 * A sampler instrument class.
 */
#include <cm/instrument.h>
#include <cm/signal.h> 
#include <cm/wavtabsmplr.h>

enum smplr_states {
    SMPLR_FREE,
    SMPLR_BUSY
};

class SmplrInst: public Instrument, public Signal {
    private:
	WavTabSmplr smplr;
	enum smplr_states state;
	int tick_counter;
    public:
	SmplrInst();
	SmplrInst(Indexable *);
	SmplrInst(Indexable *, Signal *);
	SmplrInst(Indexable *, double);
	void note_on();
	void note_on(int num_ticks);
	void note_off();
	bool is_busy();
	double tick();
	void tick(double *output, int numchnls, int buflen);
	double get_length();
};

#endif /* SMPLRINST_H */
