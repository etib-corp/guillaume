#include <guillaume/components/bound.hpp>
#include <guillaume/components/click.hpp>
#include <guillaume/components/focus.hpp>
#include <guillaume/components/hover.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/input.hpp>
#include <guillaume/entities/panel.hpp>

#include "application.hpp"

int main(int argc, char *argv[]) {
    simple_application::Application application(argc, argv);

    auto &ecs = application.getECS();
    auto &componentRegistry = ecs.getComponentRegistry();

    guillaume::entities::Panel panel(componentRegistry);
    guillaume::entities::Button button(componentRegistry);
    guillaume::entities::Input input(componentRegistry);

    ecs.addEntity(panel);
    ecs.addEntity(button);
    ecs.addEntity(input);

    auto &panelTransform =
        ecs.getComponent<guillaume::components::Transform>(panel);
    panelTransform.setPosition({960.0f, 540.0f, 0.0f});
    panelTransform.setScale({1.0f, 1.0f, 1.0f});
    auto &panelBound = ecs.getComponent<guillaume::components::Bound>(panel);
    panelBound.setSize({700.0f, 380.0f, 0.0f});

    auto &inputTransform =
        ecs.getComponent<guillaume::components::Transform>(input);
    inputTransform.setPosition({960.0f, 500.0f, 0.0f});
    inputTransform.setScale({1.0f, 1.0f, 1.0f});
    auto &inputBound = ecs.getComponent<guillaume::components::Bound>(input);
    inputBound.setSize({520.0f, 90.0f, 0.0f});
    auto &inputText = ecs.getComponent<guillaume::components::Text>(input);
    inputText.setContent("Type here: ");

    auto &buttonTransform =
        ecs.getComponent<guillaume::components::Transform>(button);
    buttonTransform.setPosition({960.0f, 620.0f, 0.0f});
    buttonTransform.setScale({1.0f, 1.0f, 1.0f});
    auto &buttonBound = ecs.getComponent<guillaume::components::Bound>(button);
    buttonBound.setSize({300.0f, 90.0f, 0.0f});
    auto &buttonText = ecs.getComponent<guillaume::components::Text>(button);
    buttonText.setContent("Click me");

    auto &buttonHover = ecs.getComponent<guillaume::components::Hover>(button);
    buttonHover.setOnHoverHandler(
        [&buttonText]() { buttonText.setContent("Click me (hover)"); });
    buttonHover.setOnUnhoverHandler(
        [&buttonText]() { buttonText.setContent("Click me"); });

    bool isClicked = false;
    auto &buttonClick = ecs.getComponent<guillaume::components::Click>(button);
    buttonClick.setOnClickHandler([&buttonText, &inputText, &isClicked]() {
        isClicked = !isClicked;
        buttonText.setContent(isClicked ? "Clicked" : "Click me");
        inputText.setContent(isClicked ? "Button clicked. Type here: "
                                       : "Type here: ");
    });

    auto &inputFocus = ecs.getComponent<guillaume::components::Focus>(input);
    inputFocus.setOnFocusHandler([]() {});

    return application.run();
}
