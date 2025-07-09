#pragma once

#include "guillaume/componentable.hpp"
#include "guillaume/primitive.hpp"
#include "guillaume/renderer.hpp"

#include <map>

namespace guigui {

class Component : public Componentable {
private:
    std::map<std::string, std::unique_ptr<Primitivable>> _primitives;

protected:
    std::shared_ptr<Renderer> _renderer;
    bool _is_visible = true;
    bool _is_enabled = true;
    ComponentIdentifier _identifier;

    void _add_primitive(std::string name, std::unique_ptr<Primitivable> primitive)
    {
        _primitives.emplace(name, std::move(primitive));
    }

    void _update_primitive(std::string name, std::unique_ptr<Primitivable> primitive)
    {
        auto it = _primitives.find(name);
        if (it != _primitives.end()) {
            it->second = std::move(primitive);
        } else {
            throw std::runtime_error("Primitive not found: " + name);
        }
    }

    void _execute_primitives()
    {
        for (auto& [name, primitive] : _primitives) {
            if (_is_enabled) {
                primitive->execute();
            }
        }
    }

public:
    Component()
        : _renderer(nullptr)
        , _is_visible(true)
        , _is_enabled(true)
    {
        _identifier = "Component_" + std::to_string(reinterpret_cast<std::uintptr_t>(this));
    }

    virtual ~Component() = default;

    void set_renderer(std::shared_ptr<Renderer> renderer) override
    {
        _renderer = std::move(renderer);
    }

    void set_visible(bool visible) override
    {
        _is_visible = visible;
    }

    bool is_visible() const override
    {
        return _is_visible;
    }

    void set_enabled(bool enabled) override
    {
        _is_enabled = enabled;
    }

    bool is_enabled() const override
    {
        return _is_enabled;
    }

    ComponentIdentifier get_identifier() const override
    {
        return _identifier;
    }
};

} // namespace guigui