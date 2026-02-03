/*
 Copyright (c) 2026 ETIB Corporation

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

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <utility/math/color.hpp>
#include <utility/math/rectangle.hpp>
#include <utility/math/vector.hpp>

#include <guillaume/font.hpp>
#include <guillaume/renderer.hpp>
#include <guillaume/text.hpp>

namespace simple_application {

class Renderer : public guillaume::Renderer {
  private:
    SDL_Window *window;
    SDL_GPUDevice *device;
    SDL_GPUGraphicsPipeline *pipeline;
    SDL_GPUBuffer *vertexBuffer;
    SDL_GPUBuffer *indexBuffer;
    SDL_GPUBuffer *uniformBuffer;
    SDL_GPUTexture *depthTexture;
    utility::math::Color<float> clearColor;

    // 3D rendering matrices
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;

    void initializeGPU(void);
    void createPipeline(void);
    void createDepthBuffer(Uint32 width, Uint32 height);
    void shutdownGPU(void);
    void updateMatrices(Uint32 width, Uint32 height);

  public:
    Renderer(void);
    ~Renderer(void);

    void clear(void) override;
    void present(void) override;

    void setDrawColor(const utility::math::Color<uint8_t> &color);
    void setViewMatrix(const glm::mat4 &view);
    void setProjectionMatrix(const glm::mat4 &projection);
    void setModelMatrix(const glm::mat4 &model);
};

} // namespace simple_application
