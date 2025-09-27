# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\interfin_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\interfin_autogen.dir\\ParseCache.txt"
  "interfin_autogen"
  )
endif()
