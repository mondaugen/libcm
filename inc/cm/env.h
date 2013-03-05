/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef ENV_H
#define ENV_H 
/*
 * A signal usually used as an envelope for modulating another signal. A table
 * of (sec,val) pairs, a rate and an optional transfer function are specified.
 * The table is read through at a rate such that, if rate is a constant of 1,
 * the envelope will have the length of the last x-coordinate in the table. The
 * transfer function can be used to transform the signal read from the table,
 * for example, if you would like to specify (time,dB) pairs and have the result
 * come out in (time,linear amplitude) pairs, you can specify a pointer to
 * double cm_dbtoa(double) to do the conversion. If no conversion is specified, the
 * default is the indentity.  */
#include <cm/signal.h>
#include <cm/indexable.h>
#include <cm/instrument.h> 
#include <cm/utils.h> 
#include <cm/lintrp_dbl_tab.h> 

enum env_states {
    ENV_BUSY,
    ENV_FREE
};

class Env: public Signal, public Instrument {
    protected:
	Indexable *tab;
	Signal *rate;
	double ptr;
	double (*tfn)(double);
	enum env_states state;
    public:
	Env();
	Env(Indexable *);
	Env(Indexable *, Signal *);
	Env(Indexable *, Signal *, double (*tfn)(double));
	double tick();
	double get_val(double sec);
	void tick(double *output, int numchnls, int buflen);
	void note_on();
	void note_off();
	bool is_busy();
	void reset();
	void set_tfn(double (*tfn)(double));
	double length();
};

class LinEnv: public Env {
    public:
	LinEnv(double *xtab, double *ytab, long int len);
	LinEnv(double *xtab, double *ytab, long int len, Signal *);
	LinEnv(double *xtab, double *ytab, long int len, Signal *, 
		double (*tfn)(double));
	~LinEnv();
};


#endif /* ENV_H */
