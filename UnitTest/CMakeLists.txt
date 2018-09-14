############################################################################
#	AUTHOR: hedengfeng
#	DESCRIPTION:
############################################################################

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_BINARY_DIR}/lib)

link_directories("/usr/local/lib")

add_executable(test_bin main.cpp)

target_link_libraries(test_bin MsgCenter pthread dl)


