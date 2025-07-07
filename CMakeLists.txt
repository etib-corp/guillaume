cmake_minimum_required(VERSION 3.10)
project(guillaume CXX)

set(CMAKE_CXX_STANDARD 20)

# Add all source files
file(GLOB SOURCES "sources/*.cpp")

# Add executable
add_executable(guillaume ${SOURCES})

# Include directories
include_directories(headers)

# Add subdirectories for external libraries
add_subdirectory(vendors/SDL)
add_subdirectory(vendors/harfbuzz)
add_subdirectory(vendors/SDL_ttf)

include_directories(${SDL3_INCLUDE_DIRS})

include_directories(${SDL3_TTF_INCLUDE_DIRS})

# Link libraries
if(TARGET SDL3::SDL3)
    target_link_libraries(guillaume SDL3::SDL3)
else()
    target_link_libraries(guillaume ${SDL3_LIBRARIES})
endif()



if(TARGET SDL3_ttf::SDL3_ttf)
    target_link_libraries(guillaume SDL3_ttf::SDL3_ttf)
else()
    target_link_libraries(guillaume ${SDL3_TTF_LIBRARIES})
endif()
