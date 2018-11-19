

#
##
if( 
  ( (NOT GTOOLS_SHARED) AND (NOT GTOOLS_STATIC) ) OR
  ( (GTOOLS_SHARED) AND (GTOOLS_STATIC) ) OR
  ( (NOT DEFINED GTOOLS_SHARED) AND (NOT GTOOLS_STATIC) ) OR
  ( (NOT DEFINED GTOOLS_STATIC) AND (NOT GTOOLS_SHARED) )
)
  message(FATAL_ERROR, 
    "\n\n"
    "   !! Require one of SHARED or STATIC setting for GTOOLS !! \n"
    "   !! You can FIX IT by GTOOLS_STATIC/GTOOLS_SHARED variables !! \n"
    "   !! Configure It By CMake DEFINITON !! \n"
    "   !! Actual GTOOLS_SHARED=${GTOOLS_SHARED} and GTOOLS_STATIC=${GTOOLS_STATIC} !! \n"
    "\n"
  )
  set(GTOOLS_FOUND OFF)
  return()
endif()

#
##
set(GTOOLS_POSSIBLE_PATH
  ${GTOOLS_DIR}
  ${GTOOLS_SUFFIX}
  ${GTOOLS_ROOT}
  "${CMAKE_SOURCE_DIR}/external/graphic-toolkit"
  "${CMAKE_SOURCE_DIR}/thirdparty/graphic-toolkit"
  "${CMAKE_SOURCE_DIR}/third-party/graphic-toolkit"
)


#
##
find_path(
  GTOOLS_INCLUDE_DIRS
  NAMES "graphic_toolkit/"
  PATHS ${GTOOLS_POSSIBLE_PATH}
  PATH_SUFFIXES "include/"
)
set(GTOOLS_INCLUDE_DIR ${GTOOLS_INCLUDE_DIRS})

#
##
set(GTOOLS_LIBRARIES_NAME "libGTOOLS")


#
##
if(${GTOOLS_STATIC})
  find_library(
    GTOOLS_LIBRARIES
    NAMES 
      "${GTOOLS_LIBRARIES_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    PATHS ${GTOOLS_POSSIBLE_PATH}
    PATH_SUFFIXES "lib/"
  )
elseif(${GTOOLS_SHARED})
  find_library(
    GTOOLS_LIBRARIES
    NAMES "${GTOOLS_LIBRARIES_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
    PATHS ${GTOOLS_POSSIBLE_PATH}
    PATH_SUFFIXES "lib/"
  )
endif()

#
##
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTOOLS REQUIRED_VARS GTOOLS_LIBRARIES GTOOLS_INCLUDE_DIRS)
