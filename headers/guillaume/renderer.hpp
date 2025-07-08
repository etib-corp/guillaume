#pragma once

#include <functional>
#include <map>
#include <queue>

#include "guillaume/color.hpp"
#include "guillaume/eventable.hpp"
#include "guillaume/font.hpp"
#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

namespace guigui {

class Renderer {
private:
    std::queue<std::unique_ptr<Eventable>> _event_queue;
    std::map<EventType, std::function<void(std::unique_ptr<Eventable>)>> _event_handlers;
    bool _is_running = true;

protected:
    void _push_event(std::unique_ptr<Eventable> event)
    {
        _event_queue.push(std::move(event));
    }

public:
    virtual ~Renderer() = default;

    virtual void init(std::string name) = 0;
    virtual void draw_rectangle(const RectanglePrimitive& rectangle)
        = 0;
    virtual void draw_text(const guigui::TextPrimitive& text, const guigui::Font& font) = 0;
    virtual Vector get_text_size(const Font& font,
        const std::string& content)
        = 0;
    virtual void set_clip_rect(const Rectangle& rectangle) = 0;
    virtual void clear(const Color& color) = 0;
    virtual void present() = 0;
    virtual void register_event_handler(std::function<void(std::unique_ptr<Eventable>)> handler, const EventType& event_type)
    {
        if (_event_handlers.find(event_type) != _event_handlers.end()) {
            throw std::runtime_error("Event handler already registered for this event type.");
        }
        _event_handlers[event_type] = handler;
    }

    virtual void poll_events() = 0;

    bool is_running() const
    {
        return _is_running;
    }

    std::unique_ptr<Eventable> pop_event()
    {
        if (_event_queue.empty()) {
            return nullptr; // No events to pop
        }
        auto event = std::move(_event_queue.front());
        _event_queue.pop();

        return event;
    }

    bool has_event() const
    {
        return !_event_queue.empty();
    }

    void handle_event(std::unique_ptr<Eventable> event)
    {
        if (event) {
            auto it = _event_handlers.find(event->get_type());
            if (it != _event_handlers.end()) {
                it->second(std::move(event));
            } else {
                throw std::runtime_error("No handler registered for this event type.");
            }
        }
    }

    void set_running(bool is_running)
    {
        _is_running = is_running;
    }
};

} // namespace guigui