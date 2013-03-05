#ifndef CSPINTRP_DBL_TAB_H
#define CSPINTRP_DBL_TAB_H 

#include <cm/indexable.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

class CspintrpDblTab: public Indexable {
    private:
	double *tabx;
	double *taby;
	gsl_spline *spline;
	gsl_interp_accel *acc;
    public:
	CspintrpDblTab (double*, long int);
	CspintrpDblTab (double*, double*, long int);
	~CspintrpDblTab ();
	double get_index(double);
	double get_index(int i);
	double length(); /* distance between x[0] and x[len-1] */
	double first_x(); /* x[0] */
	double last_x(); /* x[len-1] */
};	

#endif /* CSPINTRP_DBL_TAB_H */
