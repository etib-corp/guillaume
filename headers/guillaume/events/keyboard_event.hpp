#pragma once

#include <map>
#include <string>
#include <vector>

#include "guillaume/event.hpp"

namespace guigui {

// Keyboard event - represents a keyboard event with key code and modifiers
class KeyboardEvent : public Event {
public:
    enum class KeyType {
        KEY_PRESS, // Key pressed
        KEY_RELEASE, // Key released
    };

    enum class KeyCode {
        NONE, // No key (used when no key is pressed)
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
        CTRL, // Control key
        ALT, // Alt key
        SHIFT, // Shift key
        SUPER // Super key (Windows key on Windows, Command key on macOS)
    };

    std::map<KeyType, std::string> _key_type_map = {
        { KeyType::KEY_PRESS, "Key Press" },
        { KeyType::KEY_RELEASE, "Key Release" }
    };

    std::map<KeyCode, std::string> _key_code_map {
        { KeyCode::NONE, "None" },
        { KeyCode::A, "A" },
        { KeyCode::B, "B" },
        { KeyCode::C, "C" },
        { KeyCode::D, "D" },
        { KeyCode::E, "E" },
        { KeyCode::F, "F" },
        { KeyCode::G, "G" },
        { KeyCode::H, "H" },
        { KeyCode::I, "I" },
        { KeyCode::J, "J" },
        { KeyCode::K, "K" },
        { KeyCode::L, "L" },
        { KeyCode::M, "M" },
        { KeyCode::N, "N" },
        { KeyCode::O, "O" },
        { KeyCode::P, "P" },
        { KeyCode::Q, "Q" },
        { KeyCode::R, "R" },
        { KeyCode::S, "S" },
        { KeyCode::T, "T" },
        { KeyCode::U, "U" },
        { KeyCode::V, "V" },
        { KeyCode::W, "W" },
        { KeyCode::X, "X" },
        { KeyCode::Y, "Y" },
        { KeyCode::Z, "Z" },

        // Numeric keys
        { KeyCode::NUM_0, "0" },
        { KeyCode::NUM_1, "1" },
        { KeyCode::NUM_2, "2" },
        { KeyCode::NUM_3, "3" },
        { KeyCode::NUM_4, "4" },
        { KeyCode::NUM_5, "5" },
        { KeyCode::NUM_6, "6" },
        { KeyCode::NUM_7, "7" },
        { KeyCode::NUM_8, "8" },
        { KeyCode::NUM_9, "9" },

        // Function keys
        { KeyCode::F1, "F1" },
        { KeyCode::F2, "F2" },
        { KeyCode::F3, "F3" },
        { KeyCode::F4, "F4" },
        { KeyCode::F5, "F5" },
        { KeyCode::F6, "F6" },
        { KeyCode::F7, "F7" },
        { KeyCode::F8, "F8" },
        { KeyCode::F9, "F9" },
        { KeyCode::F10, "F10" },
        { KeyCode::F11, "F11" },
        { KeyCode::F12, "F12" },

        // Navigation keys
        { KeyCode::UP, "Up" },
        { KeyCode::DOWN, "Down" },
        { KeyCode::LEFT, "Left" },
        { KeyCode::RIGHT, "Right" },

        // Other keys
        { KeyCode::SPACE, "Space" },
        { KeyCode::ENTER, "Enter" },
        { KeyCode::ESC, "Escape" },
        { KeyCode::TAB, "Tab" },
        { KeyCode::BACKSPACE, "Backspace" },
        { KeyCode::INSERT, "Insert" },
        { KeyCode::DELETE, "Delete" },
        { KeyCode::HOME, "Home" },
        { KeyCode::END, "End" },
        { KeyCode::PAGE_UP, "Page Up" },
        { KeyCode::PAGE_DOWN, "Page Down" },

        // Numpad keys
        { KeyCode::NP_0, "Numpad 0" },
        { KeyCode::NP_1, "Numpad 1" },
        { KeyCode::NP_2, "Numpad 2" },
        { KeyCode::NP_3, "Numpad 3" },
        { KeyCode::NP_4, "Numpad 4" },
        { KeyCode::NP_5, "Numpad 5" },
        { KeyCode::NP_6, "Numpad 6" },
        { KeyCode::NP_7, "Numpad 7" },
        { KeyCode::NP_8, "Numpad 8" },
        { KeyCode::NP_9, "Numpad 9" },
        { KeyCode::NP_ADD, "Numpad Add" },
        { KeyCode::NP_SUBTRACT, "Numpad Subtract" },
        { KeyCode::NP_MULTIPLY, "Numpad Multiply" },
        { KeyCode::NP_DIVIDE, "Numpad Divide" },
        { KeyCode::NP_DECIMAL, "Numpad Decimal" },
        { KeyCode::NP_ENTER, "Numpad Enter" }
    };

    std::map<KeyModifiers, std::string> _key_modifiers_map = {
        { KeyModifiers::CTRL, "Control" },
        { KeyModifiers::ALT, "Alt" },
        { KeyModifiers::SHIFT, "Shift" },
        { KeyModifiers::SUPER, "Super" }
    };

private:
    KeyType _key_type; // Type of keyboard event (key down or key up)
    KeyCode _key_code; // Key code representing the pressed key
    std::vector<KeyModifiers> _key_modifiers; // Modifiers applied to the key event

public:
    KeyboardEvent(KeyType key_type, KeyCode key_code, std::vector<KeyModifiers> key_modifiers)
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
        std::string string_representation;

        for (const auto& modifier : _key_modifiers) {
            if (!string_representation.empty()) {
                string_representation += " + "; // Add separator for multiple modifiers
            }
            string_representation += _key_modifiers_map.at(modifier); // Convert modifier to string
        }
        string_representation += " " + _key_type_map.at(_key_type) + " " + _key_code_map.at(_key_code);
        return string_representation; // Convert to string representation
    } // Convert to string representation
    KeyType get_key_type() const { return _key_type; } // Get the type
    KeyCode get_key_code() const { return _key_code; } // Get the key
    std::vector<KeyModifiers> get_key_modifiers() const
    {
        return _key_modifiers; // Get the modifiers
    } // Get the key modifiers
};

} // namespace guigui