
#pragma once

#include "guillaume/primitive.hpp"

#include <memory>

namespace guigui {

class JumpPrimitive : public Primitive {
private:
    void* _destination;

public:
    JumpPrimitive(void* destination)
        : Primitive(PrimitiveType::JUMP, sizeof(JumpPrimitive))
        , _destination(destination)
    {
    }

    void execute() override
    {
        // Implementation for jump operation
    }

    std::unique_ptr<Primitive> clone() const override
    {
        return std::make_unique<JumpPrimitive>(*this);
    }

    std::string to_string() const override
    {
        return "JumpPrimitive(destination=" + std::to_string(reinterpret_cast<std::uintptr_t>(_destination)) + ")";
    }

    void* get_destination() const { return _destination; }
};

// Factory function to create JumpPrimitive
inline std::unique_ptr<Primitive> createJumpPrimitive(void* destination)
{
    return std::make_unique<JumpPrimitive>(destination);
}

} // namespace guigui