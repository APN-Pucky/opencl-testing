project(opencl-tyrant-optimizer)

cmake_minimum_required (VERSION 3.12)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  #set(CMAKE_BUILD_TYPE "Debug")
endif()


# CXX 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#OpenMP
find_package(OpenMP REQUIRED)


# STATIC
set( BUILD_SHARED_LIBRARIES OFF)

if(TOO_C_STATIC) # This is MinGW
  # -static-libgcc  -static 
  #set( CMAKE_EXE_LINKER_FLAGS " -static-libgcc  -static -static-libstdc++")
  #set( CMAKE_EXE_LINKER_FLAGS " -static  ")
  set( CMAKE_EXECUTABLE_SUFFIX ".exe")
endif()

if(TOO_BOOST_STATIC)
  set(Boost_USE_STATIC_LIBS   ON)
endif()

if( NOT BUILD_SHARED_LIBRARY )
  add_definitions(-Dclew_STATIC)
endif()


# BOOST 
#set(Boost_DEBUG 1)
find_package(Boost COMPONENTS filesystem system thread regex timer REQUIRED)


# MPI
find_package(MPI)
if (MPI_FOUND)
    add_definitions(-DTOO_MPI)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
else (MPI_FOUND)
    add_definitions(-DTOO_MPI)
    include_directories(SYSTEM external/msmpi/include)
    message(WARNING "This application will compile without MPI")
endif (MPI_FOUND)
# add this line only when you are using openmpi which has a different c++ bindings
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
add_definitions(-DOMPI_SKIP_MPICXX)
endif()


add_subdirectory(external)
include_directories(external/clew/include)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
if(TOO_COVERAGE)
include(CodeCoverage)
APPEND_COVERAGE_COMPILER_FLAGS()
endif()

include_directories(src/runner/h)
include_directories(src/too/h)
add_subdirectory(src)

#Tests
if(TOO_TEST_MPI)
    add_definitions(-DTOO_TEST_MPI)
endif()
if(TOO_TEST_OPENCL)
    add_definitions(-DTOO_TEST_OPENCL)
endif()


if(TOO_PERFORMANCE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -g")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -g")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -g")
endif()

include(CTest)
include(Catch)
enable_testing()
add_subdirectory(test)


if(TOO_COVERAGE)
SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME coverage                 
        EXECUTABLE ctest --output-on-failure #-j ${n_cores} # Executable in PROJECT_BINARY_DIR
        DEPENDENCIES
            opencl_find_test
            opencl_run_test
            clhybrid_test 
            test_test
            runner_test
            sim_test
            too_test
        EXCLUDE "/usr/*"
        EXCLUDE "${PROJECT_SOURCE_DIR}/external/*"
        )
endif()

