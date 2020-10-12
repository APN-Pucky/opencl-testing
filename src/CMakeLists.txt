set( too_cxx_SRC too.cpp )
set( too_ocl_SRC cl/9_tuo.cpp)

include_directories(h)
include_directories(clh)
include_directories(${Boost_INCLUDE_DIR})
 
add_library( too_lib ${too_cxx_SRC} ${too_ocl_SRC})

target_link_libraries( too_lib clew ${Boost_LIBRARIES} )

add_executable( too main.cpp )

target_link_libraries(too too_lib )



install(TARGETS too RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS too_lib RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY cl RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY clh RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	
