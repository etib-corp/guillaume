
#pragma once

#include "guillaume/primitive.hpp"

#include <memory>

namespace guigui {

class StartPrimitive : public Primitive {
private:
public:
    StartPrimitive(void* destination)
        : Primitive(PrimitiveType::START)
    {
    }

    void execute() override
    {
        // Implementation for jump operation
    }

    std::unique_ptr<Primitivable> clone() const override
    {
        return std::make_unique<StartPrimitive>(*this);
    }

    std::string to_string() const override
    {
        return "StartPrimitive()";
    }
};

// Factory function to create JumpPrimitive
inline std::unique_ptr<Primitivable> createJumpPrimitive(void* destination)
{
    return std::make_unique<StartPrimitive>(destination);
}

} // namespace guigui