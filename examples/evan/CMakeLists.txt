# Evan-backed example (Linux + GLFW).
#
# Fetches the Evan rendering engine and uses it as the Engine behind a
# Guillaume Application, drawing a rectangle, a text label and a 3D model.

include(FetchContent)

# Evan builds exactly one platform + one backend (see evan/CMakeLists.txt).
set(BUILD_FOR_LINUX ON CACHE BOOL "" FORCE)
set(BUILD_FOR_GLFW ON CACHE BOOL "" FORCE)

FetchContent_Declare(evan
    GIT_REPOSITORY https://github.com/etib-corp/evan.git
    GIT_TAG feat/engine-interface
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

# Add Evan as a subdirectory excluded from the default build target so its own
# tests/benchmarks/examples are not compiled; the `evan` library is still built
# on demand because this example links against it.
FetchContent_GetProperties(evan)
if(NOT evan_POPULATED)
    FetchContent_Populate(evan)
    add_subdirectory(${evan_SOURCE_DIR} ${evan_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

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
