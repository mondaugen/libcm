/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef SIGNAL_H
#define SIGNAL_H 
/* An abstract base class for signals */
class Signal {
    private:
	static long int samplerate;
	double k; /* for constants */
    protected:
	double last_tick; /* in order to poll values without doing new
				 calculations */
    public:
	Signal(double);
	Signal();
	virtual double tick();
	virtual void tick(double *output, int numchnls, int buflen);
	virtual void tick(float *output, int numchnls, int buflen);
	virtual double get_last_tick();
	static long int get_samplerate();
	static void set_samplerate(long int);
};
#endif /* SIGNAL_H */
