#pragma once

#include "guillaume/component.hpp"

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"

namespace guigui {

class Button : public Component {
private:
    std::string _label;
    Color _background_color;
    Color _text_color;
    Font _font;
    Rectangle _rectangle;
    std::function<void(Button&)> _on_click;
    bool _is_pressed = false;

protected:
public:
    Button(const std::string& label, const Color& background_color, const Color& text_color,
        const Font& font, const Rectangle& rectangle, std::function<void(Button&)> on_click = nullptr);

    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    void draw() override;
    void set_background_color(const Color& color);
    void set_label(const std::string& label);
};

}
