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

    guillaume::entities::Panel panel(componentRegistry);
    guillaume::entities::Button button1(componentRegistry);
    guillaume::entities::Button button2(componentRegistry);
    guillaume::entities::Button button3(componentRegistry);
    guillaume::entities::Button button4(componentRegistry);
    guillaume::entities::Input input(componentRegistry);

    ecs.addEntity(panel);
    ecs.addEntity(button1);
    ecs.addEntity(button2);
    ecs.addEntity(button3);
    ecs.addEntity(button4);
    ecs.addEntity(input);

    // Establish parent-child relationships for responsive UI
    auto &panelRelationship =
        ecs.getComponent<guillaume::components::Relationship>(panel);
    panelRelationship.addChildIdentifier(button1.getIdentifier());
    panelRelationship.addChildIdentifier(button2.getIdentifier());
    panelRelationship.addChildIdentifier(button3.getIdentifier());
    panelRelationship.addChildIdentifier(button4.getIdentifier());
    panelRelationship.addChildIdentifier(input.getIdentifier());

    auto &button1Relationship =
        ecs.getComponent<guillaume::components::Relationship>(button1);
    button1Relationship.setParentIdentifier(panel.getIdentifier());

    auto &button2Relationship =
        ecs.getComponent<guillaume::components::Relationship>(button2);
    button2Relationship.setParentIdentifier(panel.getIdentifier());

    auto &button3Relationship =
        ecs.getComponent<guillaume::components::Relationship>(button3);
    button3Relationship.setParentIdentifier(panel.getIdentifier());

    auto &button4Relationship =
        ecs.getComponent<guillaume::components::Relationship>(button4);
    button4Relationship.setParentIdentifier(panel.getIdentifier());

    auto &inputRelationship =
        ecs.getComponent<guillaume::components::Relationship>(input);
    inputRelationship.setParentIdentifier(panel.getIdentifier());

    auto &panelTransform =
        ecs.getComponent<guillaume::components::Transform>(panel);
    panelTransform.setPosition({960.0f, 540.0f, 0.0f});
    panelTransform.setScale({1.0f, 1.0f, 1.0f});
    auto &panelBound = ecs.getComponent<guillaume::components::Bound>(panel);
    panelBound.setSize({700.0f, 380.0f, 0.0f});

    auto &inputTransform =
        ecs.getComponent<guillaume::components::Transform>(input);
    inputTransform.setPosition({0.0f, -80.0f, 1.0f});
    inputTransform.setScale({1.0f, 1.0f, 1.0f});
    auto &inputBound = ecs.getComponent<guillaume::components::Bound>(input);
    inputBound.setSize({520.0f, 90.0f, 0.0f});
    auto &inputText = ecs.getComponent<guillaume::components::Text>(input);
    inputText.setContent("Type here: ");

    auto &button1Transform =
        ecs.getComponent<guillaume::components::Transform>(button1);
    button1Transform.setPosition({-280.0f, 100.0f, 1.0f});
    button1Transform.setRotation({0.0f, 0.0f, 0.0f});
    button1Transform.setScale({1.0f, 1.0f, 1.0f});
    auto &button1Bound = ecs.getComponent<guillaume::components::Bound>(button1);
    button1Bound.setSize({200.0f, 70.0f, 0.0f});
    auto &button1Text = ecs.getComponent<guillaume::components::Text>(button1);
    button1Text.setContent("Button 1");

    auto &button2Transform =
        ecs.getComponent<guillaume::components::Transform>(button2);
    button2Transform.setPosition({-280.0f, -100.0f, 1.0f});
    button2Transform.setRotation({0.0f, 0.0f, 0.1f});  // Slight rotation
    button2Transform.setScale({1.0f, 1.0f, 1.0f});
    auto &button2Bound = ecs.getComponent<guillaume::components::Bound>(button2);
    button2Bound.setSize({200.0f, 70.0f, 0.0f});
    auto &button2Text = ecs.getComponent<guillaume::components::Text>(button2);
    button2Text.setContent("Button 2");

    auto &button3Transform =
        ecs.getComponent<guillaume::components::Transform>(button3);
    button3Transform.setPosition({280.0f, 100.0f, 1.0f});
    button3Transform.setRotation({0.0f, 0.0f, -0.1f});  // Slight rotation
    button3Transform.setScale({1.0f, 1.0f, 1.0f});
    auto &button3Bound = ecs.getComponent<guillaume::components::Bound>(button3);
    button3Bound.setSize({200.0f, 70.0f, 0.0f});
    auto &button3Text = ecs.getComponent<guillaume::components::Text>(button3);
    button3Text.setContent("Button 3");

    auto &button4Transform =
        ecs.getComponent<guillaume::components::Transform>(button4);
    button4Transform.setPosition({280.0f, -100.0f, 1.0f});
    button4Transform.setRotation({0.0f, 0.0f, 0.2f});  // More rotation
    button4Transform.setScale({1.0f, 1.0f, 1.0f});
    auto &button4Bound = ecs.getComponent<guillaume::components::Bound>(button4);
    button4Bound.setSize({200.0f, 70.0f, 0.0f});
    auto &button4Text = ecs.getComponent<guillaume::components::Text>(button4);
    button4Text.setContent("Button 4");

    // Button 1 handlers
    auto &button1Hover = ecs.getComponent<guillaume::components::Hover>(button1);
    button1Hover.setOnHoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1 (hover)"); });
    button1Hover.setOnUnhoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1"); });

    bool isButton1Clicked = false;
    auto &button1Click = ecs.getComponent<guillaume::components::Click>(button1);
    button1Click.setOnClickHandler([&button1Text, &inputText, &isButton1Clicked]() {
        isButton1Clicked = !isButton1Clicked;
        button1Text.setContent(isButton1Clicked ? "Button 1 (clicked)" : "Button 1");
        inputText.setContent(isButton1Clicked ? "Button 1 clicked. Type here: "
                                              : "Type here: ");
    });

    // Button 2 handlers
    auto &button2Hover = ecs.getComponent<guillaume::components::Hover>(button2);
    button2Hover.setOnHoverHandler(
        [&button2Text]() { button2Text.setContent("Button 2 (hover)"); });
    button2Hover.setOnUnhoverHandler(
        [&button2Text]() { button2Text.setContent("Button 2"); });

    bool isButton2Clicked = false;
    auto &button2Click = ecs.getComponent<guillaume::components::Click>(button2);
    button2Click.setOnClickHandler([&button2Text, &inputText, &isButton2Clicked]() {
        isButton2Clicked = !isButton2Clicked;
        button2Text.setContent(isButton2Clicked ? "Button 2 (clicked)" : "Button 2");
        inputText.setContent(isButton2Clicked ? "Button 2 clicked. Type here: "
                                              : "Type here: ");
    });

    // Button 3 handlers
    auto &button3Hover = ecs.getComponent<guillaume::components::Hover>(button3);
    button3Hover.setOnHoverHandler(
        [&button3Text]() { button3Text.setContent("Button 3 (hover)"); });
    button3Hover.setOnUnhoverHandler(
        [&button3Text]() { button3Text.setContent("Button 3"); });

    bool isButton3Clicked = false;
    auto &button3Click = ecs.getComponent<guillaume::components::Click>(button3);
    button3Click.setOnClickHandler([&button3Text, &inputText, &isButton3Clicked]() {
        isButton3Clicked = !isButton3Clicked;
        button3Text.setContent(isButton3Clicked ? "Button 3 (clicked)" : "Button 3");
        inputText.setContent(isButton3Clicked ? "Button 3 clicked. Type here: "
                                              : "Type here: ");
    });

    // Button 4 handlers
    auto &button4Hover = ecs.getComponent<guillaume::components::Hover>(button4);
    button4Hover.setOnHoverHandler(
        [&button4Text]() { button4Text.setContent("Button 4 (hover)"); });
    button4Hover.setOnUnhoverHandler(
        [&button4Text]() { button4Text.setContent("Button 4"); });

    bool isButton4Clicked = false;
    auto &button4Click = ecs.getComponent<guillaume::components::Click>(button4);
    button4Click.setOnClickHandler([&button4Text, &inputText, &isButton4Clicked]() {
        isButton4Clicked = !isButton4Clicked;
        button4Text.setContent(isButton4Clicked ? "Button 4 (clicked)" : "Button 4");
        inputText.setContent(isButton4Clicked ? "Button 4 clicked. Type here: "
                                              : "Type here: ");
    });

    auto &inputFocus = ecs.getComponent<guillaume::components::Focus>(input);
    inputFocus.setOnFocusHandler([]() {});

    return application.run();
}
