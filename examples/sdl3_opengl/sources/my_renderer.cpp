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

void MyRenderer::initializeTextRendering(void) {
  // Initialize FreeType library
  if (FT_Init_FreeType(&_ftLibrary)) {
    throw std::runtime_error("Could not initialize FreeType library");
  }

  // Load font - trying common system font paths

#if defined(__APPLE__)
  const char *fontPaths[] = {
      "/System/Library/Fonts/Supplemental/Arial.ttf",
      "/System/Library/Fonts/Supplemental/Helvetica.ttf"};
#endif
#if defined(_WIN32)
  const char *fontPaths[] = {"C:\\Windows\\Fonts\\arial.ttf",
                             "C:\\Windows\\Fonts\\calibri.ttf"};
#endif
#if defined(__linux__)
  const char *fontPaths[] = {
      "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
      "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
      "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"};
#endif

  bool fontLoaded = false;
  for (const char *fontPath : fontPaths) {
    if (FT_New_Face(_ftLibrary, fontPath, 0, &_ftFace) == 0) {
      fontLoaded = true;
      break;
    }
  }

  if (!fontLoaded) {
    FT_Done_FreeType(_ftLibrary);
    throw std::runtime_error("Failed to load font");
  }

  // Set font size (width=0 means dynamically calculated based on height)
  FT_Set_Pixel_Sizes(_ftFace, 0, 48);

  // Disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Load first 128 ASCII characters
  for (unsigned char c = 0; c < 128; c++) {
    // Load character glyph
    if (FT_Load_Char(_ftFace, c, FT_LOAD_RENDER)) {
      std::cerr << "Failed to load Glyph for character: " << c << std::endl;
      continue;
    }

    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _ftFace->glyph->bitmap.width,
                 _ftFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 _ftFace->glyph->bitmap.buffer);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Store character for later use
    Character character = {
        texture,
        glm::ivec2(_ftFace->glyph->bitmap.width, _ftFace->glyph->bitmap.rows),
        glm::ivec2(_ftFace->glyph->bitmap_left, _ftFace->glyph->bitmap_top),
        static_cast<GLuint>(_ftFace->glyph->advance.x)};
    _characters.insert(std::pair<char, Character>(c, character));
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  // Create text-specific shader program
  const char *textVertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
    out vec2 TexCoords;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
  )";

  const char *textFragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;
    uniform sampler2D text;
    uniform vec3 textColor;
    void main() {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
    }
  )";

  _textShaderProgram =
      createShaderProgram(textVertexShaderSource, textFragmentShaderSource);

  // Configure VAO/VBO for texture quads
  glGenVertexArrays(1, &_textVAO);
  glGenBuffers(1, &_textVBO);
  glBindVertexArray(_textVAO);
  glBindBuffer(GL_ARRAY_BUFFER, _textVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void MyRenderer::createGLContext(void) {
  if (!_window) {
    throw std::runtime_error("Window must be set before creating GL context");
  }

  _glContext =
      std::unique_ptr<SDL_GLContextState, bool (*)(SDL_GLContextState *)>(
          SDL_GL_CreateContext(_window), SDL_GL_DestroyContext);
  if (_glContext == nullptr) {
    std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
              << std::endl;
    throw std::runtime_error("SDL_GL_CreateContext failed");
  }
  SDL_GL_MakeCurrent(_window, _glContext.get());
  SDL_ShowWindow(_window);
}

GLuint MyRenderer::compileShader(GLenum type, const char *source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  return shader;
}

GLuint MyRenderer::createShaderProgram(const char *vertexSource,
                                       const char *fragmentSource) {
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

  // Clean up text rendering resources
  glDeleteVertexArrays(1, &_textVAO);
  glDeleteBuffers(1, &_textVBO);
  glDeleteProgram(_textShaderProgram);

  // Clean up character textures
  for (auto &pair : _characters) {
    glDeleteTextures(1, &pair.second.textureID);
  }
  _characters.clear();

  // Clean up FreeType
  if (_ftFace) {
    FT_Done_Face(_ftFace);
  }
  if (_ftLibrary) {
    FT_Done_FreeType(_ftLibrary);
  }

  if (_glContext) {
    SDL_GL_DestroyContext(_glContext.release());
  }
  // Window is not owned by renderer, so don't destroy it
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

template <typename T>
void MyRenderer::applyTransformations(std::shared_ptr<T> primitive,
                                      std::vector<Vertex> &vertices) {
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

  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));
}

