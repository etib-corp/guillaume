#include "scenes/main.hpp"

#include <guillaume/entities/button.hpp>

namespace simple_application::scenes {

Main::Main(void) : guillaume::Scene() {
    guillaume::entities::Button::Builder buttonBuilder;
    guillaume::entities::Button::Director buttonDirector(
        this->getComponentRegistry());
    auto buttonEntity = buttonDirector.makeDefaultButton(buttonBuilder);
    this->getLogger().debug("Created button entity with ID: " +
                            std::to_string(buttonEntity->getIdentifier()));
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