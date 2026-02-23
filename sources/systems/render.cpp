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
#include "guillaume/components/relationship.hpp"
#include "guillaume/components/transform.hpp"

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

// Helper function to calculate world position by traversing parent hierarchy
static typename components::Transform::Position calculateWorldPosition(
    ecs::ComponentRegistry &componentRegistry,
    const ecs::Entity::Identifier &entityId) {
    
    const auto &transform =
        componentRegistry.getComponent<components::Transform>(entityId);
    typename components::Transform::Position worldPos = transform.getPosition();
    
    // Check if entity has a parent via Relationship component
    if (!componentRegistry.hasComponent<components::Relationship>(entityId)) {
        return worldPos;
    }
    
    const auto &relationship =
        componentRegistry.getComponent<components::Relationship>(entityId);
    const auto parentId = relationship.getParentIdentifier();
    
    // If no parent, return local position
    if (parentId == ecs::Entity::InvalidIdentifier) {
        return worldPos;
    }
    
    // Recursively get parent's world position
    typename components::Transform::Position parentWorldPos = calculateWorldPosition(componentRegistry, parentId);
    
    // Get parent's scale to apply to child's local offset
    const auto &parentTransform =
        componentRegistry.getComponent<components::Transform>(parentId);
    const auto parentScale = parentTransform.getScale();
    
    // Combine: parent_world_pos + (local_pos * parent_scale)
    typename components::Transform::Position scaledLocalPos;
    scaledLocalPos[0] = worldPos[0] * parentScale[0];
    scaledLocalPos[1] = worldPos[1] * parentScale[1];
    scaledLocalPos[2] = worldPos[2] * parentScale[2];
    
    typename components::Transform::Position result;
    result[0] = parentWorldPos[0] + scaledLocalPos[0];
    result[1] = parentWorldPos[1] + scaledLocalPos[1];
    result[2] = parentWorldPos[2] + scaledLocalPos[2];
    
    return result;
}

void Render::update(ecs::ComponentRegistry &componentRegistry,
                    const ecs::Entity::Identifier &identityIdentifier) {
    const auto &transform =
        componentRegistry.getComponent<components::Transform>(
            identityIdentifier);
    const auto &bound =
        componentRegistry.getComponent<components::Bound>(identityIdentifier);
    
    // Calculate world position considering parent hierarchy
    auto worldPosition = calculateWorldPosition(componentRegistry, identityIdentifier);

    shapes::Rectangle rectangle;
    rectangle.setPosition(worldPosition);
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
    text.setPosition(worldPosition);
    text.setRotation(transform.getRotation());

    Font font(*fontPath, 24);
    _renderer.drawText(text, font);
}

} // namespace guillaume::systems