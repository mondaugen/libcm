/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/*
 * Midi event interface.
 */

#include <cm/midi/midievnt.h> 

/* calculate this structs time_in_sec from frames per second and tempo
 * in bpm */
midimsg_err_t
MidiEvnt::calc_time_sec(int fps, double bpm)
{
    return MIDI_ERR_NOT_IMP;
}

/* calculate this structs time_in_pulses (pulses per quarter) from frames
 * per second and tempo in bpm */
midimsg_err_t
MidiEvnt::calc_time_pulses(int fps, double bpm)
{
    return MIDI_ERR_NOT_IMP;
}


