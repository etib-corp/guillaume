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
#include <guillaume/shapes/rectangle.hpp>

#include "rectangle_component.hpp"
#include "triangle_component.hpp"

namespace simple_application {

/**
 * @brief Simple system that applies rotation to shapes every frame.
 */
class RotationSystem : public guillaume::ecs::System {
  public:
    /**
     * @brief Default constructor for the RotationSystem class.
     */
    RotationSystem(void) = default;

    /**
     * @brief Virtual destructor for the RotationSystem class.
     */
    ~RotationSystem(void) = default;

    /**
     * @brief Update method called every frame to rotate shapes.
     * @param componentRegistry The component registry to access entity components.
     * @param entityIdentifier The identifier of the entity to process.
     */
void update(guillaume::ecs::ComponentRegistry &componentRegistry,
                        const guillaume::ecs::Entity::Identifier &entityIdentifier) override {
        auto getRandomAxis = []() { return rand() % 2; };

        if (componentRegistry.hasComponent<simple_application::TriangleComponent>(
                        entityIdentifier)) {
                auto &triangleComponent = componentRegistry.getComponent<simple_application::TriangleComponent>(
                                entityIdentifier);
                if (triangleComponent.isTriangleShape()) {
                        auto rotation = componentRegistry.getComponent<guillaume::components::Transform>(
                                        entityIdentifier).getRotation();
                        rotation[getRandomAxis()] += 1.0f;
                        componentRegistry.getComponent<guillaume::components::Transform>(
                                        entityIdentifier).setRotation(rotation);
                }
        } else if (componentRegistry.hasComponent<simple_application::RectangleComponent>(
                        entityIdentifier)) {
                auto rectangleComponent = componentRegistry.getComponent<simple_application::RectangleComponent>(
                                entityIdentifier);
                if (rectangleComponent.isRectangleShape()) {
                        auto rotation = componentRegistry.getComponent<guillaume::components::Transform>(
                                        entityIdentifier).getRotation();
                        rotation[getRandomAxis()] += 1.0f;
                        componentRegistry.getComponent<guillaume::components::Transform>(
                                        entityIdentifier).setRotation(rotation);
                }
        } else if (componentRegistry.hasComponent<guillaume::components::Text>(
                        entityIdentifier)) {
                auto textComponent = componentRegistry.getComponent<guillaume::components::Text>(
                                entityIdentifier);
                auto rotation = componentRegistry.getComponent<guillaume::components::Transform>(
                                entityIdentifier).getRotation();
                rotation[getRandomAxis()] += 1.0f;
                componentRegistry.getComponent<guillaume::components::Transform>(
                                entityIdentifier).setRotation(rotation);
        }
    }
};

} // namespace simple_application