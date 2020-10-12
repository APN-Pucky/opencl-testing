

add_executable( openclTest openclTest.cpp )
add_executable( openclTestC openclTestC.c )

target_link_libraries( openclTest clew )
target_link_libraries( openclTestC clew )


install(TARGETS openclTestC RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
install(TARGETS openclTest RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
	
