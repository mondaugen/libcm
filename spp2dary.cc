/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
// A 2D array of sound-points

#include <cm/spp2dary.h>
#include <cstdio> 

/* calculate the distance from a sound source to the left ear
 * given a distance from the sound, the angle of the sound and the width of the
 * observer's head. r and hw in metres, th in degrees s.t. 0 is straight ahead,
 * -90 all the way left and 90 all the way right. */
static double
calc_left_ear_dist(double r, double th, double hw)
{
    /* convert theta to radians */
    th = (90.0 - th)*M_PI/180.0;
    /* calculate distance */
    return sqrt((r*r) + (hw*r*cos(th)) + (0.25*hw*hw));
}

/* calculate the delay in samples it takes to travel a given distance in metres
 * */
static int
calc_delay(double dist)
{
    return (int)(cm_time_dist(dist)*((double)Signal::get_samplerate()));
}


/* calculate the distance from a sound source to the right ear
 * given a distance from the sound, the angle of the sound and the width of the
 * observer's head. r and hw in metres, th in degrees s.t. 0 is straight ahead,
 * -90 all the way left and 90 all the way right. */
static double
calc_right_ear_dist(double r, double th, double hw)
{
    /* convert theta to radians */
    th = (90.0 - th)*M_PI/180.0;
    /* calculate distance */
    return sqrt((r*r) - (hw*r*cos(th)) + (0.25*hw*hw));
}

// given the x and y position of a clock, calculate its distance from the
// observer's head. x and y vary from 0 to 1 less than the width or height of
// the installation respectively
double
SPP2DAry::calc_clock_distance(int x, int y)
{
    // all distances in metres

    // width of clock
    double wci = wI/(double)num_cx;
    // height of clock
    double hci = hI/(double)num_cy;

    // x coordinate of clock's centre point
    double ccix = wci*x + 0.5*wci;
    // y coordinate of clock's centre point
    double cciy = hci*y + 0.5*hci;

    // calculate the distance from the bottom of the observer to the point on
    // the floor below the clock
    double dyoc = sqrt((ccix-dxO)*(ccix-dxO) + dyO*dyO);

    // then calculate the distance from the top of the observers head to the
    // centre of the clock
    double doc = sqrt(dyoc*dyoc + (cciy-hO)*(cciy-hO));

    return doc;
}

// gives the angle of the clock from the observer looking straight ahead in
// degrees. 0 means straight ahead, 90 means all the way right and -90 means all
// the way left.
double
SPP2DAry::calc_clock_angle(int x, int y)
{

    // width of clock
    double wci = wI/(double)num_cx;
    // height of clock
    double hci = hI/(double)num_cy;

    // x coordinate of clock's centre point
    double ccix = wci*x + 0.5*wci;
    // y coordinate of clock's centre point
    double cciy = hci*y + 0.5*hci;

    // calculate x distance of clock centre to bottom of observer
    double dxoc = ccix - dxO;

    // y distance of clock centre to bottom of observer is just the distance
    // from wall to observer

    // calculate the angle and convert to degrees
    return (atan2(dxoc, dyO)/M_PI)*180.0;
}

SPP2DAry::SPP2DAry(){ }

SPP2DAry::SPP2DAry(double wI, double hI, int num_cx, int num_cy, double dxO,
	double dyO, double hO, double wO, Indexable *buf)
{
    this->wI = wI;
    this->hI = hI;
    this->num_cx = num_cx;
    this->num_cy = num_cy;
    this->dxO = dxO;
    this->dyO = dyO;
    this->hO = hO;
    this->wO = wO;
    this->buf = buf;
    for(int i =0; i < num_cy; i++){
	for(int j =0; j < num_cx; j++)
	    clocks.push_back(SoundPointPanned());
    }
    this->rate = Signal(1.0);
}

void
SPP2DAry::init_clocks()
{
    for(int i =0; i < num_cx; i++){
	for(int j =0; j < num_cy; j++){
	    double cdist = calc_clock_distance(i,j);
	    double ldist = calc_left_ear_dist(calc_clock_distance(i,j),
		    calc_clock_angle(i,j), wO);
	    double rdist = calc_right_ear_dist(calc_clock_distance(i,j),
		    calc_clock_angle(i,j), wO);
	    printf("Clock (%d,%d):\n", i, j);
	    printf("\tDistance to clock: %f\n", cdist);
	    printf("\tDistance to left ear: %f\n", ldist);
	    printf("\tDistance to right ear: %f\n", rdist);
	    int ldel = calc_delay(ldist);
	    int rdel = calc_delay(rdist);
	    double lpress = cm_dist_press(ldist, 1.0);
	    double rpress = cm_dist_press(rdist, 1.0);
	    printf("\tLeft ear delay (samples): %d\n", ldel);
	    printf("\tRight ear delay (samples): %d\n", rdel);
	    printf("\tLeft ear pressure: %f\n", lpress);
	    printf("\tRight ear pressure: %f\n", rpress);
	    sndpntpnd_init(&(clocks[j*num_cx + i]), buf, &rate,
		    ldel, rdel, lpress, rpress);
	    outL.sigs.push_back(&(clocks[j*num_cx + i].gl));
	    outR.sigs.push_back(&(clocks[j*num_cx + i].gr));
	}
    }
}

void
SPP2DAry::trigger_clock(int x, int y)
{
    clocks[x + y*num_cx].smplr.note_on(
	    clocks[x + y*num_cx].smplr.get_length());
}

void
SPP2DAry::trigger_clock(int i)
{
    clocks[i].smplr.note_on(
	    clocks[i].smplr.get_length());
}

void
SPP2DAry::trigger_all_clocks()
{
    for(int i = 0; i < num_cx; i++){
	for(int j = 0; j < num_cy; j++){
	    trigger_clock(i,j);
	}
    }
}

/* set linear gain of clock */
void
SPP2DAry::set_gain(int x, int y, double gain)
{
        clocks[x + y*num_cx].set_gain(gain);
}

/* set linear gain of clock */
void
SPP2DAry::set_gain(int i, double gain)
{
        clocks[i].set_gain(gain);
}

/* set dB gain of clock */
void
SPP2DAry::set_gain_db(int x, int y, double gain)
{
        clocks[x + y*num_cx].set_gain_db(gain);
}

/* set dB gain of clock */
void
SPP2DAry::set_gain_db(int i, double gain)
{
        clocks[i].set_gain_db(gain);
}

void
SPP2DAry::tick(double *output, int numchnls, int buflen)
{
    // if other than 2 channels, zero all output
    if(numchnls != 2){
	for(int i = 0; i < buflen*numchnls; i++)
	    output[i] = 0;
	return;
    }
    for(int i = 0; i < buflen; i++){
	for(int j = 0; j < clocks.size(); j++){
	    /* update samplers */
	    clocks[j].bus.in_tick();
	}
	/* write mixed signals to left and right outs */
	output[i*numchnls] = outL.tick();
	output[i*numchnls + 1] = outR.tick();
    }
}

void
SPP2DAry::tick(float *output, int numchnls, int buflen)
{
    // if other than 2 channels, zero all output
    if(numchnls != 2){
	for(int i = 0; i < buflen*numchnls; i++)
	    output[i] = 0;
	return;
    }
    for(int i = 0; i < buflen; i++){
	for(int j = 0; j < clocks.size(); j++){
	    /* update samplers */
	    clocks[j].bus.in_tick();
	}
	/* write mixed signals to left and right outs */
	output[i*numchnls] = (float)outL.tick();
	output[i*numchnls + 1] = (float)outR.tick();
    }
}
