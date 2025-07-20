#include "guillaume/container.hpp"

namespace guigui {

Container::Container()
    : guigui::Component()
{
}

void Container::add_child(std::unique_ptr<guigui::Componentable> child)
{
    if (_renderer) {
        child->set_renderer(_renderer);
    }
    
    // Set up the dirty callback to propagate from child to this container
    child->set_dirty_callback([this]() {
        _mark_dirty();
    });
    
    _children.push_back(std::move(child));
    _mark_dirty(); // Adding a child requires redraw
}

void Container::set_renderer(std::shared_ptr<Renderer> renderer)
{
    Component::set_renderer(renderer);
    // Update all children with the new renderer
    for (const auto& child : _children) {
        child->set_renderer(_renderer);
    }
}

void Container::draw()
{
    _execute_primitives();
    for (const auto& child : _children) {
        if (child->is_visible()) {
            child->draw();
        }
    }
}

void Container::set_visible(bool visible)
{
    Component::set_visible(visible); // Call parent implementation which handles dirty marking
    for (const auto& child : _children) {
        child->set_visible(visible);
    }
}

bool Container::is_visible() const
{
    return _is_visible;
}

void Container::handle_mouse_event(const MouseEvent& event)
{
    // First handle the event for this container
    Component::handle_mouse_event(event);
    
    // Then forward the event to all children
    for (const auto& child : _children) {
        if (child->is_enabled() && child->is_visible()) {
            child->handle_mouse_event(event);
        }
    }
}

} // namespace guigui
