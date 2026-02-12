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

#include "guillaume/ecs.hpp"

namespace guillaume {

ECS::ECS(event::EventBus &eventBus, Renderer &renderer)
    : _eventBus(eventBus), _renderer(renderer) {
    this->getLogger().info("Initializing ECS with core systems");
    registerSystem<systems::Click>(std::make_unique<systems::Click>(_eventBus));
    registerSystem<systems::Hover>(std::make_unique<systems::Hover>(_eventBus));
    registerSystem<systems::Render>(
        std::make_unique<systems::Render>(_renderer));
    registerSystem<systems::Keyboard>(
        std::make_unique<systems::Keyboard>(_eventBus));
    this->getLogger().info("ECS initialization complete");
}

} // namespace guillaume