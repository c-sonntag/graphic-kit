#
# GraphicToolkit project - externals builds
# Under the Copyright (C) Christophe-Alexandre Sonntag (http://u4a.at)


# ---  ---
include (ExternalProject)
set(EXTERNAL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")


# --- EXTERNAL SETS ---
set(EXTERNAL_STB_DIR       "stb-master")
#set(EXTERNAL_ASSIMP_DIR    "assimp")

#
##
#

# --- EXTERNAL : STB ---
set(STB_SOURCE             "${EXTERNAL_SOURCE_DIR}/${EXTERNAL_STB_DIR}")
set(STB_PREFIX             "${EXTERNAL_SOURCE_DIR}/stb")
#
macro(LINK_PRIVATE_LIB_STB_IMAGE PROJECT_TARGET)
  target_include_directories(${PROJECT_TARGET} PUBLIC ${STB_SOURCE})
endmacro()

## # --- EXTERNAL : ASSIMP ---
## set(ASSIMP_SOURCE         "${EXTERNAL_SOURCE_DIR}/${EXTERNAL_ASSIMP_DIR}")
## set(ASSIMP_PREFIX          "external/assimp")
## set(ASSIMP_BUILD           "${CMAKE_CURRENT_BINARY_DIR}/${ASSIMP_PREFIX}/build")
## set(ASSIMP_INSTALL         "${CMAKE_CURRENT_BINARY_DIR}/${ASSIMP_PREFIX}/install")
## set(PROJECT_ASSIMP_STATIC  "${ASSIMP_BUILD}/assimp.a")
## #
## ExternalProject_Add(
##   assimp
##   PREFIX ${ASSIMP_PREFIX}
##   SOURCE_DIR ${ASSIMP_SOURCE}
##   BINARY_DIR ${ASSIMP_BUILD}
##   INSTALL_DIR ${ASSIMP_INSTALL}
##   BUILD_IN_SOURCE OFF
##   BUILD_BYPRODUCTS ${PROJECT_ASSIMP_STATIC}
##   STEP_TARGETS build
##   EXCLUDE_FROM_ALL TRUE
##   CMAKE_CACHE_ARGS
##     -DCMAKE_BUILD_TYPE:STRING=Release
##     -DCMAKE_INSTALL_PREFIX:STRING=${ASSIMP_INSTALL}
##     -DBUILD_SHARED_LIBS:BOOL=OFF
##     -DBUILD_STATIC_LIBS:BOOL=ON
##     -DBUILD_TESTS:BOOL=OFF
## )
## #
## macro(LINK_PRIVATE_LIB_ASSIMP PROJECT_TARGET)
##   add_dependencies(${PROJECT_TARGET} assimp-build)
##   target_include_directories(${PROJECT_TARGET} PRIVATE ${ASSIMP_SOURCE})
##   target_link_libraries     (${PROJECT_TARGET} PRIVATE ${PROJECT_ASSIMP_STATIC})
## endmacro()
