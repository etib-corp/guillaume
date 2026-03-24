#include "scenes/main.hpp"

namespace simple_application::scenes {

Main::Main(void) : guillaume::SceneFiller<panels::DefaultPanel>() {
    getLogger().info("Main scene created with DefaultPanel");
}

Main::~Main(void) {}

void Main::onCreate(void) {}

void Main::onStart(void) {}

void Main::onPause(void) {}

void Main::onResume(void) {}

void Main::onStop(void) {}

void Main::onRestart(void) {}

void Main::onDestroy(void) {}

} // namespace simple_application::scenes