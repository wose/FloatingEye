cmake_minimum_required(VERSION 3.1.0)

project(FloatingEye)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-std=c++11)

set(IRRIMGUI_BUILD_EXAMPLES off)
add_subdirectory(3rdparty)

add_subdirectory(src)
