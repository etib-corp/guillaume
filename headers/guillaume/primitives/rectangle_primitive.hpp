#pragma once

#include "guillaume/color.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/renderer.hpp"
#include <memory>
#include "guillaume/primitive.hpp"

namespace guigui {

// Rectangle command - draws a colored rectangle
class RectanglePrimitive : public guigui::Primitive {
private:
    Rectangle _rectangle;
    Color _color;

public:
    RectanglePrimitive(std::shared_ptr<guigui::Renderer> renderer, const Rectangle& rectangle, const Color& color);

    void execute() override;
    std::unique_ptr<Primitivable> clone() const override;
    std::string to_string() const override;

    const Rectangle& get_rectangle() const;
    const Color& get_color() const;
};

// Factory function to create RectanglePrimitive
std::unique_ptr<Primitivable> createRectanglePrimitive(std::shared_ptr<guigui::Renderer> renderer, const Rectangle& rectangle, const Color& color);

} // namespace guigui