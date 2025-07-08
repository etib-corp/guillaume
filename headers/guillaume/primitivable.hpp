#pragma once

#include <memory>
#include <string>

namespace guigui {

// Interface
class Primitivable {
public:
    enum class PrimitiveType {
        START = 0,
        CLIP = 1,
        RECTANGLE = 2,
        TEXT = 3,
    };

    virtual ~Primitivable() = default;

    virtual void execute() = 0;
    virtual PrimitiveType get_type() const = 0;
    virtual std::unique_ptr<Primitivable> clone() const = 0;
    virtual std::string to_string() const = 0;
};

} // namespace guigui