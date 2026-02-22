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

#include "guillaume/systems/click.hpp"

namespace guillaume::systems {

Click::Click(event::EventBus &eventBus) : _mouseButtonSubscriber(eventBus) {}

void Click::update(ecs::ComponentRegistry &componentRegistry,
                   const ecs::Entity::Identifier &identityIdentifier) {
    if (!_mouseButtonSubscriber.hasPendingEvents()) {
        return;
    }

    const auto event = _mouseButtonSubscriber.getNextEvent();
    if (!event) {
        return;
    }

    if (!event->isButtonPressed(
            utility::event::MouseButtonEvent::MouseButton::LEFT)) {
        return;
    }

    const auto &transform =
        componentRegistry.getComponent<components::Transform>(
            identityIdentifier);
    const auto &bound =
        componentRegistry.getComponent<components::Bound>(identityIdentifier);

    const auto mousePosition = event->getPosition();
    const auto position = transform.getPosition();
    const auto size = bound.getSize();

    const float halfWidth = size[0] / 2.0f;
    const float halfHeight = size[1] / 2.0f;
    const bool isInside = mousePosition[0] >= position[0] - halfWidth &&
                          mousePosition[0] <= position[0] + halfWidth &&
                          mousePosition[1] >= position[1] - halfHeight &&
                          mousePosition[1] <= position[1] + halfHeight;

    if (!isInside) {
        return;
    }

    const auto &click =
        componentRegistry.getComponent<components::Click>(identityIdentifier);
    const auto onClick = click.getOnClickHandler();
    if (!onClick) {
        return;
    }

    onClick();
}

} // namespace guillaume::systems