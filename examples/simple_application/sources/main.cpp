#include <guillaume/components/bound.hpp>
#include <guillaume/components/click.hpp>
#include <guillaume/components/focus.hpp>
#include <guillaume/components/hover.hpp>
#include <guillaume/components/relationship.hpp>
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

    guillaume::entities::Panel::Director panelDirector;
    guillaume::entities::Button::Director buttonDirector;
    guillaume::entities::Input::Director inputDirector;

    guillaume::entities::Panel::Builder panelBuilder(componentRegistry);
    panelDirector.constructPanel(panelBuilder, {960.0f, 540.0f, 0.0f},
                                 {700.0f, 380.0f, 0.0f});
    auto panel = panelBuilder.getProduct();
    ecs.addEntity(*panel);

    guillaume::entities::Button::Builder buttonBuilder(componentRegistry);
    buttonDirector.constructButton(buttonBuilder, {-280.0f, 100.0f, 1.0f},
                                   {0.0f, 0.0f, 0.0f}, {200.0f, 70.0f, 0.0f},
                                   "Button 1", panel->getIdentifier());
    auto button1 = buttonBuilder.getProduct();
    ecs.addEntity(*button1);

    buttonDirector.constructButton(buttonBuilder, {-280.0f, -100.0f, 1.0f},
                                   {0.0f, 0.0f, 0.1f}, {200.0f, 70.0f, 0.0f},
                                   "Button 2", panel->getIdentifier());
    auto button2 = buttonBuilder.getProduct();
    ecs.addEntity(*button2);

    buttonDirector.constructButton(buttonBuilder, {280.0f, 100.0f, 1.0f},
                                   {0.0f, 0.0f, -0.1f}, {200.0f, 70.0f, 0.0f},
                                   "Button 3", panel->getIdentifier());
    auto button3 = buttonBuilder.getProduct();
    ecs.addEntity(*button3);

    buttonDirector.constructButton(buttonBuilder, {280.0f, -100.0f, 1.0f},
                                   {0.0f, 0.0f, 0.2f}, {200.0f, 70.0f, 0.0f},
                                   "Button 4", panel->getIdentifier());
    auto button4 = buttonBuilder.getProduct();
    ecs.addEntity(*button4);

    guillaume::entities::Input::Builder inputBuilder(componentRegistry);
    inputDirector.constructInput(inputBuilder, {0.0f, -80.0f, 1.0f},
                                 {520.0f, 90.0f, 0.0f},
                                 "Type here: ", panel->getIdentifier());
    auto input = inputBuilder.getProduct();
    ecs.addEntity(*input);

    // Establish parent-child relationships for responsive UI
    auto &panelRelationship =
        ecs.getComponent<guillaume::components::Relationship>(*panel);
    panelRelationship.addChildIdentifier(button1->getIdentifier());
    panelRelationship.addChildIdentifier(button2->getIdentifier());
    panelRelationship.addChildIdentifier(button3->getIdentifier());
    panelRelationship.addChildIdentifier(button4->getIdentifier());
    panelRelationship.addChildIdentifier(input->getIdentifier());

    auto &inputText = ecs.getComponent<guillaume::components::Text>(*input);

    auto &button1Text = ecs.getComponent<guillaume::components::Text>(*button1);
    auto &button2Text = ecs.getComponent<guillaume::components::Text>(*button2);
    auto &button3Text = ecs.getComponent<guillaume::components::Text>(*button3);
    auto &button4Text = ecs.getComponent<guillaume::components::Text>(*button4);

    // Button 1 handlers
    auto &button1Hover =
        ecs.getComponent<guillaume::components::Hover>(*button1);
    button1Hover.setOnHoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1 (hover)"); });
    button1Hover.setOnUnhoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1"); });

    bool isButton1Clicked = false;
    auto &button1Click =
        ecs.getComponent<guillaume::components::Click>(*button1);
    button1Click.setOnClickHandler([&button1Text, &inputText,
                                    &isButton1Clicked]() {
        isButton1Clicked = !isButton1Clicked;
        button1Text.setContent(isButton1Clicked ? "Button 1 (clicked)"
                                                : "Button 1");
        inputText.setContent(isButton1Clicked ? "Button 1 clicked. Type here: "
                                              : "Type here: ");
    });

    // Button 2 handlers
    auto &button2Hover =
        ecs.getComponent<guillaume::components::Hover>(*button2);
    button2Hover.setOnHoverHandler(
        [&button2Text]() { button2Text.setContent("Button 2 (hover)"); });
    button2Hover.setOnUnhoverHandler(
        [&button2Text]() { button2Text.setContent("Button 2"); });

    bool isButton2Clicked = false;
    auto &button2Click =
        ecs.getComponent<guillaume::components::Click>(*button2);
    button2Click.setOnClickHandler([&button2Text, &inputText,
                                    &isButton2Clicked]() {
        isButton2Clicked = !isButton2Clicked;
        button2Text.setContent(isButton2Clicked ? "Button 2 (clicked)"
                                                : "Button 2");
        inputText.setContent(isButton2Clicked ? "Button 2 clicked. Type here: "
                                              : "Type here: ");
    });

    // Button 3 handlers
    auto &button3Hover =
        ecs.getComponent<guillaume::components::Hover>(*button3);
    button3Hover.setOnHoverHandler(
        [&button3Text]() { button3Text.setContent("Button 3 (hover)"); });
    button3Hover.setOnUnhoverHandler(
        [&button3Text]() { button3Text.setContent("Button 3"); });

    bool isButton3Clicked = false;
    auto &button3Click =
        ecs.getComponent<guillaume::components::Click>(*button3);
    button3Click.setOnClickHandler([&button3Text, &inputText,
                                    &isButton3Clicked]() {
        isButton3Clicked = !isButton3Clicked;
        button3Text.setContent(isButton3Clicked ? "Button 3 (clicked)"
                                                : "Button 3");
        inputText.setContent(isButton3Clicked ? "Button 3 clicked. Type here: "
                                              : "Type here: ");
    });

    // Button 4 handlers
    auto &button4Hover =
        ecs.getComponent<guillaume::components::Hover>(*button4);
    button4Hover.setOnHoverHandler(
        [&button4Text]() { button4Text.setContent("Button 4 (hover)"); });
    button4Hover.setOnUnhoverHandler(
        [&button4Text]() { button4Text.setContent("Button 4"); });

    bool isButton4Clicked = false;
    auto &button4Click =
        ecs.getComponent<guillaume::components::Click>(*button4);
    button4Click.setOnClickHandler([&button4Text, &inputText,
                                    &isButton4Clicked]() {
        isButton4Clicked = !isButton4Clicked;
        button4Text.setContent(isButton4Clicked ? "Button 4 (clicked)"
                                                : "Button 4");
        inputText.setContent(isButton4Clicked ? "Button 4 clicked. Type here: "
                                              : "Type here: ");
    });

    auto &inputFocus = ecs.getComponent<guillaume::components::Focus>(*input);
    inputFocus.setOnFocusHandler([]() {});

    return application.run();
}
