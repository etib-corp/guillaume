#pragma once

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/renderer.hpp"
#include "guillaume/vector.hpp"

#include "guillaume/primitive.hpp"
#include <memory>
#include <string>

namespace guigui {

// Text command - draws _content at a _position
class TextPrimitive : public guigui::Primitive {
private:
    Font _font;
    Vector _position;
    Color _color;
    std::string _content;

public:
    TextPrimitive(std::shared_ptr<guigui::Renderer> renderer, const Font& font, const Vector& position, const Color& color, const std::string& content);

    TextPrimitive(std::shared_ptr<guigui::Renderer> renderer, const Font& font, const Vector& position, const Color& color, const std::string& content,
        const std::string& component_id, const std::string& primitive_name);

    void execute() override;
    std::unique_ptr<Primitivable> clone() const override;
    std::string to_string() const override;

    const Font& get_font() const;
    const Vector& get_position() const;
    const Color& get_color() const;
    const std::string& get_text() const;
};

// Factory function to create TextPrimitive
std::unique_ptr<Primitivable> createTextPrimitive(std::shared_ptr<guigui::Renderer> renderer, const Font& font, const Vector& position, const Color& color, const std::string& content);

} // namespace guigui