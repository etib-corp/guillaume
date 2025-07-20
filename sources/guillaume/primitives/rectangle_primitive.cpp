#include "guillaume/primitives/rectangle_primitive.hpp"

namespace guigui {

RectanglePrimitive::RectanglePrimitive(std::shared_ptr<guigui::Renderer> renderer, const Rectangle& rectangle, const Color& color)
    : Primitive(PrimitiveType::RECTANGLE, renderer)
    , _rectangle(rectangle)
    , _color(color)
{
}

void RectanglePrimitive::execute()
{
    if (!_renderer) {
        throw std::runtime_error("Renderer is not set for RectanglePrimitive.");
    }
    _renderer->draw_rectangle(*this);
}

std::unique_ptr<Primitivable> RectanglePrimitive::clone() const
{
    return std::make_unique<RectanglePrimitive>(*this);
}

std::string RectanglePrimitive::to_string() const
{
    return "RectanglePrimitive(rectangle=" + _rectangle.to_string() + ", color=" + _color.to_string() + ")";
}

const Rectangle& RectanglePrimitive::get_rectangle() const 
{ 
    return _rectangle; 
}

const Color& RectanglePrimitive::get_color() const 
{ 
    return _color; 
}

std::unique_ptr<Primitivable> createRectanglePrimitive(std::shared_ptr<guigui::Renderer> renderer, const Rectangle& rectangle, const Color& color)
{
    return std::make_unique<RectanglePrimitive>(renderer, rectangle, color);
}

} // namespace guigui
