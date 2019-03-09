cmake_minimum_required (VERSION 2.6)
project (Rexp)

set (CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
include_directories ("${CMAKE_HOME_DIRECTORY}/src")
add_subdirectory ("${CMAKE_HOME_DIRECTORY}/src")
add_executable (Rexp Main.cpp)
target_link_libraries (Rexp RegExpLib)

# 安装
set (CMAKE_INSTALL_PREFIX ".")
install(TARGETS Rexp
  RUNTIME DESTINATION Debug)

