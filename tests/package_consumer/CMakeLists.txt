# Out-of-tree consumer of an installed Guillaume.
#
# This project is NOT part of the main build. It checks that
# `find_package(guillaume)` works against an install prefix:
#
#   cmake --install build --prefix <prefix>
#   cmake -S tests/package_consumer -B build/package_consumer \
#       -DCMAKE_PREFIX_PATH=<prefix>
#   cmake --build build/package_consumer
cmake_minimum_required(VERSION 3.10)

project(guillaume_package_consumer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(guillaume REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE guillaume::guillaume)
