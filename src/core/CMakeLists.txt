cmake_minimum_required(VERSION 3.1.0)

project(soundlevelserver LANGUAGES CXX)

aux_source_directory( ${CMAKE_CURRENT_LIST_DIR} SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} handlers serverlib ${Boost_LIBRARIES})