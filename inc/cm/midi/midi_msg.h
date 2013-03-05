/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef MIDI_MSG_H
#define MIDI_MSG_H 

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

/* midi error type */
typedef int midimsg_err_t;

/* Defines for midi messages. */

#define MIDI_NOTE_OFF	0x80
#define MIDI_NOTE_ON	0x90
#define MIDI_POLY_PRS	0xA0
#define MIDI_CNTRL_CHNG 0xB0
#define MIDI_PRGRM_CHNG 0xC0
#define MIDI_CHN_PRS	0xD0
#define MIDI_PCH_BND	0xE0

/* Check if data within bounds */

#define MIDI_CHAN_VALID(x)	((x>=0)&&(x<16))
#define MIDI_DATA_VALID(x)	((x>=0)&&(x<128)) 
#define MIDI_CTL_VALID(v)	(((int)v>=0)&&((int)v<120))

#define MIDI_ERR_BAD_CHAN	-1
#define MIDI_ERR_BAD_DATA	-2 
#define MIDI_ERR_NOT_INIT	-3
#define MIDI_ERR_ALLOC		-4
#define MIDI_ERR_NOT_IMP	-5 /* when something is not yet implemented */ 

/* fills buf with bytes of a midi message and returns len */
midimsg_err_t
cm_midi_msg_sprintf(char *buf, int status, int channel, int data1, int data2);

__END_DECLS

#endif /* MIDI_MSG_H */
