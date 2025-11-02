# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\interface23_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\interface23_autogen.dir\\ParseCache.txt"
  "interface23_autogen"
  )
endif()
