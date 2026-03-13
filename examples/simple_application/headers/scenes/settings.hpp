#pragma once

#include <guillaume/scene.hpp>

namespace simple_application::scenes {

class Settings : public guillaume::Scene {
  public:
    Settings(void);
    ~Settings(void);

    void onApplicationAttached(void) override;
};

} // namespace simple_application::scenes