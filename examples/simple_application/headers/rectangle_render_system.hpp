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
#include <guillaume/shapes/rectangle.hpp>
#include "rectangle_component.hpp"

namespace simple_application {

/**
 * @brief Simple system that renders a test rectangle every frame.
 */
class RectangleRenderSystem : public guillaume::ecs::System {
  private:
    guillaume::Renderer &_renderer;
    guillaume::shapes::Rectangle _rectangle;

    public:
    /**
     * @brief Default constructor for the RectangleRenderSystem class.
     * @param renderer The renderer to use for drawing rectangles.
     */
    RectangleRenderSystem(guillaume::Renderer &renderer) : _renderer(renderer), _rectangle() { }

    /**
     * @brief Virtual destructor for the RectangleRenderSystem class.
     */
    ~RectangleRenderSystem(void) = default;

    /**
     * @brief Update method called every frame to render rectangles.
     * @param componentRegistry The component registry to access entity components.
     * @param entityIdentifier The identifier of the entity to process.
     */
    void update(guillaume::ecs::ComponentRegistry &componentRegistry,
                const guillaume::ecs::Entity::Identifier &entityIdentifier) override {
        bool isRectangle = componentRegistry.getComponent<simple_application::RectangleComponent>(
                entityIdentifier).isRectangleShape();
        if (!isRectangle) {
            return;
        }

        utility::math::Color color = componentRegistry.getComponent<guillaume::components::Color>(
                entityIdentifier);
        utility::math::Vector<std::float_t, 3> bound = componentRegistry.getComponent<guillaume::components::Bound>(
                entityIdentifier).getSize();
        _rectangle.setPosition(
            componentRegistry.getComponent<guillaume::components::Transform>(
                entityIdentifier)
                .getPosition()
        );
        _rectangle.setSize(
            {bound[0], bound[1]}
        );
        _rectangle.setColor(
            {color.red(), color.green(), color.blue(), color.alpha()}
        );
        _rectangle.setScale(
            componentRegistry.getComponent<guillaume::components::Transform>(
                entityIdentifier)
                .getScale()
        );
        _renderer.drawRectangle(_rectangle);
    }
};

} // namespace simple_application