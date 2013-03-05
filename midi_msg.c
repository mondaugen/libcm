/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#include <cm/midi/midi_msg.h>

int
cm_midi_msg_sprintf(char *buf, int status, int channel, int data1, int data2)
{
   if(!(MIDI_CHAN_VALID(channel)))
       return MIDI_ERR_BAD_CHAN;
   if(!(MIDI_DATA_VALID(data1) && MIDI_DATA_VALID(data2)))
       return MIDI_ERR_BAD_DATA;
   buf[0] = status + channel;
   buf[1] = data1;
   buf[2] = data2;
   if((status == MIDI_PRGRM_CHNG) || (status == MIDI_CHN_PRS))
       return 2;
   return 3;
}
