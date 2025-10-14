# SDL3 OpenGL Renderer example

# Find OpenGL
find_package(OpenGL REQUIRED)

# Fetch SDL3
include(FetchContent)
FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.24
)
FetchContent_MakeAvailable(SDL)

# Create the example with SDL3 and OpenGL dependencies
add_guillaume_example(sdl3_opengl_renderer
    LIBRARIES SDL3::SDL3 OpenGL::GL
)
