
include_directories(clh)
include_directories(${Boost_INCLUDE_DIR})
 
set( TARGET_NAME clhybrid_test)
add_executable( ${TARGET_NAME} clhybrid.cpp cl/9_tuo.cpp)

target_link_libraries( ${TARGET_NAME} clew ${Boost_LIBRARIES} )



	
install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
#install(DIRECTORY cl DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
#install(DIRECTORY clh DESTINATION ${CMAKE_INSTALL_PREFIX}/test)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
	
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


