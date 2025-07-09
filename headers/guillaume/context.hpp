#pragma once

#include <iostream>
#include <memory>

#include "guillaume/renderer.hpp"

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

    void _loop()
    {
        while (_renderer->is_running()) {
            begin();

            process_frame();
            _renderer->present();
            end();
        }
    }

protected:
public:
    Context(std::shared_ptr<Renderer> renderer)
        : _renderer(std::move(renderer))
    {
        _renderer->init("Guillaume");
        _renderer->register_event_handler(
            [this](std::unique_ptr<Eventable> event) {
                if (auto quit_event = std::dynamic_pointer_cast<QuitEvent>(std::shared_ptr<Eventable>(std::move(event)))) {
                    std::cout << "Quit event received." << std::endl;
                    _renderer->set_running(false);
                }
            },
            Eventable::EventType::QUIT);

        _renderer->register_event_handler(
            [this](std::unique_ptr<Eventable> event) {
                if (auto keyboard_event = std::dynamic_pointer_cast<KeyboardEvent>(std::shared_ptr<Eventable>(std::move(event)))) {
                    std::cout << "Keyboard event received: " << keyboard_event->to_string() << std::endl;
                }
            },
            Eventable::EventType::KEYBOARD_EVENT);

        _renderer->register_event_handler(
            [this](std::unique_ptr<Eventable> event) {
                if (auto mouse_event = std::dynamic_pointer_cast<MouseEvent>(std::shared_ptr<Eventable>(std::move(event)))) {
                    std::cout << "Mouse event received: " << mouse_event->to_string() << std::endl;
                }
            },
            Eventable::EventType::MOUSE_EVENT);

        _renderer->register_event_handler(
            [this](std::unique_ptr<Eventable> event) {
                if (auto unknown_event = std::dynamic_pointer_cast<UnknownEvent>(std::shared_ptr<Eventable>(std::move(event)))) {
                    std::cout << "Unknown event received: " << unknown_event->to_string() << std::endl;
                }
            },
            Eventable::EventType::UNKNOWN);
        _default_font = std::make_unique<Font>("Roboto", "assets/Roboto.ttf", 48);
    }

    ~Context() = default;

    void set_root_component(std::unique_ptr<Container> root_component)
    {

        _root_component = std::move(root_component);
        _root_component->set_renderer(_renderer);
    }

    std::unique_ptr<Container> get_root_component()
    {
        return std::move(_root_component);
    }

    void begin()
    {
        _renderer->poll_events();
        while (_renderer->has_event()) {
            _renderer->handle_event(_renderer->pop_event());
        }
        _renderer->clear(Color(0, 0, 0, 255)); // Clear with black color
    }

    void process_frame()
    {
        _root_component->draw();
    }

    void end()
    {
    }

    void run()
    {
        _loop();
    }
};
} // namespace guigui
