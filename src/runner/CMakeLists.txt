
#set( runner_cxx_SRC 
#runner_id.cpp  
#mpi_util.cpp)
file(GLOB_RECURSE runner_cxx_SRC "c/*.cpp")

include_directories(h)
include_directories(${Boost_INCLUDE_DIR})
 
add_library( runner_lib ${runner_cxx_SRC})

set( TARGET_LINK_LIBS ${Boost_LIBRARIES} clew)
if(MPI_FOUND)
  set( TARGET_LINK_LIBS ${TARGET_LINK_LIBS} MPI::MPI_C)
#target_link_libraries(${TARGET_NAME} PUBLIC MPI::MPI_C too_lib)
else()
  set( TARGET_LINK_LIBS ${TARGET_LINK_LIBS} ${CMAKE_SOURCE_DIR}/external/msmpi/lib/x64/msmpi.lib )
#target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/external/msmpi/lib/x64/msmpi.lib PUBLIC too_lib)
endif()
if(OpenMP_CXX_FOUND)
  set( TARGET_LINK_LIBS ${TARGET_LINK_LIBS}  OpenMP::OpenMP_CXX )
    #target_link_libraries(${TARGET_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
target_link_libraries(runner_lib ${TARGET_LINK_LIBS})

#add_executable( too main.cpp )

#target_link_libraries(too too_lib )

#add_executable( told main.cpp too_old.cpp cl/tuo.cpp )
#target_link_libraries( told clew ${Boost_LIBRARIES} )


#install(TARGETS too RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS runner_lib RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
#install(DIRECTORY cl DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
#install(DIRECTORY clh DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

#TODO move to respective area
#install(DIRECTORY cl DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
#install(DIRECTORY clh DESTINATION ${CMAKE_INSTALL_PREFIX}/test)

#install(DIRECTORY cl DESTINATION ${CMAKE_INSTALL_PREFIX}/benchmark)
#install(DIRECTORY clh DESTINATION ${CMAKE_INSTALL_PREFIX}/benchmark)
	
#add_custom_command(
# TARGET too_lib POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/too/cl ${CMAKE_CURRENT_BINARY_DIR}/cl 
# DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/cl
#)	
#
#add_custom_command(
# TARGET too_lib POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/too/clh ${CMAKE_CURRENT_BINARY_DIR}/clh 
# DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/clh
#)
