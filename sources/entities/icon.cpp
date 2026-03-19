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

#include "guillaume/entities/icon.hpp"

namespace guillaume::entities {

Icon::Icon::Builder::Builder(void) : ecs::LeafEntityBuilder() {}

Icon::Builder::~Builder(void) {}

Icon::Director::Director(ecs::ComponentRegistry &componentRegistry)
    : ecs::EntityDirector(componentRegistry) {}

Icon::Director::~Director(void) {}

void Icon::normalRender(ecs::ComponentRegistry &registry,
                        const ecs::Entity::Identifier &id, Renderer &renderer) {

    renderer.drawVertices({
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f},
         {255, 0, 0, 255}},
        {{1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {1.0f, 0.0f},
         {0, 255, 0, 255}},
        {{0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 1.0f},
         {0, 0, 255, 255}},
    });
}

void Icon::hoveredRender(ecs::ComponentRegistry &registry,
                         const ecs::Entity::Identifier &id,
                         Renderer &renderer) {
    renderer.drawVertices({
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f},
         {255, 255, 0, 255}},
        {{1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {1.0f, 0.0f},
         {255, 0, 255, 255}},
        {{0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 1.0f},
         {0, 255, 255, 255}},
    });
}

void Icon::clickedRender(ecs::ComponentRegistry &registry,
                         const ecs::Entity::Identifier &id,
                         Renderer &renderer) {
    renderer.drawVertices({
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f},
         {255, 255, 255, 255}},
        {{1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {1.0f, 0.0f},
         {255, 255, 255, 255}},
        {{0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 1.0f},
         {255, 255, 255, 255}},
    });
}

void Icon::activeRender(ecs::ComponentRegistry &registry,
                        const ecs::Entity::Identifier &id, Renderer &renderer) {
    renderer.drawVertices({
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f},
         {255, 255, 255, 255}},
        {{1.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {1.0f, 0.0f},
         {255, 255, 255, 255}},
        {{0.0f, 1.0f, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 1.0f},
         {255, 255, 255, 255}},
    });
}

Icon::Icon(ecs::ComponentRegistry &registry)
    : ecs::LeafEntityFiller<components::Transform, components::Bound,
                            components::Icon, components::Render>(registry) {
    registry.getComponent<components::Transform>(getIdentifier())
        .setPosition({0.0f, 0.0f, 0.0f})
        .setRotation({0.0f, 0.0f, 0.0f})
        .setScale({1.0f, 1.0f, 1.0f});

    registry.getComponent<components::Bound>(getIdentifier())
        .setSize({20.0f, 20.0f});

    registry.getComponent<components::Icon>(getIdentifier());

    registry.getComponent<components::Render>(getIdentifier())
        .setNormalHandler([this](ecs::ComponentRegistry &registry,
                                 const ecs::Entity::Identifier &id,
                                 Renderer &renderer) {
            normalRender(registry, id, renderer);
        })
        .setHoveredHandler([this](ecs::ComponentRegistry &registry,
                                  const ecs::Entity::Identifier &id,
                                  Renderer &renderer) {
            hoveredRender(registry, id, renderer);
        })
        .setClickedHandler([this](ecs::ComponentRegistry &registry,
                                  const ecs::Entity::Identifier &id,
                                  Renderer &renderer) {
            clickedRender(registry, id, renderer);
        })
        .setActiveHandler([this](ecs::ComponentRegistry &registry,
                                 const ecs::Entity::Identifier &id,
                                 Renderer &renderer) {
            activeRender(registry, id, renderer);
        });
}

Icon::~Icon() {}

} // namespace guillaume::entities