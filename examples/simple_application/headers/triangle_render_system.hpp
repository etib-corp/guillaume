/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <guillaume/ecs/system.hpp>
#include <guillaume/renderer.hpp>
#include <guillaume/shapes/triangle.hpp>

#include "triangle_component.hpp"

namespace simple_application {

/**
 * @brief Simple system that renders a test triangle every frame.
 */
class TriangleRenderSystem : public guillaume::ecs::System {
  private:
    guillaume::Renderer &_renderer;             ///< Reference to the renderer
    guillaume::shapes::Triangle _triangle;      ///< Triangle shape to render

  public:
    /**
     * @brief Constructor for the TriangleRenderSystem class.
     * @param renderer The renderer to use for drawing triangles.
     */
    TriangleRenderSystem(guillaume::Renderer &renderer) : _renderer(renderer), _triangle() { }

    /**
     * @brief Virtual destructor for the TriangleRenderSystem class.
     */
    ~TriangleRenderSystem(void) = default;

    /**
     * @brief Update method called every frame to render triangles.
     * @param componentRegistry The component registry to access entity components.
     * @param entityIdentifier The identifier of the entity to process.
     */
    void update(guillaume::ecs::ComponentRegistry &componentRegistry,
                const guillaume::ecs::Entity::Identifier &entityIdentifier) override {
        bool isTriangle = componentRegistry.getComponent<simple_application::TriangleComponent>(
                entityIdentifier).isTriangleShape();
        if (!isTriangle) {
            return;
        }
        utility::math::Color color = componentRegistry.getComponent<guillaume::components::Color>(
                entityIdentifier);
        _triangle.setColor(
            {color.red(), color.green(), color.blue(), color.alpha()}
        );
        _triangle.setPosition(
            componentRegistry.getComponent<guillaume::components::Transform>(
                entityIdentifier)
                .getPosition()
        );
        _triangle.setScale(
            componentRegistry.getComponent<guillaume::components::Transform>(
                entityIdentifier)
                .getScale()
        );
        _renderer.drawTriangle(_triangle);
    }
};

} // namespace simple_application
