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

#ifndef MY_RENDERER_HPP
#define MY_RENDERER_HPP

#include <map>
#include <memory>

#include <SDL3/SDL.h>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glew.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include "primitives/polygon.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/triangle.hpp"
#include "renderer.hpp"

// Structure to hold character information
struct Character {
  GLuint textureID;   // ID handle of the glyph texture
  glm::ivec2 size;    // Size of glyph
  glm::ivec2 bearing; // Offset from baseline to left/top of glyph
  GLuint advance;     // Horizontal offset to advance to next glyph
};

class MyRenderer : public Renderer {
private:
  SDL_Window *_window; // Non-owning pointer - window managed externally
  std::unique_ptr<SDL_GLContextState, bool (*)(SDL_GLContextState *)>
      _glContext;
  GLuint VBO, VAO;
  GLuint shaderProgram;

  // FreeType members
  FT_Library _ftLibrary;
  FT_Face _ftFace;
  std::map<char, Character> _characters;
  GLuint _textVAO, _textVBO;
  GLuint _textShaderProgram;

  // Private helper methods
  void createGLContext(void);
  void initializeShaders(void);
  void initializeTextRendering(void);
  void setupBuffers(void);
  void cleanupResources(void);

  GLuint compileShader(GLenum type, const char *source);
  GLuint createShaderProgram(const char *vertexSource,
                             const char *fragmentSource);
  void setupViewProjectionMatrices(void);

  template <typename T>
  void applyTransformations(std::shared_ptr<T> primitive,
                            std::vector<Vertex> &vertices);

  void bindAndDrawVertices(const std::vector<GLfloat> &vertices,
                           GLenum drawMode, size_t count);

public:
  MyRenderer(void);
  MyRenderer(SDL_Window *window);
  ~MyRenderer(void);

  // Set the window after construction
  void setWindow(SDL_Window *window);

  void clear(void) override;
  void present(void) override;

  // Override specific draw methods for each primitive type
  void drawText(std::shared_ptr<Text> text) override;
  void drawRectangle(std::shared_ptr<Rectangle> rectangle) override;
  void drawTriangle(std::shared_ptr<Triangle> triangle) override;
  void drawPolygon(std::shared_ptr<Polygon> polygon) override;

  // Accessor for SDL window
  SDL_Window *getWindow(void) const { return _window; }
};

#endif // MY_RENDERER_HPP
