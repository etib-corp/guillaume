#pragma once

#include <string>

namespace guillaume {

enum class EventType {
    KEYBOARD_EVENT,
    MOUSE_EVENT,
};

class Event {
private:
    EventType _type;

protected:
public:
    Event(EventType type)
        : _type(type)
    {
    }
    virtual ~Event() = default;

    EventType get_type() const { return _type; }

    virtual std::string to_string() const
    {
        return "Event(type=" + std::to_string(static_cast<int>(_type)) + ")";
    } // Convert to string representation
};
} // namespace guillaume
