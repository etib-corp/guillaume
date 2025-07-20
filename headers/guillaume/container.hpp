#pragma once

#include "guillaume/component.hpp"
#include "guillaume/events/mouse_event.hpp"

namespace guigui {

class Container : public guigui::Component {
private:
    std::vector<std::unique_ptr<guigui::Componentable>> _children;
    mutable std::vector<size_t> _render_order; ///< Indices for z-index sorted rendering order
    mutable bool _render_order_dirty = true;   ///< Flag to track if render order needs updating

    /**
     * @brief Updates the render order based on z-index
     * 
     * @details Creates a sorted list of indices based on component z-index values.
     * Children with higher z-index values are placed later in the order so they 
     * are rendered on top. This method is called automatically when needed.
     */
    void _update_render_order() const;

protected:
public:
    Container();
    ~Container() override = default;

    void add_child(std::unique_ptr<guigui::Componentable> child);
    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    void draw() override;
    void set_visible(bool visible) override;
    bool is_visible() const override;
    
    // Override to forward mouse events to children
    void handle_mouse_event(const MouseEvent& event) override;
};

} // namespace guigui
