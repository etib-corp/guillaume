#pragma once

#include "guillaume/color.hpp"
#include "guillaume/primitive.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/renderer.hpp"

#include <memory>

namespace guigui {

// Rectangle command - draws a colored rectangle
class RectanglePrimitive : public Primitive {
private:
    Rectangle _rectangle;
    Color _color;

public:
    RectanglePrimitive(std::shared_ptr<Renderer> renderer, const Rectangle& rectangle, const Color& color)
        : Primitive(PrimitiveType::RECTANGLE, renderer)
        , _rectangle(rectangle)
        , _color(color)
    {
    }

    void execute() override {
        _renderer->draw_rectangle(*this);
    }

    std::unique_ptr<Primitivable> clone() const override
    {
        return std::make_unique<RectanglePrimitive>(*this);
    }

    std::string to_string() const override
    {
        return "RectanglePrimitive(rectangle=" + _rectangle.to_string() + ", color=" + _color.to_string() + ")";
    }

    const Rectangle& get_rectangle() const { return _rectangle; }
    const Color& get_color() const { return _color; }
};

// Factory function to create RectanglePrimitive
inline std::unique_ptr<Primitivable>
createRectanglePrimitive(std::shared_ptr<Renderer> renderer, const Rectangle& rectangle, const Color& color)
{
    return std::make_unique<RectanglePrimitive>(renderer, rectangle, color);
}

} // namespace guigui