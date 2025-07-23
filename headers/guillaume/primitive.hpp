#pragma once

#include <memory>
#include <string>

#include "guillaume/primitivable.hpp"
#include "guillaume/renderer.hpp"

namespace guigui {

class Renderer;

class Primitive : public Primitivable {
protected:
    PrimitiveType _type;
    std::shared_ptr<guigui::Renderer> _renderer;
    std::string _component_id;
    std::string _primitive_name;

public:
    ~Primitive() override = default;

    Primitive(PrimitiveType type, std::shared_ptr<guigui::Renderer> renderer)
        : _type(type)
        , _renderer(renderer)
    {
    }

    Primitive(PrimitiveType type, std::shared_ptr<guigui::Renderer> renderer,
        const std::string& component_id, const std::string& primitive_name)
        : _type(type)
        , _renderer(renderer)
        , _component_id(component_id)
        , _primitive_name(primitive_name)
    {
    }

    void execute() override = 0;

    PrimitiveType get_type() const override { return _type; }

    std::unique_ptr<Primitivable> clone() const override = 0;

    // Getters for caching
    const std::string& get_component_id() const { return _component_id; }
    const std::string& get_primitive_name() const { return _primitive_name; }

    // Setters for caching (used when primitive is added to component)
    void set_component_id(const std::string& component_id) { _component_id = component_id; }
    void set_primitive_name(const std::string& primitive_name) { _primitive_name = primitive_name; }

    std::string to_string() const override
    {
        return "Primitive(type=" + std::to_string(static_cast<int>(_type))
            + ", component_id=" + _component_id + ", name=" + _primitive_name + ")";
    }
};

} // namespace guigui
