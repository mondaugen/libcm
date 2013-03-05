/* Holds a table of doubles and can return them linearly interpolated. This
 * doesn't support looping back around the table. */
#include <cstdlib>
#include <cstring> 
#include <cm/utils.h> 
#include <cm/lintrp_dbl_tab.h>
#include <cm/sndfio.h> 

void
LintrpDblTab::init_tabx(long int len)
{
    this->tabx = new double[len];
    for(int i=0; i<len; i++)
	this->tabx[i] = (double)i;
}

void
LintrpDblTab::init_spline()
{
    this->spline = gsl_spline_alloc(gsl_interp_linear, this->len);
    gsl_spline_init(this->spline, this->tabx, this->taby, this->len);
    this->acc = gsl_interp_accel_alloc();
}

/* tabx and taby are copied into internal arrays of this object and thus the
 * original can be freed afterwards, will be persistent until object deleted */
LintrpDblTab::LintrpDblTab (double *tabx, double *taby, long int len)
{
    this->tabx = new double[len];
    this->taby = new double[len];
    memcpy(this->tabx, tabx, len*sizeof(double));
    memcpy(this->taby, taby, len*sizeof(double));
    this->len = len;
    init_spline();
}

/* taby is copied into internal array of this object and thus the original can be
 * freed afterwards, will be persistent until object deleted. Tabx is created
 * and holds a series of indices ie for len=4 {0.0, 1.0, 2.0, 3.0}. Required for
 * interpolation schemes. Also destroyed on deletion of object. */
LintrpDblTab::LintrpDblTab (double *taby, long int len)
{
    this->taby = new double[len];
    init_tabx(len);
    memcpy(this->taby, taby, len*sizeof(double));
    this->len = len;
    init_spline();
}

LintrpDblTab::LintrpDblTab (const char *sndfile)
{
    SF_INFO sfinfo;
    double *tmpy = cm_load_sf_to_dbl_ary(sndfile, &sfinfo);
    if(sfinfo.channels != 1){
	throw "LintrpDblTab only supports mono soundfiles.\n";
	free(tmpy);
	return;
    }
    init_tabx(sfinfo.frames);
    this->taby = tmpy;
    this->len  = sfinfo.frames;
    init_spline();
}

LintrpDblTab::~LintrpDblTab()
{
    delete [] this->tabx;
    delete [] this->taby;
    gsl_spline_free(this->spline);
    gsl_interp_accel_free(this->acc);
}

/* gets an interpolated value at an index, if the index is out of bounds it is
 * clipped between x[0] and x[len-1] */
double LintrpDblTab::get_index(double i)
{
    double y;
    int err;
    i = cm_fclip(i, this->tabx[0], this->tabx[len-1]); 
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
double LintrpDblTab::get_index(int i)
{
    double y;
    int err;
    i = cm_clip(i, (int)(this->tabx[0]), (int)(this->tabx[len-1])); 
    if((err = gsl_spline_eval_e(this->spline, (double)i, this->acc, &y))
	    == GSL_EDOM){
	throw "Index Error.\n";
	return 0;
    }
    else
	return y;
}

double LintrpDblTab::length()
{
    return tabx[len-1]-tabx[0];
}

double LintrpDblTab::first_x()
{
    return tabx[0];
}

double LintrpDblTab::last_x()
{
    return tabx[len-1];
}
