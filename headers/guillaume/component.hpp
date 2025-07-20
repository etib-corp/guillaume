#pragma once

#include "guillaume/componentable.hpp"
#include "guillaume/primitive.hpp"
#include "guillaume/renderer.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/vector.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/logger.hpp"

#include <map>
#include <functional>

namespace guigui {

class Component : public Componentable {
public:
    enum class InteractionState {
        NORMAL,
        HOVERED,
        PRESSED
    };

    // Callback function types
    using HoverCallback = std::function<void(Component&, bool)>; // Component, is_entering
    using ClickCallback = std::function<void(Component&)>;
    using MouseEventCallback = std::function<void(Component&, const MouseEvent&)>;

private:
    std::map<std::string, std::unique_ptr<Primitivable>> _primitives;
    InteractionState _interaction_state = InteractionState::NORMAL;
    Rectangle _bounds; // Component bounds for interaction detection
    
    // Event callbacks
    HoverCallback _on_hover_change;
    ClickCallback _on_click;
    MouseEventCallback _on_mouse_event;

protected:
    std::shared_ptr<Renderer> _renderer;
    bool _is_visible = true;
    bool _is_enabled = true;
    ComponentIdentifier _identifier;

    void _add_primitive(std::string name, std::unique_ptr<Primitivable> primitive);
    void _update_primitive(std::string name, std::unique_ptr<Primitivable> primitive);
    void _execute_primitives();
    
    // Virtual methods for interaction handling (can be overridden by derived classes)
    virtual void _on_interaction_state_changed(InteractionState old_state, InteractionState new_state);
    virtual void _on_hover_enter();
    virtual void _on_hover_exit();
    virtual void _on_mouse_press(const MouseEvent& event);
    virtual void _on_mouse_release(const MouseEvent& event);
    virtual void _on_click_detected();

public:
    Component();
    Component(const Rectangle& bounds);
    virtual ~Component() = default;

    // Componentable interface
    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    void set_visible(bool visible) override;
    bool is_visible() const override;
    void set_enabled(bool enabled) override;
    bool is_enabled() const override;
    ComponentIdentifier get_identifier() const override;

    // Interaction system
    virtual void handle_mouse_event(const MouseEvent& event) override;
    virtual bool is_point_inside(const Vector& point) const;
    
    // Bounds management
    void set_bounds(const Rectangle& bounds);
    Rectangle get_bounds() const;
    
    // Interaction state
    void set_interaction_state(InteractionState state);
    InteractionState get_interaction_state() const;
    
    // Event callbacks
    void set_hover_callback(HoverCallback callback);
    void set_click_callback(ClickCallback callback);
    void set_mouse_event_callback(MouseEventCallback callback);
    
    // Convenience methods
    bool is_hovered() const;
    bool is_pressed() const;
};

} // namespace guigui