/* A table for holding doubles. Performs no interpolation. */

#include <cm/dbltab.h>
#include <cmath>
#include <cm/utils.h> 

DblTab::DblTab() { }

DblTab::DblTab(double *tab, long int len)
{
    this->tab = tab;
    this->len = len;
}

DblTab::~DblTab(void)
{
}

double DblTab::get_index(double i)
{
    i = cm_fclip(i, 0, len - 1);
    return tab[(long int)floor(i)];
}

double DblTab::get_index(int i)
{
    i = cm_clip(i, 0, len - 1);
    return tab[i];
}

double DblTab::length()
{
    return (double)len;
}

double DblTab::first_x()
{
    return 0;
}

double DblTab::last_x()
{
    return len-1;
}
