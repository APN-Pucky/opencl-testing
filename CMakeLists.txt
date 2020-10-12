project(opencl-tyrant-optimizer)

cmake_minimum_required (VERSION 2.6)

# CXX + STATIC
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set( BUILD_SHARED_LIBRARIES OFF)

if(TOO_C_STATIC) # This is MinGW
  set( CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
  set( CMAKE_EXECUTABLE_SUFFIX "exe")
endif()

if(TOO_BOOST_STATIC)
  set(Boost_USE_STATIC_LIBS   ON)
endif()

if( NOT BUILD_SHARED_LIBRARY )
  add_definitions(-Dclew_STATIC)
endif()


# BOOST 
#set(Boost_DEBUG 1)
find_package(Boost EXACT COMPONENTS filesystem REQUIRED)


add_subdirectory(external)
include_directories(external/clew/include)
add_subdirectory(src)
enable_testing()
add_subdirectory(test)


