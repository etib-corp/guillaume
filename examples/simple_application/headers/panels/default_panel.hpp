#pragma once

#include <guillaume/entities/panel.hpp>

namespace simple_application::panels {
class DefaultPanel : public guillaume::entities::Panel {
  public:
    DefaultPanel(guillaume::ecs::ComponentRegistry &registry, guillaume::ecs::EntityRegistry &entityRegistry,
                 const std::string &name);

    ~DefaultPanel(void);
};
} // namespace simple_application::panels