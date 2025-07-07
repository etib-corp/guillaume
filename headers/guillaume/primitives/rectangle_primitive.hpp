#pragma once

#include "guillaume/color.hpp"
#include "guillaume/primitive.hpp"
#include "guillaume/rectangle.hpp"

#include <memory>

namespace guillaume {

// Rectangle command - draws a colored rectangle
class RectanglePrimitive : public Primitive {
private:
    Rectangle _rectangle;
    Color _color;

public:
    RectanglePrimitive(const Rectangle& rectangle, const Color& color)
        : Primitive(PrimitiveType::RECTANGLE, sizeof(RectanglePrimitive))
        , _rectangle(rectangle)
        , _color(color)
    {
    }

    void execute() override
    {
        // Implementation for rectangle drawing
        // This would typically call renderer->drawRect(rect, color)
    }

    std::unique_ptr<Primitive> clone() const override
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
inline std::unique_ptr<Primitive>
createRectanglePrimitive(const Rectangle& rectangle, const Color& color)
{
    return std::make_unique<RectanglePrimitive>(rectangle, color);
}

} // namespace guillaume