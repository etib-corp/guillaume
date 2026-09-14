# hello_world uses the Evan Vulkan engine, so it needs a platform/backend
# selection. Require the caller to pass BUILD_FOR_GLFW and exactly one
# BUILD_FOR_* platform flag (mirroring Evan's own validation).
set(PLATFORM_OPTION_COUNT 0)
if(BUILD_FOR_ANDROID)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()
if(BUILD_FOR_LINUX)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()
if(BUILD_FOR_WINDOWS)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()
if(BUILD_FOR_MACOS)
    math(EXPR PLATFORM_OPTION_COUNT "${PLATFORM_OPTION_COUNT} + 1")
endif()

if(NOT BUILD_FOR_GLFW)
    message(FATAL_ERROR "hello_world requires -DBUILD_FOR_GLFW=ON")
endif()
if(NOT PLATFORM_OPTION_COUNT EQUAL 1)
    message(FATAL_ERROR "hello_world requires exactly one platform option (BUILD_FOR_LINUX, BUILD_FOR_MACOS, or BUILD_FOR_WINDOWS)")
endif()

# Fetch the Evan engine (Vulkan backend). Its tests are disabled so they do
# not leak into this example build.
include(FetchContent)
set(HELLO_WORLD_BUILD_TESTING ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(evan
    GIT_REPOSITORY https://github.com/etib-corp/evan.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(evan)

set(BUILD_TESTING ${HELLO_WORLD_BUILD_TESTING} CACHE BOOL "" FORCE)

add_executable(guillaume_hello_world
    main.cpp
)

target_link_libraries(guillaume_hello_world PRIVATE
    guillaume
    evan
)
