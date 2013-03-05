#ifndef INDEXABLE_H
#define INDEXABLE_H 

class Indexable {
    protected:
	long int len;
    public:
	virtual double get_index(double i) =0;
	virtual double get_index(int i) =0;
	virtual double length() =0; /* distance between x[0] and x[len-1] */
	virtual double first_x() =0; /* x[0] */
	virtual double last_x() =0; /* x[len-1] */
	double operator [] (double);
	double operator [] (int);
};

#endif /* INDEXABLE_H */
