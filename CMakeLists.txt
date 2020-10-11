project(opencl-tyrant-optimizer)

cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if( NOT BUILD_SHARED_LIBRARY )
  add_definitions(-Dclew_STATIC)
endif()

add_subdirectory(clew)
include_directories(clew/include)
add_subdirectory(test)

