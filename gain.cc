/* Multiply a signal by a constant */

#include <cstdlib> 
#include <cm/gain.h>

Gain::Gain()
{
    in = NULL;
    k = Signal(1.0);
}

Gain::Gain(Signal *s)
{
    in = s;
}

Gain::Gain(Signal *s, double k)
{
    in = s;
    set_gain(k);
}

void
Gain::set_gain(double k)
{
    this->k = Signal(k);
}

double
Gain::tick()
{
    last_tick = in->tick() * k.tick();
    return last_tick;
}




