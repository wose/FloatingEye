cmake_minimum_required(VERSION 3.1.0)

project(FloatingEye)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-std=c++14)

set(IRRIMGUI_BUILD_EXAMPLES off)
add_subdirectory(3rdparty)

add_subdirectory(src)

if (NOT NO_TESTS)
  include(CTest)
  add_subdirectory(3rdparty/googletest)
  enable_testing()
  add_subdirectory(tests)
else()
  message(STATUS "Tests disabled")
endif()
