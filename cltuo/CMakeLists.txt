
add_executable( clewTTest clewTest.cpp )
add_executable( clewTTestC clewTestC.c )

target_link_libraries( clewTTest clew )
target_link_libraries( clewTTestC clew )

install(TARGETS clewTTestC 
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

