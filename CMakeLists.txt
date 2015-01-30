#############################################################################
#############################################################################
#
#  This is a CMakeList.txt file originally programmed for the CLMC/AMD labs
#  at the University of Southern California and the Max-Planck-Institute for
#  Intelligent Systems. We use a mixutre of explicit makefiles and cmake, but 
#  primarily we relay on cmake for all major compile dependencies. All our
#  software is provided under a slightly modified version of the LGPL license
#  to be found at http://www-clmc.usc.edu/software/license.
#
#  Copyright by Stefan Schaal, 2014
#
#############################################################################
#############################################################################

cmake_minimum_required(VERSION 2.8)

# make this directory known to the top-level cmake 
set(utilities_directory ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

# local defines
#set(header_directory  ${CMAKE_CURRENT_SOURCE_DIR}/include)
#set(src_directory     ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(header_directory  ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(src_directory     ${CMAKE_CURRENT_SOURCE_DIR}/src)

# chaining these includes to the start
include_directories(BEFORE ${header_directory})
include_directories(BEFORE ${src_directory})

set(SOURCES 
	${src_directory}/brent.c 
	${src_directory}/control.c 
	${src_directory}/dbrent.c  
	${src_directory}/df1dim.c  
	${src_directory}/dfpmin.c  
	${src_directory}/distributions.c 
	${src_directory}/dlinmin.c  
	${src_directory}/f1dim.c  
	${src_directory}/frprmn.c 
	${src_directory}/j-fixed.c 
	${src_directory}/ldltinv.c 
	${src_directory}/linmin.c  
	${src_directory}/lubksb.c  
	${src_directory}/ludcmp.c  
	${src_directory}/mnbrak.c  
	${src_directory}/numerics_package.c  
	${src_directory}/powell.c  
	${src_directory}/stability.c  
	${src_directory}/svbksb.c  
	${src_directory}/svdcmp.c  
	${src_directory}/parm_opt.c 
	${src_directory}/lookup_table.c 
	${src_directory}/kdtree.c 
	${src_directory}/pythag.c 
	${src_directory}/choldc.c 
	${src_directory}/statistics.c 
	${src_directory}/data_sets.c
  ${src_directory}/balanc.c 
	${src_directory}/hqr.c 
	${src_directory}/elmhes.c 
	${src_directory}/utility.c )

set(HEADERS
	${header_directory}/utility.h
	${header_directory}/data_sets.h
	${header_directory}/nrutil.h 
	${header_directory}/statistics.h 
	${header_directory}/utility_macros.h
	${header_directory}/mdefs.h )

add_library(utility ${SOURCES})
target_link_libraries(utility ${LAB_STD_LIBS})
if(NOT LAB_INCLUDES)
  MESSAGE(FATAL_ERROR "LAB_INCLUDES must be defined")
endif()
if(NOT LAB_LIBDIR)
  MESSAGE(FATAL_ERROR "LAB_LIBDIR must be defined")
ELSE()
  MESSAGE(WARNING "${LAB_LIBDIR} must be defined")
endif()
install(FILES ${HEADERS} DESTINATION ${LAB_INCLUDES})
install(TARGETS utility
  ARCHIVE DESTINATION ${LAB_LIBDIR}
  LIBRARY DESTINATION ${LAB_LIBDIR})


set(utility_LIBRARIES utility PARENT_SCOPE)
