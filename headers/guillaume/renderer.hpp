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

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>
#include <utility/math/vector.hpp>

#include "guillaume/shapes/circle.hpp"
#include "guillaume/shapes/rectangle.hpp"
#include "guillaume/shapes/triangle.hpp"

#include "guillaume/font.hpp"
#include "guillaume/text.hpp"

namespace guillaume {

/**
 * @brief Renderer interface.
 * @see Shape
 * @see Text
 * @see Font
 */
class Renderer
    : protected utility::logging::Loggable<Renderer,
                                           utility::logging::StandardLogger> {
  public:
    using Position = utility::math::Vector<float, 3>; ///< Camera position type
    using Rotation = utility::math::Vector<float, 3>; ///< Camera rotation type

    /**
     * @brief Default constructor
     */
    Renderer(void);

    /**
     * @brief Default destructor
     */
    virtual ~Renderer(void) = default;

  private:
    Position _cameraPosition = {0.0f, 0.0f, 5.0f}; ///< Camera position in 3D space
    Rotation _cameraRotation = {0.0f, 0.0f, 0.0f}; ///< Camera rotation (Euler angles)
    Position _lastMousePosition = {0.0f, 0.0f, 0.0f}; ///< Last mouse position in world space

  public:

    /**
     * @brief Clear the current rendering target with the drawing color.
     */
    virtual void clear(void) = 0;

    /**
     * @brief Present the composed back buffer to the screen.
     */
    virtual void present(void) = 0;

    /**
     * @brief Draw a triangle shape.
     * @param triangle The triangle to draw.
     */
    virtual void drawTriangle(const shapes::Triangle &triangle) = 0;

    /**
     * @brief Draw a rectangle shape.
     * @param rectangle The rectangle to draw.
     */
    virtual void drawRectangle(const shapes::Rectangle &rectangle) = 0;

    /**
     * @brief Draw a circle shape.
     * @param circle The circle to draw.
     */
    virtual void drawCircle(const shapes::Circle &circle) = 0;

    /**
     * @brief Measure the size of the given text using the specified font.
     * @param text The text to measure.
     * @param font The font to use for measurement.
     * @return The size of the text as a 2D vector (width, height).
     */
    virtual utility::math::Vector<std::float_t, 2>
    measureText(const Text &text, const Font &font) = 0;

    /**
     * @brief Draw text using a specific font.
     * @param text The text to draw.
     * @param font The font to use.
     */
    virtual void drawText(const Text &text, const Font &font) = 0;

    /**
     * @brief Get the camera position in 3D space.
     * @return The camera position vector.
     */
    Position getCameraPosition(void) const { return _cameraPosition; }

    /**
     * @brief Set the camera position in 3D space.
     * @param position The new camera position vector.
     */
    void setCameraPosition(const Position &position) {
        _cameraPosition = position;
    }

    /**
     * @brief Get the camera rotation (Euler angles).
     * @return The camera rotation vector.
     */
    Rotation getCameraRotation(void) const { return _cameraRotation; }

    /**
     * @brief Set the camera rotation (Euler angles).
     * @param rotation The new camera rotation vector.
     */
    void setCameraRotation(const Rotation &rotation) {
        _cameraRotation = rotation;
    }

    /**
     * @brief Get the last mouse position in world space.
     * @return The last mouse position vector.
     */
    Position getLastMousePosition(void) const { return _lastMousePosition; }

    /**
     * @brief Set the last mouse position in world space.
     * @param position The new mouse position vector.
     */
    void setLastMousePosition(const Position &position) {
        _lastMousePosition = position;
    }
};

} // namespace guillaume
