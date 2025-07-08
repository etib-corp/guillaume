#pragma once

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/primitive.hpp"
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
    TextPrimitive(const Font& font, const Vector& position, const Color& color,
        const std::string& content)
        : Primitive(PrimitiveType::TEXT, sizeof(TextPrimitive))
        , _font(font)
        , _position(position)
        , _color(color)
        , _content(content)
    {
    }

    void execute() override
    {
        // Implementation for _content drawing
        // This would typically call renderer->drawText(_font, _position, _color,
        // _content)
    }

    std::unique_ptr<Primitive> clone() const override
    {
        return std::make_unique<TextPrimitive>(*this);
    }

    std::string to_string() const override
    {
        return "TextPrimitive(font=" + _font.to_string() + ", position=" + _position.to_string() + ", color=" + _color.to_string() + ", content=\"" + _content + "\")";
    }

    const Font& getFont() const { return _font; }
    const Vector& getPosition() const { return _position; }
    const Color& get_color() const { return _color; }
    const std::string& getText() const { return _content; }
};

// Factory function to create TextPrimitive
inline std::unique_ptr<Primitive>
createTextPrimitive(const Font& _font, const Vector& _position,
    const Color& _color, const std::string& _content)
{
    return std::make_unique<TextPrimitive>(_font, _position, _color, _content);
}

} // namespace guigui