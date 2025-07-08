#pragma once

#include "guillaume/component.hpp"

namespace guigui {

class Container : public guigui::Component {
private:
    std::vector<std::unique_ptr<guigui::Componentable>> _children;

protected:
public:
    Container()
        : guigui::Component()
    {
    }

    ~Container() override = default;

    void add_child(std::unique_ptr<guigui::Componentable> child)
    {
        _children.push_back(std::move(child));
    }

    void draw() override
    {
        _execute_primitives();
        for (const auto& child : _children) {
            if (child->is_visible()) {
                child->draw();
            }
        }
    }

    void set_visible(bool visible) override
    {
        _is_visible = visible;
        for (const auto& child : _children) {
            child->set_visible(visible);
        }
    }

    bool is_visible() const override
    {
        return _is_visible;
    }
};

} // namespace microui
