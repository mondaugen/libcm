/* Copyright 2013 Nicholas Esterer. All Rights Reserved. */
/*
 * convenience functions for loading soundfiles into tables of doubles using
 * libsndfile
 */

#include <cm/sndfio.h>
#ifdef LIBCM_DEBUG
#include <stdio.h> 
#endif 

/* Allocates a table of doubles and loads the data from a soundfile into it.
 * Returns a pointer to the table. Fills the SF_INFO struct with other
 * information like length of file, number of channels, etc. If a multi-channel
 * file, the table will be interleaved and so its actual physical length in
 * memory with be len*num_channels*sizeof(double) */
double *
cm_load_sf_to_dbl_ary(const char *filepath, SF_INFO * sfinfo)
{
    SNDFILE *sndfile;
    memset(sfinfo, 0, sizeof(SF_INFO)); /* apparently MUST be set to 0 */
#ifdef LIBCM_DEBUG 
    printf("sfinfo set to 0\n");
#endif  
    if(!(sndfile = sf_open(filepath, SFM_READ, sfinfo)))
	return NULL;
#ifdef LIBCM_DEBUG 
    printf("Soundfile %s open.\n", filepath);
#endif  
    double *tab;
    if((tab = (double*)malloc(sizeof(double)
		    * (sfinfo->frames) 
		    * (sfinfo->channels)))
	    == NULL)
	return NULL;
#ifdef LIBCM_DEBUG 
    printf("%d frames alloced.\n", (int)(sfinfo->frames*sfinfo->channels));
#endif  
    /* load the soundfile into the table */
#ifdef LIBCM_DEBUG 
    printf("Reading from soundfile %s", filepath);
#endif  
    double *tabptr = tab;
    sf_count_t num_items_read = 0;
//	tabptr += (int)num_items_read;
    if((num_items_read = sf_read_double(sndfile, tabptr, sfinfo->frames 
		* sfinfo->channels)) != (sfinfo->frames * sfinfo->channels))
	return NULL;
#ifdef LIBCM_DEBUG 
	printf("Read %ld items.\n", (long int)num_items_read);
#endif  
    sf_close(sndfile);
    return tab;
}

/* Saves an array of doubles to a file according to a pointer to an SF_INFO
 * struct */
long int
cm_write_sf_from_dbl_ary(double * tab, const char *filepath, SF_INFO *sfinfo)
{
    SNDFILE * sndfile;
    long int numframes = sfinfo->frames;
    if(!(sndfile = sf_open(filepath, SFM_WRITE, sfinfo)))
	return SNDFIO_ERR_FOPEN;
#ifdef LIBCM_DEBUG 
    printf("Writing %ld frames and %d channels to %s\n", numframes,
		sfinfo->channels, filepath);
#endif  
    if(sf_write_double(sndfile, tab, (numframes) * (sfinfo->channels))
	    != ((numframes) * (sfinfo->channels))){
	sf_close(sndfile);
	return SNDFIO_ERR_FWRITE;
    }
    sf_close(sndfile);
    return numframes * sfinfo->channels;
}

/* Saves an array of doubles to a file according to paramters passed to the
 * function. Formats are defined in <sndfile.h>. An example format is
 * (SF_FORMAT_WAV | SF_FORMAT_PCM_16) to write a 16-bit wave file.
 * samplerate in Hz. Frames is the length of the file in samples, so an n-channel
 * file requires a table of frames*n in length.*/
long int
cm_write_sf_from_dbl_ary_params(double *tab, const char *filepath,
	sf_count_t frames, int samplerate, int channels, int format)
{
    SF_INFO sfinfo;
    memset(&sfinfo, 0, sizeof(SF_INFO));
    sfinfo.frames = frames;
    sfinfo.samplerate = samplerate;
    sfinfo.channels = channels;
    sfinfo.format = format;
    return cm_write_sf_from_dbl_ary(tab, filepath, &sfinfo);
}


