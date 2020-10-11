project(opencl-tyrant-optimizer)

cmake_minimum_required (VERSION 2.6)


add_subdirectory(clew)
include_directories(clew/include)
add_subdirectory(test)

