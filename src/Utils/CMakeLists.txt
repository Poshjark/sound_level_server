cmake_minimum_required(VERSION 3.1.0)

project(UtilsLib LANGUAGES CXX)

find_package(Boost COMPONENTS thread)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCES)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

target_include_directories(${PROJECT_NAME} 
	PUBLIC 
		${Boost_INCLUDE_DIRS} 
		${CMAKE_CURRENT_LIST_DIR}
		)

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})