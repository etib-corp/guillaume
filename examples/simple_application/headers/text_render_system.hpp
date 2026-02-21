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

#include <guillaume/ecs/component.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/ecs/system.hpp>
#include <guillaume/renderer.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/components/color.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/font.hpp>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>


namespace simple_application {
    class TextRenderSystem : public guillaume::ecs::System {
    private:
        guillaume::Renderer &_renderer;
    public:
        TextRenderSystem(guillaume::Renderer &renderer) : _renderer(renderer) { }

        ~TextRenderSystem(void) = default;

        void update(guillaume::ecs::ComponentRegistry &componentRegistry,
                    const guillaume::ecs::Entity::Identifier &entityIdentifier) override {
            if (!componentRegistry.hasComponent<guillaume::components::Text>(
                    entityIdentifier)) {
                return;
            }

            const auto &textComponent = componentRegistry.getComponent<guillaume::components::Text>(
                entityIdentifier);
            const auto &transformComponent = componentRegistry.getComponent<guillaume::components::Transform>(
                entityIdentifier);
            const auto &colorComponent = componentRegistry.getComponent<guillaume::components::Color>(
                entityIdentifier);

            guillaume::Text text(textComponent.getContent());
            text.setPosition(transformComponent.getPosition());
            text.setColor(colorComponent);
            text.setRotation(transformComponent.getRotation());

            guillaume::Font font("examples/simple_application/assets/font.otf", 24); //! hard coded font path, this need to change
            _renderer.drawText(text, font);
        }
    };
}