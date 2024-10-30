# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "A6_autogen"
  "CMakeFiles\\A6_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\A6_autogen.dir\\ParseCache.txt"
  )
endif()
