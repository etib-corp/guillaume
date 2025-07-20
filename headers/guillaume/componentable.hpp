#pragma once

#include "guillaume/renderer.hpp"
#include "guillaume/events/mouse_event.hpp"

#include <memory>
#include <functional>

namespace guigui {

using ComponentIdentifier = std::string;
using DirtyCallback = std::function<void()>;

class Componentable {
private:
    DirtyCallback _dirty_callback;

protected:
    void _mark_dirty();

public:
    virtual ~Componentable() = default;
    virtual ComponentIdentifier get_identifier() const = 0;
    virtual void draw() = 0;
    virtual void set_renderer(std::shared_ptr<Renderer> renderer) = 0;
    virtual void set_visible(bool visible) = 0;
    virtual bool is_visible() const = 0;
    virtual void set_enabled(bool enabled) = 0;
    virtual bool is_enabled() const = 0;
    virtual void handle_mouse_event(const MouseEvent& event) = 0;
    
    void set_dirty_callback(DirtyCallback callback);
};

} // namespace guigui