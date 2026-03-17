#include "scenes/main.hpp"

#include "scenes/settings.hpp"

#include <guillaume/components/bound.hpp>
#include <guillaume/components/click.hpp>
#include <guillaume/components/hover.hpp>
#include <guillaume/components/relationship.hpp>
#include <guillaume/components/render.hpp>
#include <guillaume/components/text.hpp>

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/panel.hpp>

#include <guillaume/font.hpp>
#include <guillaume/local_storage.hpp>
#include <guillaume/session_storage.hpp>
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

    std::vector<utility::math::Vector<float, 3>> vertices = {
        {rectangle.getPosition()[0] - rectangle.getSize()[0] / 2.0f,
         rectangle.getPosition()[1] - rectangle.getSize()[1] / 2.0f,
         rectangle.getPosition()[2]},
        {rectangle.getPosition()[0] + rectangle.getSize()[0] / 2.0f,
         rectangle.getPosition()[1] - rectangle.getSize()[1] / 2.0f,
         rectangle.getPosition()[2]},
        {rectangle.getPosition()[0] + rectangle.getSize()[0] / 2.0f,
         rectangle.getPosition()[1] + rectangle.getSize()[1] / 2.0f,
         rectangle.getPosition()[2]},
        {rectangle.getPosition()[0] - rectangle.getSize()[0] / 2.0f,
         rectangle.getPosition()[1] + rectangle.getSize()[1] / 2.0f,
         rectangle.getPosition()[2]},
    };

    // gradient color example left to right red to blue
    // std::vector<utility::math::Color<uint8_t>> colors = {
    //     {255, 0, 0, color[3]},
    //     {0, 0, 255, color[3]},
    //     {0, 0, 255, color[3]},
    //     {255, 0, 0, color[3]},
    // };

    std::vector<utility::math::Color<uint8_t>> colors = {
        {color[0], color[1], color[2], color[3]},
        {color[0], color[1], color[2], color[3]},
        {color[0], color[1], color[2], color[3]},
        {color[0], color[1], color[2], color[3]},
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

Main::Main(void) : guillaume::Scene() {}

Main::~Main(void) {}

void Main::onApplicationAttached(void) {
    const int launchCount =
        getLocalStorage().getItemAs<int>("launchCount").value_or(0) + 1;
    getLocalStorage().setItem("launchCount", launchCount);

    const int sessionClicks =
        getSessionStorage().getItemAs<int>("sessionClicks").value_or(0);
    getLocalStorage().setItem("lastRunSessionClicks", sessionClicks);
    getSessionStorage().setItem("sessionClicks", 0);

    auto &componentRegistry = getComponentRegistry();
    guillaume::entities::Panel::Director panelDirector;
    guillaume::entities::Button::Director buttonDirector;

    guillaume::entities::Panel::Builder panelBuilder(componentRegistry);
    panelDirector.constructPanel(panelBuilder, {960.0f, 540.0f, 0.0f},
                                 {700.0f, 380.0f, 0.0f});
    auto panel = panelBuilder.getProduct();
    addEntity(*panel);

    guillaume::entities::Button::Builder buttonBuilder(componentRegistry);
    buttonDirector.constructButton(buttonBuilder, {-280.0f, 100.0f, 1.0f},
                                   {0.0f, 0.0f, 0.0f}, {200.0f, 70.0f, 0.0f},
                                   "Button 1", panel->getIdentifier());
    auto button1 = buttonBuilder.getProduct();
    addEntity(*button1);

    buttonDirector.constructButton(buttonBuilder, {280.0f, 100.0f, 1.0f},
                                   {0.0f, 0.0f, 0.0f}, {220.0f, 70.0f, 0.0f},
                                   "Open Settings", panel->getIdentifier());
    auto swapToSettingsButton = buttonBuilder.getProduct();
    addEntity(*swapToSettingsButton);

    auto &panelRelationship =
        componentRegistry.getComponent<guillaume::components::Relationship>(
            panel->getIdentifier());
    panelRelationship.addChildIdentifier(button1->getIdentifier());
    panelRelationship.addChildIdentifier(swapToSettingsButton->getIdentifier());

    auto &button1Text =
        componentRegistry.getComponent<guillaume::components::Text>(
            button1->getIdentifier());
    button1Text.setContent("Launch #" + std::to_string(launchCount));

    auto &button1Hover =
        componentRegistry.getComponent<guillaume::components::Hover>(
            button1->getIdentifier());
    button1Hover.setOnHoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1 (hover)"); });
    button1Hover.setOnUnhoverHandler(
        [&button1Text]() { button1Text.setContent("Button 1"); });

    auto &button1Click =
        componentRegistry.getComponent<guillaume::components::Click>(
            button1->getIdentifier());
    button1Click.setOnClickHandler(
        utility::event::MouseButtonEvent::MouseButton::LEFT,
        [&button1Text, this](utility::event::MouseMotionEvent::MousePosition
                             /* position */) {
            const int totalClicks =
                getLocalStorage().getItemAs<int>("totalClicks").value_or(0) + 1;
            const int newSessionClicks = getSessionStorage()
                                             .getItemAs<int>("sessionClicks")
                                             .value_or(0) +
                                         1;

            getLocalStorage().setItem("totalClicks", totalClicks);
            getSessionStorage().setItem("sessionClicks", newSessionClicks);

            button1Text.setContent(
                "Clicks: " + std::to_string(totalClicks) +
                " / Session: " + std::to_string(newSessionClicks));
        });
    button1Click.setOnReleaseHandler(
        utility::event::MouseButtonEvent::MouseButton::LEFT,
        [&button1Text](
            utility::event::MouseMotionEvent::MousePosition /* position */) {
            button1Text.setContent("Button 1");
        });

    configureButtonRender(componentRegistry, *button1, {255, 0, 0, 255},
                          {0, 255, 0, 255}, {0, 0, 255, 255});

    auto &swapToSettingsText =
        componentRegistry.getComponent<guillaume::components::Text>(
            swapToSettingsButton->getIdentifier());
    auto &swapToSettingsHover =
        componentRegistry.getComponent<guillaume::components::Hover>(
            swapToSettingsButton->getIdentifier());
    swapToSettingsHover.setOnHoverHandler([&swapToSettingsText]() {
        swapToSettingsText.setContent("Open Settings (hover)");
    });
    swapToSettingsHover.setOnUnhoverHandler([&swapToSettingsText]() {
        swapToSettingsText.setContent("Open Settings");
    });

    auto &swapToSettingsClick =
        componentRegistry.getComponent<guillaume::components::Click>(
            swapToSettingsButton->getIdentifier());
    swapToSettingsClick.setOnClickHandler(
        utility::event::MouseButtonEvent::MouseButton::LEFT,
        [this](utility::event::MouseMotionEvent::MousePosition
               /* position */) { setActiveScene<Settings>(); });
    configureButtonRender(componentRegistry, *swapToSettingsButton,
                          {255, 128, 0, 255}, {255, 180, 64, 255},
                          {220, 100, 0, 255});
}

} // namespace simple_application::scenes