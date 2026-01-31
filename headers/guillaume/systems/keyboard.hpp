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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/text.hpp"

#include "guillaume/event/event_subscriber.hpp"

#include <utility/event/event.hpp>
#include <utility/event/keyboard_event.hpp>

namespace guillaume::systems {

/**
 * @brief System handling clickable entities.
 */
class Keyboard : public ecs::SystemFiller<components::Text> {
  private:
    event::EventSubscriber<utility::event::KeyboardEvent> _keyboardSubscriber;

  public:
    /**
     * @brief Default constructor for the Keyboard system.
     * @param eventBus The event bus to subscribe to.
     */
    Keyboard(event::EventBus &eventBus) : _keyboardSubscriber(eventBus) {}

    /**
     * @brief Default destructor for the Keyboard system.
     */
    ~Keyboard(void) = default;

    /**
     * @brief Update the Keyboard system for the specified entity.
     * @param identityIdentifier The identifier of the entity to update.
     */
    void update(const ecs::Entity::Identifier &identityIdentifier) override {
        info("Updating button entity " +
                         std::to_string(identityIdentifier));
    }
};

} // namespace guillaume::systems