#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cwalk" for configuration "Debug"
set_property(TARGET cwalk APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(cwalk PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libcwalk.a"
  )

list(APPEND _cmake_import_check_targets cwalk )
list(APPEND _cmake_import_check_files_for_cwalk "${_IMPORT_PREFIX}/lib/libcwalk.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
