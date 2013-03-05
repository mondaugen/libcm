#ifndef DBLTAB_H
#define DBLTAB_H 

#include <cm/indexable.h>

class DblTab: public Indexable {
    private:
	double	    *tab;
	long int    len;
    public:
	DblTab (double *, long int);
	DblTab();
	~DblTab (void);
	double get_index(double i);
	double get_index(int i);
	double length(); /* distance between x[0] and x[len-1] */
	double first_x(); /* x[0] */
	double last_x(); /* x[len-1] */
};

#endif /* DBLTAB_H */