void MyRenderer::bindAndDrawVertices(const std::vector<GLfloat> &vertices,
                                     GLenum drawMode, size_t count) {
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
    : Renderer(), _window(nullptr), _glContext{nullptr, SDL_GL_DestroyContext},
      _ftLibrary(nullptr), _ftFace(nullptr) {
  // Window will be set later via setWindow()
}

MyRenderer::MyRenderer(SDL_Window *window)
    : Renderer(), _window(window), _glContext{nullptr, SDL_GL_DestroyContext},
      _ftLibrary(nullptr), _ftFace(nullptr) {
  if (_window) {
    createGLContext();
    initializeShaders();
    initializeTextRendering();
    setupBuffers();
  }
}

void MyRenderer::setWindow(SDL_Window *window) {
  _window = window;
  if (_window && !_glContext) {
    createGLContext();
    initializeShaders();
    initializeTextRendering();
    setupBuffers();
  }
}

MyRenderer::~MyRenderer(void) { cleanupResources(); }

void MyRenderer::clear(void) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);
  setupViewProjectionMatrices();
}

void MyRenderer::present(void) { SDL_GL_SwapWindow(_window); }

void MyRenderer::drawText(std::shared_ptr<Text> text) {
  // Disable depth testing for text overlay
  glDisable(GL_DEPTH_TEST);

  // Enable blending for text transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Use text shader program
  glUseProgram(_textShaderProgram);

  // Set up orthographic projection for 2D text rendering
  int width, height;
  SDL_GetWindowSize(_window, &width, &height);
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f,
                                    static_cast<float>(height));
  glUniformMatrix4fv(glGetUniformLocation(_textShaderProgram, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));

  // Set text color to white for visibility on dark background
  glUniform3f(glGetUniformLocation(_textShaderProgram, "textColor"), 1.0f, 1.0f,
              1.0f);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(_textVAO);

  // Get text properties
  std::string content = text->getContent();
  // Adjust position - convert from component space to screen space
  // Add offset to make text more visible
  float x = text->getPosition().getX() + 50.0f; // Offset from left
  float y = static_cast<float>(height) - text->getPosition().getY() -
            100.0f; // Flip Y and offset from top
  float scale = text->getScale();

  // Iterate through all characters
  for (char c : content) {
    if (_characters.find(c) == _characters.end()) {
      continue; // Skip characters we don't have
    }

    Character ch = _characters[c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    // Update VBO for each character
    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};

    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);

    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, _textVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Advance cursor for next glyph (note that advance is number of 1/64
    // pixels)
    x += (ch.advance >> 6) *
         scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glUseProgram(0);
}

void MyRenderer::drawRectangle(std::shared_ptr<Rectangle> rectangle) {
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
  auto packedVertices = triangle->getVertices();
  if (packedVertices.size() < 3) {
    return;
  }

  applyTransformations(triangle, packedVertices);

  std::vector<GLfloat> vertices;
  for (int i = 0; i < 3; i++) {
    vertices.push_back(packedVertices[i].position.getX());
    vertices.push_back(packedVertices[i].position.getY());
    vertices.push_back(packedVertices[i].position.getZ());
    return;
  }

  glUseProgram(shaderProgram);
  bindAndDrawVertices(vertices, GL_TRIANGLES, 3);
  glUseProgram(0);
}

void MyRenderer::drawPolygon(std::shared_ptr<Polygon> polygon) {
  auto packedVertices = polygon->getVertices();
  if (packedVertices.size() < 3) {
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
  }

  glUseProgram(shaderProgram);
  bindAndDrawVertices(vertices, GL_TRIANGLE_FAN, packedVertices.size());
  glUseProgram(0);
}
