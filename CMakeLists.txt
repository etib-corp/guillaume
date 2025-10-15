cmake_minimum_required(VERSION 3.29)

project(guillaume
    VERSION 1.0.0
	DESCRIPTION "Guillaume"
	HOMEPAGE_URL "https://etib-corp.github.io/guillaume"
    LANGUAGES CXX
)

# Platform-specific configuration
if(WIN32)
    include(cmake/ConfigWindows.cmake)
elseif(APPLE)
    include(cmake/ConfigMacOS.cmake)
elseif(UNIX)
    include(cmake/ConfigLinux.cmake)
endif()


# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)

# Set the files directories
set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")
set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

# Add the include directories
include_directories(
    ${HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE SOURCES
    "${SOURCES_DIR}/*.cpp"
)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Set the target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Add the include directories to the target
target_include_directories(${PROJECT_NAME} PRIVATE
    ${HEADERS_DIR}
)

# For each library in the libraries directory, add it as a subdirectory
file(GLOB LIBRARIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries/*")
foreach(LIBRARY_DIR ${LIBRARIES_DIR})
    if (IS_DIRECTORY ${LIBRARY_DIR})
        add_subdirectory(${LIBRARY_DIR})
    else()
        message(STATUS "Skipping non-directory: ${LIBRARY_DIR}")
    endif()
endforeach()

# Link SDL3
set(SDL_TEST_LIBRARY OFF)

include(FetchContent)
FetchContent_Declare(
  SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.24
)
FetchContent_MakeAvailable(SDL)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Link OpenGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
else()
	message(FATAL_ERROR "OpenGL not found!")
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "GLEW not found!")
endif()

find_package(glm CONFIG REQUIRED)
if (glm_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
else()
    message(FATAL_ERROR "GLM not found!")
endif()

# Add the tests
enable_testing()
add_subdirectory(${TESTS_DIR})
