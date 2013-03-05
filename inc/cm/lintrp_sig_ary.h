#ifndef LINTRP_SIG_ARY_H
#define LINTRP_SIG_ARY_H 

#include <cm/signal.h>
#include <cm/lintrp_dbl_tab.h> 

class LintrpSigAry: public Signal {
    private:
	Signal **tab; /* an array of pointers to signals */
	long int len; /* length of this array */
	double idx;   /* current index */
    public:
	LintrpSigAry(Signal**, long int);
	~LintrpSigAry();
	double tick();
	void set_index(double);
};

#endif /* LINTRP_SIG_ARY_H */
