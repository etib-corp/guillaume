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

#include "guillaume/entities/text.hpp"

namespace guillaume::entities {

Text::Text::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
                             ecs::EntityRegistry &entityRegistry)
    : ecs::LeafEntityBuilder<Text>(componentRegistry, entityRegistry) {}

Text::Builder::~Builder(void) {}

void Text::Builder::registerEntity(void) {
    _text = std::make_unique<Text>(getComponentRegistry(), _content, _fontSize,
                                   _color);
    getEntityRegistry().addEntity(std::move(_text));
}

void Text::Builder::reset(void) {
    _text.reset();
    _content.clear();
    _fontSize = 24;
    _color = {255, 255, 255, 255};
}

Text::Builder &Text::Builder::withContent(const std::string &content) {
    _content = content;
    return *this;
}

Text::Builder &Text::Builder::withFontSize(const std::size_t &fontSize) {
    _fontSize = fontSize;
    return *this;
}

Text::Builder &Text::Builder::withColor(const Color &color) {
    _color = color;
    return *this;
}

Text::Director::Director(void) : ecs::EntityDirector() {}

Text::Director::~Director(void) {}

void Text::Director::makeDefaultText(Builder &builder,
                                     const std::string &content,
                                     const std::size_t &fontSize,
                                     const Color &color) {
    builder.withContent(content)
        .withFontSize(fontSize)
        .withColor(color)
        .registerEntity();
}

Text::Text(ecs::ComponentRegistry &registry, const std::string &content,
           const std::size_t &fontSize, const Color &color)
    : ecs::LeafEntityFiller<components::Transform, components::Text>(registry),
      _content(content), _fontSize(fontSize), _color(color) {
    registry.getComponent<components::Transform>(getIdentifier())
        .setPosition({200.0f, 200.0f, 0.0f})
        .setRotation({0.0f, 0.0f, 0.0f})
        .setScale({1.0f, 1.0f, 1.0f});

    registry.getComponent<components::Text>(getIdentifier())
        .setContent(_content)
        .setFontSize(_fontSize)
        .setColor(_color);
}

Text::~Text() {}

} // namespace guillaume::entities