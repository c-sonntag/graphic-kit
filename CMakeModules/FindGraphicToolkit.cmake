

#
##
if( 
  ( (NOT GraphicToolkit_SHARED) AND (NOT GraphicToolkit_STATIC) ) OR
  ( (GraphicToolkit_SHARED) AND (GraphicToolkit_STATIC) ) OR
  ( (NOT DEFINED GraphicToolkit_SHARED) AND (NOT GraphicToolkit_STATIC) ) OR
  ( (NOT DEFINED GraphicToolkit_STATIC) AND (NOT GraphicToolkit_SHARED) )
)
  message(FATAL_ERROR, 
    "\n\n"
    "   !! Require one of SHARED or STATIC setting for GraphicToolkit !! \n"
    "   !! You can FIX IT by GraphicToolkit_STATIC/GraphicToolkit_SHARED variables !! \n"
    "   !! Configure It By CMake DEFINITON !! \n"
    "   !! Actual GraphicToolkit_SHARED=${GraphicToolkit_SHARED} and GraphicToolkit_STATIC=${GraphicToolkit_STATIC} !! \n"
    "\n"
  )
  set(GraphicToolkit_FOUND OFF)
  return()
endif()

#
##
set(GraphicToolkit_POSSIBLE_PATH
  "${GraphicToolkit_ROOT}"
  "${GraphicToolkit_SUFFIX}"
  "${CMAKE_SOURCE_DIR}/external/graphic-toolkit"
  "${CMAKE_SOURCE_DIR}/thirdparty/graphic-toolkit"
  "${CMAKE_SOURCE_DIR}/third-party/graphic-toolkit"
)


#
##
find_path(
  GraphicToolkit_INCLUDE_DIRS
  NAMES "graphic_toolkit/"
  PATHS ${GraphicToolkit_POSSIBLE_PATH}
  PATH_SUFFIXES "include/"
)
set(GraphicToolkit_INCLUDE_DIR ${GraphicToolkit_INCLUDE_DIRS})

#
##
set(GraphicToolkit_LIBRARIES_NAME "libGraphicToolkit")


#
##
if(${GraphicToolkit_STATIC})
  find_library(
    GraphicToolkit_LIBRARIES
    NAMES 
      "${GraphicToolkit_LIBRARIES_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}"
    PATHS ${GraphicToolkit_POSSIBLE_PATH}
    PATH_SUFFIXES "lib/"
  )
elseif(${GraphicToolkit_SHARED})
  find_library(
    GraphicToolkit_LIBRARIES
    NAMES "${GraphicToolkit_LIBRARIES_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
    PATHS ${GraphicToolkit_POSSIBLE_PATH}
    PATH_SUFFIXES "lib/"
  )
endif()


message(STATUS "GraphicToolkit_INCLUDE_DIRS : ${GraphicToolkit_INCLUDE_DIRS}")
message(STATUS "GraphicToolkit_LIBRARIES : ${GraphicToolkit_LIBRARIES}")


#
##
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GraphicToolkit REQUIRED_VARS GraphicToolkit_LIBRARIES GraphicToolkit_INCLUDE_DIRS)
