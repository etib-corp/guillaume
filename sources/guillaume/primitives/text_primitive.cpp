#include "guillaume/primitives/text_primitive.hpp"

namespace guigui {

TextPrimitive::TextPrimitive(std::shared_ptr<guigui::Renderer> renderer, const Font& font, const Vector& position, const Color& color, const std::string& content)
    : Primitive(PrimitiveType::TEXT, renderer)
    , _font(font)
    , _position(position)
    , _color(color)
    , _content(content)
{
}

TextPrimitive::TextPrimitive(std::shared_ptr<guigui::Renderer> renderer, const Font& font, const Vector& position, const Color& color, const std::string& content,
                            const std::string& component_id, const std::string& primitive_name)
    : Primitive(PrimitiveType::TEXT, renderer, component_id, primitive_name)
    , _font(font)
    , _position(position)
    , _color(color)
    , _content(content)
{
}

void TextPrimitive::execute()
{
    if (!_renderer) {
        throw std::runtime_error("Renderer is not set for TextPrimitive.");
    }
    _renderer->draw_text(*this);
}

std::unique_ptr<Primitivable> TextPrimitive::clone() const
{
    return std::make_unique<TextPrimitive>(*this);
}

std::string TextPrimitive::to_string() const
{
    return "TextPrimitive(font=" + _font.to_string() + ", position=" + _position.to_string() + ", color=" + _color.to_string() + ", content=\"" + _content + "\")";
}

const Font& TextPrimitive::get_font() const 
{ 
    return _font; 
}

const Vector& TextPrimitive::get_position() const 
{ 
    return _position; 
}

const Color& TextPrimitive::get_color() const 
{ 
    return _color; 
}

const std::string& TextPrimitive::get_text() const 
{ 
    return _content; 
}

std::unique_ptr<Primitivable> createTextPrimitive(std::shared_ptr<guigui::Renderer> renderer, const Font& font, const Vector& position, const Color& color, const std::string& content)
{
    return std::make_unique<TextPrimitive>(renderer, font, position, color, content);
}

} // namespace guigui
