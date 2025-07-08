#pragma once

#include <string>

namespace guigui {

enum class EventType {
    QUIT,
    KEYBOARD_EVENT,
    MOUSE_EVENT,
    UNKNOWN
};

class Eventable {
public:
    virtual ~Eventable() = default;
    virtual EventType get_type() const = 0;
    virtual std::string to_string() const = 0;
};

} // namespace guigui
