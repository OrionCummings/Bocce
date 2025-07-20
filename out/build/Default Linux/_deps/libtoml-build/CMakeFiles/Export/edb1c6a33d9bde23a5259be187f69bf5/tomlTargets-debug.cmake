#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "toml::toml" for configuration "Debug"
set_property(TARGET toml::toml APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(toml::toml PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libtoml.a"
  )

list(APPEND _cmake_import_check_targets toml::toml )
list(APPEND _cmake_import_check_files_for_toml::toml "${_IMPORT_PREFIX}/lib/libtoml.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
