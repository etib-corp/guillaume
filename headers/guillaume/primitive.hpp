#pragma once

#include <memory>
#include <string>

#include "guillaume/color.hpp"
#include "guillaume/font.hpp"
#include "guillaume/rectangle.hpp"
#include "guillaume/vector.hpp"

namespace guigui {

enum class PrimitiveType {
    JUMP = 0,
    CLIP = 1,
    RECTANGLE = 2,
    TEXT = 3,
};

class Primitive {
protected:
    PrimitiveType _type;
    int _size;

public:
    Primitive(PrimitiveType type, int size)
        : _type(type)
        , _size(size)
    {
    }

    virtual ~Primitive() = default;

    virtual void execute() = 0;

    PrimitiveType get_type() const { return _type; }
    int get_size() const { return _size; }

    virtual std::unique_ptr<Primitive> clone() const = 0;

    virtual std::string to_string() const
    {
        return "Primitive(type=" + std::to_string(static_cast<int>(_type))
            + ", size=" + std::to_string(_size) + ")";
    }
};

} // namespace guigui
