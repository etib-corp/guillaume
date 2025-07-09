#pragma once

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/primitive.hpp"
#include "guillaume/renderer.hpp"
#include "guillaume/vector.hpp"

#include <memory>
#include <string>

namespace guigui {

// Text command - draws _content at a _position
class TextPrimitive : public Primitive {
private:
    Font _font;
    Vector _position;
    Color _color;
    std::string _content;

public:
    TextPrimitive(std::shared_ptr<Renderer> renderer, const Font& font, const Vector& position, const Color& color,
        const std::string& content)
        : Primitive(PrimitiveType::TEXT, renderer)
        , _font(font)
        , _position(position)
        , _color(color)
        , _content(content)
    {
    }

    void execute() override
    {
        _renderer->draw_text(*this);
    }

    std::unique_ptr<Primitivable> clone() const override
    {
        return std::make_unique<TextPrimitive>(*this);
    }

    std::string to_string() const override
    {
        return "TextPrimitive(font=" + _font.to_string() + ", position=" + _position.to_string() + ", color=" + _color.to_string() + ", content=\"" + _content + "\")";
    }

    const Font& get_font() const { return _font; }
    const Vector& get_position() const { return _position; }
    const Color& get_color() const { return _color; }
    const std::string& get_text() const { return _content; }
};

// Factory function to create TextPrimitive
inline std::unique_ptr<Primitivable>
createTextPrimitive(std::shared_ptr<Renderer> renderer, const Font& font, const Vector& position,
    const Color& color, const std::string& content)
{
    return std::make_unique<TextPrimitive>(renderer, font, position, color, content);
}

} // namespace guigui