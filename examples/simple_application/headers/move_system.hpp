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
#include <guillaume/shapes/triangle.hpp>

#include "rectangle_component.hpp"
#include "triangle_component.hpp"

#include <guillaume/event/event_subscriber.hpp>
#include <utility/event/event.hpp>
#include <utility/event/mouse_motion_event.hpp>

namespace simple_application {
    class MoveSystem : public guillaume::ecs::System {
    private:
        guillaume::event::EventSubscriber<utility::event::MouseMotionEvent> _mouseMotionSubscriber;

    public:
        /**
         * @brief Default constructor for the MoveSystem class.
         * @param eventBus The event bus to subscribe to.
         */
        MoveSystem(guillaume::event::EventBus &eventBus) :
            _mouseMotionSubscriber(eventBus) {
            }

        /**
         * @brief Virtual destructor for the MoveSystem class.
         */
        ~MoveSystem(void) = default;

        /**
         * @brief Update method called every frame to move shapes based on mouse motion.
         * @param componentRegistry The component registry to access entity components.
         * @param entityIdentifier The identifier of the entity to process.
         */
        void update(guillaume::ecs::ComponentRegistry &componentRegistry,
                    const guillaume::ecs::Entity::Identifier &entityIdentifier) override {
            getLogger().debug("##############################################     Processing MoveSystem for entity " + std::to_string(entityIdentifier));
            if (_mouseMotionSubscriber.hasPendingEvents()) {
                getLogger().debug("##############################################     got event" + std::to_string(entityIdentifier));
                auto event = _mouseMotionSubscriber.getNextEvent();
                auto position = event->getPosition();
                if (componentRegistry.hasComponent<simple_application::TriangleComponent>(
                        entityIdentifier)) {
                    auto &triangleComponent = componentRegistry.getComponent<simple_application::TriangleComponent>(
                            entityIdentifier);
                    if (triangleComponent.isTriangleShape()) {
                        componentRegistry.getComponent<guillaume::components::Transform>(
                                entityIdentifier).setPosition({position[0], position[1], 0.0f});
                    }
                // } else if (componentRegistry.hasComponent<simple_application::RectangleComponent>(
                //         entityIdentifier)) {
                //     auto rectangleComponent = componentRegistry.getComponent<simple_application::RectangleComponent>(
                //             entityIdentifier);
                //     if (rectangleComponent.isRectangleShape()) {
                //         componentRegistry.getComponent<guillaume::components::Transform>(
                //                 entityIdentifier).setPosition({position[0], position[1], 0.0f});
                //     }
                }
            }
        }
    };
}