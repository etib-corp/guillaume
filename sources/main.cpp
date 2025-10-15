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

#ifndef TESTING

#include <iostream>
#include <memory>
#include <typeinfo>

#include <SDL3/SDL.h>
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
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#else
#include <GL/gl.h>
#include <GL/glew.h>
#endif

#include "application.hpp"
#include "button.hpp"
#include "component.hpp"
#include "container.hpp"
#include "label.hpp"
#include "primitives/polygon.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/triangle.hpp"
#include "renderer.hpp"

class MyRenderer : public Renderer {
private:
  std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> _window{
      nullptr, SDL_DestroyWindow};
  std::unique_ptr<SDL_GLContextState, bool (*)(SDL_GLContextState *)>
      _glContext{nullptr, SDL_GL_DestroyContext};
  GLuint VBO, VAO;
  GLuint shaderProgram;

public:
  MyRenderer(void) : Renderer() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
      std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
      throw std::runtime_error("SDL_Init failed");
    }

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    // SDL_GL_CONTEXT_PROFILE_CORE);

    try {
      _window = std::unique_ptr<SDL_Window, void (*)(SDL_Window *)>(
          SDL_CreateWindow("Guillaume", 800, 600,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE),
          SDL_DestroyWindow);
    } catch (std::exception &execption) {
      std::cerr << "Failed to create SDL window: " << execption.what()
                << std::endl;
      throw std::runtime_error("Failed to create SDL window");
    }

    _glContext =
        std::unique_ptr<SDL_GLContextState, bool (*)(SDL_GLContextState *)>(
            SDL_GL_CreateContext(_window.get()), SDL_GL_DestroyContext);
    if (_glContext == nullptr) {
      std::cerr << "Failed to create OpenGL context: " << SDL_GetError()
                << std::endl;
      throw std::runtime_error("SDL_GL_CreateContext failed");
    }
    std::cout << "Renderer initialized with SDL and OpenGL." << std::endl;
    SDL_GL_MakeCurrent(_window.get(), _glContext.get());
    SDL_ShowWindow(_window.get());

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

    // Create, compile, and attach shaders (error checking omitted)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  }

  ~MyRenderer(void) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    SDL_GL_DestroyContext(_glContext.release());
    SDL_DestroyWindow(_window.release());
    SDL_Quit();

    std::cout << "Renderer destroyed." << std::endl;
  }

  void clear(void) override {
    std::cout << "Renderer cleared." << std::endl;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
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

  void present(void) override {
    std::cout << "Renderer presented." << std::endl;
    SDL_GL_SwapWindow(_window.get());
  }

  // Override specific draw methods for each primitive type
  void drawText(std::shared_ptr<Text> text) override {
    std::cout << "Drawing text: \"" << text->getContent()
              << "\" at 3D position (" << text->getPosition().getX() << ", "
              << text->getPosition().getY() << ", "
              << text->getPosition().getZ() << ")"
              << " rotation (" << text->getRotation().getX() << ", "
              << text->getRotation().getY() << ", "
              << text->getRotation().getZ() << ")"
              << " scale: " << text->getScale() << std::endl;
  }

  void drawRectangle(std::shared_ptr<Rectangle> rectangle) override {
    std::cout << "Drawing 3D rectangle with vertices:" << std::endl;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(rectangle->getTranslation()[0],
                                            rectangle->getTranslation()[1],
                                            rectangle->getTranslation()[2]));
    model = glm::rotate(model, rectangle->getRotation()[0], glm::vec3(1, 0, 0));
    model = glm::rotate(model, rectangle->getRotation()[1], glm::vec3(0, 1, 0));
    model = glm::rotate(model, rectangle->getRotation()[2], glm::vec3(0, 0, 1));

    // Get the packed vertices
    auto packedVertices = rectangle->getVertices();
    for (int i = 0; i < 4; i++) {
      glm::vec3 pos =
          model * glm::vec4(packedVertices[i].position.getX(),
                            packedVertices[i].position.getY(),
                            packedVertices[i].position.getZ(), 1.0f);
      packedVertices[i].position = Point(pos.x, pos.y, pos.z);
    }
    GLfloat vertices[] = {
        // positions
        packedVertices[0].position.getX(), packedVertices[0].position.getY(),
        packedVertices[0].position.getZ(), packedVertices[0].color.r(),
        packedVertices[0].color.g(),       packedVertices[0].color.b(),
        packedVertices[0].color.a(), // Top-left
        packedVertices[1].position.getX(), packedVertices[1].position.getY(),
        packedVertices[1].position.getZ(), packedVertices[1].color.r(),
        packedVertices[1].color.g(),       packedVertices[1].color.b(),
        packedVertices[1].color.a(), // Top-right
        packedVertices[2].position.getX(), packedVertices[2].position.getY(),
        packedVertices[2].position.getZ(), packedVertices[2].color.r(),
        packedVertices[2].color.g(),       packedVertices[2].color.b(),
        packedVertices[2].color.a(), // Bottom-right
        packedVertices[2].position.getX(), packedVertices[2].position.getY(),
        packedVertices[2].position.getZ(), packedVertices[2].color.r(),
        packedVertices[2].color.g(),       packedVertices[2].color.b(),
        packedVertices[2].color.a(), // Bottom-right
        packedVertices[3].position.getX(), packedVertices[3].position.getY(),
        packedVertices[3].position.getZ(), packedVertices[3].color.r(),
        packedVertices[3].color.g(),       packedVertices[3].color.b(),
        packedVertices[3].color.a(), // Bottom-left
        packedVertices[0].position.getX(), packedVertices[0].position.getY(),
        packedVertices[0].position.getZ(), packedVertices[0].color.r(),
        packedVertices[0].color.g(),       packedVertices[0].color.b(),
        packedVertices[0].color.a() // Top-left
    };
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
                       GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
  }

  void drawTriangle(std::shared_ptr<Triangle> triangle) override {
    std::cout << "Drawing 3D triangle with vertices:" << std::endl;
    auto packedVertices = triangle->getVertices();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(triangle->getTranslation()[0],
                                            triangle->getTranslation()[1],
                                            triangle->getTranslation()[2]));
    model = glm::rotate(model, triangle->getRotation()[0], glm::vec3(1, 0, 0));
    model = glm::rotate(model, triangle->getRotation()[1], glm::vec3(0, 1, 0));
    model = glm::rotate(model, triangle->getRotation()[2], glm::vec3(0, 0, 1));

    // Get the packed vertices

    for (int i = 0; i < 4; i++) {
      glm::vec3 pos =
          model * glm::vec4(packedVertices[i].position.getX(),
                            packedVertices[i].position.getY(),
                            packedVertices[i].position.getZ(), 1.0f);
      packedVertices[i].position = Point(pos.x, pos.y, pos.z);
    }
    GLfloat vertices[] = {
        // positions
        packedVertices[0].position.getX(), packedVertices[0].position.getY(),
        packedVertices[0].position.getZ(), packedVertices[0].color.r(),
        packedVertices[0].color.g(),       packedVertices[0].color.b(),
        packedVertices[0].color.a(), // Top-left
        packedVertices[1].position.getX(), packedVertices[1].position.getY(),
        packedVertices[1].position.getZ(), packedVertices[1].color.r(),
        packedVertices[1].color.g(),       packedVertices[1].color.b(),
        packedVertices[1].color.a(), // Top-right
        packedVertices[2].position.getX(), packedVertices[2].position.getY(),
        packedVertices[2].position.getZ(), packedVertices[2].color.r(),
        packedVertices[2].color.g(),       packedVertices[2].color.b(),
        packedVertices[2].color.a(), // Bottom-right
    };
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
  }

  void drawPolygon(std::shared_ptr<Polygon> polygon) override {
    std::cout << "Drawing 3D polygon with vertices:" << std::endl;
    auto packedVertices = polygon->getVertices();
    if (packedVertices.size() < 3) {
      std::cout << "Polygon has less than 3 vertices, cannot draw."
                << std::endl;
      return;
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(polygon->getTranslation()[0],
                                            polygon->getTranslation()[1],
                                            polygon->getTranslation()[2]));
    model = glm::rotate(model, polygon->getRotation()[0], glm::vec3(1, 0, 0));
    model = glm::rotate(model, polygon->getRotation()[1], glm::vec3(0, 1, 0));
    model = glm::rotate(model, polygon->getRotation()[2], glm::vec3(0, 0, 1));

    // Get the packed vertices

    for (int i = 0; i < packedVertices.size(); i++) {
      glm::vec3 pos =
          model * glm::vec4(packedVertices[i].position.getX(),
                            packedVertices[i].position.getY(),
                            packedVertices[i].position.getZ(), 1.0f);
      packedVertices[i].position = Point(pos.x, pos.y, pos.z);
    }
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
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, packedVertices.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
  }
};

