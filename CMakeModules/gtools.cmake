

## 
function(gtools_copy_sharedlibs target_from target_dist ) 

  #

  
  #
  set(copied_files_list)
  foreach(archive_file ${ARGN})
    get_filename_component(absolute_archive_file "${archive_file}" ABSOLUTE)
    get_filename_component(absolute_archive_dir "${absolute_archive_file}" DIRECTORY)
    
    foreach(path_suffix_check ${path_suffix_check_list})
    
      #
      get_filename_component(check_dir "${absolute_archive_dir}/${path_suffix_check}" REALPATH)
      file(GLOB check_dir_libs_files_paths "${check_dir}/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
      
      foreach(lib_filepath ${check_dir_libs_files_paths})
      
        #
        list(FIND copied_files_list "${lib_filepath}" lib_filepath_already_copied_index)
        if(NOT ${lib_filepath_already_copied_index} EQUAL -1)
          continue()
        endif()
      
        #
        get_filename_component(lib_filename "${lib_filepath}" NAME)
        set(output_copy "${CMAKE_CURRENT_BINARY_DIR}/${lib_filename}")
        execute_process(
          COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${lib_filepath}" "${output_copy}"
          WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
         )
        list(APPEND copied_files_list "${lib_filepath}")
        
      endforeach()
    
    endforeach()
  endforeach()
  
  #
  message(STATUS "copy_shared_near_archives : ${copied_files_list}") 
  
  
endfunction() 
