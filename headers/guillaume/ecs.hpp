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

#include "guillaume/ecs/ecs.hpp"

#include "guillaume/component_registry.hpp"
#include "guillaume/renderer.hpp"

#include "guillaume/event/event_bus.hpp"

#include "guillaume/systems/click.hpp"
#include "guillaume/systems/hover.hpp"
#include "guillaume/systems/keyboard.hpp"
#include "guillaume/systems/render.hpp"

namespace guillaume {

/**
 * @brief Main ECS class using the default ComponentRegistry.
 */
class ECS : public ecs::ECS<ComponentRegistry> {
  private:
    event::EventBus &_eventBus;
    Renderer &_renderer;

    /**
     * @brief Utility to register all core systems.
     * @tparam SystemTypes The system types to register.
     */
    template <ecs::InheritFromSystem SystemTypes>
    void registerSystem(std::unique_ptr<SystemTypes> system) {
        _systemRegistry.registerNewSystem<SystemTypes>(std::move(system));
    }

  public:
    /**
     * @brief Default constructor.
     * @param eventBus The event bus to use for event dispatching.
     * @param renderer The renderer to use for rendering operations.
     */
    ECS(event::EventBus &eventBus, Renderer &renderer)
        : _eventBus(eventBus), _renderer(renderer) {
        registerSystem<systems::Click>(
            std::make_unique<systems::Click>(_eventBus));
        registerSystem<systems::Hover>(
            std::make_unique<systems::Hover>(_eventBus));
        registerSystem<systems::Render>(
            std::make_unique<systems::Render>(_renderer));
        registerSystem<systems::Keyboard>(
            std::make_unique<systems::Keyboard>(_eventBus));
    }

    /**
     * @brief Default destructor.
     */
    ~ECS(void) = default;
};

} // namespace guillaume