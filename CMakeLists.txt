cmake_minimum_required(VERSION 3.10)

project(guillaume
    VERSION 1.0.0
    DESCRIPTION "A C++ UI framework designed for modern desktop and extended reality (XR) applications."
    HOMEPAGE_URL "https://etib-corp.github.io/guillaume"
    LANGUAGES CXX
)

# Fetch external dependencies
include(FetchContent)

FetchContent_Declare(
    utility
    GIT_REPOSITORY https://github.com/etib-corp/utility
    GIT_TAG main
)

FetchContent_MakeAvailable(utility)

# Include common functions
include(cmake/CommonFunctions.cmake)

# Platform-specific configuration
if(WIN32)
    include(cmake/ConfigWindows.cmake)
elseif(APPLE)
    include(cmake/ConfigMacOS.cmake)
elseif(UNIX)
    include(cmake/ConfigLinux.cmake)
endif()

# Set global C++ standard and properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set the files directories
set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE SOURCES
    "${SOURCES_DIR}/*.cpp"
)

# Create the main library
add_library(${PROJECT_NAME} ${SOURCES})

# Set common target properties
set_guillaume_target_properties(${PROJECT_NAME})

# Link private dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        utility
)

# Configure public interface
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Optional components
option(BUILD_TESTING "Build the tests" ON)
option(BUILD_DOCS "Build documentation" ON)
option(BUILD_EXAMPLES "Build the examples" ON)

if(BUILD_TESTING)
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
else()
    message(STATUS "Skipping docs...")
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
else()
    message(STATUS "Skipping examples...")
endif()
