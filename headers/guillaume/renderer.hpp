#pragma once

#include <functional>
#include <map>
#include <queue>

#include "guillaume/color.hpp"
#include "guillaume/event.hpp"
#include "guillaume/font.hpp"
#include "guillaume/primitives/rectangle_primitive.hpp"
#include "guillaume/primitives/text_primitive.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

namespace guillaume {

class Renderer {
private:
    std::queue<Event> _event_queue;
    std::map<EventType, std::function<void(const Event&)>> _event_handlers;
    bool _is_running = true;

protected:
    void _push_event(const Event& event)
    {
        _event_queue.push(event);
    }
    void _pop_event()
    {
        if (!_event_queue.empty()) {
            _event_queue.pop();
        }
    }
    bool _has_event() const
    {
        return !_event_queue.empty();
    }

    void _set_running(bool is_running)
    {
        _is_running = is_running;
    }

public:
    virtual ~Renderer() = default;

    virtual void init(std::string name) = 0;
    virtual void draw_rectangle(const RectanglePrimitive& rectangle)
        = 0;
    virtual void draw_text(const guillaume::TextPrimitive& text, const guillaume::Font& font) = 0;
    virtual Vector get_text_size(const Font& font,
        const std::string& content)
        = 0;
    virtual void set_clip_rect(const Rectangle& rectangle) = 0;
    virtual void clear(const Color& color) = 0;
    virtual void present() = 0;
    virtual void register_event_handler(const Event& event,
        std::function<void(const Event&)> handler)
    {
        _event_handlers[event.get_type()] = handler;
    }
    virtual void unregister_event_handler(const Event& event)
    {
        _event_handlers.erase(event.get_type());
    }
    virtual void poll_events() = 0;

    bool is_running() const
    {
        return _is_running;
    }
};

} // namespace guillaume