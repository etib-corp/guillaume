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
#include "guillaume/systems/detail/world_transform_utils.hpp"

#include <cmath>
#include <optional>

namespace {

/**
 * @brief Intersect a world-space ray with the UI plane.
 * @param ray Input world-space ray.
 * @param planeZ Target plane depth.
 * @return Intersection point when it exists, otherwise nullopt.
 */
static std::optional<typename guillaume::components::Transform::Position>
intersectRayWithUiPlane(const typename guillaume::Renderer::Ray &ray,
                        const float planeZ = 0.0f) {
    const auto origin = ray.getOrigin();
    const auto direction = ray.getDirection();

    constexpr float epsilon = 1.0e-6f;
    if (std::abs(direction[2]) <= epsilon) {
        return std::nullopt;
    }

    const float distanceParameter = (planeZ - origin[2]) / direction[2];
    if (distanceParameter < 0.0f) {
        return std::nullopt;
    }

    return ray.pointAt(distanceParameter);
}

/**
 * @brief Test whether a world point lies inside a rotated/scaled entity bound.
 * @param point Point in world space.
 * @param entityCenter Entity center in world space.
 * @param boundSize Entity unscaled bound size.
 * @param entityScale Entity world scale.
 * @param entityRotation Entity world rotation.
 * @return True when the point is inside projected bounds.
 */
static bool isPointInsideEntityBounds(
    const typename guillaume::components::Transform::Position &point,
    const typename guillaume::components::Transform::Position &entityCenter,
    const typename guillaume::components::Bound::Size &boundSize,
    const typename guillaume::components::Transform::Scale &entityScale,
    const typename guillaume::components::Transform::Rotation &entityRotation) {
    constexpr float degreeToRadian = 0.01745329251994329576923690768489f;
    const float rotationRadians = entityRotation[2] * degreeToRadian;
    const float cosine = std::cos(rotationRadians);
    const float sine = std::sin(rotationRadians);

    const float dx = point[0] - entityCenter[0];
    const float dy = point[1] - entityCenter[1];

    const float localX = dx * cosine - dy * sine;
    const float localY = dx * sine + dy * cosine;

    const float halfWidth = (boundSize[0] * entityScale[0]) / 2.0f;
    const float halfHeight = (boundSize[1] * entityScale[1]) / 2.0f;

    return std::abs(localX) <= halfWidth && std::abs(localY) <= halfHeight;
}

} // namespace

namespace guillaume::systems {

Hover::Hover(event::EventBus &eventBus, Renderer &renderer)
    : _mouseMotionSubscriber(eventBus), _renderer(renderer) {}

void Hover::update(ecs::ComponentRegistry &componentRegistry,
                   const ecs::Entity::Identifier &entityIdentifier) {
    getLogger().debug("Updating Hover system for entity " +
                      std::to_string(entityIdentifier));
    if (!_mouseMotionSubscriber.hasPendingEvents()) {
        // No new events, but we still need to check current mouse position
        // Use the renderer's stored mouse position
    } else {
        // Process new mouse motion events and update renderer's stored position
        auto event = _mouseMotionSubscriber.getNextEvent();
        if (event) {
            const auto mousePosition = event->getPosition();
            typename guillaume::components::Transform::Position mousePos3D;
            mousePos3D[0] = mousePosition[0];
            mousePos3D[1] = mousePosition[1];
            mousePos3D[2] = 0.0f;

            const auto mouseRay =
                _renderer.getViewRayFromScreenPosition(mousePos3D);
            _renderer.setLastMouseRay(mouseRay);

            const auto worldMousePos = intersectRayWithUiPlane(mouseRay);
            if (worldMousePos) {
                _renderer.setLastMousePosition(*worldMousePos);
            }
        }
    }

    const auto &bound =
        componentRegistry.getComponent<components::Bound>(entityIdentifier);
    auto &hover =
        componentRegistry.getComponent<components::Hover>(entityIdentifier);

    const auto worldMousePos = _renderer.getLastMousePosition();
    const auto worldTransform =
        detail::calculateWorldTransform(componentRegistry, entityIdentifier);
    const auto size = bound.getSize();

    guillaume::components::Transform::Position trueCenter;
    trueCenter[0] = worldTransform.position[0];
    trueCenter[1] =
        worldTransform.position[1] - (size[1] * worldTransform.scale[1] / 2.0f);

    const bool isInside = isPointInsideEntityBounds(worldMousePos, trueCenter,
                                                    size, worldTransform.scale,
                                                    worldTransform.rotation);

    if (isInside) {
        if (hover.isHovered()) {
            // Already hovering, no change
            return;
        }

        hover.setHovered(true);
        const auto onHover = hover.getOnHoverHandler();
        if (onHover) {
            onHover();
        }
        return;
    }

    if (!hover.isHovered()) {
        // Mouse is outside and wasn't hovering, no change
        return;
    }

    hover.setHovered(false);
    const auto onUnhover = hover.getOnUnhoverHandler();
    if (onUnhover) {
        onUnhover();
    }
}

} // namespace guillaume::systems