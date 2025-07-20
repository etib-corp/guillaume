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

    void _add_primitive(std::string name, std::unique_ptr<Primitivable> primitive);
    void _update_primitive(std::string name, std::unique_ptr<Primitivable> primitive);
    void _execute_primitives();

public:
    Component();
    virtual ~Component() = default;

    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    void set_visible(bool visible) override;
    bool is_visible() const override;
    void set_enabled(bool enabled) override;
    bool is_enabled() const override;
    ComponentIdentifier get_identifier() const override;
};

} // namespace guigui