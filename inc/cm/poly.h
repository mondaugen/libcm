#ifndef POLY_H
#define POLY_H 
#include <string.h> 
#include <gsl/gsl_complex.h> 
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_poly.h> 
#include <gsl/gsl_errno.h> 
#include <cm/cm_errno.h> 

/* The coefficients in all polynomials are stored: a[0] + a[1]x^1 + ... +
 * a[n]x^n */

/* Multiply two complex numbers that are adjecently packed in arrays in
 * rectangular form (a[0] + ia[1]). Result goes in c. */
#define cm_cmplx_mult_a_rect_(c,a,b) do { (c)[0] = (((a)[0])*((b)[0]))\
    - (((a)[1])*((b)[1])); (c)[1] = (((a)[0])*((b)[1])) + (((a)[1])*((b)[0])); }\
    while (0)

/* Multiply two complex numbers that are adjecently packed in arrays in polar
 * form (a[0]exp(ia[1])). Result goes in c. */
#define cm_cmplx_mult_a_pol_(c,a,b) do { (c)[0] = (((a)[0])*((b)[0]));\
    (c)[1] = (((a)[1])+((b)[1])); } while (0)

/* Multiply two complex numbers that are adjecently packed in arrays in
 * rectangular form and add them to the value already in c. Result goes in c. */
#define cm_cmplx_mult_accum_a_rect_(c,a,b) do { (c)[0] += (((a)[0])*((b)[0]))\
    - (((a)[1])*((b)[1])); (c)[1] += (((a)[0])*((b)[1])) + (((a)[1])*((b)[0])); }\
    while (0)

/* Multiply two polynomials a and b with real coefficients of order Na and Nb
 * respectively. This means that a and b are doubles in arrays of length Na + 1
 * and Nb + 1. C must be an array of length Na + Nb + 1. Result is put in C. C
 * doesn't need to be initialized to 0. Does not work with overlapping arrays. */
static inline int
cm_poly_mult_real(double *a, size_t Na, double *b, size_t Nb, double *c)
{
    memset(c,0,sizeof(double)*(Na + Nb + 1));
    size_t i, j;
    for(i=0; i<=Na; i++)
	for(j=0; j<=Nb; j++) 
	    c[i+j] += a[i]*b[j];
    return 0;
}

/* Multiply two polynomials a and b with real coefficients of order Na and Nb
 * respectively. This means that a and b are doubles in arrays of length Na + 1
 * and Nb + 1. C must be an array of length Na + Nb + 1. Result is put in C. C
 * doesn't need to be initialized to 0. In this case a, b and c can overlap so
 * you can, in effect, do "inplace" polynomial mulitplies, replacing the
 * coefficients already in c (not in the efficient sense). */
static inline int
cm_poly_mult_real_mcpy(double *a, size_t Na, double *b, size_t Nb, double *c)
{
    double A[Na+1];
    double B[Nb+1];
    memcpy(A,a,sizeof(double)*(Na+1));
    memcpy(B,b,sizeof(double)*(Nb+1));
    poly_mult_real(A,Na,B,Nb,c);
}

/* Multiply two polynomials a and b with imaginary coefficients of order Na and
 * Nb respectively. This means that a and b are doubles in arrays of length (Na
 * + 1)*2 and (Nb + 1)*2 because the numbers are packed as real/imaginary pairs.
 * C must be an array of length (Na + Nb + 1)*2. Result is put in C. C doesn't
 * need to be initialized to 0. The complex numbers are in rectangular form. */
static inline int
cm_poly_mult_im_rect(double *a, size_t Na, double *b, size_t Nb, double *c)
{
    memset(c,0,sizeof(double)*(Na + Nb + 1)*2);
    size_t i, j;
    for(i=0; i<=Na; i++)
	for(j=0; j<=Nb; j++) 
	    cm_cmplx_mult_accum_a_rect_((c+(2*(i+j))),(a+(2*i)),(b+(2*j)));
    return 0;
}

/* Multiply two polynomials a and b with imaginary coefficients of order Na and
 * Nb respectively. This means that a and b are doubles in arrays of length (Na
 * + 1)*2 and (Nb + 1)*2 because the numbers are packed as real/imaginary pairs.
 * C must be an array of length (Na + Nb + 1)*2. Result is put in C. C doesn't
 * need to be initialized to 0. The complex numbers are in rectangular form. In
 * this case a, b and c can overlap so you can, in effect, do "inplace"
 * polynomial mulitplies, replacing the coefficients already in c (not in the
 * efficient sense). */
static inline int
cm_poly_mult_im_rect_mcpy(double *a, size_t Na, double *b, size_t Nb, double *c)
{
    double A[(Na+1)*2];
    double B[(Nb+1)*2];
    memcpy(A,a,sizeof(double)*(Na+1)*2);
    memcpy(B,b,sizeof(double)*(Nb+1)*2);
    return cm_poly_mult_im_rect(A,Na,B,Nb,c);
}

