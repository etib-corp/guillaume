cmake_minimum_required(VERSION 3.10)

# Fetch SDL3 and SDL3_ttf
include(FetchContent)
FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.0
)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
    SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
)
FetchContent_MakeAvailable(SDL_ttf)

# Set project name
set(EXAMPLE_NAME simple_application)

# Collect all header files
file(GLOB_RECURSE EXAMPLE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp")

# Collect all source files
file(GLOB_RECURSE EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")

# Create executable
add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS})

# Include directories
target_include_directories(${EXAMPLE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${PROJECT_SOURCE_DIR}/headers
)

# Link against the main library
target_link_libraries(${EXAMPLE_NAME} PRIVATE ${PROJECT_NAME})

# Link against SDL3 and SDL3_ttf
target_link_libraries(${EXAMPLE_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# Set C++ standard
set_property(TARGET ${EXAMPLE_NAME} PROPERTY CXX_STANDARD 20)
