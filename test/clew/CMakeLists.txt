
add_executable( clewTest2 clewTest.cpp )
add_executable( clewTestC2 clewTestC.c )

target_link_libraries( clewTest2 clew )
target_link_libraries( clewTestC2 clew )

install(TARGETS clewTest2 
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

