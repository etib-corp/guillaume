/*
 Copyright (c) 2025 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "my_renderer.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/matrix.hpp>

#if defined(__APPLE__)
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

// Private helper methods implementation

void MyRenderer::initializeSDL(void) {
  if (SDL_Init(SDL_INIT_VIDEO) == false) {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    throw std::runtime_error("SDL_Init failed");
  }
}

void MyRenderer::createWindow(void) {
  try {
    _window = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>(
        SDL_CreateWindow("Guillaume", 800, 600,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE),
        SDL_DestroyWindow);
  } catch (std::exception &exception) {
    std::cerr << "Failed to create SDL window: " << exception.what()
              << std::endl;
    throw std::runtime_error("Failed to create SDL window");
  }
}

void MyRenderer::createGLContext(void) {
  _glContext =
      std::unique_ptr<SDL_GLContextState, bool (*)(SDL_GLContextState *)>(
          SDL_GL_CreateContext(_window.get()), SDL_GL_DestroyContext);
  if (_glContext == nullptr) {
    std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
              << std::endl;
    throw std::runtime_error("SDL_GL_CreateContext failed");
  }
  SDL_GL_MakeCurrent(_window.get(), _glContext.get());
  SDL_ShowWindow(_window.get());
  std::cout << "Renderer initialized with SDL and OpenGL." << std::endl;
}

GLuint MyRenderer::compileShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  return shader;
}

GLuint MyRenderer::createShaderProgram(const char* vertexSource, const char* fragmentSource) {
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

void MyRenderer::initializeShaders(void) {
  const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec4 aColor;
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 model;
    out vec4 vertexColor;
    void main() {
        vertexColor = aColor;
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    )";

  const char *fragmentShaderSource = R"(
        #version 330 core
        in vec4 vertexColor;
        out vec4 FragColor;
        void main() {
            FragColor = vertexColor;
        }
    )";

  shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void MyRenderer::setupBuffers(void) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
}

void MyRenderer::cleanupResources(void) {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  SDL_GL_DestroyContext(_glContext.release());
  SDL_DestroyWindow(_window.release());
  SDL_Quit();
}

void MyRenderer::setupViewProjectionMatrices(void) {
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), // Camera position
                               glm::vec3(0.0f, 0.0f, 0.0f), // Look at point
                               glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
  );
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                     (const GLfloat *)glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                     GL_FALSE, (const GLfloat *)glm::value_ptr(projection));
}

template<typename T>
void MyRenderer::applyTransformations(std::shared_ptr<T> primitive, std::vector<Vertex>& vertices) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(primitive->getTranslation()[0],
                                          primitive->getTranslation()[1],
                                          primitive->getTranslation()[2]));
  model = glm::rotate(model, primitive->getRotation()[0], glm::vec3(1, 0, 0));
  model = glm::rotate(model, primitive->getRotation()[1], glm::vec3(0, 1, 0));
  model = glm::rotate(model, primitive->getRotation()[2], glm::vec3(0, 0, 1));

  for (size_t i = 0; i < vertices.size(); i++) {
    glm::vec3 pos = model * glm::vec4(vertices[i].position.getX(),
                                      vertices[i].position.getY(),
                                      vertices[i].position.getZ(), 1.0f);
    vertices[i].position = Point(pos.x, pos.y, pos.z);
  }
  
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
                     GL_FALSE, glm::value_ptr(model));
}

void MyRenderer::bindAndDrawVertices(const std::vector<GLfloat>& vertices, GLenum drawMode, size_t count) {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);
  
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  
  // Color attribute
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  
  glDrawArrays(drawMode, 0, count);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

// Public methods implementation

MyRenderer::MyRenderer(void)
    : Renderer(), _window{nullptr, SDL_DestroyWindow},
      _glContext{nullptr, SDL_GL_DestroyContext} {
  initializeSDL();
  createWindow();
  createGLContext();
  initializeShaders();
  setupBuffers();
}

MyRenderer::~MyRenderer(void) {
  cleanupResources();
  std::cout << "Renderer destroyed." << std::endl;
}

void MyRenderer::clear(void) {
  std::cout << "Renderer cleared." << std::endl;
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);
  setupViewProjectionMatrices();
}

void MyRenderer::present(void) {
  std::cout << "Renderer presented." << std::endl;
  SDL_GL_SwapWindow(_window.get());
}

void MyRenderer::draw(std::shared_ptr<Primitive> primitive) {
  // Dispatch to specific draw methods based on primitive type
  if (auto text = std::dynamic_pointer_cast<Text>(primitive)) {
    drawText(text);
  } else if (auto rectangle = std::dynamic_pointer_cast<Rectangle>(primitive)) {
    drawRectangle(rectangle);
  } else if (auto triangle = std::dynamic_pointer_cast<Triangle>(primitive)) {
    drawTriangle(triangle);
  } else if (auto polygon = std::dynamic_pointer_cast<Polygon>(primitive)) {
    drawPolygon(polygon);
  }
}

void MyRenderer::drawText(std::shared_ptr<Text> text) {
  std::cout << "Drawing text: \"" << text->getContent()
            << "\" at 3D position (" << text->getPosition().getX() << ", "
            << text->getPosition().getY() << ", "
            << text->getPosition().getZ() << ")"
            << " rotation (" << text->getRotation().getX() << ", "
            << text->getRotation().getY() << ", "
            << text->getRotation().getZ() << ")"
            << " scale: " << text->getScale() << std::endl;
}

void MyRenderer::drawRectangle(std::shared_ptr<Rectangle> rectangle) {
  std::cout << "Drawing 3D rectangle with vertices:" << std::endl;

  auto packedVertices = rectangle->getVertices();
  applyTransformations(rectangle, packedVertices);

  std::vector<GLfloat> vertices;
  // Create two triangles for the rectangle
  std::vector<int> indices = {0, 1, 2, 2, 3, 0};
  for (int idx : indices) {
    vertices.push_back(packedVertices[idx].position.getX());
    vertices.push_back(packedVertices[idx].position.getY());
    vertices.push_back(packedVertices[idx].position.getZ());
    vertices.push_back(packedVertices[idx].color.r());
    vertices.push_back(packedVertices[idx].color.g());
    vertices.push_back(packedVertices[idx].color.b());
    vertices.push_back(packedVertices[idx].color.a());
  }

  glUseProgram(shaderProgram);
  bindAndDrawVertices(vertices, GL_TRIANGLES, 6);
  glUseProgram(0);
}

void MyRenderer::drawTriangle(std::shared_ptr<Triangle> triangle) {
  std::cout << "Drawing 3D triangle with vertices:" << std::endl;
  
  auto packedVertices = triangle->getVertices();
  if (packedVertices.size() < 3) {
    std::cout << "Triangle has less than 3 vertices (" << packedVertices.size() 
              << "), cannot draw." << std::endl;
    return;
  }
  
  applyTransformations(triangle, packedVertices);

  std::vector<GLfloat> vertices;
  for (int i = 0; i < 3; i++) {
    vertices.push_back(packedVertices[i].position.getX());
    vertices.push_back(packedVertices[i].position.getY());
    vertices.push_back(packedVertices[i].position.getZ());
    vertices.push_back(packedVertices[i].color.r());
    vertices.push_back(packedVertices[i].color.g());
    vertices.push_back(packedVertices[i].color.b());
    vertices.push_back(packedVertices[i].color.a());
  }

  glUseProgram(shaderProgram);
  bindAndDrawVertices(vertices, GL_TRIANGLES, 3);
  glUseProgram(0);
}

void MyRenderer::drawPolygon(std::shared_ptr<Polygon> polygon) {
  std::cout << "Drawing 3D polygon with vertices:" << std::endl;
  
  auto packedVertices = polygon->getVertices();
  if (packedVertices.size() < 3) {
    std::cout << "Polygon has less than 3 vertices, cannot draw." << std::endl;
    return;
  }

  applyTransformations(polygon, packedVertices);

  std::vector<GLfloat> vertices;
  for (const auto &vertex : packedVertices) {
    vertices.push_back(vertex.position.getX());
    vertices.push_back(vertex.position.getY());
    vertices.push_back(vertex.position.getZ());
    vertices.push_back(vertex.color.r());
    vertices.push_back(vertex.color.g());
    vertices.push_back(vertex.color.b());
    vertices.push_back(vertex.color.a());
    std::cout << "  Vertex at (" << vertex.position.getX() << ", "
              << vertex.position.getY() << ", " << vertex.position.getZ()
              << ") with color (" << vertex.color.r() << ", "
              << vertex.color.g() << ", " << vertex.color.b() << ", "
              << vertex.color.a() << ")" << std::endl;
  }

  glUseProgram(shaderProgram);
  bindAndDrawVertices(vertices, GL_TRIANGLE_FAN, packedVertices.size());
  glUseProgram(0);
}
