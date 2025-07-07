cmake_minimum_required(VERSION 3.10)
project(guillaume CXX)

set(CMAKE_CXX_STANDARD 20)

# Add all source files
file(GLOB SOURCES "sources/*.cpp")

# Add executable
add_executable(guillaume ${SOURCES})

# Include directories
include_directories(headers)

# Find SDL3
find_package(SDL3 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find SDL3_ttf
find_package(SDL3_ttf REQUIRED)
include_directories(${SDL3_TTF_INCLUDE_DIRS})

# Link libraries
if(TARGET SDL3::SDL3)
    target_link_libraries(guillaume SDL3::SDL3 SDL3_ttf::SDL3_ttf)
else()
    target_link_libraries(guillaume ${SDL3_LIBRARIES})
endif()
