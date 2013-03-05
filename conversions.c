/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* functions for converting common units */
#include <cm/utils.h>
#include <cm/defs.h> 
#include <math.h>

/* convert a linear amplitude (0-1] to a db value */
double cm_atodb(double a)
{
    return (log(a)/log(2.0))*6.0;
}

/* convert a db amplitude to a linear amplitude (-inf,0] */
double cm_dbtoa(double db)
{
    return pow(2,db/6.0);
}

/* convert a midi note to a frequency, 69 = 440 Hz */
double cm_mtof(double m)
{
    return 440.0*pow(2.0,(m - 69.0)/12);
}

/* convert a frequency to a wavetable lookup rate given a sampling rate,
 * wavetable size and a frequency */
double cm_ftowtr(double f, double sr, double tablen)
{
    return (f*tablen)/sr;
}

/* Calculate the time in seconds it takes a sound to travel the given distance
 * (m). */
double cm_time_dist(double dist)
{
    return dist/CM_SPEED_OF_SOUND;
}

/* Given the sound pressure of a sound at 1 metre, calculate its sound pressure
 * at a given distance. This is normalized sound pressure (for digital audio)
 * with a ceiling of +-1. */
double cm_dist_press(double dist, double sp_1_m)
{
    return sp_1_m/dist;
}

