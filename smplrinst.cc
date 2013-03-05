/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#include <cm/smplrinst.h>

SmplrInst::SmplrInst()
{
}

SmplrInst::SmplrInst(Indexable *i)
{
    this->smplr = WavTabSmplr(i);
    this->state = SMPLR_FREE;
}

/* DON'T USE THIS, it needs to be fixed */
SmplrInst::SmplrInst(Indexable *i, double rate)
{
    this->smplr = WavTabSmplr(i);
    this->state = SMPLR_FREE;
    (this->smplr).set_rate(rate);
}

SmplrInst::SmplrInst(Indexable *i, Signal *s)
{
    this->smplr = WavTabSmplr(i,s);
    this->state = SMPLR_FREE;
}

void
SmplrInst::note_on()
{
    this->state = SMPLR_BUSY;
    smplr.set_phase(0.0f);
}

/* The note will be on for num_ticks. Will interrupt previous notes. */
void
SmplrInst::note_on(int num_ticks)
{
    smplr.set_phase(0.0f);
    this->tick_counter = num_ticks;
    this->state = SMPLR_BUSY;
}

void
SmplrInst::note_off()
{
    tick_counter = -1;
    this->state = SMPLR_FREE;
}

bool
SmplrInst::is_busy()
{
    if(state == SMPLR_BUSY)
	return true;
    return false;
}

double
SmplrInst::tick()
{
    if(state == SMPLR_BUSY){
	if(tick_counter > 0){
	    tick_counter--;
	    last_tick = smplr.tick(); 
	    return last_tick;
	}else if(tick_counter == 0){
	    note_off();
	    last_tick = 0.0f;
	    return last_tick;
	}else{ /* tick counter < 0, but note triggered by note_on() */
	    last_tick = smplr.tick(); 
	    return last_tick;
	}
    }else{
	last_tick = 0.0f;
	return last_tick;
    }
}

void 
SmplrInst::tick(double *output, int numchnls, int buflen)
{
    Signal::tick(output, numchnls, buflen);
}

double
SmplrInst::get_length()
{
    return smplr.get_length();
}
