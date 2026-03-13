#include "scenes/settings.hpp"

#include "scenes/main.hpp"

#include <guillaume/components/bound.hpp>
#include <guillaume/components/click.hpp>
#include <guillaume/components/hover.hpp>
#include <guillaume/components/relationship.hpp>
#include <guillaume/components/render.hpp>
#include <guillaume/components/text.hpp>

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/panel.hpp>

#include <guillaume/font.hpp>
#include <guillaume/shapes/rectangle.hpp>
#include <guillaume/systems/detail/world_transform_utils.hpp>
#include <guillaume/text.hpp>

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace simple_application::scenes {

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

void drawButtonWithColor(
    guillaume::ecs::ComponentRegistry &componentRegistry,
    const guillaume::ecs::Entity::Identifier &identityIdentifier,
    guillaume::Renderer &renderer, const std::array<std::uint8_t, 4> &color) {
    const auto &bound =
        componentRegistry.getComponent<guillaume::components::Bound>(
            identityIdentifier);

    const auto worldTransform =
        guillaume::systems::detail::calculateWorldTransform(componentRegistry,
                                                            identityIdentifier);

    guillaume::shapes::Rectangle rectangle;
    rectangle.setPosition(worldTransform.position);
    rectangle.setRotation(worldTransform.rotation);
    rectangle.setScale(worldTransform.scale);
    rectangle.setSize({bound.getSize()[0], bound.getSize()[1]});
    rectangle.setColor({color[0], color[1], color[2], color[3]});

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
}

void configureButtonRender(guillaume::ecs::ComponentRegistry &componentRegistry,
                           guillaume::ecs::Entity &button,
                           const std::array<std::uint8_t, 4> &normalColor,
                           const std::array<std::uint8_t, 4> &hoverColor,
                           const std::array<std::uint8_t, 4> &clickedColor) {
    auto &render =
        componentRegistry.getComponent<guillaume::components::Render>(
            button.getIdentifier());

    render.setNormalHandler(
        [normalColor](
            guillaume::ecs::ComponentRegistry &componentRegistry,
            const guillaume::ecs::Entity::Identifier &identityIdentifier,
            guillaume::Renderer &renderer) {
            drawButtonWithColor(componentRegistry, identityIdentifier, renderer,
                                normalColor);
        });

    render.setHoveredHandler(
        [hoverColor](
            guillaume::ecs::ComponentRegistry &componentRegistry,
            const guillaume::ecs::Entity::Identifier &identityIdentifier,
            guillaume::Renderer &renderer) {
            drawButtonWithColor(componentRegistry, identityIdentifier, renderer,
                                hoverColor);
        });

    render.setClickedHandler(
        [clickedColor](
            guillaume::ecs::ComponentRegistry &componentRegistry,
            const guillaume::ecs::Entity::Identifier &identityIdentifier,
            guillaume::Renderer &renderer) {
            drawButtonWithColor(componentRegistry, identityIdentifier, renderer,
                                clickedColor);
        });
}

} // namespace

Settings::Settings(void) : guillaume::Scene() {}

Settings::~Settings(void) {}

void Settings::onApplicationAttached(void) {
    auto &componentRegistry = getComponentRegistry();
    guillaume::entities::Panel::Director panelDirector;
    guillaume::entities::Button::Director buttonDirector;

    guillaume::entities::Panel::Builder panelBuilder(componentRegistry);
    panelDirector.constructPanel(panelBuilder, {960.0f, 540.0f, 0.0f},
                                 {600.0f, 300.0f, 0.0f});
    auto panel = panelBuilder.getProduct();
    addEntity(*panel);

    guillaume::entities::Button::Builder buttonBuilder(componentRegistry);
    buttonDirector.constructButton(buttonBuilder, {0.0f, 70.0f, 1.0f},
                                   {0.0f, 0.0f, 0.0f}, {220.0f, 70.0f, 0.0f},
                                   "Back To Main", panel->getIdentifier());
    auto backToMainButton = buttonBuilder.getProduct();
    addEntity(*backToMainButton);

    buttonDirector.constructButton(buttonBuilder, {0.0f, -60.0f, 1.0f},
                                   {0.0f, 0.0f, 0.0f}, {260.0f, 60.0f, 0.0f},
                                   "Settings Scene", panel->getIdentifier());
    auto settingsLabel = buttonBuilder.getProduct();
    addEntity(*settingsLabel);

    auto &panelRelationship =
        componentRegistry.getComponent<guillaume::components::Relationship>(
            panel->getIdentifier());
    panelRelationship.addChildIdentifier(backToMainButton->getIdentifier());
    panelRelationship.addChildIdentifier(settingsLabel->getIdentifier());

    auto &backToMainHover =
        componentRegistry.getComponent<guillaume::components::Hover>(
            backToMainButton->getIdentifier());
    auto &backToMainText =
        componentRegistry.getComponent<guillaume::components::Text>(
            backToMainButton->getIdentifier());
    backToMainHover.setOnHoverHandler(
        [&backToMainText]() { backToMainText.setContent("Back (hover)"); });
    backToMainHover.setOnUnhoverHandler(
        [&backToMainText]() { backToMainText.setContent("Back To Main"); });

    auto &backToMainClick =
        componentRegistry.getComponent<guillaume::components::Click>(
            backToMainButton->getIdentifier());
    backToMainClick.setOnClickHandler(
        [this](utility::event::MouseMotionEvent::MousePosition
               /* position */) { setActiveScene<Main>(); });

    configureButtonRender(componentRegistry, *backToMainButton,
                          {90, 90, 255, 255}, {130, 130, 255, 255},
                          {60, 60, 220, 255});

    configureButtonRender(componentRegistry, *settingsLabel, {50, 50, 50, 255},
                          {50, 50, 50, 255}, {50, 50, 50, 255});
}

} // namespace simple_application::scenes