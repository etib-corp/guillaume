#pragma once

#include <iostream>
#include <memory>

#include "guillaume/renderer.hpp"
#include "guillaume/logger.hpp"

#include "guillaume/event.hpp"
#include "guillaume/eventable.hpp"

#include "guillaume/events/keyboard_event.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/events/quit_event.hpp"
#include "guillaume/events/unknown_event.hpp"

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"

#include "guillaume/component.hpp"
#include "guillaume/componentable.hpp"
#include "guillaume/container.hpp"

namespace guigui {

class Context {

private:
    std::shared_ptr<Renderer> _renderer;
    std::unique_ptr<Font> _default_font;
    std::unique_ptr<Container> _root_component;

    void _loop();

protected:
public:
    Context(std::shared_ptr<Renderer> renderer);
    ~Context() = default;

    void set_root_component(std::unique_ptr<Container> root_component);
    std::unique_ptr<Container> get_root_component();

    void begin();
    void process_frame();
    void end();
    void run();
};
} // namespace guigui
