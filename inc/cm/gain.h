/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef GAIN_H
#define GAIN_H 

#include <cm/signal.h> 

class Gain : public Signal {
    private:
	Signal *in;
	Signal k;
    public:
	Gain();
	Gain(Signal*);
	Gain(Signal*,double);
	void	set_gain(double);
	double	get_gain();
	double	tick();
};

#endif /* GAIN_H */
