#include <cm/poly.h>
#include <math.h> 
#include <cm/utils.h> 

#define CM_POLY_DEBUG_REVERSE 

/* reverse an array in place */
static void
cm_reverse_array(double *s, size_t len)
{
    size_t c, i, j;

    for(i = 0, j = (len-1); i < j; i++, j--){
	c = s[i];
	s[i] = s[j];
	s[j] = c;
    }

}

/* Multiply two polynomials a and b with imaginary coefficients of order Na and
 * Nb respectively. This means that a and b are doubles in arrays of length (Na
 * + 1)*2 and (Nb + 1)*2 because the numbers are packed as real/imaginary pairs.
 * C must be an array of length (Na + Nb + 1)*2. Result is put in C. C doesn't
 * need to be initialized to 0. The complex numbers are in polar form. */
int
cm_poly_mult_im_pol(double *a, size_t Na, double *b, size_t Nb, double *c)
{
    memset(c,0,sizeof(double)*(Na + Nb + 1)*2);
    size_t i, j;
    for(i=0; i<=Na; i++){
	for(j=0; j<=Nb; j++){ 
	    gsl_complex x,y,z;
	    z = gsl_complex_polar(c[2*(i+j)],c[2*(i+j)+1]);
	    x = gsl_complex_polar(a[2*i],a[2*i+1]);
	    y = gsl_complex_polar(b[2*j],b[2*j+1]);
	    z = gsl_complex_add(z,gsl_complex_mul(x,y));
	    c[2*(i+j)] = gsl_complex_abs(z);
	    c[2*(i+j)+1] = gsl_complex_arg(z);
	}
    }
    return 0;
}

/* Multiply two polynomials a and b with imaginary coefficients of order Na and
 * Nb respectively. This means that a and b are doubles in arrays of length (Na
 * + 1)*2 and (Nb + 1)*2 because the numbers are packed as real/imaginary pairs.
 * C must be an array of length (Na + Nb + 1)*2. Result is put in C. C doesn't
 * need to be initialized to 0. The complex numbers are in polar form. In this
 * case a, b and c can overlap so you can, in effect, do "inplace" polynomial
 * mulitplies, replacing the coefficients already in c (not in the efficient
 * sense). */
int
cm_poly_mult_im_pol_mcpy(double *a, size_t Na, double *b, size_t Nb, double *c)
{
    double A[(Na+1)*2];
    double B[(Nb+1)*2];
    memcpy(A,a,sizeof(double)*(Na+1)*2);
    memcpy(B,b,sizeof(double)*(Nb+1)*2);
    return cm_poly_mult_im_pol(A,Na,B,Nb,c);
}

/* Given an array r of n imaginary roots, fill c with the coefficients of the
 * polynomial their product gives. c must have length 2(n + 1). Multiplies
 * polynomials of order 1 of this form (-r[2*i]exp(r[2*i+1]) + x) where j is
 * sqrt(-1). Numbers are packed in the array in real/imaginary pairs in
 * polar form. */
int
cm_poly_from_roots_im_pol(double *r, size_t n, double *c)
{
    double a[4*n];
    size_t i;
    for(i=0; i < n; i++){
	a[i*4]	 = r[i*2]*-1.0;
	a[i*4+1] = r[i*2+1];
	a[i*4+2] = 1;
	a[i*4+3] = 0;
    }
    for(i=1; i<n; i++)
	cm_poly_mult_im_pol_mcpy(a,i,a+(i*4),1,a);
    memcpy(c,a,sizeof(double)*(n+1)*2);
    return 0;
}

/* Convert an array r of n imaginary roots in rectangular form to polar form.
 * The array is packed as above and has a true physical length of
 * 2*n(sizeof(double)) */
int
cm_poly_rect_to_pol_a(double *r, size_t n)
{
    while(n-- > 0){
	gsl_complex z = gsl_complex_rect(r[n*2],r[n*2+1]);
	r[n*2] = gsl_complex_abs(z);
	r[n*2+1] = gsl_complex_arg(z);
    }
    return 0;
}

/* Convert an array r of n imaginary roots in polar form to rectangular form.
 * The array is packed as above and has a true physical length of
 * 2*n(sizeof(double)) */
int
cm_poly_pol_to_rect_a(double *r, size_t n)
{
    while(n-- > 0){
	gsl_complex z = gsl_complex_polar(r[n*2],r[n*2+1]);
	r[n*2] = GSL_REAL(z);
	r[n*2+1] = GSL_IMAG(z);
    }
    return 0;
}

/* Given an array r of n imaginary roots in polar form, check their stability by
 * ensuring the modulus of each complex number is less than a given threshold.
 * If it is not, replace the root by its reciprocal. Numbers are in packed
 * modulus, argument pairs. n is the number of roots, so r is 2*n long. a is the
 * modulus to check against (commonly it is 1 for filters). */
int
cm_poly_make_stable_pol(double *r, size_t n, double a)
{
    while(n-- > 0){
	if(fabs(r[n*2]) > a)
	    r[n*2] = 1.0/(r[n*2]);
    }
    return 0;

}

/* Given an array r of n imaginary roots in rectangular form, check their
 * stability by ensuring the modulus of each complex number is less than a given
 * threshold.  If it is not, replace the root by its reciprocal. Numbers are in
 * packed modulus, argument pairs. n is the number of roots, so r is 2*n long. a
 * is the modulus to check against (commonly it is 1 for filters). */
