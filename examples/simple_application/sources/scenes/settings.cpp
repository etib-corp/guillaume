#include "scenes/settings.hpp"

namespace simple_application::scenes {

Settings::Settings(void) : guillaume::SceneFiller<panels::DefaultPanel>() {
    getLogger().info("Settings scene created with DefaultPanel");
}

Settings::~Settings(void) {}

void Settings::onCreate(void) {}

void Settings::onStart(void) {}

void Settings::onPause(void) {}

void Settings::onResume(void) {}

void Settings::onStop(void) {}

void Settings::onRestart(void) {}

void Settings::onDestroy(void) {}

} // namespace simple_application::scenes