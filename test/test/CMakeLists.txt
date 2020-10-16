set( test_SRC 
  test.cpp 
  test.test.cpp
)

set( TARGET_NAME test)
add_executable( ${TARGET_NAME} ../test.cpp ${too_test_SRC})

#target_link_libraries(${TARGET_NAME} PUBLIC too_lib)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/catch/ )



add_test(  NAME ${TARGET_NAME}  COMMAND ${TARGET_NAME} -o report.xml -r junit)


install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)