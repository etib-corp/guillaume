#pragma once

#include <iostream>
#include <memory>

#include "guillaume/events/keyboard_event.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/events/quit_event.hpp"
#include "guillaume/events/unknown_event.hpp"
#include "guillaume/renderer.hpp"

namespace guigui {

class Context {

private:
    std::unique_ptr<Renderer> _renderer;

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
    Context(std::unique_ptr<Renderer> renderer)
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
            EventType::QUIT);

        _renderer->register_event_handler(
            [this](std::unique_ptr<Eventable> event) {
                if (auto keyboard_event = std::dynamic_pointer_cast<KeyboardEvent>(std::shared_ptr<Eventable>(std::move(event)))) {
                    std::cout << "Keyboard event received: " << keyboard_event->to_string() << std::endl;
                }
            },
            EventType::KEYBOARD_EVENT);

        _renderer->register_event_handler(
            [this](std::unique_ptr<Eventable> event) {
                if (auto mouse_event = std::dynamic_pointer_cast<MouseEvent>(std::shared_ptr<Eventable>(std::move(event)))) {
                    std::cout << "Mouse event received: " << mouse_event->to_string() << std::endl;
                }
            },
            EventType::MOUSE_EVENT);

        _renderer->register_event_handler(
            [this](std::unique_ptr<Eventable> event) {
                if (auto unknown_event = std::dynamic_pointer_cast<UnknownEvent>(std::shared_ptr<Eventable>(std::move(event)))) {
                    std::cout << "Unknown event received: " << unknown_event->to_string() << std::endl;
                }
            },
            EventType::UNKNOWN);
    }

    ~Context() = default;

    void begin()
    {
        _renderer->poll_events();
        while (_renderer->has_event()) {
            _renderer->handle_event(_renderer->pop_event());
        }
    }

    void process_frame()
    {
        _renderer->clear(Color(0, 0, 0, 255)); // Clear with black color
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
