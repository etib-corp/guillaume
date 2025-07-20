#include "guillaume/container.hpp"
#include <algorithm>

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
        _render_order_dirty = true; // Children order may have changed due to z-index updates
        _mark_dirty();
    });
    
    _children.push_back(std::move(child));
    _render_order_dirty = true; // New child added, need to update render order
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
    // Ensure render order is up to date before drawing
    _update_render_order();
    
    _execute_primitives();
    
    // Draw children in z-index order (lowest to highest)
    for (size_t index : _render_order) {
        if (index < _children.size() && _children[index]->is_visible()) {
            _children[index]->draw();
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
    
    // Then forward the event to all children in reverse z-index order (top to bottom)
    // This ensures that components on top receive events first
    _update_render_order();
    
    // Iterate in reverse order so higher z-index components get events first
    for (auto it = _render_order.rbegin(); it != _render_order.rend(); ++it) {
        size_t index = *it;
        if (index < _children.size()) {
            const auto& child = _children[index];
            if (child->is_enabled() && child->is_visible()) {
                child->handle_mouse_event(event);
            }
        }
    }
}

void Container::_update_render_order() const
{
    if (_render_order_dirty) {
        // Create a vector of indices
        _render_order.clear();
        _render_order.reserve(_children.size());
        
        for (size_t i = 0; i < _children.size(); ++i) {
            _render_order.push_back(i);
        }
        
        // Sort indices by z-index (lower z-index first, higher z-index last)
        // This ensures that higher z-index components are drawn on top
        std::sort(_render_order.begin(), _render_order.end(),
            [this](size_t a, size_t b) {
                // Try to cast to Component to get z-index
                const auto* comp_a = dynamic_cast<const guigui::Component*>(_children[a].get());
                const auto* comp_b = dynamic_cast<const guigui::Component*>(_children[b].get());
                
                int z_a = comp_a ? comp_a->get_z_index() : 0;
                int z_b = comp_b ? comp_b->get_z_index() : 0;
                
                return z_a < z_b; // Sort in ascending order of z-index
            });
        _render_order_dirty = false;
    }
}

} // namespace guigui
