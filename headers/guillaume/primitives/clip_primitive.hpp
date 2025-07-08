#pragma once

#include "guillaume/primitive.hpp"
#include "guillaume/rectangle.hpp"

#include <memory>

namespace guigui {

// Clip command - defines clipping rectangle
class ClipPrimitive : public Primitive {
private:
    Rectangle _rectangle; // The rectangle that defines the clipping area

public:
    ClipPrimitive(const Rectangle& rectangle)
        : Primitive(PrimitiveType::CLIP, sizeof(ClipPrimitive))
        , _rectangle(rectangle) // Initialize with the given rectangle
    {
    }

    void execute() override
    {
        // Implementation for clipping operation
        // This would typically call renderer->setClipRect(rect)
    }

    std::unique_ptr<Primitive> clone() const override
    {
        return std::make_unique<ClipPrimitive>(*this);
    }

    std::string to_string() const override
    {
        return "ClipPrimitive(rectangle=" + _rectangle.to_string() + ")";
    }

    const Rectangle& get_rectangle() const { return _rectangle; }
};

// Factory function to create ClipPrimitive
inline std::unique_ptr<Primitive> createClipPrimitive(const Rectangle& rectangle)
{
    return std::make_unique<ClipPrimitive>(rectangle);
}

} // namespace guigui