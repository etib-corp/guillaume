#include "simple_application.hpp"

#include <iostream>

namespace guillaume::simple_application {

SimpleApplication::SimpleApplication(int argc, const char *const argv[],
                                     const char *const envp[])
    : guillaume::Application<SimpleWindow, SimpleRenderer,
                             SimpleEventHandler>() {
    // Store command-line arguments (optional, for future use)
    (void)argc;
    (void)argv;
    (void)envp;

    guillaume::Metadata metadata;
    metadata.setName("Guillaume Simple Application");
    metadata.setVersion("1.0.0");
    metadata.setIdentifier("guillaume.simple");
    metadata.setCreator("Guillaume example");
    metadata.setCopyright("(c) 2026");
    metadata.setUrl("https://example.com");
    metadata.setType("example");
    setMetadata(metadata);

    // Set up event handling
    _eventHandler.setEventCallback(
        [this](guillaume::Event &event) { this->handleEvent(event); });
}

void SimpleApplication::handleEvent(guillaume::Event &event) {
    switch (event.type) {
    case guillaume::EventType::WindowClose: {
        _logger.info("Window close requested");
        // Mark all windows for closure
        break;
    }

    case guillaume::EventType::WindowResize: {
        auto &resizeEvent = dynamic_cast<guillaume::WindowResizeEvent &>(event);
        _logger.info("Window resized to " + std::to_string(resizeEvent.width) +
                     "x" + std::to_string(resizeEvent.height));
        break;
    }

    case guillaume::EventType::AppQuit: {
        _logger.info("Application quit requested");
        break;
    }

    case guillaume::EventType::KeyPressed: {
        auto &keyEvent = dynamic_cast<guillaume::KeyPressedEvent &>(event);
        if (keyEvent.keyCode == guillaume::KeyCode::Escape) {
            _logger.info("Escape key pressed - requesting quit");
        }
        break;
    }

    case guillaume::EventType::MouseButtonPressed: {
        _logger.info("Mouse button pressed");
        break;
    }

    case guillaume::EventType::MouseMoved: {
        // Too verbose, can be commented out
        // auto &mouseEvent = dynamic_cast<guillaume::MouseMovedEvent &>(event);
        // _logger.debug("Mouse moved to " + std::to_string(mouseEvent.x) +
        //               "," + std::to_string(mouseEvent.y));
        break;
    }

    default:
        // Handle other events if needed
        break;
    }
}

int SimpleApplication::run(void) {
    // Add default window if none exists
    if (!hasOpenWindows()) {
        addWindow("main");
        auto &window = getWindow("main");
        window.setTitle("Guillaume - Simple");
    }

    _logger.info("Starting application main loop");

    // Main application loop
    while (hasOpenWindows() && !shouldQuit()) {
        // Poll and handle events
        _eventHandler.pollEvents();

        // Render the main window (access through getWindow instead of _windows)
        if (!shouldQuit() && hasOpenWindows()) {
            auto &window = getWindow("main");
            if (!window.shouldClose()) {
                auto &renderer = window.getRenderer();
                renderer.setDrawColor(utility::Color<uint8_t>(30, 30, 30, 255));
                renderer.clear();
                renderer.present();
            }
        }
    }

    _logger.info("Application exited cleanly");
    return 0;
}

} // namespace guillaume::simple_application