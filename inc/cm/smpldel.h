/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef SMPLDEL_H
#define SMPLDEL_H 

#include <cm/signal.h> 

class SmplDel: public Signal {
    private:
	Signal	*in;
	double	*tab;
	int	len;
	int	d_time;
	int	ptr;
	void	init_tab(int len);
	void	__init();
    public:
	SmplDel();
	SmplDel(int len, int d_time);
	SmplDel(int len, int d_time, Signal *s);
	~SmplDel();
	double	tick();
	void	set_d_time_ms(double);
	double	get_d_time_ms();
	void	set_d_time_samples(int);
	int	get_d_time_samples();
	void	set_input(Signal*);
};

#endif /* SMPLDEL_H */
