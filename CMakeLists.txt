cmake_minimum_required(VERSION 3.12)
project(loggerBinary)

set(CMAKE_CXX_STANDARD 17)

# Set the include and source directories
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Collect all source files
file(GLOB_RECURSE SRCS ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE HDRS ${INC_DIR}/*.hpp)

# Add the executable target
add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})

# Set the include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIR})