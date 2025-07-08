#pragma once

#include <map>
#include <string>

#include "guillaume/event.hpp"
#include "guillaume/vector.hpp"

namespace guigui {

// Mouse event - represents a mouse event with position and button state
class MouseEvent : public Event {
public:
    enum class MouseEventType {
        MOUSE_MOTION, // Mouse movement event
        MOUSE_BUTTON_PRESS, // Mouse button press event
        MOUSE_BUTTON_RELEASE, // Mouse button release event
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

        NONE // No button (used for mouse motion events)
    };

private:
    MouseEventType _type; // Type of mouse event
    MouseButton _button; // Mouse button state
    Vector _position; // Mouse position

    std::map<MouseEventType, std::string> _mouse_event_type_map = {
        { MouseEventType::MOUSE_MOTION, "Mouse Motion" },
        { MouseEventType::MOUSE_BUTTON_PRESS, "Mouse Button Press" },
        { MouseEventType::MOUSE_BUTTON_RELEASE, "Mouse Button Release" }
    };

    std::map<MouseButton, std::string> _mouse_button_map = {
        { MouseButton::LEFT, "Left Button" },
        { MouseButton::RIGHT, "Right Button" },
        { MouseButton::MIDDLE, "Middle Button" },
        { MouseButton::SIDE_1, "Side Button 1" },
        { MouseButton::SIDE_2, "Side Button 2" },
        { MouseButton::WHEEL_UP, "Wheel Up" },
        { MouseButton::WHEEL_DOWN, "Wheel Down" },
        { MouseButton::WHEEL_LEFT, "Wheel Left" },
        { MouseButton::WHEEL_RIGHT, "Wheel Right" },
        { MouseButton::NONE, "No Button" }
    };

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
        std::string string_representation = "MouseEvent(";
        string_representation += "Type: " + _mouse_event_type_map.at(_type) + ", ";
        string_representation += "Button: " + _mouse_button_map.at(_button) + ", ";
        string_representation += "Position: " + _position.to_string() + ")";
        return string_representation;

    } // Convert to string representation
    Vector get_position() const { return _position; } // Get mouse position
    MouseButton get_button() const { return _button; } // Get mouse button state
    MouseEventType get_event_type() const { return _type; } // Get mouse event type
};

} // namespace guigui