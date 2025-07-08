#pragma once

#include <string>

#include "guillaume/event.hpp"

namespace guigui {

class QuitEvent : public Event {
public:
    QuitEvent()
        : Event(EventType::QUIT) // Initialize base event type
    {
    }
    ~QuitEvent() override = default;
    std::string to_string() const override
    {
        return "QuitEvent()"; // Convert to string representation
    }
};
} // namespace guigui
