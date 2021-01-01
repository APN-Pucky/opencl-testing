set( sim_test_SRC 
  sim_attack.test.cpp 
)

include_directories(../../src/too/clh)
include_directories(${Boost_INCLUDE_DIR})


set( TARGET_NAME sim_test)
add_executable( ${TARGET_NAME} ../test.cpp ${sim_test_SRC})

target_link_libraries(${TARGET_NAME} PUBLIC too_lib)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/catch/ )

#add_test(  NAME ${TARGET_NAME}  COMMAND ${TARGET_NAME} -o report.xml -r junit)
catch_discover_tests(${TARGET_NAME})


install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)


	
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

