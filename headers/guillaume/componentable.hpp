#pragma once

#include "guillaume/renderer.hpp"

#include <memory>

namespace guigui {

using ComponentIdentifier = std::string;

class Componentable {

public:
    virtual ~Componentable() = default;
    virtual ComponentIdentifier get_identifier() const = 0;
    virtual void draw() = 0;
    virtual void set_renderer(std::shared_ptr<Renderer> renderer) = 0;
    virtual void set_visible(bool visible) = 0;
    virtual bool is_visible() const = 0;
    virtual void set_enabled(bool enabled) = 0;
    virtual bool is_enabled() const = 0;
};

} // namespace guigui