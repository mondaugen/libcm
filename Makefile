OBJS.cc		= $(patsubst %.cc,%.o,$(wildcard *.cc))
HDRS.cc		= $(patsubst %.cc,%.h,$(wildcard *.cc))
OBJS.c		= $(patsubst %.c,%.o,$(wildcard *.c))
HDRS.c		= $(patsubst %.c,%.h,$(wildcard *.c))
OBJS		= $(OBJS.cc) $(OBJS.c)
HDRDIR		= cm/
CMINC		= ./inc/
INC		= -I$(CMINC) -I/opt/local/include/\
		  -I/opt/local/include/glib-2.0 -I/opt/local/lib/glib-2.0/include
VPATH 		= inc
USRLIBPTH 	= /usr/local/lib/
USRINCPTH	= /usr/local/include/
LIBNAME		= libcm.a
DEFINES		= -DLIBCM_DEBUG
OPT		= -ggdb3 #-O3

${LIBNAME} : $(OBJS)
	ar rvs libcm.a $(OBJS); rm $(OBJS)

clip.o	: clip.c cm/utils.h 
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

cspintrp_dbl_tab.o : cspintrp_dbl_tab.cc cm/utils.h cm/cspintrp_dbl_tab.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

dbltab.o : dbltab.cc cm/utils.h cm/dbltab.h 
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

genrand.o : genrand.c cm/gen_tab.h 
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

gensinsum.o : gensinsum.c cm/gen_tab.h 
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

indexable.o : indexable.cc cm/indexable.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

lintrp_dbl_tab.o : lintrp_dbl_tab.cc cm/utils.h cm/lintrp_dbl_tab.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

lintrp_sig_ary.o : lintrp_sig_ary.cc cm/signal.h cm/lintrp_sig_ary.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

signal.o : signal.cc cm/signal.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

wander.o : wander.c cm/utils.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

wavtab.o : wavtab.cc cm/wavtab.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

wavtabsmplr.o : wavtabsmplr.cc cm/wavtabsmplr.h cm/utils.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

wrap.o : wrap.c cm/utils.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

conversions.o : conversions.c cm/utils.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

sndbffr.o: sndbffr.cc cm/utils.h cm/sndfio.h cm/sndbffr.h cm/indexable.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

sndfio.o : sndfio.c cm/sndfio.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

mxr.o : mxr.cc cm/mxr.h cm/signal.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

env.o : env.cc cm/signal.h cm/indexable.h cm/instrument.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

smplrinst.o : smplrinst.cc cm/smplrinst.h cm/instrument.h cm/signal.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

sndpntpnd.o : sndpntpnd.cc cm/sndpntpnd.h cm/signal.h cm/smplrinst.h\
    		cm/bus.h cm/smpldel.h cm/gain.h cm/indexable.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

spp2dary.o : spp2dary.cc cm/spp2dary.h cm/signal.h cm/sndpntpnd.h cm/mxr.h\
    cm/indexable.h cm/utils.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

smpldel.o : smpldel.cc cm/smpldel.h cm/signal.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

bus.o : bus.cc cm/signal.h cm/bus.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

gain.o : gain.cc cm/signal.h cm/gain.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

bffralloc.o : bffralloc.cc cm/bffralloc.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

txtfio.o : txtfio.c cm/txtfio.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

midievnt.o : midievnt.cc cm/midi/midievnt.h cm/midi/midi_msg.h cm/screvnt.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

midi_msg.o : midi_msg.c cm/midi/mnevnt.h cm/midi/midievnt.h cm/midi/midi_msg.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

mnevnt.o : mnevnt.cc cm/midi/mnevnt.h cm/midi/midievnt.h cm/midi/midi_msg.h\
		cm/screvnt.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

anymidievnt.o : anymidievnt.cc cm/midi/midievnt.h \
    		cm/midi/midi_msg.h cm/midi/anymidievnt.h cm/screvnt.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

frm_trees.o : frm_trees.c cm/frm_trees.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

sigutils.o : sigutils.c cm/utils.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

cmfiler.o : cmfiler.c cm/cmfiler.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

interputils.o : interputils.c cm/interputils.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

filter.o : filter.c cm/filter.h cm/utils.h cm/cm_errno.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

lpcmatrix.o : lpcmatrix.c cm/lpcmatrix.h cm/utils.h cm/cm_errno.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

windowfun.o : windowfun.c cm/windowfun.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

poly.o : poly.c cm/poly.h
	$(CC) $(OPT) $(INC) ${DEFINES} -c $< -o $@

install :
	mkdir $(USRINCPTH)$(HDRDIR)
	cp $(LIBNAME) $(USRLIBPTH); cp -R $(CMINC)$(HDRDIR) $(USRINCPTH)$(HDRDIR)

uninstall :
	rm $(USRLIBPTH)$(LIBNAME); rm -r $(USRINCPTH)$(HDRDIR)

clean :
	rm $(OBJS) $(LIBNAME)
