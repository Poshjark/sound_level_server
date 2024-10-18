cmake_minimum_required(VERSION 3.1.0)

project(BetaServer LANGUAGES CXX)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ServerLib UtilsLib)

target_include_directories(${PROJECT_NAME} 
	PUBLIC 
		${Boost_INCLUDE_DIRS} 
		${CMAKE_CURRENT_LIST_DIR}
		${CMAKE_CURRENT_LIST_DIR}/Utils
		)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME})


set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})