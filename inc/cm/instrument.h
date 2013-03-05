#ifndef INTSRUMENT_H
#define INTSRUMENT_H 
/*
 * Abstract base class for instruments
 */

class Instrument {
    public:
	virtual void note_on() =0;
	virtual void note_off() =0;
	virtual bool is_busy() =0;
};

#endif /* INTSRUMENT_H */
