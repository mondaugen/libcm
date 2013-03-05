/* An audio bus. Use to avoid stack-overflows on fed-back signals. */
#include <cm/signal.h> 
#include <cm/bus.h> 

Bus::Bus()
{
    data = 0;
}

Bus::Bus(Signal *s)
{
    in = s;
}

void
Bus::in_tick()
{
    data = in->tick();
}

double
Bus::tick()
{
    last_tick = data;
    return data;
}