int main(int argc, char *argv[], char *envp[]) {
  // Create the application with a renderer
  std::unique_ptr<Application<MyRenderer>> application = nullptr;

  try {
    application = std::make_unique<Application<MyRenderer>>();
  } catch (std::exception &exception) {
    std::cerr << "Failed to create Application: " << exception.what()
              << std::endl;
    return EXIT_FAILURE;
  }

  // Get the root container
  auto root = application->getRoot();

  // Create a nested component tree
  auto header = std::make_shared<Container>();
  auto content = std::make_shared<Container>();
  auto footer = std::make_shared<Container>();

  // Header components
  auto titleLabel = std::make_shared<Label>("Guillaume UI Demo");
  auto subtitleLabel = std::make_shared<Label>("A tiny reactive hierarchy");

  // Content components
  auto infoLabel = std::make_shared<Label>("Clicks: 0");
  // auto actionButton = std::make_shared<Button>("Increment");
  // auto resetButton = std::make_shared<Button>("Reset");

  // Footer components
  auto statusLabel = std::make_shared<Label>("Status: idle");

  // Compose tree
  // header->addChild(titleLabel);
  // header->addChild(subtitleLabel);

  // content->addChild(infoLabel);
  // // content->addChild(actionButton);
  // // content->addChild(resetButton);

  // footer->addChild(statusLabel);

  // root->addChild(header);
  // root->addChild(content);
  // root->addChild(footer);

  // Local counter state simulated via Label + State inside it
  int counter = 0;

  // Wire up button behaviors by mutating label states
  // actionButton->setOnClick([&]() {
  //   counter += 1;
  //   infoLabel->setText("Clicks: " + std::to_string(counter));
  //   statusLabel->setText("Status: incremented");
  // });

  // resetButton->setOnClick([&]() {
  //   counter = 0;
  //   infoLabel->setText("Clicks: 0");
  //   statusLabel->setText("Status: reset");
  // });

  // Add 3D primitives to demonstrate all draw methods with depth
  auto trianglePrimitive = std::make_shared<Triangle>();
  auto trianglePos = Vector<float, 3>({-0.3f, -0.5f, 0.0f});
  trianglePrimitive->setTranslation(trianglePos);

  std::vector<Vertex> polygonVertices = {
      Vertex(Point(0.5f, 0.0f, 0.0f), Color(0.0f, 1.0f, 0.0f)), // Bottom-left
      Vertex(Point(0.25f, 0.433f, 0.0f),
             Color(0.0f, 1.0f, 0.0f)), // Bottom-right
      Vertex(Point(-0.25f, 0.433f, 0.0f), Color(0.0f, 1.0f, 0.0f)), // Right
      Vertex(Point(-0.5f, 0.0f, 0.0f), Color(0.0f, 1.0f, 0.0f)),    // Top-right
      Vertex(Point(-0.25f, -0.433f, 0.0f), Color(0.0f, 1.0f, 0.0f)), // Top-left
      Vertex(Point(0.25f, -0.433f, 0.0f), Color(0.0f, 1.0f, 0.0f)),  // Left
  };

  auto polygonPrimitive = std::make_shared<Polygon>(polygonVertices);
  auto polygonPos = Vector<float, 3>({-0.5f, 0.5f, 0.0f});
  polygonPrimitive->setTranslation(polygonPos);

  // Create a 3D rectangle with different Z coordinates
  auto rect3D = std::make_shared<Rectangle>();
  auto rect3DPos = Vector<float, 3>({0.5f, 0.0f, 0.0f});
  rect3D->setTranslation(rect3DPos);

  // Create text with 3D positioning, rotation, and scaling
  auto text3D =
      std::make_shared<Text>("3D Text Demo", Point(100, 50, 25),
                             Point(0.2f, 0.5f, 0.1f), // rotation in radians
                             1.5f                     // scale factor
      );

  root->addPrimitive(trianglePrimitive);
  root->addPrimitive(polygonPrimitive);
  root->addPrimitive(rect3D);
  // root->addPrimitive(text3D);

  // Initial render pass
  std::cout << "\n-- Initial run --" << std::endl;
  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }
    application->update();
  }

  // // Simulate some user interactions and updates
  // std::cout << "\n-- Simulate 3 clicks --" << std::endl;
  // for (int i = 0; i < 3; ++i) {
  //   // Dispatch a synthetic click by directly calling the handler
  //   // actionButton->onEvent(Event("click", actionButton));
  //   // Trigger application update and redraw
  //   application->update();
  // }

  // std::cout << "\n-- Reset --" << std::endl;
  // // resetButton->onEvent(Event("click", resetButton));
  // application->update();

  // std::cout << "\n-- One more click --" << std::endl;
  // // actionButton->onEvent(Event("click", actionButton));
  // application->update();

  return EXIT_SUCCESS;
}

#endif
