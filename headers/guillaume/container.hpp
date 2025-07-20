#pragma once

#include "guillaume/component.hpp"

namespace guigui {

class Container : public guigui::Component {
private:
    std::vector<std::unique_ptr<guigui::Componentable>> _children;

protected:
public:
    Container();
    ~Container() override = default;

    void add_child(std::unique_ptr<guigui::Componentable> child);
    void set_renderer(std::shared_ptr<Renderer> renderer) override;
    void draw() override;
    void set_visible(bool visible) override;
    bool is_visible() const override;
};

} // namespace guigui
