#pragma once

#include <memory>

#include "guillaume/renderer.hpp"

namespace guillaume {

class Context {

private:
    std::unique_ptr<Renderer> _renderer;

    void _loop()
    {
        while (_renderer->is_running()) {
            begin();
            process_frame();
            _renderer->present();
            end();
        }
    }

protected:
public:
    Context(std::unique_ptr<Renderer> renderer)
        : _renderer(std::move(renderer))
    {
        _renderer->init("Guillaume");
    }

    ~Context() = default;

    void begin()
    {
        _renderer->poll_events();
    }

    void process_frame()
    {
        _renderer->clear(Color(0, 0, 0, 255)); // Clear with black color
    }

    void end()
    {
    }

    void run()
    {
        _loop();
    }
};
} // namespace guillaume
