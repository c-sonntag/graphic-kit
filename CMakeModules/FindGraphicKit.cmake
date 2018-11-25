#
## Import lib ZLIB with CMakeCustomModules Utils
#

#
##
include("${CMAKE_CURRENT_LIST_DIR}/CMakeCustomModules.cmake")


#
##
LIB_IMPORTER(
  GraphicKit      # LIB_NAME 
  "gk/"           # LIB_INCLUDE_NAMES 
  "GraphicKit"    # LIB_IMPORT_NAMES 
  ""              # LIB_DEFINITONS 
  GraphicKit      # OUTPUT_TARGET_NAME
)








