#pragma once

#include "guillaume/event.hpp"

namespace guigui {

// Keyboard event - represents a keyboard event with key code and modifiers
class KeyboardEvent : public Event {
    enum class KeyType {
        KEY_PRESS, // Key pressed
        KEY_RELEASE, // Key released
    };

    enum class KeyCode {
        // Alphanumeric keys
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        NUM_0,
        NUM_1,
        NUM_2,
        NUM_3,
        NUM_4,
        NUM_5,
        NUM_6,
        NUM_7,
        NUM_8,
        NUM_9,

        // Function keys
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        // Navigation keys
        UP,
        DOWN,
        LEFT,
        RIGHT,

        // Other keys
        SPACE,
        ENTER,
        ESC,
        TAB,
        BACKSPACE,
        INSERT,
        DELETE,
        HOME,
        END,
        PAGE_UP,
        PAGE_DOWN,

        // Numpad keys
        NP_0,
        NP_1,
        NP_2,
        NP_3,
        NP_4,
        NP_5,
        NP_6,
        NP_7,
        NP_8,
        NP_9,
        NP_ADD,
        NP_SUBTRACT,
        NP_MULTIPLY,
        NP_DIVIDE,
        NP_DECIMAL,
        NP_ENTER

    };

    enum class KeyModifiers {
        CTRL = 1 << 0, // Control key
        ALT = 1 << 1, // Alt key
        SHIFT = 1 << 2, // Shift key
        SUPER = 1 << 3, // Super key (Windows key or Command key on macOS)
    };

private:
    KeyType _key_type; // Type of keyboard event (key down or key up)
    KeyCode _key_code; // Key code representing the pressed key
    KeyModifiers _key_modifiers; // Modifiers applied during the key event
public:
    KeyboardEvent(KeyType key_type, KeyCode key_code, KeyModifiers key_modifiers)
        : Event(EventType::KEYBOARD_EVENT)
        , // Initialize base event type
        _key_type(key_type)
        , // Set the key type
        _key_code(key_code)
        , // Set the key code
        _key_modifiers(key_modifiers) // Set the modifiers
    {
    }
    ~KeyboardEvent() override = default;
    std::string to_string() const override
    {
        return "KeyboardEvent(type="
            + std::to_string(static_cast<int>(get_type())) + ", key_type="
            + std::to_string(static_cast<int>(_key_type)) + ", key_code="
            + std::to_string(static_cast<int>(_key_code)) + ", modifiers="
            + std::to_string(static_cast<int>(_key_modifiers)) + ")";
    } // Convert to string representation
    KeyType get_key_type() const { return _key_type; } // Get the type
    KeyCode get_key_code() const { return _key_code; } // Get the key
    KeyModifiers get_key_modifiers() const { return _key_modifiers; } // Get the modifiers
};

} // namespace guigui