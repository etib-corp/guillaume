#pragma once

#include "guillaume/componentable.hpp"
#include "guillaume/primitive.hpp"

#include <map>

namespace guigui {

class Component : public Componentable {
private:
    std::map<std::string, std::unique_ptr<Primitivable>> _primitives;

protected:
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
    {
        _is_visible = true;
        _is_enabled = true;
    }

    virtual ~Component() = default;

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