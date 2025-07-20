#include "guillaume/component.hpp"

namespace guigui {

Component::Component()
    : _renderer(nullptr)
    , _is_visible(true)
    , _is_enabled(true)
    , _bounds(0, 0, 0, 0) // Default empty bounds
{
    _identifier = "Component_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
}

Component::Component(const Rectangle& bounds)
    : _renderer(nullptr)
    , _is_visible(true)
    , _is_enabled(true)
    , _bounds(bounds)
{
    _identifier = "Component_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
}

void Component::set_renderer(std::shared_ptr<Renderer> renderer)
{
    _renderer = std::move(renderer);
}

void Component::set_visible(bool visible)
{
    if (_is_visible != visible) {
        _is_visible = visible;
        _mark_dirty(); // Mark as dirty when visibility changes
    }
}

bool Component::is_visible() const
{
    return _is_visible;
}

void Component::set_enabled(bool enabled)
{
    if (_is_enabled != enabled) {
        _is_enabled = enabled;
        _mark_dirty(); // Mark as dirty when enabled state changes
    }
}

bool Component::is_enabled() const
{
    return _is_enabled;
}

ComponentIdentifier Component::get_identifier() const
{
    return _identifier;
}

void Component::_add_primitive(std::string name, std::unique_ptr<Primitivable> primitive)
{
    _primitives.emplace(name, std::move(primitive));
    _mark_dirty(); // Mark as dirty when primitive is added
}

void Component::_update_primitive(std::string name, std::unique_ptr<Primitivable> primitive)
{
    auto it = _primitives.find(name);
    if (it != _primitives.end()) {
        it->second = std::move(primitive);
        _mark_dirty(); // Mark as dirty when primitive is updated
    } else {
        throw std::runtime_error("Primitive not found: " + name);
    }
}

void Component::_execute_primitives()
{
    for (auto& [name, primitive] : _primitives) {
        if (_is_enabled) {
            primitive->execute();
        }
    }
}

// Interaction handling methods
void Component::handle_mouse_event(const MouseEvent& event)
{
    if (!is_enabled() || !is_visible()) {
        return;
    }

    Vector mouse_pos = event.get_position();
    bool mouse_inside = is_point_inside(mouse_pos);
    Component::InteractionState old_state = _interaction_state;

    switch (event.get_event_type()) {
        case MouseEvent::MouseEventType::MOUSE_MOTION:
            if (mouse_inside && _interaction_state == Component::InteractionState::NORMAL) {
                LOG_DEBUG_F("Component {} entering hover state at position ({}, {})", 
                           get_identifier(), mouse_pos.get_x(), mouse_pos.get_y());
                set_interaction_state(Component::InteractionState::HOVERED);
            } else if (!mouse_inside && _interaction_state == Component::InteractionState::HOVERED) {
                LOG_DEBUG_F("Component {} exiting hover state at position ({}, {})", 
                           get_identifier(), mouse_pos.get_x(), mouse_pos.get_y());
                set_interaction_state(Component::InteractionState::NORMAL);
            }
            break;

        case MouseEvent::MouseEventType::MOUSE_BUTTON_PRESS:
            if (mouse_inside && event.get_button() == MouseEvent::MouseButton::LEFT) {
                LOG_INFO_F("Component {} pressed at position ({}, {})", 
                          get_identifier(), mouse_pos.get_x(), mouse_pos.get_y());
                set_interaction_state(Component::InteractionState::PRESSED);
                _on_mouse_press(event);
            }
            break;

        case MouseEvent::MouseEventType::MOUSE_BUTTON_RELEASE:
            if (event.get_button() == MouseEvent::MouseButton::LEFT) {
                if (mouse_inside && _interaction_state == Component::InteractionState::PRESSED) {
                    // Complete click detected
                    LOG_INFO_F("Component {} clicked successfully at position ({}, {})", 
                              get_identifier(), mouse_pos.get_x(), mouse_pos.get_y());
                    _on_click_detected();
                    set_interaction_state(Component::InteractionState::HOVERED);
                } else {
                    LOG_DEBUG_F("Component {} released at position ({}, {}) - {}click", 
                               get_identifier(), mouse_pos.get_x(), mouse_pos.get_y(),
                               mouse_inside ? "incomplete " : "outside ");
                    set_interaction_state(mouse_inside ? Component::InteractionState::HOVERED : Component::InteractionState::NORMAL);
                }
                _on_mouse_release(event);
            }
            break;
    }

    // Call the general mouse event callback if set
    if (_on_mouse_event) {
        _on_mouse_event(*this, event);
    }
}

bool Component::is_point_inside(const Vector& point) const
{
    return point.get_x() >= _bounds.get_x() &&
           point.get_x() <= _bounds.get_x() + _bounds.get_width() &&
           point.get_y() >= _bounds.get_y() &&
           point.get_y() <= _bounds.get_y() + _bounds.get_height();
}

void Component::set_bounds(const Rectangle& bounds)
{
    _bounds = bounds;
}

Rectangle Component::get_bounds() const
{
    return _bounds;
}

void Component::set_interaction_state(Component::InteractionState state)
{
    if (_interaction_state != state) {
        Component::InteractionState old_state = _interaction_state;
        _interaction_state = state;
        
        // Log state transition
        const char* state_names[] = {"NORMAL", "HOVERED", "PRESSED"};
        LOG_DEBUG_F("Component {} state transition: {} to {}", 
                   get_identifier(), 
                   state_names[static_cast<int>(old_state)], 
                   state_names[static_cast<int>(state)]);
        
        // Handle state transitions
        if (old_state != Component::InteractionState::HOVERED && state == Component::InteractionState::HOVERED) {
            _on_hover_enter();
        } else if (old_state == Component::InteractionState::HOVERED && state != Component::InteractionState::HOVERED) {
            _on_hover_exit();
        }
        
        _on_interaction_state_changed(old_state, state);
        _mark_dirty(); // Mark as dirty when interaction state changes
    }
}

Component::InteractionState Component::get_interaction_state() const
{
    return _interaction_state;
}

void Component::set_hover_callback(HoverCallback callback)
{
    _on_hover_change = callback;
}

void Component::set_click_callback(ClickCallback callback)
{
    _on_click = callback;
}

void Component::set_mouse_event_callback(MouseEventCallback callback)
{
    _on_mouse_event = callback;
}

bool Component::is_hovered() const
{
    return _interaction_state == Component::InteractionState::HOVERED;
}

bool Component::is_pressed() const
{
    return _interaction_state == Component::InteractionState::PRESSED;
}

// Virtual methods with default implementations
void Component::_on_interaction_state_changed(Component::InteractionState old_state, Component::InteractionState new_state)
{
    // Default implementation - can be overridden by derived classes
}

void Component::_on_hover_enter()
{
    LOG_DEBUG_F("Component {} hover ENTER", get_identifier());
    if (_on_hover_change) {
        _on_hover_change(*this, true); // true = entering hover
    }
}

void Component::_on_hover_exit()
{
    LOG_DEBUG_F("Component {} hover EXIT", get_identifier());
    if (_on_hover_change) {
        _on_hover_change(*this, false); // false = exiting hover
    }
}

void Component::_on_mouse_press(const MouseEvent& event)
{
    // Default implementation - can be overridden by derived classes
}

void Component::_on_mouse_release(const MouseEvent& event)
{
    // Default implementation - can be overridden by derived classes
}

void Component::_on_click_detected()
{
    LOG_INFO_F("Component {} CLICK detected", get_identifier());
    if (_on_click) {
        _on_click(*this);
    }
}

void Component::set_z_index(int z_index)
{
    if (_z_index != z_index) {
        _z_index = z_index;
        _mark_dirty(); // Mark as dirty when z-index changes to trigger container reordering
    }
}

int Component::get_z_index() const
{
    return _z_index;
}

} // namespace guigui
