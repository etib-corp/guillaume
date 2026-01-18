#pragma once

#include <application.hpp>

#include "simple_event_handler.hpp"
#include "simple_renderer.hpp"
#include "simple_window.hpp"

namespace guillaume::simple_application {

class SimpleApplication
    : public guillaume::Application<SimpleWindow, SimpleRenderer,
                                    SimpleEventHandler> {
  private:
    /**
     * @brief Handle incoming events.
     * @param event The event to handle.
     */
    void handleEvent(guillaume::Event &event);

  public:
    SimpleApplication(int argc, const char *const argv[],
                      const char *const envp[]);
    ~SimpleApplication(void) override = default;

    /**
     * @brief Run the application main loop.
     * @return Exit code (0 for success, non-zero for error).
     */
    int run(void);
};

} // namespace guillaume::simple_application
