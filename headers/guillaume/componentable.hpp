#pragma once

namespace guigui {

using ComponentIdentifier = std::string;

class Componentable {

public:
    virtual ~Componentable() = default;
    virtual ComponentIdentifier get_identifier() const = 0;
    virtual void draw() = 0;
    virtual void set_visible(bool visible) = 0;
    virtual bool is_visible() const = 0;
    virtual void set_enabled(bool enabled) = 0;
    virtual bool is_enabled() const = 0;
};

} // namespace guigui