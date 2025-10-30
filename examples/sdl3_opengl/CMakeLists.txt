# SDL3 OpenGL example

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)

# Create the example with SDL3 and OpenGL dependencies
add_guillaume_example(sdl3_opengl
    LIBRARIES SDL3::SDL3 OpenGL::GL GLEW::GLEW glm::glm Freetype::Freetype
)
