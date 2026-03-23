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

#include <functional>
#include <map>
#include <utility/event/mouse_button_event.hpp>
#include <utility/event/mouse_motion_event.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components {

/**
 * @brief Component representing a clickable entity.
 *
 * @code
 * components::Click click;
 * click.setOnClickHandler(utility::event::MouseButtonEvent::MouseButton::Left,
 * [](utility::event::MouseMotionEvent::MousePosition position) {
 *     // Handle click.
 * });
 * @endcode
 *
 * @see systems::Interaction
 */
class Click : public ecs::Component {
  public:
    using Handler = std::function<void(
        utility::event::MouseMotionEvent::MousePosition)>; ///< Click event
                                                           ///< handler type

  private:
    std::map<utility::event::MouseButtonEvent::MouseButton, Handler>
        _onClickHandlers{}; ///< Click event handlers
    std::map<utility::event::MouseButtonEvent::MouseButton, Handler>
        _onReleaseHandlers{}; ///< Release event handlers

    std::map<utility::event::MouseButtonEvent::MouseButton, bool>
        _isClicked{}; ///< Flag indicating if the entity is currently clicked
    std::map<utility::event::MouseButtonEvent::MouseButton, bool>
        _pressedInside{}; ///< Flag indicating if the button was pressed inside
                          ///< the entity bounds

    bool _isEntityClicked{false}; ///< Flag indicating if the entity is
                                  ///< currently clicked (any button)

  public:
    /**
     * @brief Default constructor for the Click component.
     */
    Click(void) {
        for (const auto button :
                         {utility::event::MouseButtonEvent::MouseButton::Left,
                            utility::event::MouseButtonEvent::MouseButton::Middle,
                            utility::event::MouseButtonEvent::MouseButton::Right,
              utility::event::MouseButtonEvent::MouseButton::X1,
              utility::event::MouseButtonEvent::MouseButton::X2}) {
            _isClicked[button] = false;
            _pressedInside[button] = false;
            _onClickHandlers[button] = nullptr;
            _onReleaseHandlers[button] = nullptr;
        }
    };

    /**
     * @brief Default destructor for the Click component.
     */
    ~Click(void) = default;

    /**
     * @brief Set the onClick event handler.
     * @param button The mouse button to associate with the handler.
     * @param handler The function to call on click events.
     * @return Reference to this Click component for chaining.
     */
    Click &setOnClickHandler(
        const utility::event::MouseButtonEvent::MouseButton &button,
        const Handler &handler) {
        _onClickHandlers[button] = handler;
        return *this;
    }

    /**
     * @brief Set the onRelease event handler.
     * @param button The mouse button to associate with the handler.
     * @param handler The function to call on release events.
     * @return Reference to this Click component for chaining.
     */
    Click &setOnReleaseHandler(
        const utility::event::MouseButtonEvent::MouseButton &button,
        const Handler &handler) {
        _onReleaseHandlers[button] = handler;
        return *this;
    }

    /**
     * @brief Get the onClick event handler.
     * @return A map of mouse buttons to their associated onClick event
     * handlers.
     */
    const std::map<utility::event::MouseButtonEvent::MouseButton, Handler> &
    getOnClickHandlers() const {
        return _onClickHandlers;
    }

    /**
     * @brief Get the onRelease event handler.
     * @return A map of mouse buttons to their associated onRelease event
     * handlers.
     */
    const std::map<utility::event::MouseButtonEvent::MouseButton, Handler> &
    getOnReleaseHandlers() const {
        return _onReleaseHandlers;
    }

    /**
     * @brief Check if the entity is currently clicked.
     * @param button The mouse button to check.
     * @return True if the entity is clicked, false otherwise.
     */
    bool isClicked(
        const utility::event::MouseButtonEvent::MouseButton &button) const {
        return _isClicked.at(button);
    }

    /**
     * @brief Set the clicked state of the entity.
     * @param button The mouse button to set.
     * @param clicked True if the entity is clicked, false otherwise.
     * @return Reference to this Click component for chaining.
     */
    Click &
    setClicked(const utility::event::MouseButtonEvent::MouseButton &button,
               bool clicked) {
        _isEntityClicked = clicked;
        _isClicked[button] = clicked;
        return *this;
    }

    /**
     * @brief Check if the entity is currently clicked (any button).
     * @return True if the entity is clicked, false otherwise.
     */
    bool isEntityClicked() const { return _isEntityClicked; }

    /**
     * @brief Set whether a button was pressed inside the entity bounds.
     * @param button The mouse button.
     * @param pressedInside True if the button was pressed inside, false
     * otherwise.
     * @return Reference to this Click component for chaining.
     */
    Click &setPressedInside(
        const utility::event::MouseButtonEvent::MouseButton &button,
        bool pressedInside) {
        _pressedInside[button] = pressedInside;
        return *this;
    }

    /**
     * @brief Check if a button was pressed inside the entity bounds.
     * @param button The mouse button.
     * @return True if the button was pressed inside, false otherwise.
     */
    bool isPressedInside(
        const utility::event::MouseButtonEvent::MouseButton &button) const {
        return _pressedInside.at(button);
    }
};

} // namespace guillaume::components