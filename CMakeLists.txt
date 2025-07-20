cmake_minimum_required(VERSION 3.10)
project(guillaume CXX)

set(CMAKE_CXX_STANDARD 20)

# Add all source files
file(GLOB_RECURSE SOURCES "sources/*.cpp")

# Add executable
add_executable(guillaume ${SOURCES})

# Include directories
include_directories(headers)

# Set options for external libraries
option(SDLTTF_VENDORED "Use vendored SDL_ttf" ON)
option(SDLTTF_PLUTOSVG "Use PlutoSVG for SDL_ttf" OFF)
option(SDLTTF_HARFBUZZ "Use HarfBuzz for SDL_ttf" OFF)
option(SDLTTF_FREETYPE "Use FreeType for SDL_ttf" ON)

# Fix for Windows build: Disable GameInput support to avoid missing gameinput.h header issues
# GameInput requires the Microsoft Gaming Input API headers which may not be available
# in all Windows SDK versions or CI environments
set(HAVE_GAMEINPUT_H FALSE CACHE BOOL "Force disable GameInput header detection")

# Add subdirectories for external libraries
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_ttf)

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