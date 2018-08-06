#
# GraphicToolkit project - externals builds
# Under the Copyright (C) Christophe-Alexandre Sonntag (http://u4a.at)


# ---  ---
include (ExternalProject)
set(EXTERNAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external")


# --- EXTERNAL SETS ---
set(EXTERNAL_STB_DIR       "stb-master")

#
##
#

# --- EXTERNAL : STB ---
set(STB_SOURCE             "${EXTERNAL_SOURCE_DIR}/${EXTERNAL_STB_DIR}")
set(STB_PREFIX              "external/stb")
#
macro(LINK_PRIVATE_LIB_STB_IMAGE PROJECT_TARGET)
  target_include_directories(${PROJECT_TARGET} PRIVATE ${STB_SOURCE})
endmacro()



