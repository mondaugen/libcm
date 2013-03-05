/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/* Routines for constructing autocorrelation and covariance matrices for use
 * with linear predicition routines. */

#include <cm/lpcmatrix.h>
#include <cm/windowfun.h> 
#include <cm/utils.h> 

/* Make a toeplitz matrix from a signal x with N rows and p columns. The columns
 * are filled, from top to bottom: x[n-p] x[n+1-p] ... x[N-1-p] for 1 <= p <= P.
 * m must have #rows = N, #columns = P. This is not a covariance matrix, but
 * becomes one when multiplied by its own transpose.*/
int
cm_make_lp_toeplitz_covariance(const double *x, gsl_matrix *m, size_t N, size_t P)
{
    if ((N < 1) || (P < 1)) {
	return -1;
    }
    if ((N != m->size1) || (P != m->size2))
	return -2;
    size_t n, p;
    for (p = 1; p <= P; p++)
	for (n = 0; n < N; n++)
	    gsl_matrix_set(m, n, p-1, x[n-p]);
    return 0;
}

/* Make an autocorrelation matrix from the signal x. N is the length of the
 * signal and P is the dimension of the square matrix.*/
int
cm_make_lp_autocorr_matrix(double *x, gsl_matrix *m, size_t N, size_t P)
{
#ifdef  CM_LPC_DEBUG
    fprintf(stderr,"\n");
    size_t __i;
    for(__i = 0; __i < N; __i++)
	fprintf(stderr,"%g ",x[__i]);
    fprintf(stderr,"\n");
#endif  /* CM_LPC_DEBUG */ 
    if ((N < 1) || (P < 1)) {
	return -1;
    }
    if ((P != m->size1) || (P != m->size2))
	return -2;
    size_t i, j;
    for (i = 1; i <= P; i++){
	for (j = 1; j <= P; j++){
	    double d;
	    d = cm_autocorr(x,N,i-j);
#ifdef  CM_LPC_DEBUG
	    fprintf(stderr,"%g ",d);
#endif  /* CM_LPC_DEBUG */ 
	    m->data[P*(i-1) + (j-1)] = d;
	    //gsl_matrix_set(m,i-1,j-1,d);
	}
#ifdef  CM_LPC_DEBUG
	fprintf(stderr,"\n");
#endif  /* CM_LPC_DEBUG */ 
    }
#ifdef  CM_LPC_DEBUG
	fprintf(stderr,"\n");
#endif  /* CM_LPC_DEBUG */ 
    return 0;
}

/* Make an autocorrelation vector from the signal x, N is the length of the
 * signal and P is the length of the vector. */
int
cm_make_lp_autocorr_vector(double *x, gsl_vector *v, size_t N, size_t P)
{
    if ((N < 1) || (P < 1)) {
	return -1;
    }
    if ((P != v->size))
	return -2;
    size_t i;
    for(i = 1; i <= P; i++){
	double d = cm_autocorr(x,N,i);
#ifdef  CM_LPC_DEBUG
	    fprintf(stderr,"%g ",d);
#endif  /* CM_LPC_DEBUG */ 
	gsl_vector_set(v,i-1,d);
    }
#ifdef  CM_LPC_DEBUG
    fprintf(stderr,"\n\n");
#endif  /* CM_LPC_DEBUG */ 
    return 0;
}

/* Accepts a pointer to a matrix m, a vector to place the solutions into and a
 * vector y which are the values against which to compare the error. Returns -1
 * if the number of rows in m does not equal the number of rows in y, -2 for
 * memory allocation problems and 0 otherwise. The product of m and its
 * transpose must be a positive definite matrix to take advantage of Cholesky
 * decomposition. A real matrix multiplied by its transpose will always be
 * positive definite, but if for some reason it is not (perhaps due to roundoff
 * errors) then the solutions obtained are all set to 0 */
int
cm_normal_eqn_solve_cholesky(gsl_matrix *m, gsl_vector *a, const gsl_vector *y)
{
    /* if the coordinate length of y is not equal to the number of rows, error*/
    if ((y->size != m->size1))
	return -1;
    /* if the coordinate length of a is not equal to the number of columns... */
    if ((a->size != m->size2))
	return -2;
    /* the matrix holding the product of the multiplication of transpose(m)m */
    double c[m->size2 * m->size2];
    gsl_matrix_view C = gsl_matrix_view_array(c, m->size2, m->size2);
    /* the vector holding the product of the multiplication of transpose(m)y */
    double d[y->size];
    gsl_vector_view D = gsl_vector_view_array(d, a->size);
    int err;
    if(err = gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, m, m, 0.0, &C.matrix)){
	perror("cm_normal_eqn_solve_cholesky matrix matrix product:");
	return err;
    }
    if(err = gsl_blas_dgemv(CblasTrans, 1.0, m, y, 0.0, &D.vector)) {
	perror("cm_normal_eqn_solve_cholesky matrix vector product:");
	return err;
    }
    /* now we solve the normal equations */
    /* first decompose matrix C */
    if(err = gsl_linalg_cholesky_decomp(&C.matrix)) {
	perror("cm_normal_eqn_solve_cholesky decomposing matrix:");
	if(err == GSL_EDOM){
	    /* this means the matrix was not positive definite, we just set a to
	     * 0 */
	    gsl_vector_set_zero(a);
	    return 0;
	}else{
	    return err;
	}
    }
    /* now solve the system putting result in a */
    if(err = gsl_linalg_cholesky_solve(&C.matrix, &D.vector, a)) {
	perror("cm_normal_eqn_solve_cholesky solving matrix:");
	return err;
    }
    return 0;
}

