cmake_minimum_required(VERSION 3.26)

project(Solver VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

configure_file(src/config/config.h.in ../src/include/config.h)

add_executable(solver src/main.cxx)

target_include_directories(solver PUBLIC src/include)
