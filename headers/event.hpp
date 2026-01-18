/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <cstdint>
#include <string>

#include <vector.hpp>

namespace guillaume {

/**
 * @brief Event types supported by Guillaume.
 */
enum class EventType {
    None = 0,

    // Window events
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    WindowMinimized,
    WindowMaximized,
    WindowRestored,

    // Keyboard events
    KeyPressed,
    KeyReleased,
    KeyTyped,

    // Mouse events
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
    MouseEntered,
    MouseLeft,

    // Application events
    AppTick,
    AppUpdate,
    AppRender,
    AppQuit
};

/**
 * @brief Mouse button codes.
 */
enum class MouseButton { Left = 0, Middle = 1, Right = 2, X1 = 3, X2 = 4 };

/**
 * @brief Key codes.
 */
enum class KeyCode {
    Unknown = 0,

    // Alphanumeric keys
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,

    Num0 = 48,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    Semicolon = 59,
    Equal = 61,

    A = 65,
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

    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,

    // Function keys
    Escape = 256,
    Enter,
    Tab,
    Backspace,
    Insert,
    Delete,
    Right,
    Left,
    Down,
    Up,
    PageUp,
    PageDown,
    Home,
    End,

    CapsLock = 280,
    ScrollLock,
    NumLock,
    PrintScreen,
    Pause,

    F1 = 290,
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
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,

    // Keypad
    KP0 = 320,
    KP1,
    KP2,
    KP3,
    KP4,
    KP5,
    KP6,
    KP7,
    KP8,
    KP9,
    KPDecimal,
    KPDivide,
    KPMultiply,
    KPSubtract,
    KPAdd,
    KPEnter,
    KPEqual,

    // Modifiers
    LeftShift = 340,
    LeftControl,
    LeftAlt,
    LeftSuper,
    RightShift,
    RightControl,
    RightAlt,
    RightSuper,
    Menu
};

/**
 * @brief Base event class.
 */
class Event {
  public:
    EventType type;       ///< The type of the event.
    bool handled = false; ///< Whether the event has been handled.

    /**
     * @brief Construct an event with the specified type.
     * @param type The event type.
     */
    explicit Event(EventType type) : type(type) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~Event() = default;

    /**
     * @brief Get a string representation of the event.
     * @return String representation of the event.
     */
    virtual std::string toString() const = 0;
};

/**
 * @brief Window close event.
 */
class WindowCloseEvent : public Event {
  public:
    /**
     * @brief Construct a window close event.
     */
    WindowCloseEvent() : Event(EventType::WindowClose) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override { return "WindowCloseEvent"; }
};

/**
 * @brief Window resize event.
 */
class WindowResizeEvent : public Event {
  public:
    std::size_t width;  ///< The new window width.
    std::size_t height; ///< The new window height.

    /**
     * @brief Construct a window resize event.
     * @param width The new window width.
     * @param height The new window height.
     */
    WindowResizeEvent(std::size_t width, std::size_t height)
        : Event(EventType::WindowResize), width(width), height(height) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override {
        return "WindowResizeEvent: " + std::to_string(width) + "x" +
               std::to_string(height);
    }
};

/**
 * @brief Key pressed event.
 */
class KeyPressedEvent : public Event {
  public:
    KeyCode keyCode; ///< The key code of the pressed key.
    bool repeat;     ///< Whether this is a repeat event.

    /**
     * @brief Construct a key pressed event.
     * @param keyCode The key code of the pressed key.
     * @param repeat Whether this is a key repeat event.
     */
    KeyPressedEvent(KeyCode keyCode, bool repeat = false)
        : Event(EventType::KeyPressed), keyCode(keyCode), repeat(repeat) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override {
        return "KeyPressedEvent: " + std::to_string(static_cast<int>(keyCode)) +
               (repeat ? " (repeat)" : "");
    }
};

/**
 * @brief Key released event.
 */
class KeyReleasedEvent : public Event {
  public:
    KeyCode keyCode; ///< The key code of the released key.

    /**
     * @brief Construct a key released event.
     * @param keyCode The key code of the released key.
     */
    explicit KeyReleasedEvent(KeyCode keyCode)
        : Event(EventType::KeyReleased), keyCode(keyCode) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override {
        return "KeyReleasedEvent: " + std::to_string(static_cast<int>(keyCode));
    }
};

/**
 * @brief Mouse button pressed event.
 */
class MouseButtonPressedEvent : public Event {
  public:
    MouseButton button; ///< The button that was pressed.

    /**
     * @brief Construct a mouse button pressed event.
     * @param button The mouse button that was pressed.
     */
    explicit MouseButtonPressedEvent(MouseButton button)
        : Event(EventType::MouseButtonPressed), button(button) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override {
        return "MouseButtonPressedEvent: " +
               std::to_string(static_cast<int>(button));
    }
};

/**
 * @brief Mouse button released event.
 */
class MouseButtonReleasedEvent : public Event {
  public:
    MouseButton button; ///< The button that was released.

    /**
     * @brief Construct a mouse button released event.
     * @param button The mouse button that was released.
     */
    explicit MouseButtonReleasedEvent(MouseButton button)
        : Event(EventType::MouseButtonReleased), button(button) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override {
        return "MouseButtonReleasedEvent: " +
               std::to_string(static_cast<int>(button));
    }
};

/**
 * @brief Mouse moved event.
 */
class MouseMovedEvent : public Event {
  public:
    float x; ///< X coordinate.
    float y; ///< Y coordinate.

    /**
     * @brief Construct a mouse moved event.
     * @param x The x position.
     * @param y The y position.
     */
    MouseMovedEvent(float x, float y)
        : Event(EventType::MouseMoved), x(x), y(y) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override {
        return "MouseMovedEvent: (" + std::to_string(x) + ", " +
               std::to_string(y) + ")";
    }
};

/**
 * @brief Mouse scrolled event.
 */
class MouseScrolledEvent : public Event {
  public:
    float xOffset; ///< X scroll offset.
    float yOffset; ///< Y scroll offset.

    /**
     * @brief Construct a mouse scrolled event.
     * @param xOffset The horizontal scroll offset.
     * @param yOffset The vertical scroll offset.
     */
    MouseScrolledEvent(float xOffset, float yOffset)
        : Event(EventType::MouseScrolled), xOffset(xOffset), yOffset(yOffset) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override {
        return "MouseScrolledEvent: (" + std::to_string(xOffset) + ", " +
               std::to_string(yOffset) + ")";
    }
};

/**
 * @brief Application quit event.
 */
class AppQuitEvent : public Event {
  public:
    /**
     * @brief Construct an application quit event.
     */
    AppQuitEvent() : Event(EventType::AppQuit) {}

    /**
     * @brief Get string representation.
     * @return String representation of the event.
     */
    std::string toString() const override { return "AppQuitEvent"; }
};

} // namespace guillaume
