/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
#ifndef SNDFIO_H
#define SNDFIO_H 


#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define  __BEGIN_DECLS extern "C" {
# define  __END_DECLS }
#else  
# define  __BEGIN_DECLS /* empty */
# define  __END_DECLS /* empty */
#endif

__BEGIN_DECLS

#include <sndfile.h>
#include <stdlib.h> 
#include <string.h> 
#define SNDFIO_BUF_SIZE 4096

#define SNDFIO_ERR_FOPEN -1 /* returned on error opening a file */
#define SNDFIO_ERR_FWRITE -2 /* return on error writing file */ 


/* Allocates a table of doubles and loads the data from a soundfile into it.
 * Returns a pointer to the table. Fills the SF_INFO struct with other
 * information like length of file, number of channels, etc. If a multi-channel
 * file, the table will be interleaved and so its actual physical length in
 * memory with be len*num_channels */
double *
cm_load_sf_to_dbl_ary(const char *filepath, SF_INFO * sfinfo);

/* Saves an array of doubles to a file according to a pointer to an SF_INFO
 * struct */
long int
cm_write_sf_from_dbl_ary(double * tab, const char *filepath, SF_INFO *sfinfo);


/* Saves an array of doubles to a file according to paramters passed to the
 * function. Formats are defined in <sndfile.h>. An example format is
 * (SF_FORMAT_WAV | SF_FORMAT_PCM_16) to write a 16-bit wave file.
 * samplerate in Hz. */
long int
cm_write_sf_from_dbl_ary_params(double *tab, const char *filepath,
	sf_count_t frames, int samplerate, int channels, int format);


__END_DECLS

#endif /* SNDFIO_H */
