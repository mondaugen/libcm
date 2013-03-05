/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef ANYMIDIEVNT_H
#define ANYMIDIEVNT_H 
/*
 * A class that stores the type of event, when it occurs and data about the
 * event. Avoids having to subclass midievent for every kind of message if you
 * don't want to yet (although that method is much preferred in the long term).
 *
 */
#include <cm/midi/midievnt.h> 

class AnyMidiEvnt: public MidiEvnt {
    protected:
	char type;
	int datalen; /*length of the message in bytes*/
	char data1;
	char data2;
    public:
	AnyMidiEvnt(double time_in_sec, char type, char data1);
	AnyMidiEvnt(double time_in_sec, char type, char data1, char data2);
	AnyMidiEvnt(long int time_in_pulses, char type, char data1);
	AnyMidiEvnt(long int time_in_pulses, char type, char data1, char data2);
	midimsg_err_t add_to_smf_track_sec(smf_track_t *, char);
	midimsg_err_t add_to_smf_track_pulses(smf_track_t *, char);
};

#endif /* ANYMIDIEVNT_H */
