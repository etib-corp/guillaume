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

#include "guillaume/text.hpp"

namespace guillaume {

Text::Text(const std::string &content, utility::math::Color<uint8_t> color,
           utility::math::Vector<float, 3> position, utility::math::Vector<float, 3> rotation)
    : _content(content), _color(color), _position(position), _rotation(rotation),
      _alignment(TextAlignment::Left), _isDirty(true), _bounds(0, 0, 0, 0) {}

const std::string &Text::getContent() const { return _content; }

void Text::setContent(const std::string &content) {
    if (_content != content) {
        _content = content;
        _isDirty = true;
    }
}

const utility::math::Color<uint8_t> &Text::getColor() const { return _color; }

void Text::setColor(const utility::math::Color<uint8_t> &color) {
    if (_color != color) {
        _color = color;
        _isDirty = true;
    }
}

const utility::math::Vector<float, 3> &Text::getPosition() const {
    return _position;
}

void Text::setPosition(const utility::math::Vector<float, 3> &position) {
    _position = position;
}

const utility::math::Vector<float, 3> &Text::getRotation() const {
    return _rotation;
}

void Text::setRotation(const utility::math::Vector<float, 3> &rotation) {
    _rotation = rotation;
}

TextAlignment Text::getAlignment() const { return _alignment; }

void Text::setAlignment(TextAlignment alignment) {
    if (_alignment != alignment) {
        _alignment = alignment;
        _isDirty = true;
    }
}

bool Text::isDirty() const { return _isDirty; }

void Text::markClean() { _isDirty = false; }

void Text::markDirty() { _isDirty = true; }

const utility::math::Rectangle<std::size_t> &Text::getBounds() const {
    return _bounds;
}

void Text::setBounds(const utility::math::Rectangle<std::size_t> &bounds) {
    _bounds = bounds;
}

bool Text::isEmpty() const { return _content.empty(); }

std::size_t Text::length() const { return _content.length(); }

} // namespace guillaume
