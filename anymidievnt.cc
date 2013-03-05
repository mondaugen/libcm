/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#include <cm/midi/anymidievnt.h> 

AnyMidiEvnt::AnyMidiEvnt(double time_in_sec, char type, char data1)
{
    this->time_in_sec = time_in_sec;
    this->time_in_pulses = -1;
    this->type = type;
    this->data1 = data1;
    this->data2 = 0;
    this->datalen = 2;
}

AnyMidiEvnt::AnyMidiEvnt(double time_in_sec, char type, char data1, char data2)
{
    this->time_in_sec = time_in_sec;
    this->time_in_pulses = -1;
    this->type = type;
    this->data1 = data1;
    this->data2 = data2;
    this->datalen = 3;
}

AnyMidiEvnt::AnyMidiEvnt(long int time_in_pulses, char type, char data1)
{
    this->time_in_pulses = time_in_pulses;
    this->time_in_sec = -1;
    this->type = type;
    this->data1 = data1;
    this->data2 = 0;
    this->datalen = 2;
}

AnyMidiEvnt::AnyMidiEvnt(long int time_in_pulses, char type, char data1, char data2)
{
    this->time_in_pulses = time_in_pulses;
    this->time_in_sec = -1;
    this->type = type;
    this->data1 = data1;
    this->data2 = data2;
    this->datalen = 3;
}

midimsg_err_t 
AnyMidiEvnt::add_to_smf_track_sec(smf_track_t *track, char channel)
{
    if((time_in_sec < 0))
	return MIDI_ERR_NOT_INIT;
    char onmess[3];
    cm_midi_msg_sprintf(onmess, type, channel, data1, data2);
    smf_event_t *onevent = smf_event_new_from_pointer(onmess,datalen);
    if(onevent == NULL)
	return MIDI_ERR_ALLOC;
    smf_track_add_event_seconds(track, onevent, time_in_sec) ;
    return 0;
}

midimsg_err_t 
AnyMidiEvnt::add_to_smf_track_pulses(smf_track_t *track , char channel)
{
    if((time_in_pulses < 0))
	return MIDI_ERR_NOT_INIT;
    char onmess[3];
    cm_midi_msg_sprintf(onmess, type, channel, data1, data2);
    smf_event_t *onevent = smf_event_new_from_pointer(onmess,datalen);
    if(onevent == NULL)
	return MIDI_ERR_ALLOC;
    smf_track_add_event_pulses(track, onevent, time_in_pulses) ;
    return 0;
}
