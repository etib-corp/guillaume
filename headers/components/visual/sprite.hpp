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

#include <cstdint>
#include <string>

#include "component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Sprite component for rendering images or textures.
 */
class Sprite : public Component {
  private:
    std::string _texture_id_{};
    std::uint32_t _color_{0xFFFFFFFF};

  public:
    /**
     * @brief Default constructor.
     */
    Sprite(void) = default;

    /**
     * @brief Default destructor.
     */
    ~Sprite(void) override = default;

    /**
     * @brief Set the texture ID.
     * @param texture_id The texture identifier.
     */
    void setTextureId(const std::string &texture_id) {
        _texture_id_ = texture_id;
    }

    /**
     * @brief Get the texture ID.
     * @return The texture identifier.
     */
    const std::string &getTextureId(void) const { return _texture_id_; }

    /**
     * @brief Set the color.
     * @param color The color value (RGBA).
     */
    void setColor(std::uint32_t color) { _color_ = color; }

    /**
     * @brief Get the color.
     * @return The color value (RGBA).
     */
    std::uint32_t getColor(void) const { return _color_; }
};

} // namespace guillaume::components::visual
