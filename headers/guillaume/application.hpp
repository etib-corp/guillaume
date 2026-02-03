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

#include <map>
#include <memory>
#include <string>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include "ecs.hpp"
#include "metadata.hpp"
#include "renderer.hpp"

#include "event/event_bus.hpp"
#include "event/event_handler.hpp"

namespace guillaume {

/**
 * @brief Application base class.
 *
 * @tparam RendererType The type of the renderer used by the application.
 * @tparam EventHandlerType The type of the event handler used by the
 * application.
 */
template <typename RendererType, typename EventHandlerType>
class Application;

template <typename RendererType, typename EventHandlerType>
class Application : protected utility::logging::Loggable<
                        Application<RendererType, EventHandlerType>,
                        utility::logging::StandardLogger> {
  private:
    RendererType _renderer;         ///< Main application renderer
    EventHandlerType _eventHandler; ///< Application event handler
    event::EventBus _eventBus;      ///< Event bus dispatching to systems
    std::unique_ptr<ECS> _ecs;      ///< ECS instance

  public:
    /**
     * @brief Default constructor
     */
    Application(void) : _eventHandler(), _eventBus(), _ecs(nullptr) {
        _ecs = std::make_unique<ECS>(_eventBus, _renderer);
        _eventHandler.setEventCallback(
            [this](std::unique_ptr<utility::event::Event> &event) {
                this->_eventBus.publish(std::move(event));
            });
    }

    /**
     * @brief Default destructor
     */
    virtual ~Application(void) = default;

    /**
     * @brief Get the ECS instance.
     * @return Reference to the ECS instance.
     */
    ECS &getECS(void) { return *_ecs; }

    /**
     * @brief Run the application main loop.
     * @return Exit code.
     */
    int run(void) {
        this->getLogger().info("Entering main loop");
        while (!_eventHandler.shouldQuit()) {
            try {
                _eventHandler.pollEvents();
                if (!_eventHandler.gotNewEvents()) {
                    continue;
                }
                _ecs->routine();
                this->getLogger().debug("Processed a frame");
            } catch (const std::exception &exception) {
                this->getLogger().error(std::string("Application error: ") +
                                        exception.what());
                return EXIT_FAILURE;
            }
        }
        this->getLogger().info("Exiting main loop");
        return EXIT_SUCCESS;
    }
};

} // namespace guillaume