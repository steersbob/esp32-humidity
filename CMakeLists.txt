# The following four lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_RULE_MESSAGES OFF)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(humidity)

idf_build_set_property(COMPILE_OPTIONS "-fdiagnostics-color=always" APPEND)
