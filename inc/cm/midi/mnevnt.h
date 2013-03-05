/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef MNEVNT_H
#define MNEVNT_H 
/*
 * Midi note event class.
 *
 * A class that stores an onset time in seconds or pulses, the note-on
 * pitch/velocity, the note-off velocity and the duration. Inherits methods from
 * the MidiEvnt class to add its data to a smf_track.
 */

#include <cm/midi/midievnt.h> 

class MNEvnt: public MidiEvnt {
    protected:
	double len_in_sec;
	long int len_in_pulses;
	char pitch;
	char onset_vel;
	char rel_vel;
    public:
	MNEvnt();
	MNEvnt(double time_in_sec, double len_in_sec, char pitch, char onset_vel,
		char rel_vel);
	MNEvnt(long int time_in_pulses, long int len_in_pulses, char pitch,
		char onset_vel, char rel_vel);
	midimsg_err_t add_to_smf_track_sec(smf_track_t *, char);
	midimsg_err_t add_to_smf_track_pulses(smf_track_t *, char);
};

#endif /* MNEVNT_H */
