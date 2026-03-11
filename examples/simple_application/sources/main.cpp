#include <guillaume/components/bound.hpp>
#include <guillaume/components/click.hpp>
#include <guillaume/components/focus.hpp>
#include <guillaume/components/hover.hpp>
#include <guillaume/components/relationship.hpp>
#include <guillaume/components/render.hpp>
#include <guillaume/components/text.hpp>
#include <guillaume/components/transform.hpp>

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/input.hpp>
#include <guillaume/entities/panel.hpp>

#include <guillaume/local_storage.hpp>
#include <guillaume/session_storage.hpp>

#include <guillaume/systems/detail/world_transform_utils.hpp>

#include <iostream>

#include "application.hpp"

namespace {

std::optional<std::string> resolveDefaultFontPath(void) {
    static const std::vector<std::string> candidates = {
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/Avenir.ttc",
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    };

    for (const auto &candidate : candidates) {
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
    }

    return std::nullopt;
}

} // namespace

int main(int argc, char *argv[]) {
    simple_application::Application application(argc, argv);

    guillaume::LocalStorage localStorage(std::filesystem::current_path() /
                                         ".simple-application-storage.db");
    guillaume::SessionStorage sessionStorage;

    const int launchCount =
        localStorage.getItemAs<int>("launchCount").value_or(0) + 1;
    localStorage.setItem("launchCount", launchCount);

    const int sessionClicks =
        sessionStorage.getItemAs<int>("sessionClicks").value_or(0);
    localStorage.setItem("lastRunSessionClicks", sessionClicks);
    sessionStorage.setItem("sessionClicks", 0);

    auto &ecs = application.getECS();
    auto &componentRegistry = ecs.getComponentRegistry();

    guillaume::entities::Panel::Director panelDirector;
    guillaume::entities::Button::Director buttonDirector;

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

    // Establish parent-child relationships for responsive UI
    auto &panelRelationship =
        ecs.getComponent<guillaume::components::Relationship>(*panel);
    panelRelationship.addChildIdentifier(button1->getIdentifier());

    auto &button1Text = ecs.getComponent<guillaume::components::Text>(*button1);
    button1Text.setContent("Launch #" + std::to_string(launchCount));

    // Button 1 handlers
    auto &button1Hover =
        ecs.getComponent<guillaume::components::Hover>(*button1);
    button1Hover.setOnHoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1 (hover)"); });
    button1Hover.setOnUnhoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1"); });

    auto &button1Click =
        ecs.getComponent<guillaume::components::Click>(*button1);
    button1Click.setOnClickHandler(
        [&button1Text, &localStorage,
         &sessionStorage](utility::event::MouseMotionEvent::MousePosition
                          /* position */) {
            const int totalClicks =
                localStorage.getItemAs<int>("totalClicks").value_or(0) + 1;
            const int newSessionClicks =
                sessionStorage.getItemAs<int>("sessionClicks").value_or(0) + 1;

            localStorage.setItem("totalClicks", totalClicks);
            sessionStorage.setItem("sessionClicks", newSessionClicks);

            button1Text.setContent(
                "Clicks: " + std::to_string(totalClicks) +
                " / Session: " + std::to_string(newSessionClicks));
        });
    button1Click.setOnReleaseHandler(
        [&button1Text](
            utility::event::MouseMotionEvent::MousePosition /* position */) {
            button1Text.setContent("Button 1");
        });

    auto &button1Render =
        ecs.getComponent<guillaume::components::Render>(*button1);

    button1Render.setNormalHandler(
        [](guillaume::ecs::ComponentRegistry &componentRegistry,
           const guillaume::ecs::Entity::Identifier &identityIdentifier,
           guillaume::Renderer &renderer) {
            const auto &bound =
                componentRegistry.getComponent<guillaume::components::Bound>(
                    identityIdentifier);

            const auto worldTransform =
                guillaume::systems::detail::calculateWorldTransform(
                    componentRegistry, identityIdentifier);

            guillaume::shapes::Rectangle rectangle;
            rectangle.setPosition(worldTransform.position);
            rectangle.setRotation(worldTransform.rotation);
            rectangle.setScale(worldTransform.scale);
            rectangle.setSize({bound.getSize()[0], bound.getSize()[1]});
            rectangle.setColor({255, 0, 0, 255});

            renderer.drawRectangle(rectangle);

            if (!componentRegistry.hasComponent<guillaume::components::Text>(
                    identityIdentifier)) {
                return;
            }

            const auto &textComponent =
                componentRegistry.getComponent<guillaume::components::Text>(
                    identityIdentifier);
            std::string text = textComponent.getContent();
            if (text.empty()) {
                return;
            }

            const auto &fontPath = resolveDefaultFontPath();
            if (!fontPath) {
                return;
            }

            guillaume::Text textObj(text);
            textObj.setPosition(worldTransform.position);
            textObj.setRotation(worldTransform.rotation);

            guillaume::Font font(*fontPath, 24);
            renderer.drawText(textObj, font);
        });

    button1Render.setHoveredHandler(
        [](guillaume::ecs::ComponentRegistry &componentRegistry,
           const guillaume::ecs::Entity::Identifier &identityIdentifier,
           guillaume::Renderer &renderer) {
            const auto &bound =
                componentRegistry.getComponent<guillaume::components::Bound>(
                    identityIdentifier);

            const auto worldTransform =
                guillaume::systems::detail::calculateWorldTransform(
                    componentRegistry, identityIdentifier);

            guillaume::shapes::Rectangle rectangle;
            rectangle.setPosition(worldTransform.position);
            rectangle.setRotation(worldTransform.rotation);
            rectangle.setScale(worldTransform.scale);
            rectangle.setSize({bound.getSize()[0], bound.getSize()[1]});
            rectangle.setColor({0, 255, 0, 255});

            renderer.drawRectangle(rectangle);

            if (!componentRegistry.hasComponent<guillaume::components::Text>(
                    identityIdentifier)) {
                return;
            }

            const auto &textComponent =
                componentRegistry.getComponent<guillaume::components::Text>(
                    identityIdentifier);
            std::string text = textComponent.getContent();
            if (text.empty()) {
                return;
            }

            const auto &fontPath = resolveDefaultFontPath();
            if (!fontPath) {
                return;
            }

            guillaume::Text textObj(text);
            textObj.setPosition(worldTransform.position);
            textObj.setRotation(worldTransform.rotation);

            guillaume::Font font(*fontPath, 24);
            renderer.drawText(textObj, font);
        });

    button1Render.setClickedHandler(
        [](guillaume::ecs::ComponentRegistry &componentRegistry,
           const guillaume::ecs::Entity::Identifier &identityIdentifier,
           guillaume::Renderer &renderer) {
            const auto &bound =
                componentRegistry.getComponent<guillaume::components::Bound>(
                    identityIdentifier);

            const auto worldTransform =
                guillaume::systems::detail::calculateWorldTransform(
                    componentRegistry, identityIdentifier);

            guillaume::shapes::Rectangle rectangle;
            rectangle.setPosition(worldTransform.position);
            rectangle.setRotation(worldTransform.rotation);
            rectangle.setScale(worldTransform.scale);
            rectangle.setSize({bound.getSize()[0], bound.getSize()[1]});
            rectangle.setColor({0, 0, 255, 255});

            renderer.drawRectangle(rectangle);

            if (!componentRegistry.hasComponent<guillaume::components::Text>(
                    identityIdentifier)) {
                return;
            }

            const auto &textComponent =
                componentRegistry.getComponent<guillaume::components::Text>(
                    identityIdentifier);
            std::string text = textComponent.getContent();
            if (text.empty()) {
                return;
            }

            const auto &fontPath = resolveDefaultFontPath();
            if (!fontPath) {
                return;
            }

            guillaume::Text textObj(text);
            textObj.setPosition(worldTransform.position);
            textObj.setRotation(worldTransform.rotation);

            guillaume::Font font(*fontPath, 24);
            renderer.drawText(textObj, font);
        });
    return application.run();
}
