/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef SNDPNTPND_H
#define SNDPNTPND_H 

#include <cm/signal.h>
#include <cm/smplrinst.h>
#include <cm/bus.h>
#include <cm/smpldel.h>
#include <cm/gain.h>
#include <cm/indexable.h>
#include <cm/conversions.h>

class SoundPointPanned: public Signal {
    public:
	SmplrInst   smplr;
	Bus	    bus;
	SmplDel	    dell;
	SmplDel	    delr;
	Gain	    gl;
	Gain	    gr;
	Gain	    gain;
	void	    tick(double *output, int numchnls, int buflen);
	void	    set_gain(double);
	void	    set_gain_db(double);
};

void
sndpntpnd_init(SoundPointPanned *spp, Indexable *buf, Signal *rate, int dell, 
	int delr, double gl, double gr);

#endif /* SNDPNTPND_H */
