# Evan-backed example (macOS / Linux / Windows + GLFW).
#
# Fetches the Evan rendering engine and uses it as the Engine behind a
# Guillaume Application, drawing a container, a button and a 3D model.

include(FetchContent)

# Evan builds exactly one platform + one backend (see evan/CMakeLists.txt).
# Select the platform matching the host automatically.
set(BUILD_FOR_GLFW ON CACHE BOOL "" FORCE)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(BUILD_FOR_MACOS ON CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(BUILD_FOR_LINUX ON CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(BUILD_FOR_WINDOWS ON CACHE BOOL "" FORCE)
else()
    message(FATAL_ERROR "The evan example supports macOS, Linux and Windows")
endif()

FetchContent_Declare(evan
    GIT_REPOSITORY https://github.com/etib-corp/evan.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

# Add Evan as a subdirectory excluded from the default build target so its own
# benchmarks/examples are not compiled; the `evan` library is still built on
# demand because this example links against it. Testing is disabled while Evan
# is configured so its CTest registration does not leak into Guillaume's suite.
set(EVAN_BUILD_TESTING ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(evan)
if(NOT evan_POPULATED)
    FetchContent_Populate(evan)
    add_subdirectory(${evan_SOURCE_DIR} ${evan_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(BUILD_TESTING ${EVAN_BUILD_TESTING} CACHE BOOL "" FORCE)

add_executable(guillaume_evan_example
    main.cpp
)

target_link_libraries(guillaume_evan_example PRIVATE
    guillaume
    evan
)

# Copy runtime assets next to the built binary.
add_custom_command(TARGET guillaume_evan_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:guillaume_evan_example>
)
