
include_directories(clh)
include_directories(${Boost_INCLUDE_DIR})
 
add_executable( clhybridTest clhybrid.cpp cl/9_tuo.cpp)

target_link_libraries( clhybridTest clew ${Boost_LIBRARIES} )



install(TARGETS clhybridTest RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	