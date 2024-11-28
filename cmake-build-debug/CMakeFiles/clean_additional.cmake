# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\radarmmstest_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\radarmmstest_autogen.dir\\ParseCache.txt"
  "radarmmstest_autogen"
  )
endif()
