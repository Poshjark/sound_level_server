set(FRAME_LIB_SOURCES framereader.cpp framewriter.cpp abstractframe.cpp)

add_library(frame SHARED ${FRAME_LIB_SOURCES})

target_compile_definitions(frame PUBLIC frame)

set_target_properties(frame PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_target_properties(frame PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set_target_properties(frame PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})