project(opencl-tyrant-optimizer)

cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set( BUILD_SHARED_LIBRARIES OFF)
if(WIN32)
set( CMAKE_EXE_LINKER_FLAGS "-static")
if(DEFINED ENV{BOOST_ROOT_LATEST})
	file(TO_CMAKE_PATH $ENV{BOOST_ROOT_LATEST} CMAKE_BOOST_ROOT)
    	list(APPEND BOOST_ROOT "$ENV{CMAKE_BOOST_ROOT}")
    	list(APPEND BOOST_INCLUDEDIR "$ENV{CMAKE_BOOST_ROOT}/include")
    	list(APPEND BOOST_LIBRARYDIR "$ENV{CMAKE_BOOST_ROOT}/lib")
endif()
endif()

set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost EXACT COMPONENTS system thread filesystem regex timer REQUIRED)


if( NOT BUILD_SHARED_LIBRARY )
  add_definitions(-Dclew_STATIC)
endif()

add_subdirectory(clew)
include_directories(clew/include)
add_subdirectory(test)

