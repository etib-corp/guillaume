#pragma once

#include <guillaume/scene.hpp>
#include "panels/default_panel.hpp"

namespace simple_application::scenes {

class Settings : public guillaume::SceneFiller<panels::DefaultPanel> {
  public:
    Settings(void);

    ~Settings(void);

    void onCreate(void) override;

    void onStart(void) override;

    void onPause(void) override;

    void onResume(void) override;

    void onStop(void) override;

    void onRestart(void) override;

    void onDestroy(void) override;
};

} // namespace simple_application::scenes