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

#include "guillaume/entities/button.hpp"

namespace guillaume::entities {

Button::Button::Builder::Builder(void) : ecs::NodeEntityBuilder() {}

Button::Builder::~Builder(void) {}

Button::Director::Director(ecs::ComponentRegistry &componentRegistry)
    : ecs::EntityDirector(componentRegistry) {}

Button::Director::~Director(void) {}

void Button::hoverHandler(void) {}

void Button::unHoverHandler(void) {}

void Button::leftClickHandler(
    utility::event::MouseMotionEvent::MousePosition mousePosition) {}

void Button::normalRender(ecs::ComponentRegistry &registry,
                          const ecs::Entity::Identifier &id,
                          Renderer &renderer) {

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

void Button::hoveredRender(ecs::ComponentRegistry &registry,
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

void Button::clickedRender(ecs::ComponentRegistry &registry,
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

void Button::activeRender(ecs::ComponentRegistry &registry,
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

Button::Button(ecs::ComponentRegistry &registry, TogleState toggleState,
               ColorStyle colorStyle, Shape shape, Size size,
               MorphState morphState)
    : ecs::NodeEntityFiller<components::Transform, components::Bound,
                            components::Hover, components::Click,
                            components::Render>(

          registry) {
    registry.getComponent<components::Transform>(getIdentifier())
        .setPosition({0.0f, 0.0f, 0.0f})
        .setRotation({0.0f, 0.0f, 0.0f})
        .setScale({1.0f, 1.0f, 1.0f});

    registry.getComponent<components::Bound>(getIdentifier())
        .setSize({20.0f, 20.0f});

    registry.getComponent<components::Hover>(getIdentifier())
        .setOnHoverHandler([this]() { this->hoverHandler(); })
        .setOnUnhoverHandler([this]() { this->unHoverHandler(); });

    registry.getComponent<components::Click>(getIdentifier())
        .setOnClickHandler(
            utility::event::MouseButtonEvent::MouseButton::LEFT,
            [this](
                utility::event::MouseMotionEvent::MousePosition mousePosition) {
                this->leftClickHandler(mousePosition);
            });

    registry.getComponent<components::Render>(getIdentifier())
        .setNormalHandler([this](ecs::ComponentRegistry &registry,
                                 const ecs::Entity::Identifier &id,
                                 Renderer &renderer) {
            this->normalRender(registry, id, renderer);
        })
        .setHoveredHandler([this](ecs::ComponentRegistry &registry,
                                  const ecs::Entity::Identifier &id,
                                  Renderer &renderer) {
            this->hoveredRender(registry, id, renderer);
        })
        .setClickedHandler([this](ecs::ComponentRegistry &registry,
                                  const ecs::Entity::Identifier &id,
                                  Renderer &renderer) {
            this->clickedRender(registry, id, renderer);
        })
        .setActiveHandler([this](ecs::ComponentRegistry &registry,
                                 const ecs::Entity::Identifier &id,
                                 Renderer &renderer) {
            this->activeRender(registry, id, renderer);
        });
}

Button::~Button() {}

} // namespace guillaume::entities