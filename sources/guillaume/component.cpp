#include "guillaume/component.hpp"

namespace guigui {

Component::Component()
    : _renderer(nullptr)
    , _is_visible(true)
    , _is_enabled(true)
{
    _identifier = "Component_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
}

void Component::set_renderer(std::shared_ptr<Renderer> renderer)
{
    _renderer = std::move(renderer);
}

void Component::set_visible(bool visible)
{
    _is_visible = visible;
}

bool Component::is_visible() const
{
    return _is_visible;
}

void Component::set_enabled(bool enabled)
{
    _is_enabled = enabled;
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
}

void Component::_update_primitive(std::string name, std::unique_ptr<Primitivable> primitive)
{
    auto it = _primitives.find(name);
    if (it != _primitives.end()) {
        it->second = std::move(primitive);
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

} // namespace guigui
