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

#include "guillaume/systems/hover.hpp"

#include "guillaume/components/relationship.hpp"
#include "guillaume/components/transform.hpp"

namespace {

// Helper function to calculate world position by traversing parent hierarchy
static typename guillaume::components::Transform::Position calculateWorldPosition(
    guillaume::ecs::ComponentRegistry &componentRegistry,
    const guillaume::ecs::Entity::Identifier &entityId) {
    
    const auto &transform =
        componentRegistry.getComponent<guillaume::components::Transform>(entityId);
    typename guillaume::components::Transform::Position worldPos = transform.getPosition();
    
    // Check if entity has a parent via Relationship component
    if (!componentRegistry.hasComponent<guillaume::components::Relationship>(entityId)) {
        return worldPos;
    }
    
    const auto &relationship =
        componentRegistry.getComponent<guillaume::components::Relationship>(entityId);
    const auto parentId = relationship.getParentIdentifier();
    
    // If no parent, return local position
    if (parentId == guillaume::ecs::Entity::InvalidIdentifier) {
        return worldPos;
    }
    
    // Recursively get parent's world position
    typename guillaume::components::Transform::Position parentWorldPos = calculateWorldPosition(componentRegistry, parentId);
    
    // Get parent's scale to apply to child's local offset
    const auto &parentTransform =
        componentRegistry.getComponent<guillaume::components::Transform>(parentId);
    const auto parentScale = parentTransform.getScale();
    
    // Combine: parent_world_pos + (local_pos * parent_scale)
    typename guillaume::components::Transform::Position scaledLocalPos;
    scaledLocalPos[0] = worldPos[0] * parentScale[0];
    scaledLocalPos[1] = worldPos[1] * parentScale[1];
    scaledLocalPos[2] = worldPos[2] * parentScale[2];
    
    typename guillaume::components::Transform::Position result;
    result[0] = parentWorldPos[0] + scaledLocalPos[0];
    result[1] = parentWorldPos[1] + scaledLocalPos[1];
    result[2] = parentWorldPos[2] + scaledLocalPos[2];
    
    return result;
}

// Helper function to convert screen-space mouse position to world-space position
// Assumes orthographic projection (camera projects from Z distance towards Z=0 plane)
static typename guillaume::components::Transform::Position convertMouseToWorldPosition(
    const typename guillaume::Renderer::Position &mousePos2D,
    const typename guillaume::Renderer::Position &cameraPos) {
    
    // For orthographic projection, we assume:
    // - Mouse position is in screen coordinates (x, y in pixel space)
    // - We need to transform it considering camera position
    // - Z component determines depth for 2D UI elements
    
    // Simple orthographic transformation:
    // Screen (x, y) maps directly to world (x, y) with camera offset
    typename guillaume::components::Transform::Position worldPos;
    worldPos[0] = mousePos2D[0] + cameraPos[0];
    worldPos[1] = mousePos2D[1] + cameraPos[1];
    worldPos[2] = cameraPos[2];  // Keep camera Z depth
    
    return worldPos;
}

} // namespace

namespace guillaume::systems {

Hover::Hover(event::EventBus &eventBus, Renderer &renderer)
    : _mouseMotionSubscriber(eventBus), _renderer(renderer) {}

void Hover::update(ecs::ComponentRegistry &componentRegistry,
                   const ecs::Entity::Identifier &identityIdentifier) {
    if (!_mouseMotionSubscriber.hasPendingEvents()) {
        // No new events, but we still need to check current mouse position
        // Use the renderer's stored mouse position
    } else {
        // Process new mouse motion events and update renderer's stored position
        auto event = _mouseMotionSubscriber.getNextEvent();
        if (event) {
            const auto mousePosition = event->getPosition();
            const auto cameraPos = _renderer.getCameraPosition();
            typename guillaume::components::Transform::Position mousePos3D;
            mousePos3D[0] = mousePosition[0];
            mousePos3D[1] = mousePosition[1];
            mousePos3D[2] = 0.0f;
            const auto worldMousePos = convertMouseToWorldPosition(mousePos3D, cameraPos);
            _renderer.setLastMousePosition(worldMousePos);
        }
    }

    const auto &bound =
        componentRegistry.getComponent<components::Bound>(identityIdentifier);
    auto &hover =
        componentRegistry.getComponent<components::Hover>(identityIdentifier);

    const auto worldMousePos = _renderer.getLastMousePosition();
    const auto worldPosition = calculateWorldPosition(componentRegistry, identityIdentifier);
    const auto size = bound.getSize();

    const float halfWidth = size[0] / 2.0f;
    const float halfHeight = size[1] / 2.0f;
    const bool isInside = worldMousePos[0] >= worldPosition[0] - halfWidth &&
                          worldMousePos[0] <= worldPosition[0] + halfWidth &&
                          worldMousePos[1] >= worldPosition[1] - halfHeight &&
                          worldMousePos[1] <= worldPosition[1] + halfHeight;

    if (isInside) {
        if (hover.getIsHovered()) {
            // Already hovering, no change
            return;
        }

        hover.setIsHovered(true);
        const auto onHover = hover.getOnHoverHandler();
        if (onHover) {
            onHover();
        }
        return;
    }

    if (!hover.getIsHovered()) {
        // Mouse is outside and wasn't hovering, no change
        return;
    }

    hover.setIsHovered(false);
    const auto onUnhover = hover.getOnUnhoverHandler();
    if (onUnhover) {
        onUnhover();
    }
}

} // namespace guillaume::systems