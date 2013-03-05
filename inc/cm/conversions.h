/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define  __BEGIN_DECLS extern "C" {
# define  __END_DECLS }
#else  
# define  __BEGIN_DECLS /* empty */
# define  __END_DECLS /* empty */
#endif
__BEGIN_DECLS

/* convert a linear amplitude (0-1] to a db value */
double cm_atodb(double a);
/* convert a db amplitude to a linear amplitude (-inf,0] */
double cm_dbtoa(double db);
/* convert a midi note to a frequency, 69 = 440 Hz */
double cm_mtof(double m);
/* convert a frequency to a wavetable lookup rate given a sampling rate,
 * wavetable size and a frequency */
double cm_ftowtr(double f, double sr, double tablen);
/* Calculate the time in seconds it takes a sound to travel the given distance
 * (m). */
double cm_time_dist(double dist);
/* Given the sound pressure of a sound at 1 metre, calculate its sound pressure
 * at a given distance. This is normalized sound pressure (for digital audio)
 * with a ceiling of +-1. */
double cm_dist_press(double dist, double sp_1_m);

__END_DECLS
#endif /* CONVERSIONS_H */
