set( too_test_SRC 
  runner_test.cpp
  #runner_cpu.test.cpp
  runner_all.test.cpp
  runner_mpi.test.cpp
  runner_opencl.test.cpp
)

include_directories(../../src/clh)
include_directories(${Boost_INCLUDE_DIR})

set( TARGET_NAME runner_test)
add_executable( ${TARGET_NAME} ../test.cpp ${too_test_SRC})
if(MPI_FOUND)
target_link_libraries(${TARGET_NAME} PUBLIC MPI::MPI_C too_lib)
else()
target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/msmpi/lib/x64/msmpi.lib PUBLIC too_lib)
endif()
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/catch/ )

add_test(  NAME ${TARGET_NAME}  COMMAND ${TARGET_NAME} -o report.xml -r junit)


install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)