/*
 * A class storing an array of samples, the number of samples, the number of
 * channels and the sampling rate. Implements Indexable interface for easy
 * retrieval of values.
 */

#include <cm/sndbffr.h>
#include <cm/sndfio.h>
#include <cm/utils.h> 
#include <math.h> 


SndBffr::SndBffr(double *tab, long int len, long int samplingrate, int channels)
{
    this->tab = tab;
    this->len = len;
    this->sr = samplingrate;
    this->channels = channels;
    this->channel_view = 0;
}

SndBffr::SndBffr(const char *filepath)
{
    SF_INFO sfinfo;
    this->tab = cm_load_sf_to_dbl_ary(filepath, &sfinfo);
    this->len = sfinfo.frames;
    this->sr = sfinfo.samplerate;
    this->channels = sfinfo.channels;
    this->channel_view = 0;
}

SndBffr::~SndBffr()
{
    free(this->tab);
}

double
SndBffr::length()
{
    return (double)len;
}

double
SndBffr::first_x()
{
    return 0;
}

double
SndBffr::last_x()
{
    return (double)(len-1);
}

/* get the value of the sample on the current channel */
double
SndBffr::get_index(double i)
{
    return tab[cm_clip((long int)floor(i), (long int)first_x(), 
	    (long int)last_x())*channels + channel_view];
}

/* set the channel from which to receive values */
void
SndBffr::set_channel(int c)
{
    this->channel_view = c;
}

/* copy the contents of an entire channel to a single channel array and return a
 * pointer to it. Doesn't affect current channel_view. */
double *
SndBffr::get_entire_channel(int channel)
{
    double * rslt = (double*)malloc(sizeof(double)*len);
    /* copy all values into rslt */
    for(int i = 0; i < len; i++)
	rslt[i] = tab[i*channels + channel];
    return rslt;
}
