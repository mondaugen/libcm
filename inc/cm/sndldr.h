#ifndef SNDLDR_H
#define SNDLDR_H 

#include <sndfile.h>
#include <stdlib.h> 
#define SNDLDR_BUF_SIZE 4096

/* Allocates a table of doubles and loads the data from a soundfile into it.
 * Returns a pointer to the table. Fills the SF_INFO struct with other
 * information like length of file, number of channels, etc. If a multi-channel
 * file, the table will be interleaved and so its actual physical length in
 * memory with be len*num_channels */
double *
cm_load_sf_to_dbl_ary(const char *filepath, SF_INFO * sfinfo);

#endif /* SNDLDR_H */
