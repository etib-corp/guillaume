#pragma once

#include <guillaume/scene.hpp>

namespace simple_application::scenes {

class Main : public guillaume::Scene {
  public:
    Main(void);
    ~Main(void);

    void onApplicationAttached(void) override;
};

} // namespace simple_application::scenes