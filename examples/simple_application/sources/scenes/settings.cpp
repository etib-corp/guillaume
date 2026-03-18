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
    static const std::array<std::string, 6> candidates = {
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
    guillaume::Renderer &renderer, const utility::math::Color<uint8_t> &color) {
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
    rectangle.setColor(color);

    const auto position = rectangle.getPosition();
    const auto size = rectangle.getSize();
    const auto scale = rectangle.getScale();
    const float halfWidth =
        static_cast<float>(size[0]) * static_cast<float>(scale[0]) * 0.5f;
    const float halfHeight =
        static_cast<float>(size[1]) * static_cast<float>(scale[1]) * 0.5f;
    const float rotation = rectangle.getRotation()[2];
    const float cosR = std::cos(rotation);
    const float sinR = std::sin(rotation);

    const utility::math::Vector<float, 3> localBottomLeft{-halfWidth,
                                                        -halfHeight, 0.0f};
    const utility::math::Vector<float, 3> localBottomRight{halfWidth,
                                                        -halfHeight, 0.0f};
    const utility::math::Vector<float, 3> localTopRight{halfWidth, halfHeight,
                                                        0.0f};
    const utility::math::Vector<float, 3> localTopLeft{-halfWidth, halfHeight,
                                                    0.0f};
    auto rotateAndTranslate =
        [&](const utility::math::Vector<float, 3> &local)
        -> utility::math::Vector<float, 3> {
            const float x = local[0];
            const float y = local[1];
            const float rotatedX = x * cosR - y * sinR;
            const float rotatedY = x * sinR + y * cosR;
            return {position[0] + rotatedX, position[1] + rotatedY,
                    position[2]};
        };

    std::array<utility::math::Vector<float, 3>, 4> vertices = {
        rotateAndTranslate(localBottomLeft),
        rotateAndTranslate(localBottomRight),
        rotateAndTranslate(localTopRight),
        rotateAndTranslate(localTopLeft),
    };

    std::array<utility::math::Color<uint8_t>, 4> colors = {
        rectangle.getColor(), rectangle.getColor(), rectangle.getColor(),
        rectangle.getColor(),
    };

    std::vector<utility::math::Vertex<float, uint8_t>> vertexData = {
        utility::math::Vertex<float, uint8_t>(vertices[0], {0.0f, 0.0f, 1.0f},
                                               {0.0f, 0.0f}, colors[0]),
        utility::math::Vertex<float, uint8_t>(vertices[1], {0.0f, 0.0f, 1.0f},
                                               {1.0f, 0.0f}, colors[1]),
        utility::math::Vertex<float, uint8_t>(vertices[2], {0.0f, 0.0f, 1.0f},
                                               {1.0f, 1.0f}, colors[2]),
        utility::math::Vertex<float, uint8_t>(vertices[3], {0.0f, 0.0f, 1.0f},
                                               {0.0f, 1.0f}, colors[3]),
    };

    renderer.drawVertices(vertexData);

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
                           const utility::math::Color<uint8_t> &normalColor,
                           const utility::math::Color<uint8_t> &hoverColor,
                           const utility::math::Color<uint8_t> &clickedColor) {
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
        utility::event::MouseButtonEvent::MouseButton::LEFT,
        [this](utility::event::MouseMotionEvent::MousePosition
               /* position */) { setActiveScene<Main>(); });

    configureButtonRender(componentRegistry, *backToMainButton,
                          {90, 90, 255, 255}, {130, 130, 255, 255},
                          {60, 60, 220, 255});

    configureButtonRender(componentRegistry, *settingsLabel, {50, 50, 50, 255},
                          {50, 50, 50, 255}, {50, 50, 50, 255});
}

} // namespace simple_application::scenes