int
cm_poly_make_stable_rect(double *r, size_t n, double a)
{
    while(n-- > 0){
	gsl_complex z = gsl_complex_rect(r[n*2],r[n*2+1]);
	double magz;
	if((magz = gsl_complex_abs(z)) > a){
	    /* Reflect the pole */
	    z = gsl_complex_mul_real(z, 1.0/magz);
	    r[n*2] = GSL_REAL(z);
	    r[(n*2)+1] = -1.0 * GSL_IMAG(z);
	}
    }
    //cm_poly_rect_to_pol_a(r,n);
    //cm_poly_make_stable_pol(r,n,a);
    //cm_poly_pol_to_rect_a(r,n);
    return 0;
}

/* Make the radii of the poles less than or equal to a */
int
cm_poly_norm_poles_pol(double *r, size_t n, double a)
{
    double ra[n]; /* to hold the radii */
    size_t i;
    for(i = 0; i < n; i++)
	ra[i] = r[i*2];
    double max = cm_find_max(ra, n);
    for(i = 0; i < n; i++)
	r[i*2] = (r[i*2]/max) * a;
    return 0;
}

int
cm_poly_norm_poles_rect(double *r, size_t n, double a)
{
    cm_poly_rect_to_pol_a(r,n);
    cm_poly_norm_poles_pol(r,n,a);
    cm_poly_pol_to_rect_a(r,n);
    return 0;
}

/* Given an array of p real polynomial coefficients, factor the polynomial to
 * find the roots of it, make the roots stable by checking againts some radius r
 * and inverting them if needed, then re-multiply the polynomial, returning it
 * to the array of coefficients. The polynomial is of order n, so p is length
 * n+1. r is the radius of the unit circle (often 1 for filters). w is a
 * gsl_poly_complex_workspace of size n+1. If adj is 1, the final polynomial
 * will be multiplied by the highest order coefficient of the original. */
int
cm_poly_make_stable(double *p, size_t n, double r, int adj,
	gsl_poly_complex_workspace *w)
{
    double z[n*2]; /* for the roots */
    double g = p[n]; /* Highest order coefficient */
    if(gsl_poly_complex_solve(p,n+1,w,z) == GSL_EFAILED)
	return CM_ERR_NO_CONVERGE;
    cm_poly_make_stable_rect(z,n,r);
    /* For the perhaps complex coefficients of the new polynomial */
    double q[(n+1)*2];
    cm_poly_from_roots_im_rect(z,n,q);
    /* copy only the real part of q back into p (the imaginary parts are very
     * small for low order polynomials) */
    size_t i;
    for(i=0; i <= n; i++)
	p[i] = adj ? q[i*2]*g : q[i*2];
    return 0;
}

/* Because we only calculate the coefficients and assume a0 to be 1, we must
 * include it when factoring the polynomial. This accepts array a of
 * coefficients of size P and a radius r to check the poles against. */
int
cm_poly_make_stable_from_lpc(double *a, size_t P, double r, int adj,
	gsl_poly_complex_workspace *w)
{
    double A[P+1]; /* A as a polynomial */
    A[0] = 1.0;	/* The missing 1.0 */
    memcpy((A+1),a,sizeof(double)*P); /* Copy the P coefficients after the 1 */
    /*cm_reverse_array(A,P+1);*/ /* reverse the array (equivalent to factoring out
				the highest power) this is because the z
				transform is in negative powers of z and we want
				positive*/
    int err;
    if(err = cm_poly_make_stable(A,P,r,adj,w))
	return err;
    //cm_reverse_array(A,P+1); /* reverse back */
    memcpy(a,(A+1),sizeof(double)*P); /* Copy the coefficients back into a */
    return 0;
}

/* Find the root with the largest modulus and normalize to r and multiply all
 * others by this scaling */
int
cm_poly_make_norm(double *a, size_t n, double r, int adj,
	gsl_poly_complex_workspace *w)
{
    double z[n*2];  /* for the roots */
    double g = a[n]; /* Highest order coefficient */
    double ra[n];   /* for the radii */
    if(gsl_poly_complex_solve(a,n+1,w,z) == GSL_EFAILED)
	return CM_ERR_NO_CONVERGE;
    cm_poly_norm_poles_rect(z,n,r);
    /* For the perhaps complex coefficients of the new polynomial */
    double q[(n+1)*2];
    cm_poly_from_roots_im_rect(z,n,q);
    /* copy only the real part of q back into p (the imaginary parts are very
     * small for low order polynomials) */
    size_t i;
    for(i=0; i <= n; i++)
	a[i] = adj ? q[i*2]*g : q[i*2];
    return 0;
}

/* Because we only calculate the coefficients and assume a0 to be 1, we must
 * include it when factoring the polynomial. This accepts array a of
 * coefficients of size P and a radius r to check the poles against. */
int
cm_poly_make_norm_from_lpc(double *a, size_t P, double r, int adj,
	gsl_poly_complex_workspace *w)
{
    double A[P+1]; /* A as a polynomial */
    A[0] = 1.0;	/* The missing 1.0 */
    memcpy((A+1),a,sizeof(double)*P); /* Copy the P coefficients after the 1 */
    cm_reverse_array(A,P+1); /* reverse the array (equivalent to factoring out
				the highest power) this is because the z
				transform is in negative powers of z and we want
				positive*/
    int err;
    if(err = cm_poly_make_norm(A,P,r,adj,w))
	return err;
    cm_reverse_array(A,P+1); /* reverse back */
    memcpy(a,(A+1),sizeof(double)*P); /* Copy the coefficients back into a */
    return 0;
}



