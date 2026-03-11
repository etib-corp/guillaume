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

#include "guillaume/systems/render.hpp"


#include "guillaume/components/click.hpp"
// TODO #include "guillaume/components/focus.hpp"
#include "guillaume/components/hover.hpp"
#include "guillaume/components/render.hpp"
#include <utility/event/mouse_button_event.hpp>

namespace guillaume::systems {

void Render::update(ecs::ComponentRegistry &componentRegistry, const ecs::Entity::Identifier &identityIdentifier) {
    if (!componentRegistry.hasComponent<components::Render>(identityIdentifier)) {
        getLogger().warning("Entity " + std::to_string(identityIdentifier) + " does not have a Render component");
        return;
    }

    guillaume::components::Render &renderComponent =
        componentRegistry.getComponent<components::Render>(identityIdentifier);

    if (componentRegistry.hasComponent<components::Click>(identityIdentifier) && renderComponent.getClickedHandler()) {
        if (componentRegistry.getComponent<components::Click>(identityIdentifier).isEntityClicked()) {
            renderComponent.getClickedHandler()(componentRegistry, identityIdentifier, _renderer);
            return;
        }
    }

    if (componentRegistry.hasComponent<components::Hover>(identityIdentifier) && renderComponent.getHoveredHandler()) {
        if (componentRegistry.getComponent<components::Hover>(identityIdentifier).isHovered()) {
            renderComponent.getHoveredHandler()(componentRegistry, identityIdentifier, _renderer);
            return;
        }
    }

    if (renderComponent.getNormalHandler()) {
        renderComponent.getNormalHandler()(componentRegistry, identityIdentifier, _renderer);
        return;
    }
    getLogger().warning("Entity " + std::to_string(identityIdentifier) + " has Render component but no handlers defined");
}
} // namespace guillaume::systems