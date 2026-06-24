# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/traintechdata_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/traintechdata_autogen.dir/ParseCache.txt"
  "traintechdata_autogen"
  )
endif()
