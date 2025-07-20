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

public:
    ~Primitive() override = default;

    Primitive(PrimitiveType type, std::shared_ptr<guigui::Renderer> renderer)
        : _type(type)
        , _renderer(renderer)
    {
    }

    void execute() override = 0;

    PrimitiveType get_type() const override { return _type; }

    std::unique_ptr<Primitivable> clone() const override = 0;

    std::string to_string() const override
    {
        return "Primitive(type=" + std::to_string(static_cast<int>(_type))
            + ")";
    }
};

} // namespace guigui
