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

#include "guillaume/drawables/rectangle.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace {

utility::math::Vector<std::float_t, 2>
rotatePoint(const utility::math::Vector<std::float_t, 2> &point,
            std::float_t theta) {
    const std::float_t cosine = std::cos(theta);
    const std::float_t sine = std::sin(theta);
    return {(point[0] * cosine) - (point[1] * sine),
            (point[0] * sine) + (point[1] * cosine)};
}

utility::math::Vector<std::float_t, 2>
toPlaneVector(const guillaume::Drawable::Position &position) {
    return {position[0], position[1]};
}

} // namespace

namespace guillaume::drawables {

Rectangle::Rectangle(void) : Drawable() {}

Rectangle::Rectangle(Position position, Rotation rotation, Scale scale,
                     Color color, Size size, BorderRadius borderRadius)
    : Drawable(position, rotation, scale, color), _size(size),
      _leftTopRadius(borderRadius), _rightTopRadius(borderRadius),
      _rightBottomRadius(borderRadius), _leftBottomRadius(borderRadius) {}

std::vector<Drawable::Vertex> Rectangle::getVertices(void) {
    std::vector<Drawable::Vertex> vertices;

    const auto center3D = getPosition();
    const auto scale = getScale();
    const auto rotation = getRotation();
    const auto color = getColor();

    const utility::math::Vector<std::float_t, 2> center =
        toPlaneVector(center3D);
    const std::float_t theta =
        rotation[2] * (std::numbers::pi_v<std::float_t> / 180.0f);

    const std::float_t halfWidth = (_size[0] * scale[0]) * 0.5f;
    const std::float_t halfHeight = (_size[1] * scale[1]) * 0.5f;

    const std::float_t epsilon = 1.0e-4f;
    if (halfWidth <= epsilon || halfHeight <= epsilon) {
        return vertices;
    }

    std::float_t radiusTopLeft =
        std::clamp(_leftTopRadius, 0.0f, std::min(halfWidth, halfHeight));
    std::float_t radiusTopRight =
        std::clamp(_rightTopRadius, 0.0f, std::min(halfWidth, halfHeight));
    std::float_t radiusBottomRight =
        std::clamp(_rightBottomRadius, 0.0f, std::min(halfWidth, halfHeight));
    std::float_t radiusBottomLeft =
        std::clamp(_leftBottomRadius, 0.0f, std::min(halfWidth, halfHeight));

    const int arcSegments = 16;
    auto appendArc =
        [&](const utility::math::Vector<std::float_t, 2> &localArcCenter,
            std::float_t radius, std::float_t startAngle,
            std::float_t endAngle) -> void {
        if (radius <= epsilon) {
            const utility::math::Vector<std::float_t, 2> rotated =
                rotatePoint(localArcCenter, theta) + center;
            vertices.push_back(
                {{rotated[0], rotated[1], center3D[2]},
                 {0.0f, 0.0f, 1.0f},
                 {0.0f, 0.0f},
                 {color.red(), color.green(), color.blue(), color.alpha()}});
            return;
        }

        for (int i = 0; i <= arcSegments; ++i) {
            const std::float_t ratio = static_cast<std::float_t>(i) /
                                       static_cast<std::float_t>(arcSegments);
            const std::float_t angle =
                startAngle + ((endAngle - startAngle) * ratio);
            const utility::math::Vector<std::float_t, 2> localPoint =
                localArcCenter +
                utility::math::Vector<std::float_t, 2>{
                    std::cos(angle) * radius, std::sin(angle) * radius};
            const utility::math::Vector<std::float_t, 2> rotated =
                rotatePoint(localPoint, theta) + center;

            vertices.push_back(
                {{rotated[0], rotated[1], center3D[2]},
                 {0.0f, 0.0f, 1.0f},
                 {0.0f, 0.0f},
                 {color.red(), color.green(), color.blue(), color.alpha()}});
        }
    };

    appendArc({halfWidth - radiusTopRight, -halfHeight + radiusTopRight},
              radiusTopRight, -std::numbers::pi_v<std::float_t> / 2.0f, 0.0f);
    appendArc({halfWidth - radiusBottomRight, halfHeight - radiusBottomRight},
              radiusBottomRight, 0.0f, std::numbers::pi_v<std::float_t> / 2.0f);
    appendArc({-halfWidth + radiusBottomLeft, halfHeight - radiusBottomLeft},
              radiusBottomLeft, std::numbers::pi_v<std::float_t> / 2.0f,
              std::numbers::pi_v<std::float_t>);
    appendArc({-halfWidth + radiusTopLeft, -halfHeight + radiusTopLeft},
              radiusTopLeft, std::numbers::pi_v<std::float_t>,
              3.0f * std::numbers::pi_v<std::float_t> / 2.0f);

    if (!vertices.empty()) {
        vertices.push_back(vertices.front());
    }

    return vertices;
}

} // namespace guillaume::drawables
