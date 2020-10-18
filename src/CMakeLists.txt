set( too_cxx_SRC too.cpp runner.cpp)
set( too_ocl_SRC cl/tuo.cpp cl/test.cpp)

include_directories(h)
include_directories(clh)
include_directories(${Boost_INCLUDE_DIR})
 
add_library( too_lib ${too_cxx_SRC} ${too_ocl_SRC})

target_link_libraries( too_lib clew ${Boost_LIBRARIES} MPI::MPI_C )

add_executable( too main.cpp )

target_link_libraries(too too_lib )

add_executable( told main.cpp too_old.cpp cl/tuo.cpp )
target_link_libraries( told clew ${Boost_LIBRARIES} )


install(TARGETS too RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS told RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS too_lib RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY cl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY clh DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	
add_custom_command(
 TARGET too_lib POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/cl ${CMAKE_CURRENT_BINARY_DIR}/cl 
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/cl
)	

add_custom_command(
 TARGET too_lib POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/clh ${CMAKE_CURRENT_BINARY_DIR}/clh 
 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/clh
)