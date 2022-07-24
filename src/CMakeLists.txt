add_executable(soundlevelserver core/main.cpp)

add_subdirectory(handlers)
add_subdirectory(server)
add_subdirectory(client)

target_link_libraries(soundlevelserver handlers serverlib ${Boost_LIBRARIES})

#set_target_properties(soundlevelserver PROPERTIES EXECUTABLE_OUTPUT_PATH ../../../../../../bin)