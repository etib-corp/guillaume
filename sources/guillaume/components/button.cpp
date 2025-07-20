#include "guillaume/components/button.hpp"

namespace guigui {

Button::Button(const std::string& label, const Color& background_color, const Color& text_color,
    const Font& font, const Rectangle& rectangle, std::function<void(Button&)> on_click)
    : _label(label)
    , _background_color(background_color)
    , _text_color(text_color)
    , _font(font)
    , _rectangle(rectangle)
    , _on_click(on_click)
{
    // Primitives will be added when renderer is set
}

void Button::set_renderer(std::shared_ptr<Renderer> renderer)
{
    Component::set_renderer(renderer);
    _add_primitive("background", createRectanglePrimitive(_renderer, _rectangle, _background_color));
    _add_primitive("text", createTextPrimitive(_renderer, _font, Vector(_rectangle.get_x(), _rectangle.get_y()), _text_color, _label));
}

void Button::draw()
{
    if (is_visible()) {
        _execute_primitives();
    }
}

void Button::set_background_color(const Color& color)
{
    _background_color = color;
    if (_renderer) {
        _update_primitive("background", createRectanglePrimitive(_renderer, _rectangle, _background_color));
    }
}

void Button::set_label(const std::string& label)
{
    _label = label;
    if (_renderer) {
        _update_primitive("text", createTextPrimitive(_renderer, _font, Vector(_rectangle.get_x(), _rectangle.get_y()), _text_color, _label));
    }
}

} // namespace guigui
