#pragma once

#include "guillaume/eventable.hpp"

namespace guigui {

class Event : public Eventable {
protected:
    EventType _type;

public:
    virtual ~Event() = default;

    Event(EventType type)
        : _type(type)
    {
    }

    EventType get_type() const override { return _type; }

    virtual std::string to_string() const override = 0;
};

} // namespace guigui
