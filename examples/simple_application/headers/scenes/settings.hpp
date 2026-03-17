#pragma once

#include <guillaume/scene.hpp>

namespace simple_application::scenes {

class Settings : public guillaume::Scene {
  public:
    Settings(void);

    ~Settings(void);

    void onCreate(void);

    void onStart(void);

    void onPause(void);

    void onResume(void);

    void onStop(void);

    void onRestart(void);

    void onDestroy(void);
};

} // namespace simple_application::scenes