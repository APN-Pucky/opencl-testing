
include_directories(clh)
add_executable( clhybridTest clhybrid.cpp cl/9_tuo.cpp)

find_package(Boost EXACT COMPONENTS system thread filesystem regex timer REQUIRED)

target_link_libraries( clhybridTest clew ${Boost_LIBRARIES} )



install(TARGETS clhybridTest RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	