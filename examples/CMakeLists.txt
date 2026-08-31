# Guillaume examples

option(BUILD_EXAMPLES "Build the runnable examples" OFF)

if(BUILD_EXAMPLES)
    add_subdirectory(hello_world)
endif()
