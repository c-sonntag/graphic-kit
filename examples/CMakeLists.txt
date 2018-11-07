#
# Tools project
# Under the Copyright (C) Christophe-Alexandre Sonntag (http://u4a.at)

#
##
#

# --- PROFILE ENVIRONNEMENT ---
if(CMAKE_COMPILER_IS_GNUCXX)                ## (GNU COMMAND)
  add_definitions("-Wall" "-pedantic")        # Toutes erreurs
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

# --- LIB : GLFW ---
set(GLFW_USE_STATIC_LIBS OFF)
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_DIRECTORIES})
link_libraries(${GLFW_LIBRARIES})
add_definitions(${GLFW_DEFINITIONS})


# --- PARENT LIB ---
include_directories(${PROJECT_SUBPROJECT_HEADER_PATH})
link_libraries(${PROJECT_SUBPROJECT_LINKING})

#
## COMMON
#

include_directories("common")
file(GLOB_RECURSE COMMON_FILES "common/*.c"  "common/*.cpp"  "common/*.hpp")
add_library(common SHARED ${COMMON_FILES})


#
## EXEMPLES C++
#

file(GLOB files "*.c" "*.cpp")
foreach(FILE_PATH ${files} )

  #
  get_filename_component(EXECUTABLE_NAME ${FILE_PATH} NAME_WE)

  #
  set(EXTRA_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${EXECUTABLE_NAME}")
  set(EXTRA_FILES "")
  set(WITH_EXTRA "")
  if(EXISTS "${EXTRA_SRC}")
    file(GLOB_RECURSE EXTRA_FILES "${EXTRA_SRC}/*.c"  "${EXTRA_SRC}/*.cpp"  "${EXTRA_SRC}/*.hpp")
    file(GLOB_RECURSE EXTRA_RES_FILES "${EXTRA_SRC}/*.erc")
    set(WITH_EXTRA "(extra)")
  endif()

  #
  message(STATUS "examples : ${EXECUTABLE_NAME} ${WITH_EXTRA}")
  add_executable(${EXECUTABLE_NAME} ${FILE_PATH} ${EXTRA_FILES})
  if(WITH_EXTRA)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${EXTRA_SRC})
    if(EXTRA_RES_FILES)
      message(STATUS "erc_add_resources ${EXECUTABLE_NAME}  RES : ${EXTRA_RES_FILES}")
      erc_add_resources(${EXECUTABLE_NAME} ADD_INTO_IDE ${EXTRA_RES_FILES})
    endif()
  endif()

  #
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE common)

  # --- INSTALL THIS TOOLS ---
  install(TARGETS ${EXECUTABLE_NAME}
          RUNTIME DESTINATION "bin")

endforeach()

