cmake_minimum_required(VERSION 3.10)

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


# Set C++ standard
set_property(TARGET ${EXAMPLE_NAME} PROPERTY CXX_STANDARD 20)
