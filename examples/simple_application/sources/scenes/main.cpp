#include "scenes/main.hpp"

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/icon.hpp>
#include <guillaume/entities/text.hpp>

namespace simple_application::scenes {

Main::Main(void) : guillaume::Scene() {
    guillaume::entities::Button::Builder buttonBuilder(getComponentRegistry(),
                                                       getEntityRegistry());
    guillaume::entities::Button::Director buttonDirector;

    buttonDirector.makeIconTextButton(buttonBuilder, "add", "Like", [this]() {
        this->getLogger().info("Button clicked!");
    });
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