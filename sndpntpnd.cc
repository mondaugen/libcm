/* A panned sound source */

#include <cm/sndpntpnd.h> 

void
sndpntpnd_init(SoundPointPanned *spp, Indexable *buf, Signal *rate, int dell, int delr,
	double gl, double gr)
{
    spp->smplr	= SmplrInst(buf,rate);
    spp->bus	= Bus(&(spp->smplr));
    spp->gain	= Gain(&(spp->bus),1.0); /* initial gain of 1.0 */
    spp->dell	= SmplDel(dell,dell);
    (spp->dell).set_input(&(spp->gain));
    spp->delr	= SmplDel(delr,delr);
    (spp->delr).set_input(&(spp->gain));
    spp->gl	= Gain(&(spp->dell),gl);
    spp->gr	= Gain(&(spp->delr),gr);
}

void 
SoundPointPanned::tick(double *output, int numchnls, int buflen)
{
    // if other than 2 channels, zero all output
    if(numchnls != 2){
	for(int i = 0; i < buflen*numchnls; i++)
	    output[i] = 0;
	return;
    }
    for(int i = 0 ; i < buflen; i++){
	bus.in_tick(); //update bus
	output[i*numchnls]   = gl.tick();
	output[i*numchnls+1] = gr.tick();
    }
}

/* set a linear overall gain so 0.0 is quietest and 1.0 is loudest although no
 * clipping occurs */
void
SoundPointPanned::set_gain(double g)
{
    gain.set_gain(g);
}

/* set the same gain in dB */
void
SoundPointPanned::set_gain_db(double g)
{
    gain.set_gain(cm_dbtoa(g));
}


