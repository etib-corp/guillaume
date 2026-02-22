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

#include "guillaume/systems/render.hpp"

#include "guillaume/font.hpp"
#include "guillaume/text.hpp"

#include "guillaume/components/click.hpp"
#include "guillaume/components/focus.hpp"
#include "guillaume/components/text.hpp"

#include "guillaume/shapes/rectangle.hpp"

#include <filesystem>
#include <optional>
#include <vector>

namespace {

std::optional<std::string> resolveDefaultFontPath(void) {
    static const std::vector<std::string> candidates = {
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/Avenir.ttc",
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    };

    for (const auto &candidate : candidates) {
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }

    return std::nullopt;
}

} // namespace

namespace guillaume::systems {

void Render::update(ecs::ComponentRegistry &componentRegistry,
                    const ecs::Entity::Identifier &identityIdentifier) {
    const auto &transform =
        componentRegistry.getComponent<components::Transform>(
            identityIdentifier);
    const auto &bound =
        componentRegistry.getComponent<components::Bound>(identityIdentifier);

    shapes::Rectangle rectangle;
    rectangle.setPosition(transform.getPosition());
    rectangle.setRotation(transform.getRotation());
    rectangle.setScale(transform.getScale());
    rectangle.setSize({bound.getSize()[0], bound.getSize()[1]});

    if (componentRegistry.hasComponent<components::Click>(identityIdentifier)) {
        rectangle.setColor({46, 88, 170, 255});
    } else if (componentRegistry.hasComponent<components::Focus>(
                   identityIdentifier)) {
        rectangle.setColor({60, 60, 60, 255});
    } else {
        rectangle.setColor({110, 110, 110, 255});
    }

    _renderer.drawRectangle(rectangle);

    if (!componentRegistry.hasComponent<components::Text>(identityIdentifier)) {
        return;
    }

    const auto &textComponent =
        componentRegistry.getComponent<components::Text>(identityIdentifier);
    if (textComponent.getContent().empty()) {
        return;
    }

    static const auto fontPath = resolveDefaultFontPath();
    if (!fontPath) {
        return;
    }

    Text text(textComponent.getContent());
    text.setPosition(transform.getPosition());
    text.setRotation(transform.getRotation());

    Font font(*fontPath, 24);
    _renderer.drawText(text, font);
}

} // namespace guillaume::systems