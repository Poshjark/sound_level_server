add_library(serverlib SHARED server.cpp)

target_link_libraries(serverlib ${Boost_LIBRARIES} handlers)

target_compile_definitions(serverlib PUBLIC serverlib)

set_target_properties(serverlib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_target_properties(serverlib PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})