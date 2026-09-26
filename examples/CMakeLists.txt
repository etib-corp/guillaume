# Guillaume examples

option(BUILD_EXAMPLES "Build the runnable examples" OFF)

if(BUILD_EXAMPLES)
    add_subdirectory(hello_world)
    if(CMAKE_SYSTEM_NAME MATCHES "Darwin|Linux|Windows")
        add_subdirectory(evan)
    endif()
endif()
