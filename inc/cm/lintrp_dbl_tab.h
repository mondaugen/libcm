#ifndef LINTRP_DBL_TAB_H
#define LINTRP_DBL_TAB_H 

#include <cm/indexable.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

class LintrpDblTab: public Indexable {
    private:
	double *tabx;
	double *taby;
	gsl_spline *spline;
	gsl_interp_accel *acc;
	void init_tabx(long int len);
	void init_spline();
    public:
	LintrpDblTab (double*, long int);
	LintrpDblTab (double*, double*, long int);
	LintrpDblTab (const char *sndfile);
	~LintrpDblTab ();
	double get_index(double);
	double get_index(int i);
	double length(); /* distance between x[0] and x[len-1] */
	double first_x(); /* x[0] */
	double last_x(); /* x[len-1] */
};	

#endif /* LINTRP_DBL_TAB_H */
