/* An abstract base class for indexable objects */

#include <cm/indexable.h> 

double Indexable::operator[] (double i)
{
    return this->get_index(i);
}

double Indexable::operator[] (int i)
{
    return this->get_index(i);
}
