cmake_minimum_required(VERSION 3.1.0)

project(serverlib LANGUAGES CXX)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCES)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} handlers)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})