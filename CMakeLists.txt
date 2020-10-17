project(opencl-tyrant-optimizer)

cmake_minimum_required (VERSION 3.0)

# CXX + STATIC
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set( BUILD_SHARED_LIBRARIES OFF)

if(TOO_C_STATIC) # This is MinGW
  set( CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
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
find_package(Boost COMPONENTS filesystem REQUIRED)


find_package(MPI REQUIRED)
if (MPI_FOUND)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
else (MPI_FOUND)
    message(SEND_ERROR "This application cannot compile without MPI")
endif (MPI_FOUND)
# add this line only when you are using openmpi which has a different c++ bindings
#add_definitions(-DOMPI_SKIP_MPICXX)



add_subdirectory(external)
include_directories(external/clew/include)

if(TOO_COVERAGE)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
include(CodeCoverage)
APPEND_COVERAGE_COMPILER_FLAGS()
endif()

include_directories(src/h)
add_subdirectory(src)

#Tests
enable_testing()
add_subdirectory(test)

if(TOO_COVERAGE)
SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME coverage                 
        EXECUTABLE ctest #-j ${n_cores} # Executable in PROJECT_BINARY_DIR
        DEPENDENCIES
            test_test
            too_test
            runner_test
        EXCLUDE "/usr/*"
        EXCLUDE "${PROJECT_SOURCE_DIR}/external/*"
        )
endif()

