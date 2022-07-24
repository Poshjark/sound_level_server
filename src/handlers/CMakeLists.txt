set(HANDLERS_LIB_SOURCES command_handler.cpp media_commands_handler.cpp volume_handler.cpp)

add_library(handlers SHARED ${HANDLERS_LIB_SOURCES})

target_compile_definitions(handlers PUBLIC handlers)

set_target_properties(handlers PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_target_properties(handlers PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set_target_properties(handlers PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})