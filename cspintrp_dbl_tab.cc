/* Holds a table of doubles and interpolates them using cublic splines with
 * periodic end conditions. When the tables are initialized, they are made one
 * longer than the length of the table provided and the first point of this
 * table is put at the end once again, to eliminate discontinuities. If you do
 * not want periodic interpolation, consider using lintrp_dbl_tab.h or
 * csintrp_dbl_tab.h */

/* NOTE August 31st: len is not really representative of the distance between
 * the smallest x in the table and the largest. minx should be x[0] and maxx
 * should be x[len-1] and the length() function should retrun maxx - minx */
#include <cstdlib>
#include <cstring> 
#include <cm/utils.h> 
#include <cm/cspintrp_dbl_tab.h>

/* tabx and taby are copied into internal arrays of this object and thus the
 * original can be freed afterwards, will be persistent until object deleted */
CspintrpDblTab::CspintrpDblTab (double *tabx, double *taby, long int len)
{
    /* for this one we leave it to you to make sure that the first and last
     * entries in the y table are the same, if not an exception is thrown */
    if(tabx[0] != tabx[len-1]){
	throw "CspintrpDblTab init err: First and last " 
		    "entries in tabx must be equal";
    }
    this->tabx = new double[len];
    this->taby = new double[len];
    memcpy(this->tabx, tabx, (len)*sizeof(double));
    memcpy(this->taby, taby, (len)*sizeof(double));
    this->len = len;
    this->spline = gsl_spline_alloc(gsl_interp_cspline_periodic, this->len);
    gsl_spline_init(this->spline, this->tabx, this->taby, this->len);
    this->acc = gsl_interp_accel_alloc();
}

/* taby is copied into internal array of this object and thus the original can be
 * freed afterwards, will be persistent until object deleted. Tabx is created
 * and holds a series of indices ie for len=4 {0.0, 1.0, 2.0, 3.0}. Required for
 * interpolation schemes. Also destroyed on deletion of object. */
CspintrpDblTab::CspintrpDblTab (double *taby, long int len)
{
    this->tabx = new double[len+1];
    this->taby = new double[len+1];
    for(int i=0; i<(len+1); i++)
	this->tabx[i] = (double)i;
    memcpy(this->taby, taby, len*sizeof(double));
    this->taby[len] = this->taby[0]; /* set first and last to equal */
    this->len = len;
    this->spline = gsl_spline_alloc(gsl_interp_cspline_periodic, len+1);
    gsl_spline_init(this->spline, this->tabx, this->taby, len+1);
    this->acc = gsl_interp_accel_alloc();
}

CspintrpDblTab::~CspintrpDblTab()
{
    delete [] this->tabx;
    delete [] this->taby;
    gsl_spline_free(this->spline);
    gsl_interp_accel_free(this->acc);
}

/* gets an interpolated value at an index, if the index is out of bounds it is
 * clipped between x[0] and x[len-1] */
double CspintrpDblTab::get_index(double i)
{
    double y;
    int err;
    i = cm_fwrap(i, this->tabx[0], this->tabx[len-1]); 
    if((err = gsl_spline_eval_e(this->spline, i, this->acc, &y))
	    == GSL_EDOM){
	throw "Index Error.\n";
	return 0;
    }
    else
	return y;
}

/* gets an interpolated value at an index, if the index is out of bounds it is
 * clipped between x[0] and x[len-1] */
double CspintrpDblTab::get_index(int i)
{
    double y;
    int err;
    i = cm_wrap(i, (int)(this->tabx[0]), (int)(this->tabx[len-1])); 
    if((err = gsl_spline_eval_e(this->spline, (double)i, this->acc, &y))
	    == GSL_EDOM){
	throw "Index Error.\n";
	return 0;
    }
    else
	return y;
}

/* get the distance from x[0] to x[len-1] */
double CspintrpDblTab::length()
{
    return tabx[len-1]-tabx[0];
}

/* get the lowest value for tabx */
double CspintrpDblTab::first_x()
{
    return tabx[0];
}
/* get the highest value for tabx */
double CspintrpDblTab::last_x()
{
    return tabx[len-1];
}
