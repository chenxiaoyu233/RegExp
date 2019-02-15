cmake_minimum_required (VERSION 2.6)
project (Rexp)

aux_source_directory (. source_list)
add_executable (Rexp ${source_list})

# 安装
set (CMAKE_INSTALL_PREFIX ".")
install(TARGETS Rexp
  RUNTIME DESTINATION Debug)

