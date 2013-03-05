#ifndef SPP2DARY_H
#define SPP2DARY_H 

#include <cm/signal.h>
#include <cm/sndpntpnd.h>
#include <cm/mxr.h> 
#include <cm/indexable.h> 
#include <cmath>
#include <cm/utils.h> 

class SPP2DAry: public Signal {
    private:
	Signal	    rate; // rate at which clocks play
	Indexable   *buf; // the sound to play when triggered
    public:
	// all distances in metres u.o.s.
	double	wI;	// width of installation
	double	hI;	// height of installation
	int	num_cx; // number of clocks across
	int	num_cy; // number of clocks down
	double	dxO;	// x distance of observer from bottom left corner of
			// installation
	double	dyO;	// y distance of observer from bottom left corner of
			// installation
	double	hO;	// height of observer
	double	wO;	// width of head of observer
	vector<SoundPointPanned> clocks; // vector of clocks indexed [x + y*num_cx]
	Mxr	outL;	// left channel
	Mxr	outR;	// right channel
	void	trigger_clock(int x, int y);
	void	trigger_clock(int i);
	void	trigger_all_clocks();
	void	set_gain(int x, int y, double gain);
	void	set_gain(int i, double gain);
	void	set_gain_db(int x, int y, double gain);
	void	set_gain_db(int i, double gain);
	void	tick(double *output, int numchnls, int buflen);
	void	tick(float *output, int numchnls, int buflen);
	SPP2DAry();
	SPP2DAry(double wI, double hI, int num_cx, int num_cy, double dxO,
		    double dyO, double hO, double wO, Indexable *buf);
	void	init_clocks();
	double	calc_clock_distance(int x, int y);
	double	calc_clock_angle(int x, int y);
};

#endif /* SPP2DARY_H */
