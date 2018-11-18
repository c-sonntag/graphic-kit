#
# GraphicTools project
# Under the Copyright (C) Christophe-Alexandre Sonntag (http://u4a.at)

# --- PROJECT ---
project(GraphicTools)
cmake_minimum_required(VERSION 3.2)
#
set(GraphicTools_VERSION_MAJOR 1)
set(GraphicTools_VERSION_MINOR 0)
set(PROJECT_GraphicTools_STATIC "${PROJECT_NAME}_static")
set(PROJECT_GraphicTools_SHARED "${PROJECT_NAME}_shared")
set(PROJECT_GraphicTools_LIB_NAME "gtools")

# --- EXTERNALS LIBS ---
include("external/CMakeLists.txt")

#
##
#

# --- PROFILE ENVIRONNEMENT ---
if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions("-Wall" "-pedantic")
  add_definitions("-Wno-unused-function" "-Wno-unknown-pragmas")
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")     ## (not debug)
    add_definitions("-DNDEBUG=1")               ### https://en.cppreference.com/w/c/error/assert
    add_compile_options("-fno-builtin")         ### Break __buildin function (so slowest)
    add_compile_options("-O3")                  ### Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")            ### Strip binary (no work on debug)
  endif()
endif()

set(CMAKE_CXX_STANDARD 14)

# --- LINK CONFIG ---
set(INTERNAL_DEV_BUILD_MODE ON)

#
##
#

## # --- SET CMAKE MODULES ---
## set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
include("CMakeAutoSharedFilesCopier")


# --- LIB : EmbeddedResource ---
find_package(EmbeddedResource REQUIRED)
include_directories(${ERC_INCLUDE_DIRS})
link_directories(${ERC_DIRECTORIES})
link_libraries(${ERC_LIBRARIES})
add_definitions(${ERC_DEFINITIONS})
message(STATUS "ERC_LIBRARIES : ${ERC_LIBRARIES}")

# --- LIB : GLM ---
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_DIRECTORIES})
link_libraries(${GLM_LIBRARIES})
add_definitions(${GLM_DEFINITIONS})

# --- LIB : OpenGL ---
find_package(OPENGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_directories(${OPENGL_DIRECTORIES})
link_libraries(${OPENGL_LIBRARIES})
add_definitions(${OPENGL_DEFINITIONS})

# --- LIB : GLEW ---
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_DIRECTORIES})
link_libraries(${GLEW_LIBRARIES})
add_definitions(${GLEW_DEFINITIONS})
#copy_shared_near_archives(${GLEW_LIBRARIES})

