############################################################################
#	AUTHOR: hedengfeng
#	DESCRIPTION:
############################################################################
cmake_minimum_required (VERSION 3.5)

#MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
#MESSAGE(STATUS "This is SOURCE dir "${PROJECT_SOURCE_DIR})

PROJECT(EventBus)

# 目标程序的运行环境
set(CMAKE_SYSTEM_NAME Linux)

SET(CMAKE_C_COMPILER "/usr/bin/x86_64-linux-gnu-gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/x86_64-linux-gnu-g++")
set(STRIP_TOOL /usr/bin/x86_64-linux-gnu-strip)

# 交叉编译时搜索头文件和库的路径
set(CMAKE_FIND_ROOT_PATH /usr)

# 仅在目标机环境下搜索头文件和库
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_SKIP_RPATH TRUE)

# 查看链接时使用的命令, 以便于检查链接了哪些库, 用了哪些优化选项
set(CMAKE_VERBOSE_MAKEFILE TRUE)

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_BINARY_DIR}/lib)

link_directories("/usr/local/lib")


#SET(CMAKE_CXX_FLAGS "-g -Wall")
#SET(CMAKE_C_FLAGS "-g -Wall")

ADD_DEFINITIONS(
-std=c++11 -Wno-deprecated-declarations -fpermissive -g -ggdb -Wno-psabi -rdynamic -ldl
)

#ADD_DEFINITIONS(
#-std=c++11 -Wno-deprecated-declarations -fpermissive -O2
#)


# 源文件列表
set(SRCS EventTarget.cpp EventBus.cpp EventClient.cpp MThread.cpp)
#set(SRCS EventBus.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_BINARY_DIR}/lib)

add_library(EventBus STATIC ${SRCS})

target_link_libraries(EventBus zmq pthread)

# 单元测试目录
add_subdirectory(UnitTest)


# 交叉编译时, 所有库使用动态链接，使用静态链接会报错，原因未知
#set_target_properties(test_bin PROPERTIES LINK_FLAGS "-rdynamic")

#set_target_properties(test_bin PROPERTIES COMPILE_FLAGS "-fstack-check")

#target_link_libraries(test_bin  -Wl,-Bdynamic -lpthread -ldl)

#target_link_libraries(test_bin -lpthread)



#set(CMAKE_INSTALL_PREFIX /home/LT/)

#INSTALL(TARGETS LocEngine
#RUNTIME DESTINATION bin
#)


