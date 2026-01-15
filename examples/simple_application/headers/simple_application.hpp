#pragma once

#include <application.hpp>

#include "simple_event_handler.hpp"
#include "simple_renderer.hpp"
#include "simple_window.hpp"

namespace guillaume::simple_application {

class SimpleApplication
    : public guillaume::Application<SimpleWindow, SimpleRenderer,
                                    SimpleEventHandler> {
  public:
    SimpleApplication(void);
    ~SimpleApplication(void) override = default;

    /**
     * @brief Initialize the application with components and event handling.
     */
    void initialize(void);

    /**
     * @brief Run the application main loop.
     */
    void run(void);
};

} // namespace guillaume::simple_application
