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
    auto buttonEntity =
        buttonDirector.makeDefaultButton(buttonBuilder, "Click me!", [this]() {
            this->getLogger().info("Button clicked!");
        });
    auto textEntity = textDirector.makeDefaultText(
        textBuilder, "Hello, World!", 24,
        guillaume::entities::Text::Color{255, 255, 255, 255});

    this->getEntityRegistry().addEntity(std::move(buttonEntity));
    this->getEntityRegistry().addEntity(std::move(textEntity));
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