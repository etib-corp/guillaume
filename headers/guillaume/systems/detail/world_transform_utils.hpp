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

#pragma once

#include "guillaume/components/relationship.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

#include <cmath>

namespace guillaume::systems::detail {

struct WorldTransform {
    components::Transform::Position position;
    components::Transform::Rotation rotation;
    components::Transform::Scale scale;
};

inline WorldTransform
calculateWorldTransform(ecs::ComponentRegistry &componentRegistry,
                        const ecs::Entity::Identifier &entityId) {
    const auto &transform =
        componentRegistry.getComponent<components::Transform>(entityId);

    WorldTransform worldTransform{
        transform.getPosition(), transform.getRotation(), transform.getScale()};

    if (!componentRegistry.hasComponent<components::Relationship>(entityId)) {
        return worldTransform;
    }

    const auto &relationship =
        componentRegistry.getComponent<components::Relationship>(entityId);
    const auto parentId = relationship.getParentIdentifier();

    if (parentId == ecs::Entity::InvalidIdentifier) {
        return worldTransform;
    }

    const auto parentWorldTransform =
        calculateWorldTransform(componentRegistry, parentId);

    components::Transform::Position scaledLocalPosition;
    scaledLocalPosition[0] =
        worldTransform.position[0] * parentWorldTransform.scale[0];
    scaledLocalPosition[1] =
        worldTransform.position[1] * parentWorldTransform.scale[1];
    scaledLocalPosition[2] =
        worldTransform.position[2] * parentWorldTransform.scale[2];

    constexpr float degreeToRadian = 0.01745329251994329576923690768489f;
    const float parentRotationZRadians =
        parentWorldTransform.rotation[2] * degreeToRadian;
    const float cosine = std::cos(parentRotationZRadians);
    const float sine = std::sin(parentRotationZRadians);

    components::Transform::Position rotatedLocalPosition;
    rotatedLocalPosition[0] =
        scaledLocalPosition[0] * cosine - scaledLocalPosition[1] * sine;
    rotatedLocalPosition[1] =
        scaledLocalPosition[0] * sine + scaledLocalPosition[1] * cosine;
    rotatedLocalPosition[2] = scaledLocalPosition[2];

    worldTransform.position[0] =
        parentWorldTransform.position[0] + rotatedLocalPosition[0];
    worldTransform.position[1] =
        parentWorldTransform.position[1] + rotatedLocalPosition[1];
    worldTransform.position[2] =
        parentWorldTransform.position[2] + rotatedLocalPosition[2];

    worldTransform.rotation[0] += parentWorldTransform.rotation[0];
    worldTransform.rotation[1] += parentWorldTransform.rotation[1];
    worldTransform.rotation[2] += parentWorldTransform.rotation[2];

    worldTransform.scale[0] *= parentWorldTransform.scale[0];
    worldTransform.scale[1] *= parentWorldTransform.scale[1];
    worldTransform.scale[2] *= parentWorldTransform.scale[2];

    return worldTransform;
}

} // namespace guillaume::systems::detail
