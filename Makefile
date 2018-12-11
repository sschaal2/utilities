#############################################################################
#############################################################################
#
#  This is a Makefile file originally programmed for the CLMC/AMD labs
#  at the University of Southern California and the Max-Planck-Institute for
#  Intelligent Systems. We use a mixutre of explicit makefiles and cmake, but 
#  primarily we relay on cmake for all major compile dependencies. All our
#  software is provided under the license in the LICENSE file.
#
#  Copyright by Stefan Schaal, 2014
#
#############################################################################
#############################################################################

SUBDIRS = $(MACHTYPE)

include $(LAB_ROOT)/config/make/LAB.mk
