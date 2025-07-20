#include "guillaume/context.hpp"
#include <iostream>
#include <chrono>
#include <thread>

namespace guigui {

Context::Context(std::shared_ptr<Renderer> renderer)
    : _renderer(std::move(renderer))
{
    LOG_INFO("Initializing Guillaume context...");
    
    _renderer->init("Guillaume");
    LOG_DEBUG("Renderer initialized");
    
    _renderer->register_event_handler(
        [this](std::unique_ptr<Eventable> event) {
            if (auto quit_event = dynamic_cast<QuitEvent*>(event.get())) {
                LOG_INFO("Quit event received - shutting down application");
                _renderer->set_running(false);
            }
        },
        Eventable::EventType::QUIT);

    _renderer->register_event_handler(
        [this](std::unique_ptr<Eventable> event) {
            if (auto keyboard_event = dynamic_cast<KeyboardEvent*>(event.get())) {
                LOG_DEBUG_F("Keyboard event received: {}", keyboard_event->to_string());
            }
        },
        Eventable::EventType::KEYBOARD_EVENT);

    _renderer->register_event_handler(
        [this](std::unique_ptr<Eventable> event) {
            if (auto mouse_event = dynamic_cast<MouseEvent*>(event.get())) {
                LOG_DEBUG_F("Mouse event received: {}", mouse_event->to_string());
                
                // Forward mouse event to root component
                if (_root_component) {
                    _root_component->handle_mouse_event(*mouse_event);
                }
            }
        },
        Eventable::EventType::MOUSE_EVENT);

    _renderer->register_event_handler(
        [this](std::unique_ptr<Eventable> event) {
            if (auto unknown_event = dynamic_cast<UnknownEvent*>(event.get())) {
                LOG_WARN_F("Unknown event received: {}", unknown_event->to_string());
            }
        },
        Eventable::EventType::UNKNOWN);
        
    _default_font = std::make_unique<Font>("Roboto", "assets/Roboto.ttf", 48);
    LOG_DEBUG("Default font loaded");
    
    LOG_INFO("Guillaume context initialization complete");
}

void Context::set_root_component(std::unique_ptr<Container> root_component)
{
    LOG_DEBUG("Setting root component");
    _root_component = std::move(root_component);
    _root_component->set_renderer(_renderer);
    
    // Set up dirty callback to mark context for redraw when component changes
    _root_component->set_dirty_callback([this]() {
        mark_dirty();
    });
    
    // Initial redraw needed after setting root component
    _needs_redraw = true;
    
    LOG_DEBUG("Root component set successfully");
}

std::unique_ptr<Container> Context::get_root_component()
{
    return std::move(_root_component);
}

void Context::begin()
{
    _renderer->poll_events();
    while (_renderer->has_event()) {
        _renderer->handle_event(_renderer->pop_event());
        // Mark as dirty when events are processed
        _needs_redraw = true;
    }
    
    // Only clear if we need to redraw
    if (_needs_redraw) {
        _renderer->clear(Color(0, 0, 0, 255)); // Clear with black color
    }
}

void Context::process_frame()
{
    // Only process frame if we need to redraw
    if (_needs_redraw && _root_component) {
        _root_component->draw();
    }
}

void Context::end()
{
    // Only present if we need to redraw
    if (_needs_redraw) {
        _renderer->present();
        _needs_redraw = false; // Reset the dirty flag
    }
}

void Context::run()
{
    _loop();
}

void Context::_loop()
{
    LOG_INFO("Starting main application loop with frame-on-demand rendering");
    int frame_count = 0;
    int rendered_frame_count = 0;
    
    while (_renderer->is_running()) {
        frame_count++;
        if (frame_count % 1000 == 0) {
            LOG_DEBUG_F("Processed {} frames ({} rendered) - {:.1f}% frame skip", 
                       frame_count, rendered_frame_count,
                       100.0 * (frame_count - rendered_frame_count) / frame_count);
        }
        
        begin();
        process_frame();
        if (_needs_redraw) {
            rendered_frame_count++;
        }
        end();
        
        // Small sleep to prevent excessive CPU usage when no rendering is needed
        if (!_needs_redraw) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }
    
    LOG_INFO_F("Main loop ended after {} frames ({} rendered) - {:.1f}% efficiency", 
               frame_count, rendered_frame_count,
               100.0 * rendered_frame_count / frame_count);
}

void Context::mark_dirty()
{
    _needs_redraw = true;
}

bool Context::needs_redraw() const
{
    return _needs_redraw;
}

} // namespace guigui
