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
#include <utility/math/camera.hpp>
#include <utility/math/ray.hpp>
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
    using Camera = utility::math::Camera<float>;      ///< Camera model type
    using Ray = utility::math::Ray<float, 3>;         ///< Picking ray type
    using Position = utility::math::Vector<float, 3>; ///< Camera position type
    using Rotation = utility::math::Vector<float, 3>; ///< Camera rotation type
    using ViewportSize =
        utility::math::Vector<float, 2>; ///< Viewport size (width, height)

    /**
     * @brief Default constructor
     */
    Renderer(void);

    /**
     * @brief Default destructor
     */
    virtual ~Renderer(void) = default;

  private:
    Camera _camera; ///< Camera state
    Rotation _cameraRotation = {0.0f, 0.0f,
                                0.0f}; ///< Camera rotation (Euler angles)
    Ray _lastMouseRay;                 ///< Last mouse ray in world space
    Position _lastMousePosition = {
        0.0f, 0.0f, 0.0f}; ///< Last mouse position in world space

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
     * @brief Get the current viewport size in pixels.
     * @return The viewport size vector (width, height).
     */
    virtual ViewportSize getViewportSize(void) const { return {1.0f, 1.0f}; }

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
    Position getCameraPosition(void) const { return _camera.getPosition(); }

    /**
     * @brief Get the full camera model.
     * @return The camera instance.
     */
    Camera getCamera(void) const { return _camera; }

    /**
     * @brief Set the camera position in 3D space.
     * @param position The new camera position vector.
     * @note This also refreshes the cached last mouse ray origin.
     */
    void setCameraPosition(const Position &position) {
        _camera.setPosition(position);
        _lastMouseRay = Ray(_camera.getPosition(), _camera.getForward());
    }

    /**
     * @brief Set the full camera model.
     * @param camera The new camera instance.
        * @note Synchronizes cached Euler rotation and last mouse ray.
     */
    void setCamera(const Camera &camera);

    /**
     * @brief Get the camera rotation (Euler angles).
     * @return The camera rotation vector.
     */
    Rotation getCameraRotation(void) const { return _cameraRotation; }

    /**
     * @brief Set the camera rotation (Euler angles).
     * @param rotation The new camera rotation vector.
        * @note Synchronizes camera forward/up vectors and last mouse ray.
     */
    void setCameraRotation(const Rotation &rotation);

    /**
        * @brief Build an orthographic world-space view ray from a screen position.
     * @param screenPosition The screen position (x, y in pixels).
        * @return A world-space ray parallel to camera forward.
     */
    Ray getViewRayFromScreenPosition(const Position &screenPosition) const {
        const auto cameraPosition = _camera.getPosition();
        const auto right = _camera.right();
        const auto up = _camera.getUp();

        const auto rayOrigin = cameraPosition + (right * screenPosition[0]) +
                               (up * screenPosition[1]);

        return Ray(rayOrigin, _camera.getForward());
    }

    /**
     * @brief Get the last mouse ray in world space.
     * @return The last mouse ray.
     */
    Ray getLastMouseRay(void) const { return _lastMouseRay; }

    /**
     * @brief Set the last mouse ray in world space.
     * @param ray The new mouse ray.
     */
    void setLastMouseRay(const Ray &ray) { _lastMouseRay = ray; }

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
