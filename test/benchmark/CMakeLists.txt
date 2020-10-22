set( too_test_SRC 
  #runner_cpu.test.cpp
  benchmark.test.cpp
)

include_directories(../../src/clh)
include_directories(${Boost_INCLUDE_DIR})

set( TARGET_NAME benchmark)
add_executable( ${TARGET_NAME} ../runner.cpp ${too_test_SRC})
set( TARGET_LINK_LIBS too_lib)
if(MPI_FOUND)
  set( TARGET_LINK_LIBS ${TARGET_LINK_LIBS} MPI::MPI_C)
#target_link_libraries(${TARGET_NAME} PUBLIC MPI::MPI_C too_lib)
else()
  set( TARGET_LINK_LIBS ${TARGET_LINK_LIBS} ${CMAKE_SOURCE_DIR}/external/msmpi/lib/x64/msmpi.lib )
#target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/msmpi/lib/x64/msmpi.lib PUBLIC too_lib)
endif()
if(OpenMP_CXX_FOUND)
  set( TARGET_LINK_LIBS ${TARGET_LINK_LIBS}  OpenMP::OpenMP_CXX )
    #target_link_libraries(${TARGET_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
target_link_libraries(${TARGET_NAME} PUBLIC ${TARGET_LINK_LIBS})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/catch/ )

#add_test(  NAME ${TARGET_NAME}  COMMAND ${TARGET_NAME} -o report.xml -r junit)
#catch_discover_tests(${TARGET_NAME})


install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/benchmark)
	
add_custom_command(
 TARGET ${TARGET_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/cl ${CMAKE_CURRENT_BINARY_DIR}/cl 
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/cl
)	

add_custom_command(
 TARGET ${TARGET_NAME} POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/clh ${CMAKE_CURRENT_BINARY_DIR}/clh 
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/clh
)

