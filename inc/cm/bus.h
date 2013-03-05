/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef BUS_H
#define BUS_H 

class Bus: public Signal {
    private:
	Signal	*in;
	double	data;
    public:
	Bus();
	Bus(Signal*);
	void	in_tick();
	double	tick();
};

#endif /* BUS_H */
