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
    bool _is_pressed = false;

protected:
public:
    Button(const std::string& label, const Color& background_color, const Color& text_color,
        const Font& font, const Rectangle& rectangle)
        : _label(label)
        , _background_color(background_color)
        , _text_color(text_color)
        , _font(font)
        , _rectangle(rectangle)
    {
        _add_primitive("background", createRectanglePrimitive(_renderer, _rectangle, _background_color));
        _add_primitive("text", createTextPrimitive(_renderer, _font, Vector(_rectangle.get_x(), _rectangle.get_y()), _text_color, _label));
    }

    void draw() override
    {
        if (is_visible()) {
            _execute_primitives();
        }
    }

    void set_background_color(const Color& color)
    {
        _background_color = color;
        _update_primitive("background", createRectanglePrimitive(_renderer, _rectangle, _background_color));
    }

    void set_label(const std::string& label)
    {
        _label = label;
        _update_primitive("text", createTextPrimitive(_renderer, _font, Vector(_rectangle.get_x(), _rectangle.get_y()), _text_color, _label));
    }
};

}
