cmake_minimum_required(VERSION 3.1.0)

project (TestClient LANGUAGES CXX)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR} SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})