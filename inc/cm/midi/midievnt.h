/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef MIDIEVNT_H
#define MIDIEVNT_H 
#include <smf.h>
#include <cm/screvnt.h> 
#include <cm/midi/midi_msg.h> 

class MidiEvnt: public ScrEvnt {
    protected:
	double time_in_sec;
	long int time_in_pulses;
    public:
	virtual  midimsg_err_t add_to_smf_track_sec(smf_track_t *, char) =0;
	virtual  midimsg_err_t add_to_smf_track_pulses(smf_track_t *, char) =0;
	/* calculate this structs time_in_sec from frames per second and tempo
	 * in bpm */
	midimsg_err_t calc_time_sec(int fps, double bpm);
	/* calculate this structs time_in_pulses (pulses per quarter) from frames
	 * per second and tempo in bpm */
	midimsg_err_t calc_time_pulses(int fps, double bpm);
};

#endif /* MIDIEVNT_H */
