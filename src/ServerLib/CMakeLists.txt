cmake_minimum_required(VERSION 3.1.0)

project(ServerLib LANGUAGES CXX)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCES)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} UtilsLib)

target_include_directories(${PROJECT_NAME} 
	PUBLIC 
		${Boost_INCLUDE_DIRS} 
		${CMAKE_CURRENT_LIST_DIR}
		${CMAKE_CURRENT_LIST_DIR}/Utils
		)

#target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME})


set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})