#include "scenes/main.hpp"

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/icon.hpp>
#include <guillaume/entities/text.hpp>

namespace simple_application::scenes {

Main::Main(void) : guillaume::Scene() {
    guillaume::entities::Button::Builder buttonBuilder;
    guillaume::entities::Button::Director buttonDirector(
        this->getComponentRegistry());
    guillaume::entities::Icon::Builder iconBuilder;
    guillaume::entities::Icon::Director iconDirector(
        this->getComponentRegistry());
    guillaume::entities::Text::Builder textBuilder;
    guillaume::entities::Text::Director textDirector(
        this->getComponentRegistry());
    auto buttonEntity = buttonDirector.makeDefaultButton(buttonBuilder);
    auto iconEntity = iconDirector.makeDefaultIcon(iconBuilder);
    auto textEntity = textDirector.makeDefaultText(textBuilder);
    this->getLogger().debug("Created button entity with ID: " +
                            std::to_string(buttonEntity->getIdentifier()));
    this->getLogger().debug("Created icon entity with ID: " +
                            std::to_string(iconEntity->getIdentifier()));
    this->getLogger().debug("Created text entity with ID: " +
                            std::to_string(textEntity->getIdentifier()));
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