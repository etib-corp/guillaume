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

#include <color.hpp>

#include "ecs/component.hpp"

namespace guillaume::components::visual {

/**
 * @brief Sprite component for rendering images or textures.
 */
class Sprite : public ecs::Component {
  private:
    std::string _texture_id_{};
    utility::Color<std::uint8_t> _color_{255, 255, 255, 255};
    bool _sprite_sheet_{false};
    int _frame_{0};

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
    void setColor(const utility::Color<std::uint8_t> &color) {
        _color_ = color;
    }

    /**
     * @brief Get the color.
     * @return Reference to the color value (RGBA).
     */
    utility::Color<std::uint8_t> &getColor(void) { return _color_; }

    /**
     * @brief Set sprite sheet flag.
     * @param sprite_sheet True if using sprite sheet.
     */
    void setSpriteSheet(bool sprite_sheet) { _sprite_sheet_ = sprite_sheet; }

    /**
     * @brief Get sprite sheet flag.
     * @return True if using sprite sheet.
     */
    bool getSpriteSheet(void) const { return _sprite_sheet_; }

    /**
     * @brief Set the frame number.
     * @param frame The frame number.
     */
    void setFrame(int frame) { _frame_ = frame; }

    /**
     * @brief Get the frame number.
     * @return The frame number.
     */
    int getFrame(void) const { return _frame_; }
};

} // namespace guillaume::components::visual
