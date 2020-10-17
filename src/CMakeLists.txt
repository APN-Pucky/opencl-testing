set( too_cxx_SRC too.cpp runner.cpp)
set( too_ocl_SRC cl/tuo.cpp cl/test.cpp)

include_directories(h)
include_directories(clh)
include_directories(${Boost_INCLUDE_DIR})
 
add_library( too_lib ${too_cxx_SRC} ${too_ocl_SRC})

target_link_libraries(${TARGET_NAME} PUBLIC MPI::MPI_C )
target_link_libraries( too_lib clew ${Boost_LIBRARIES} )

add_executable( too main.cpp )

target_link_libraries(too too_lib )

add_executable( told main.cpp too_old.cpp cl/tuo.cpp )
target_link_libraries( told clew ${Boost_LIBRARIES} )


install(TARGETS too RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS told RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS too_lib RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY cl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY clh DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	
