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
    _children.push_back(std::move(child));
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
    _is_visible = visible;
    for (const auto& child : _children) {
        child->set_visible(visible);
    }
}

bool Container::is_visible() const
{
    return _is_visible;
}

} // namespace guigui
