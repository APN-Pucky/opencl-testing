cmake_minimum_required(VERSION 2.6)
# This file is part of MXE. See LICENSE.md for licensing information.

# partial module - included by src/cmake/CMakeLists.txt

#set(TGT test-${PKG}-cmake)



#set( CMAKE_CXX_STANDARD 11 )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Ofast")
#set( BUILD_SHARED_LIBRARIES OFF)
#set( BOOST_USE_STATIC_LIBS ON )
#set( CMAKE_EXE_LINKER_FLAGS "-static")
add_definitions(-DTYRANT_OPTIMIZER_VERSION="${VERSION}" ${DEBUG} -DNQUEST)

#project(tuo)
file(GLOB TUO_SOURCES "c/*.cpp")
include_directories("h")
add_library(tuo_lib ${TUO_SOURCES})
set( TARGET_LINK_LIBS ${Boost_LIBRARIES} )
#if(OpenMP_CXX_FOUND)
#  set( TARGET_LINK_LIBS ${TARGET_LINK_LIBS}  OpenMP::OpenMP_CXX )
#    #target_link_libraries(${TARGET_NAME} PUBLIC OpenMP::OpenMP_CXX)
#endif()

target_link_libraries(tuo_lib ${TARGET_LINK_LIBS})

add_executable(tuo tyrant_optimize.cpp)
target_link_libraries(tuo tuo_lib)

#option(USE_OPENMP "Use OpenMP instead of Boost" OFF)
#if(USE_OPENMP)
## src: https://stackoverflow.com/a/12404666
#find_package(OpenMP REQUIRED)
#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()
#endif()
#find_package(Boost EXACT COMPONENTS system thread filesystem regex timer REQUIRED)


#install(TARGETS ${TGT} DESTINATION bin)
