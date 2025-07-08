#pragma once

#include "guillaume/event.hpp"
#include "guillaume/vector.hpp"

namespace guigui {

// Mouse event - represents a mouse event with position and button state
class MouseEvent : public Event {
    enum class MouseEventType {
        MOUSE_MOTION, // Mouse movement event
        MOUSE_BUTTON_PRESS, // Mouse button press event
        MOUSE_BUTTON_RELEASE, // Mouse button release event
        MOUSE_DEVICE_EVENT, // Mouse device event (e.g., mouse connected/disconnected)
    };

    enum class MouseButton {
        // Mouse buttons
        LEFT, // Left mouse button
        RIGHT, // Right mouse button
        MIDDLE, // Middle mouse button (wheel press)
        SIDE_1, // Additional side button 1 (often "back" button)
        SIDE_2, // Additional side button 2 (often "forward" button)

        WHEEL_UP, // Scroll wheel up
        WHEEL_DOWN, // Scroll wheel down
        WHEEL_LEFT, // Tilt wheel left (if supported)
        WHEEL_RIGHT, // Tilt wheel right (if supported)
    };

private:
    MouseEventType _type; // Type of mouse event
    MouseButton _button; // Mouse button state
    Vector _position; // Mouse position

public:
    MouseEvent(const MouseEventType& type, const MouseButton& button,
        const Vector& position)
        : Event(EventType::MOUSE_EVENT)
        , // Initialize base event type
        _type(type)
        , // Set the mouse event type
        _button(button)
        , // Set the mouse button state
        _position(position) // Set the mouse position
    {
    }
    ~MouseEvent() override = default;
    std::string to_string() const override
    {
        return "MouseEvent(type=" + std::to_string(static_cast<int>(get_type()))
            + ", position=(" + std::to_string(_position.get_x()) + ", "
            + std::to_string(_position.get_y()) + "), button="
            + std::to_string(static_cast<int>(_button)) + ", event_type="
            + std::to_string(static_cast<int>(_type)) + ")";

    } // Convert to string representation
    Vector get_position() const { return _position; } // Get mouse position
    MouseButton get_button() const { return _button; } // Get mouse button state
    MouseEventType get_event_type() const { return _type; } // Get mouse event type
};

} // namespace guigui