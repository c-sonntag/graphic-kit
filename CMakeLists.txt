#
# GraphicToolkit project
# Under the Copyright (C) Christophe-Alexandre Sonntag (http://u4a.at)

# --- PROJECT ---
project(GraphicToolkit)
cmake_minimum_required(VERSION 3.2)
#
set(GraphicToolkit_VERSION_MAJOR 1)
set(GraphicToolkit_VERSION_MINOR 0)
set(PROJECT_GraphicToolkit_STATIC "${PROJECT_NAME}_static")
set(PROJECT_GraphicToolkit_SHARED "${PROJECT_NAME}_shared")

# --- EXTERNALS LIBS ---
include("external/CMakeLists.txt")

#
##
#

# --- PROFILE ENVIRONNEMENT ---
if(CMAKE_COMPILER_IS_GNUCXX)                ## (GNU COMMAND)
  add_definitions("-Wall" "-pedantic")        # Toutes erreurs sauf :
  add_definitions("-Wno-unused-function" "-Wno-unknown-pragmas") #
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")   ## (not debug)
    add_compile_options("-fno-builtin")         ## Break __buildin function (so slowest)
    add_compile_options("-O3")                  ### Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")            ### Strip binary (no work on debug)
  endif()
endif()

set(CMAKE_CXX_STANDARD 14)

#
##
#

## # --- SET CMAKE MODULES ---
## set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
## include("CMakeAutoSharedFilesCopier")


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

# --- LIB : RAIIGL ---
find_package(RAIIGL REQUIRED)
include_directories(${RAIIGL_INCLUDE_DIRS})
link_directories(${RAIIGL_DIRECTORIES})
link_libraries(${RAIIGL_LIBRARIES})
add_definitions(${RAIIGL_DEFINITIONS})


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
SET(GraphicToolkit_SHARED OFF CACHE BOOL "Build shared GraphicToolkit library")
SET(GraphicToolkit_STATIC ON CACHE BOOL "Build static GraphicToolkit library")

# --- CHECK BUILD OPTIONS ---
if( (NOT ${GraphicToolkit_SHARED}) AND (NOT ${GraphicToolkit_STATIC}) )
  message(FATAL_ERROR, "\n\n   !! Require one of SHARED or STATIC for GraphicToolkit !! \n\n")
  set(GraphicToolkit_FOUND OFF)
  return()
endif()


#
##
#

# --- BUILD OPTIONS  ---
## set(GraphicToolkit_BUILD_TOOLS    ON CACHE BOOL "Build tools for GraphicToolkit library")
## set(GraphicToolkit_BUILD_TESTS    ON CACHE BOOL "Build tests for GraphicToolkit library")
set(GraphicToolkit_BUILD_EXAMPLES ON CACHE BOOL "Build examples for GraphicToolkit library")

# --- SUB BUILD LINKING ---
set(PROJECT_SUBPROJECT_HEADER_PATH "${CMAKE_CURRENT_LIST_DIR}/${HEADER_PATH}")
if(GraphicToolkit_SHARED)
  set(PROJECT_SUBPROJECT_LINKING ${PROJECT_GraphicToolkit_SHARED})
elseif(GraphicToolkit_STATIC)
  set(PROJECT_SUBPROJECT_LINKING ${PROJECT_GraphicToolkit_STATIC})
endif()

#
##
#


#
##
#

set(RESOURCES_FILES
  "resources/opengl/opengl.erc"
)

# --- BUILDS LIB TARGET STATIC ---
if(${GraphicToolkit_STATIC})

  # --- ---
  add_library(${PROJECT_GraphicToolkit_STATIC} STATIC ${TARGET_SRC_FILES} ${TARGET_HEADER_FILES} ${generated_files_path} ${embedded_resource_generation_FILES})
  set_target_properties(${PROJECT_GraphicToolkit_STATIC} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
  
  # --- PRIVATE LIBS ---
  LINK_PRIVATE_LIB_STB_IMAGE(${PROJECT_GraphicToolkit_STATIC})

  # --- EMBEDDED RESOURCES ---
  erc_add_resources(${PROJECT_GraphicToolkit_STATIC} ADD_INTO_IDE ${RESOURCES_FILES})

  # --- INSTALL LIB ---
  install(TARGETS ${PROJECT_GraphicToolkit_STATIC}
          LIBRARY DESTINATION "lib"
          ARCHIVE DESTINATION "lib")

endif()

# --- BUILDS LIB TARGET SHARED ---
if(${GraphicToolkit_SHARED})

  # --- ---
  add_library(${PROJECT_GraphicToolkit_SHARED} SHARED ${TARGET_SRC_FILES} ${TARGET_HEADER_FILES})
  set_target_properties(${PROJECT_GraphicToolkit_SHARED} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

  # --- PRIVATE LIBS ---
  LINK_PRIVATE_LIB_STB_IMAGE(${PROJECT_GraphicToolkit_SHARED})

  # --- EMBEDDED RESOURCES ---
  erc_add_resources(${PROJECT_GraphicToolkit_SHARED} ADD_INTO_IDE ${RESOURCES_FILES})

  # --- INSTALL LIB ---
  install(TARGETS ${PROJECT_GraphicToolkit_SHARED}
          RUNTIME DESTINATION "bin"
          LIBRARY DESTINATION "lib"
          ARCHIVE DESTINATION "lib")

endif()

# --- INSTALL INCLUDE ---
install(DIRECTORY "${HEADER_PATH}/." DESTINATION "include")

#
## Subproject
#

set(PROJECT_GraphicToolkit_HEADER_PATH "${CMAKE_CURRENT_LIST_DIR}/${HEADER_PATH}")


#
##
#

# # --- BUILDS TOOLS  ---
# if(${GraphicToolkit_BUILD_TOOLS})
#   ##add_subdirectory("tools/")
# endif()

# --- BUILDS EXAMPLES ---
if(${GraphicToolkit_BUILD_EXAMPLES})
 add_subdirectory("examples/")
endif()

# # --- BUILDS TESTS ---
# if(${GraphicToolkit_BUILD_TESTS} AND EXISTS ${ERC_BINARY_PACKAGER})
#   add_subdirectory("tests/")
# endif()
