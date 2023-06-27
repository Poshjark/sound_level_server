cmake_minimum_required(VERSION 3.1.0)

project(handlers LANGUAGES CXX)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCES)

add_library(handlers SHARED ${SOURCES})

target_compile_definitions(handlers PUBLIC handlers)

set_target_properties(handlers PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_target_properties(handlers PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set_target_properties(handlers PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})