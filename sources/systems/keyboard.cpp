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

#include "guillaume/systems/keyboard.hpp"

#include <cctype>

namespace guillaume::systems {

Keyboard::Keyboard(event::EventBus &eventBus) : _keyboardSubscriber(eventBus) {}

void Keyboard::update(ecs::ComponentRegistry &componentRegistry,
                      const ecs::Entity::Identifier &identityIdentifier) {
    if (!_keyboardSubscriber.hasPendingEvents()) {
        return;
    }

    const auto event = _keyboardSubscriber.getNextEvent();
    if (!event || !event->getIsDownEvent()) {
        return;
    }

    auto &text =
        componentRegistry.getComponent<components::Text>(identityIdentifier);
    std::string content = text.getContent();

    const auto keycode = event->getKeycode();
    const bool isShiftPressed =
        event->isModifierSet(
            utility::event::KeyboardEvent::KeyModifiers::LSHIFT) ||
        event->isModifierSet(
            utility::event::KeyboardEvent::KeyModifiers::RSHIFT);

    if (keycode == utility::event::KeyboardEvent::KeyCode::BACKSPACE) {
        if (!content.empty()) {
            content.pop_back();
            text.setContent(content);
        }
        return;
    }

    if (keycode == utility::event::KeyboardEvent::KeyCode::SPACE) {
        text.setContent(content + " ");
        return;
    }

    const auto keycodeValue = static_cast<std::int32_t>(keycode);
    if (keycodeValue >= static_cast<std::int32_t>(
                            utility::event::KeyboardEvent::KeyCode::A) &&
        keycodeValue <= static_cast<std::int32_t>(
                            utility::event::KeyboardEvent::KeyCode::Z)) {
        char character = static_cast<char>(keycodeValue);
        if (!isShiftPressed) {
            character = static_cast<char>(std::tolower(character));
        }
        text.setContent(content + character);
        return;
    }

    if (keycodeValue >= static_cast<std::int32_t>(
                            utility::event::KeyboardEvent::KeyCode::NUM_0) &&
        keycodeValue <= static_cast<std::int32_t>(
                            utility::event::KeyboardEvent::KeyCode::NUM_9)) {
        const char character = static_cast<char>(keycodeValue);
        text.setContent(content + character);
    }
}

} // namespace guillaume::systems