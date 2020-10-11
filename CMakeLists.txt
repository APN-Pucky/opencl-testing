project(opencl-tyrant-optimizer)

cmake_minimum_required (VERSION 2.6)

if( NOT BUILD_SHARED_LIBRARY )
  add_definitions(-Dclew_STATIC)
endif()

add_subdirectory(clew)
include_directories(clew/include)
add_subdirectory(test)

