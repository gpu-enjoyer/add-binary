cmake_minimum_required(VERSION 3.16)
project(add-binary)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
 
# set(CMAKE_CXX_STANDARD ...)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(add-binary add-binary.cpp Number.cpp Number.h conf.h)
install(TARGETS add-binary)