/* Uses the covariance method. Finds the L.P. coefficients of the signal vector
 * x using a linear predictor of order P. x must have a length of at least N + P
 * because the linear predictor calculates the covariance of the signal using a
 * window of size N sliding along the vector x and calculating the covariance at
 * N equally spaced points. The vector a must have size P. */
int
cm_find_lp_coeffs_covar(const gsl_vector *x, size_t N, size_t P, gsl_vector *a)
{
    if(x->size < (N + P))
	return -1;
    if(a->size != P)
	return -2;
    double m[N*P];
    gsl_matrix_view M = gsl_matrix_view_array(m,N,P);
    int err;
    if(err = cm_make_lp_toeplitz_covariance(x->data + P, &M.matrix, N, P)) {
	perror("Error making toeplitz");
	return err - 10;
    }
    gsl_vector_const_view Y = gsl_vector_const_subvector(x, P, N);
    if(err = cm_normal_eqn_solve_cholesky(&M.matrix, a, &Y.vector)) {
	perror("Error solving normal eqns");
	return err - 20;
    }
    return 0;
}

/* Uses the autocorrelation method. Finds the L.P. coefficients of the signal
 * vector x using a linear predictor of order P. N is the length of the signal.
 * The coefficients are deposited in a. */
int
cm_find_lp_coeffs_autocorr(const gsl_vector *x, size_t N, size_t P, gsl_vector *a)
{
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
    gsl_error_handler_t *gslerr = gsl_set_error_handler_off();
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
    /* if N or P are bad, the autocorrelation matrix will just be filled with
     * entries of 0 */
    if(a->size != P){
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
	gsl_set_error_handler(gslerr);
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
	return -2;
    }
#ifdef CM_LPC_DEBUG
    fprintf(stderr,"\n");
    gsl_vector_fprintf(stderr,x,"%g");
    fprintf(stderr,"\n");
#endif /* CM_LPC_DEBUG */ 
    double m[P*P];
    gsl_matrix_view M = gsl_matrix_view_array(m,P,P);
    int err;
    if(err = cm_make_lp_autocorr_matrix(x->data, &M.matrix, N, P)){
	perror("Error making autocorrellation matrix");
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
	gsl_set_error_handler(gslerr);
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
	return err - 10;
    }
#ifdef CM_LPC_DEBUG
    /* Print matrix */
    size_t k, l;
    for(k = 0; k < P; k++){
	for(l = 0; l < P; l++)
	    fprintf(stderr, "%+5.5f ", m[P*k + l]);
	fprintf(stderr,"\n");
    }
    fprintf(stderr,"\n");
    fflush(stderr);
#endif /* CM_LPC_DEBUG */ 
    double y[P];
    gsl_vector_view Y = gsl_vector_view_array(y,P);
    if(err = cm_make_lp_autocorr_vector(x->data, &Y.vector, N, P)){
	perror("Error making autocorellation vector");
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
	gsl_set_error_handler(gslerr);
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
	return err - 20;
    }
    /* Now we solve the system */
    /* first decompose matrix M */
    if(err = gsl_linalg_cholesky_decomp(&M.matrix)) {
	perror("cm_find_lp_coeffs_autocorr decomposing matrix:");
	if(err == GSL_EDOM){
	    /* this means the matrix was not positive definite, we just set a to
	     * 0 */
	    gsl_vector_set_zero(a);
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
	    gsl_set_error_handler(gslerr);
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
	    return 0;
	}else{
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
	gsl_set_error_handler(gslerr);
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
	    return err - 30;
	}
    }
    /* now solve the system putting result in a */
    if(err = gsl_linalg_cholesky_solve(&M.matrix, &Y.vector, a)) {
	perror("cm_find_lp_coeffs_autocorr solving matrix:");
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
	gsl_set_error_handler(gslerr);
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
	return err - 40;
    }
#ifdef CM_LPC_NO_GSL_ERROR_HANDLER
	gsl_set_error_handler(gslerr);
#endif /* CM_LPC_NO_GSL_ERROR_HANDLER */
    return 0;
}

/* Can be called directly on arrays of doubles, putting the result in a. x must
 * at least have length N + P and a must have length P. No boundary checking
 * is done. BAD NAME. */
int
cm_find_lp_coeffs_sig(double *x, size_t N, size_t P, double *a,
	pf_find_lps_t find_lps)
{
    gsl_vector_view X = gsl_vector_view_array(x, N + P);
    gsl_vector_view A = gsl_vector_view_array(a, P);
    int err;
    if(err = cm_find_lp_coeffs_covar(&X.vector, N, P, &A.vector))
	return err;
    return 0;
}

/* Can be called directly on arrays of doubles, putting the result in a. x must
 * at least have length N + P and a must have length P. No boundary checking
 * is done. */
int
cm_find_lp_coeffs_sig_covar(double *x, size_t N, size_t P, double *a)
{
    gsl_vector_view X = gsl_vector_view_array(x, N + P);
    gsl_vector_view A = gsl_vector_view_array(a, P);
    int err;
    if(err = cm_find_lp_coeffs_covar(&X.vector, N, P, &A.vector))
	return err;
    return 0;
}

/* Can be called directly on arrays of doubles, putting the result in a. x must
 * at least have length N + P and a must have length P. No boundary checking
 * is done. */
int
cm_find_lp_coeffs_sig_autocorr(double *x, size_t N, size_t P, double *a)
{
    gsl_vector_view X = gsl_vector_view_array(x, N);
    gsl_vector_view A = gsl_vector_view_array(a, P);
    int err;
    if(err = cm_find_lp_coeffs_autocorr(&X.vector, N, P, &A.vector))
	return err;
    return 0;
}

/* Writes successive lp coefficients to a file open for writing. N and P are as
 * before, H is the hop size. This will write P coefficients (len-(N))/H + 1
 * times to the file. This will not write a header to the file, if you want to,
 * use cm_make_cm_file_formatter and cm_write_cm_file_formatter. HAS BUG: M is
 * too small so the last window can run off the end of x when using covariance
 * method. */
int
cm_find_successive_lpcs(double *x, size_t len, size_t N, size_t P, size_t H,
	pf_find_lps_t find_lps, FILE *f)
{
    size_t M = (len - N)/H + 1;
    size_t i;
    for(i = 0; i < M; i++){
	double a[P];
	cm_find_lp_coeffs_sig(x + i*H, N, P, a, find_lps);
	if(fwrite(a, sizeof(double), P, f) < P) {
	    perror("cm_find_successive_lpcs writing coeffs:");
	    return(-1);
	}
    }
    return(0);
}

/* Writes successive lp coefficients to a file open for writing. N and P are as
 * before, H is the hop size. This will write P coefficients (len-(N+P))/H + 1
 * times to the file. This will not write a header to the file, if you want to,
 * use cm_make_cm_file_formatter and cm_write_cm_file_formatter. */
int
cm_find_successive_lpcs_covar(double *x, size_t len, size_t N, size_t P, size_t H,
				FILE *f)
{
    size_t M = (len - N - P)/H + 1;
    size_t i;
    for(i = 0; i < M; i++){
	double a[P];
	cm_find_lp_coeffs_sig_covar(x + i*H, N, P, a);
	if(fwrite(a, sizeof(double), P, f) < P) {
	    perror("cm_find_successive_lpcs writing coeffs:");
	    return(-1);
	}
    }
    return(0);
}

/* Writes successive lp coefficients to a file open for writing. N and P are as
 * before, H is the hop size. This will write P coefficients (len-N)/H + 1
 * times to the file. This will not write a header to the file, if you want to,
 * use cm_make_cm_file_formatter and cm_write_cm_file_formatter. w is a window
 * that the signal is multiplied by and should have length equal to N. x remains
 * untouched, the windowed signal is placed in a buffer. */
int
cm_find_successive_lpcs_autocorr(double *x, size_t len, size_t N, size_t P,
				    size_t H, double *w, FILE *f)
{
    size_t M = (len - N)/H + 1;
    size_t i;
    for(i = 0; i < M; i++){
	double a[P];
	double y[N];
	cm_window(y, x + i*H, w, N);
#ifdef CM_LPC_DEBUG
	size_t j;
	fprintf(stderr,"Unwindowed:\n");
	for(j = 0; j < N; j++)
	    fprintf(stderr, "%4.4f ", x[i*H + j]);
	fprintf(stderr,"\nWindowed:\n");
	for(j = 0; j < N; j++)
	    fprintf(stderr, "%4.4f ", y[j]);
	fprintf(stderr,"\n");
#endif /* CM_LPC_DEBUG */
	int err;
	if(err = cm_find_lp_coeffs_sig_autocorr(y, N, P, a))
	    fprintf(stderr,"Error %d finding autocorrellation coefficients.\n",err);
	if(fwrite(a, sizeof(double), P, f) < P) {
	    perror("cm_find_successive_lpcs writing coeffs:");
	    return(-1);
	}
    }
    return(0);
}
