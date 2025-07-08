#pragma once

#include <string>

#include "guillaume/event.hpp"

namespace guigui {

class UnknownEvent : public Event {
public:
    UnknownEvent()
        : Event(EventType::QUIT) // Initialize base event type
    {
    }
    ~UnknownEvent() override = default;
    std::string to_string() const override
    {
        return "UnknownEvent()"; // Convert to string representation
    }
};
} // namespace guigui
