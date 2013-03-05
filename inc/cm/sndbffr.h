#ifndef SNDBFFR_H
#define SNDBFFR_H 
/*
 * A class storing an array of samples, the number of samples, the number of
 * channels and the sampling rate. Implements Indexable interface for easy
 * retrieval of values.
 */
#include <cm/indexable.h>
#include <sndfile.h>

class SndBffr: public Indexable {
    private:
	double *tab;
	long int len;
	long int sr;
	int channels;
	int channel_view; /* from what channel a call to get_index(int i) will
			     return a value */
    public:
	SndBffr(double*,long int, long int, int channels);
	SndBffr(const char *); /* from filename */
	~SndBffr();
	void set_channel(int);
	double get_index(double i);
	double length();
	double first_x();
	double last_x();
	double *get_entire_channel(int);
};

#endif /* SNDBFFR_H */
