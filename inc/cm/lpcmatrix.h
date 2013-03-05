#ifndef LPCMATRIX_H
#define LPCMATRIX_H 

#include <stdlib.h> 
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_errno.h> 
#include <stdarg.h> 

/* File formats */
/* Format 1000 has entries:
 *  <size>	<what>
 *  int		format number
 *  size_t	number of rows
 *  size_t	number of columns (P, the order of the linear predictor)
 *  size_t	hop size in samples
 *  double	data...
 *  double	...
 */

//#define CM_LPC_DEBUG 
//#define CM_LPC_NO_GSL_ERROR_HANDLER 
#define LPC_MATRIX_FILE_FORMAT_CURRENT	    1000 /* may change */

typedef int (*pf_find_lps_t)(const gsl_vector*, size_t, size_t, gsl_vector*);

/* Make a toeplitz matrix from a signal x with N rows and p columns. The columns
 * are filled, from top to bottom: x[n-p] x[n+1-p] ... x[N-1-p] for 1 <= p <= P.
 * m must have #rows = N, #columns = P*/
int
cm_make_lp_toeplitz_covariance(const double *x, gsl_matrix *m, size_t N, size_t P);

/* Accepts a pointer to a matrix m, a vector to place the solutions into and a
 * vector y which are the values against which to compare the error. Returns -1
 * if the number of rows in m does not equal the number of rows in y, -2 for
 * memory allocation problems and 0 otherwise. The product of m and its
 * transpose must be a symmetric matrix to take advantage of Cholesky
 * decomposition (otherwise erroneous results are obtained) (haha)! */
int
cm_normal_eqn_solve_cholesky(gsl_matrix *m, gsl_vector *a, const gsl_vector *y);

/* Finds the L.P. coefficients of the signal vector x using a linear predictor
 * of order P. x must have a length of at least 2N-1 because the linear
 * predictor calculates the covariance of the signal using a window of size N
 * sliding along the vector x and calculating the covariance at N equally spaced
 * points. The vector a must have size P. */
int
cm_find_lp_coeffs_covar(const gsl_vector *x, size_t N, size_t P, gsl_vector *a);

/* Can be called directly on arrays of doubles, putting the result in a. x must
 * at least have length N + P and a must have length P. No boundary checking
 * is done. */
int
cm_find_lp_coeffs_sig(double *x, size_t N, size_t P, double *a,
	pf_find_lps_t find_lps);

/* Writes successive lp coefficients to a file open for writing. N and P are as
 * before, H is the hop size. This will write P coefficients (len-N)/H + 1 times
 * to the file. This will not write a header to the file, if you want to, use
 * cm_write_lpc_file_header. */
int
cm_find_successive_lpcs(double *x, size_t len, size_t N, size_t P, size_t H,
	pf_find_lps_t find_lps, FILE *f);

/* Writes successive lp coefficients to a file open for writing. N and P are as
 * before, H is the hop size. This will write P coefficients (len-(N+P))/H + 1
 * times to the file. This will not write a header to the file, if you want to,
 * use cm_make_cm_file_formatter and cm_write_cm_file_formatter. */
int
cm_find_successive_lpcs_covar(double *x, size_t len, size_t N, size_t P, size_t H,
				FILE *f);
/* Writes successive lp coefficients to a file open for writing. N and P are as
 * before, H is the hop size. This will write P coefficients (len-(N))/H + 1
 * times to the file. This will not write a header to the file, if you want to,
 * use cm_make_cm_file_formatter and cm_write_cm_file_formatter. w is a window
 * that the signal is multiplied by and should have length equal to N. x remains
 * untouched, the windowed signal is placed in a buffer. */
int
cm_find_successive_lpcs_autocorr(double *x, size_t len, size_t N, size_t P,
				    size_t H, double *w, FILE *f);

#endif /* LPCMATRIX_H */
