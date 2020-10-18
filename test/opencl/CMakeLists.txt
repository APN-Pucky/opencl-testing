

add_executable( opencl_find_test opencl_find_test.cpp )
add_executable( opencl_run_test opencl_run_test.c )

target_link_libraries( opencl_find_test clew )
target_link_libraries( opencl_run_test clew )


install(TARGETS opencl_run_test RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
install(TARGETS opencl_find_test RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
	

add_test(NAME opencl_run_test  COMMAND opencl_run_test )
add_test(NAME opencl_find_test COMMAND opencl_find_test )