/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/*
 * Midi Note event class.
 */
#include <cm/midi/mnevnt.h>

MNEvnt::MNEvnt() { }

MNEvnt::MNEvnt( double	time_in_sec,
		double	len_in_sec,
		char	pitch,
		char	onset_vel,
		char	rel_vel )
{
    this->time_in_sec = time_in_sec;
    this->time_in_pulses = -1;
    this->len_in_sec = len_in_sec;
    this->len_in_pulses = -1;
    this->pitch = pitch;
    this->onset_vel = onset_vel;
    this->rel_vel   = rel_vel;
}

MNEvnt::MNEvnt( long int    time_in_pulses,
		long int    len_in_pulses,
		char	    pitch,
		char	    onset_vel,
		char	    rel_vel )
{
    this->time_in_pulses = time_in_pulses;
    this->time_in_sec = -1;
    this->len_in_pulses = len_in_pulses;
    this->len_in_sec = -1;
    this->pitch = pitch;
    this->onset_vel = onset_vel;
    this->rel_vel   = rel_vel;
}

midimsg_err_t
MNEvnt::add_to_smf_track_sec(smf_track_t *track, char channel)
{
    if((len_in_sec < 0) || (time_in_sec < 0))
	return MIDI_ERR_NOT_INIT;
    char onmess[3], offmess[3];
    smf_event_t *onevent = smf_event_new_from_pointer(onmess,
	    cm_midi_msg_sprintf(onmess, MIDI_NOTE_ON, channel, pitch,
		onset_vel));
    smf_event_t *offevent = smf_event_new_from_pointer(offmess,
	    cm_midi_msg_sprintf(offmess, MIDI_NOTE_OFF, channel, pitch,
		rel_vel));
    if(onevent == NULL || offevent == NULL)
	return MIDI_ERR_ALLOC;
    smf_track_add_event_seconds(track, onevent, time_in_sec) ;
    smf_track_add_event_seconds(track, offevent, time_in_sec + len_in_sec) ;
    return 0;
}

midimsg_err_t
MNEvnt::add_to_smf_track_pulses(smf_track_t *track, char channel)
{
    if((len_in_pulses < 0) || (time_in_pulses < 0))
	return MIDI_ERR_NOT_INIT;
    char onmess[3], offmess[3];
    smf_event_t *onevent = smf_event_new_from_pointer(onmess,
	    cm_midi_msg_sprintf(onmess, MIDI_NOTE_ON, channel, pitch,
		onset_vel));
    smf_event_t *offevent = smf_event_new_from_pointer(offmess,
	    cm_midi_msg_sprintf(offmess, MIDI_NOTE_OFF, channel, pitch,
		rel_vel));
    if(onevent == NULL || offevent == NULL)
	return MIDI_ERR_ALLOC;
    smf_track_add_event_pulses(track, onevent, time_in_pulses) ;
    smf_track_add_event_pulses(track, offevent, time_in_pulses + len_in_pulses) ;
    return 0;
}