# --- LIB : RAIIGL ---
if(RAIIGL_BASE_DIR AND INTERNAL_DEV_BUILD_MODE)
  if(NOT TARGET raiigl_shared)
    set(RAIIGL_SHARED ON)
    set(RAIIGL_STATIC OFF)
    add_subdirectory(${RAIIGL_BASE_DIR} "${CMAKE_BINARY_DIR}/raiigl_shared")
  endif()
  include_directories("${RAIIGL_BASE_DIR}/include")
  link_libraries(raiigl_shared)
  set_target_properties(raiigl_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
else()
  find_package(RAIIGL REQUIRED)
  include_directories(${RAIIGL_INCLUDE_DIRS})
  link_directories(${RAIIGL_DIRECTORIES})
  link_libraries(${RAIIGL_LIBRARIES})
  add_definitions(${RAIIGL_DEFINITIONS})
endif()

#
##
#

# --- TARGETS PATH ---
set(SRC_PATH "src")
set(HEADER_PATH "include")

# --- TARGETS FILES ---
file(GLOB_RECURSE TARGET_SRC_FILES "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.c")
file(GLOB_RECURSE TARGET_HEADER_FILES "${HEADER_PATH}/*.h" "${HEADER_PATH}/*.hpp" "${HEADER_PATH}/*.ipp")

# --- COMPILE OPTIONS ---
include_directories(${HEADER_PATH})

#
##
#

# --- BUILD OPTIONS : STATIC/SHARED ---
SET(GraphicTools_SHARED ON CACHE BOOL "Build shared GraphicTools library")
SET(GraphicTools_STATIC ON CACHE BOOL "Build static GraphicTools library")

# --- CHECK BUILD OPTIONS ---
if( (NOT ${GraphicTools_SHARED}) AND (NOT ${GraphicTools_STATIC}) )
  message(FATAL_ERROR, "\n\n   !! Require one of SHARED or STATIC for GraphicTools !! \n\n")
  set(GraphicTools_FOUND OFF)
  return()
endif()


#
##
#

# --- BUILD OPTIONS  ---
## set(GraphicTools_BUILD_TOOLS    ON CACHE BOOL "Build tools for GraphicTools library")
## set(GraphicTools_BUILD_TESTS    ON CACHE BOOL "Build tests for GraphicTools library")
set(GraphicTools_BUILD_EXAMPLES ON CACHE BOOL "Build examples for GraphicTools library")

# --- SUB BUILD LINKING ---
set(PROJECT_SUBPROJECT_HEADER_PATH "${CMAKE_CURRENT_LIST_DIR}/${HEADER_PATH}")
if(GraphicTools_SHARED)
  set(PROJECT_SUBPROJECT_LINKING ${PROJECT_GraphicTools_SHARED})
elseif(GraphicTools_STATIC)
  set(PROJECT_SUBPROJECT_LINKING ${PROJECT_GraphicTools_STATIC})
endif()

#
##
#

# --- EMBEDDED RESOURCES LIST ---
set(RESOURCES_FILES
  "resources/opengl/opengl.erc"
)

# --- BUILDS LIB TARGET STATIC ---
if(${GraphicTools_STATIC})

  # --- ---
  add_library(${PROJECT_GraphicTools_STATIC} STATIC ${TARGET_SRC_FILES} ${TARGET_HEADER_FILES} ${generated_files_path} ${embedded_resource_generation_FILES})
  set_target_properties(${PROJECT_GraphicTools_STATIC} PROPERTIES OUTPUT_NAME ${PROJECT_GraphicTools_LIB_NAME})
  
  # --- PRIVATE LIBS ---
  LINK_PRIVATE_LIB_STB_IMAGE(${PROJECT_GraphicTools_STATIC})

  # --- EMBEDDED RESOURCES ---
  erc_add_resources(${PROJECT_GraphicTools_STATIC} ADD_INTO_IDE ${RESOURCES_FILES})

  # --- INSTALL LIB ---
  install(TARGETS ${PROJECT_GraphicTools_STATIC}
          LIBRARY DESTINATION "lib"
          ARCHIVE DESTINATION "lib")

endif()

# --- BUILDS LIB TARGET SHARED ---
if(${GraphicTools_SHARED})

  # --- ---
  add_library(${PROJECT_GraphicTools_SHARED} SHARED ${TARGET_SRC_FILES} ${TARGET_HEADER_FILES})
  set_target_properties(${PROJECT_GraphicTools_SHARED} PROPERTIES OUTPUT_NAME ${PROJECT_GraphicTools_LIB_NAME})

  # --- PRIVATE LIBS ---
  LINK_PRIVATE_LIB_STB_IMAGE(${PROJECT_GraphicTools_SHARED})

  # --- EMBEDDED RESOURCES ---
  erc_add_resources(${PROJECT_GraphicTools_SHARED} ADD_INTO_IDE ${RESOURCES_FILES})

  # --- INSTALL LIB ---
  install(TARGETS ${PROJECT_GraphicTools_SHARED}
          RUNTIME DESTINATION "bin"
          LIBRARY DESTINATION "lib"
          ARCHIVE DESTINATION "lib")

endif()

# --- INSTALL INCLUDE ---
install(DIRECTORY "${HEADER_PATH}/." DESTINATION "include")

#
## Subproject
#

set(PROJECT_GraphicTools_HEADER_PATH "${CMAKE_CURRENT_LIST_DIR}/${HEADER_PATH}")


#
##
#

# # --- BUILDS TOOLS  ---
# if(${GraphicTools_BUILD_TOOLS})
#   ##add_subdirectory("tools/")
# endif()

# --- BUILDS EXAMPLES ---
if(${GraphicTools_BUILD_EXAMPLES})
 add_subdirectory("examples/")
endif()

# # --- BUILDS TESTS ---
# if(${GraphicTools_BUILD_TESTS} AND EXISTS ${ERC_BINARY_PACKAGER})
#   add_subdirectory("tests/")
# endif()