/* Multiply two polynomials a and b with imaginary coefficients of order Na and
 * Nb respectively. This means that a and b are doubles in arrays of length (Na
 * + 1)*2 and (Nb + 1)*2 because the numbers are packed as real/imaginary pairs.
 * C must be an array of length (Na + Nb + 1)*2. Result is put in C. C doesn't
 * need to be initialized to 0. The complex numbers are in polar form. */
int
cm_poly_mult_im_pol(double *a, size_t Na, double *b, size_t Nb, double *c);

/* Multiply two polynomials a and b with imaginary coefficients of order Na and
 * Nb respectively. This means that a and b are doubles in arrays of length (Na
 * + 1)*2 and (Nb + 1)*2 because the numbers are packed as real/imaginary pairs.
 * C must be an array of length (Na + Nb + 1)*2. Result is put in C. C doesn't
 * need to be initialized to 0. The complex numbers are in polar form. In this
 * case a, b and c can overlap so you can, in effect, do "inplace" polynomial
 * mulitplies, replacing the coefficients already in c (not in the efficient
 * sense). */
int
cm_poly_mult_im_pol_mcpy(double *a, size_t Na, double *b, size_t Nb, double *c);

/* Given an array r of n real roots, fill c with the coefficients of the
 * polynomial their product gives. c must have length n + 1. Multiplies
 * polynomials of order 1 of this form (-r[i] + x). */
static inline int
cm_poly_from_roots_real(double *r, size_t n, double *c)
{
    double a[2*n];
    size_t i;
    for(i=0; i < n; i++){
	a[i*2] = r[i]*-1.0;
	a[i*2+1] = 1;
    }
    for(i=1; i<n; i++)
	cm_poly_mult_real_mcpy(a,i,a+(i*2),1,a);
    memcpy(c,a,sizeof(double)*(n+1));
    return 0;
}

/* Given an array r of n imaginary roots, fill c with the coefficients of the
 * polynomial their product gives. c must have length 2(n + 1). Multiplies
 * polynomials of order 1 of this form (-r[2*i]-r[2*i+1]j + x) where j is
 * sqrt(-1). Numbers are packed in the array in real/imaginary pairs in
 * rectangular form. */
static inline int
cm_poly_from_roots_im_rect(double *r, size_t n, double *c)
{
    double a[4*n];
    size_t i;
    for(i=0; i < n; i++){
	a[i*4] = r[i*2]*-1.0;
	a[i*4+1] = r[i*2+1]*-1.0;
	a[i*4+2] = 1;
	a[i*4+3] = 0;
    }
    for(i=1; i<n; i++)
	cm_poly_mult_im_rect_mcpy(a,i,a+(i*4),1,a);
    memcpy(c,a,sizeof(double)*(n+1)*2);
    return 0;
}

/* Given an array r of n imaginary roots, fill c with the coefficients of the
 * polynomial their product gives. c must have length 2(n + 1). Multiplies
 * polynomials of order 1 of this form (-r[2*i]exp(r[2*i+1]) + x) where j is
 * sqrt(-1). Numbers are packed in the array in real/imaginary pairs in
 * polar form. */
int
cm_poly_from_roots_im_pol(double *r, size_t n, double *c);

/* Convert an array r of n imaginary roots in rectangular form to polar form.
 * The array is packed as above and has a true physical length of
 * 2*n(sizeof(double)) */
int
cm_poly_rect_to_pol_a(double *r, size_t n);

/* Convert an array r of n imaginary roots in polar form to rectangular form.
 * The array is packed as above and has a true physical length of
 * 2*n(sizeof(double)) */
int
cm_poly_pol_to_rect_a(double *r, size_t n);

/* Given an array r of n imaginary roots in polar form, check their stability by
 * ensuring the modulus of each complex number is less than a given threshold.
 * If it is not, replace the root by its reciprocal. Numbers are in packed
 * modulus, argument pairs. n is the number of roots, so r is 2*n long. a is the
 * modulus to check against (commonly it is 1 for filters). */
int
cm_poly_make_stable_pol(double *r, size_t n, double a);

/* Given an array r of n imaginary roots in rectangular form, check their
 * stability by ensuring the modulus of each complex number is less than a given
 * threshold.  If it is not, replace the root by its reciprocal. Numbers are in
 * packed modulus, argument pairs. n is the number of roots, so r is 2*n long. a
 * is the modulus to check against (commonly it is 1 for filters). */
int
cm_poly_make_stable_rect(double *r, size_t n, double a);

/* Given an array of p real polynomial coefficients, factor the polynomial to
 * find the roots of it, make the roots stable by checking againts some radius r
 * and inverting them if needed, then re-multiply the polynomial, returning it
 * to the array of coefficients. The polynomial is of order n, so p is length
 * n+1. r is the radius of the unit circle (often 1 for filters). w is a
 * gsl_poly_complex_workspace of size n+1. If adj is 1, the final polynomial
 * will be multiplied by the highest order coefficient of the original. */
int
cm_poly_make_stable(double *p, size_t n, double r, int adj, 
	gsl_poly_complex_workspace *w);

#endif /* POLY_H */
