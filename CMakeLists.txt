cmake_minimum_required(VERSION 3.26)

project(Solver VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
# set(CMAKE_BUILD_TYPE Debug)

configure_file(src/config/config.h.in ../src/include/config.h)

add_executable(solver src/main.cxx)

# ImageMagick
find_package(ImageMagick COMPONENTS Magick++)
target_link_libraries(solver PRIVATE ImageMagick::Magick++)

target_include_directories(solver PUBLIC src/include)
