/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "guillaume/entities/button.hpp"

#include <utility>

#include "guillaume/drawables/rectangle.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities {

Button::Button::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
                                 ecs::EntityRegistry &entityRegistry)
    : ecs::NodeEntityBuilder<Button>(componentRegistry, entityRegistry) {
    _iconBuilder =
        std::make_unique<Icon::Builder>(componentRegistry, entityRegistry);
    _textBuilder =
        std::make_unique<Text::Builder>(componentRegistry, entityRegistry);
    reset();
}

Button::Builder::~Builder(void) {}

void Button::Builder::registerEntity(void) {
    if (!_iconName.empty()) {
        _iconDirector.makeDefaultIcon(*_iconBuilder, _iconName);
    }
    if (!_label.empty()) {
        _textDirector.makeDefaultText(*_textBuilder, _label, 24,
                                      Text::Color{255, 255, 255, 255});
    }
    _button = std::make_unique<Button>(this->getComponentRegistry(),
                                       _toggleState, _colorStyle, _shape, _size,
                                       _morphState, nullptr, nullptr, _onClick);
    this->getEntityRegistry().addEntity(std::move(_button));
}

void Button::Builder::reset(void) {
    ecs::NodeEntityBuilder<Button>::reset();
    _button.reset();
    _toggleState = ToggleState::Default;
    _colorStyle = ColorStyle::Filled;
    _shape = Shape::Round;
    _size = Size::Small;
    _morphState = MorphState::Default;
    _icon.reset();
    _textLabel.reset();
    _iconName.clear();
    _label.clear();
    _onClick = {};
}

Button::Builder &Button::Builder::withIcon(const std::string &iconName) {
    _iconName = iconName;
    return *this;
}

Button::Builder &Button::Builder::withLabel(const std::string &label) {
    _label = label;
    return *this;
}

Button::Builder &
Button::Builder::withOnClick(const std::function<void(void)> &onClick) {
    _onClick = std::move(onClick);
    return *this;
}

Button::Builder &
Button::Builder::withToggleState(const ToggleState &toggleState) {
    _toggleState = toggleState;
    return *this;
}

Button::Builder &Button::Builder::withColorStyle(const ColorStyle &colorStyle) {
    _colorStyle = colorStyle;
    return *this;
}

Button::Builder &Button::Builder::withShape(const Shape &shape) {
    _shape = shape;
    return *this;
}

Button::Builder &Button::Builder::withSize(const Size &size) {
    _size = size;
    return *this;
}

Button::Builder &Button::Builder::withMorphState(const MorphState &morphState) {
    _morphState = morphState;
    return *this;
}

Button::Director::Director(void) : ecs::EntityDirector() {}

Button::Director::~Director(void) {}

void Button::Director::makeTextButton(Builder &builder,
                                      const std::string &label,
                                      std::function<void(void)> onClick) {
    builder.withLabel(label).withOnClick(onClick).registerEntity();
}

void Button::Director::makeIconButton(Builder &builder,
                                      const std::string &iconName,
                                      std::function<void(void)> onClick) {
    builder.withIcon(iconName).withOnClick(onClick).registerEntity();
}

void Button::Director::makeIconTextButton(Builder &builder,
                                          const std::string &iconName,
                                          const std::string &label,
                                          std::function<void(void)> onClick) {
    builder.withIcon(iconName)
        .withLabel(label)
        .withOnClick(onClick)
        .registerEntity();
}

namespace {
using Color = utility::math::Color<uint8_t>;

constexpr float kDisabledOpacity = 0.38f;
constexpr float kHoverOpacity = 0.08f;
constexpr float kFocusOpacity = 0.12f;
constexpr float kPressedOpacity = 0.12f;

const Color kTransparent{0, 0, 0, 0};

const Scheme &getButtonScheme(void) { return defaultTheme.getLightScheme(); }

Color getThemeColor(SchemeColorRole role) {
    return getButtonScheme().getColor(role).getColor();
}

// Apply opacity to color by modifying alpha channel
Color applyOpacity(Color color, float opacity) {
    uint8_t newAlpha =
        static_cast<uint8_t>(static_cast<float>(color.alpha()) * opacity);
    return Color(color.red(), color.green(), color.blue(), newAlpha);
}
} // namespace

// Get dimensions for button size
auto getButtonDimensions(Button::Size size)
    -> std::pair<float, float> { // width, height
    switch (size) {
    case Button::Size::ExtraSmall:
        return {40.0f, 32.0f}; // Compact size
    case Button::Size::Small:
        return {64.0f, 40.0f}; // M-16dp padding
    case Button::Size::Medium:
        return {96.0f, 48.0f}; // M default
    case Button::Size::Large:
        return {128.0f, 56.0f}; // L size
    case Button::Size::ExtraLarge:
        return {160.0f, 64.0f}; // XL size
    }
    return {96.0f, 48.0f}; // Default to Medium
}

// Get corner radius based on size, shape, and morph state
float getCornerRadius(Button::Size size, Button::Shape shape,
                      Button::MorphState morphState) {
    // Pressed state has reduced corner radius
    if (morphState == Button::MorphState::Pressed) {
        switch (size) {
        case Button::Size::ExtraSmall:
        case Button::Size::Small:
            return 8.0f;
        case Button::Size::Medium:
            return 12.0f;
        case Button::Size::Large:
        case Button::Size::ExtraLarge:
            return 16.0f;
        }
    }

    // Selected state has reduced corner radius (like pressed)
    if (morphState == Button::MorphState::Selected) {
        switch (size) {
        case Button::Size::ExtraSmall:
        case Button::Size::Small:
            return 12.0f;
        case Button::Size::Medium:
            return 16.0f;
        case Button::Size::Large:
        case Button::Size::ExtraLarge:
            return 28.0f;
        }
    }

    // Default state (round button)
    if (shape == Button::Shape::Round) {
        return 9999.0f; // Full radius for round button
    }

    // Square button default state
    switch (size) {
    case Button::Size::ExtraSmall:
    case Button::Size::Small:
        return 12.0f;
    case Button::Size::Medium:
        return 16.0f;
    case Button::Size::Large:
    case Button::Size::ExtraLarge:
        return 28.0f;
    }
    return 16.0f;
}

// Get colors based on style and state
auto getColorForStyle(Button::ColorStyle style,
                      [[maybe_unused]] Button::Size size, bool isEnabled,
                      bool isHovered, bool isFocused, bool isPressed,
                      Button::ToggleState toggleState) -> Color {
    const Color primary = getThemeColor(SchemeColorRole::Primary);
    const Color onPrimary = getThemeColor(SchemeColorRole::OnPrimary);
    const Color secondary = getThemeColor(SchemeColorRole::Secondary);
    const Color secondaryContainer =
        getThemeColor(SchemeColorRole::SecondaryContainer);
    const Color inverseSurface = getThemeColor(SchemeColorRole::InverseSurface);
    const Color inverseOnSurface =
        getThemeColor(SchemeColorRole::InverseOnSurface);

    if (!isEnabled) {
        // Disabled state - reduce opacity
        switch (style) {
        case Button::ColorStyle::Elevated:
        case Button::ColorStyle::Filled:
        case Button::ColorStyle::Tonal:
            return applyOpacity(onPrimary, kDisabledOpacity);
        case Button::ColorStyle::Outlined:
            return applyOpacity(primary, kDisabledOpacity);
        case Button::ColorStyle::Text:
            return applyOpacity(primary, kDisabledOpacity);
        }
    }

    switch (style) {
    case Button::ColorStyle::Elevated: {
        if (isPressed) {
            return primary;
        }
        if (isHovered) {
            return primary;
        }
        if (isFocused) {
            return primary;
        }
        return primary;
    }

    case Button::ColorStyle::Filled: {
        if (toggleState == Button::ToggleState::Selected) {
            return primary;
        }
        if (isPressed) {
            return applyOpacity(primary, 1.0f - kPressedOpacity);
        }
        if (isHovered) {
            return applyOpacity(primary, 1.0f - kHoverOpacity);
        }
        if (isFocused) {
            return applyOpacity(primary, 1.0f - kFocusOpacity);
        }
        return primary;
    }

    case Button::ColorStyle::Tonal: {
        if (toggleState == Button::ToggleState::Selected) {
            return secondary;
        }
        if (isPressed) {
            return applyOpacity(secondaryContainer, 1.0f - kPressedOpacity);
        }
        if (isHovered) {
            return applyOpacity(secondaryContainer, 1.0f - kHoverOpacity);
        }
        if (isFocused) {
            return applyOpacity(secondaryContainer, 1.0f - kFocusOpacity);
        }
        return secondaryContainer;
    }

    case Button::ColorStyle::Outlined: {
        // Outlined button has transparent container, visible outline
        if (toggleState == Button::ToggleState::Selected) {
            return inverseSurface;
        }
        if (isPressed) {
            return applyOpacity(inverseOnSurface, kPressedOpacity);
        }
        if (isHovered) {
            return applyOpacity(primary, kHoverOpacity);
        }
        if (isFocused) {
            return applyOpacity(primary, kFocusOpacity);
        }
        return kTransparent;
    }

    case Button::ColorStyle::Text: {
        // Text button has transparent container
        if (isPressed) {
            return applyOpacity(primary, kPressedOpacity);
        }
        if (isHovered) {
            return applyOpacity(primary, kHoverOpacity);
        }
        if (isFocused) {
            return applyOpacity(primary, kFocusOpacity);
        }
        return kTransparent;
    }
    }

    return primary;
}

// Get label/text color based on style and state
auto getLabelColorForStyle(Button::ColorStyle style, bool isEnabled,
                           Button::ToggleState toggleState) -> Color {
    const Color primary = getThemeColor(SchemeColorRole::Primary);
    const Color onPrimary = getThemeColor(SchemeColorRole::OnPrimary);
    const Color onSurface = getThemeColor(SchemeColorRole::OnSurface);
    const Color onSecondary = getThemeColor(SchemeColorRole::OnSecondary);
    const Color onSecondaryContainer =
        getThemeColor(SchemeColorRole::OnSecondaryContainer);
    const Color inverseOnSurface =
        getThemeColor(SchemeColorRole::InverseOnSurface);

    if (!isEnabled) {
        return applyOpacity(onSurface, kDisabledOpacity);
    }

    switch (style) {
    case Button::ColorStyle::Elevated:
    case Button::ColorStyle::Filled:
        return onPrimary;
    case Button::ColorStyle::Tonal:
        return (toggleState == Button::ToggleState::Selected)
                   ? onSecondary
                   : onSecondaryContainer;
    case Button::ColorStyle::Outlined:
        return (toggleState == Button::ToggleState::Selected) ? inverseOnSurface
                                                              : onSurface;
    case Button::ColorStyle::Text:
        return primary;
    }
    return onPrimary;
}

// Get outline color for outlined button
auto getOutlineColorForStyle(Button::ColorStyle style, bool isEnabled,
                             bool isFocused) -> Color {
    const Color primary = getThemeColor(SchemeColorRole::Primary);
    const Color onSurface = getThemeColor(SchemeColorRole::OnSurface);
    const Color outline = getThemeColor(SchemeColorRole::Outline);

    if (!isEnabled) {
        return applyOpacity(onSurface, kDisabledOpacity);
    }
    if (isFocused) {
        return primary;
    }
    if (style == Button::ColorStyle::Outlined) {
        return outline;
    }
    return kTransparent;
}

void Button::hoverHandler(void) {
    // State transitions will be handled by the render system
    // This handler is called when mouse enters the button
}

void Button::unHoverHandler(void) {
    // State transitions will be handled by the render system
    // This handler is called when mouse leaves the button
}

void Button::leftClickHandler(
    utility::event::MouseMotionEvent::MousePosition mousePosition) {
    if (_toggleState == ToggleState::Default) {
        _toggleState = ToggleState::Selected;
        _morphState = MorphState::Selected;
    } else {
        _toggleState = ToggleState::Default;
        _morphState = MorphState::Default;
    }
    if (_onClick) {
        _onClick();
    }
}

void Button::normalRender(ecs::ComponentRegistry &registry,
                          const ecs::Entity::Identifier &id,
                          Renderer &renderer) {
    auto &transform = registry.getComponent<components::Transform>(id);
    auto &bound = registry.getComponent<components::Bound>(id);

    // Get button dimensions based on size
    auto [width, height] = getButtonDimensions(_size);
    bound.setSize({width, height});

    // Calculate corner radius for enabled state
    float cornerRadius = getCornerRadius(_size, _shape, MorphState::Default);

    // Get container color
    Color containerColor = getColorForStyle(_colorStyle, _size, true, false,
                                            false, false, _toggleState);

    // Draw container
    drawables::Rectangle rectangle(
        transform.getPosition(), transform.getRotation(), transform.getScale(),
        containerColor, bound.getSize(), cornerRadius);
    auto vertices = rectangle.getVertices();
    renderer.drawVertices(vertices);

    // Draw outline if outlined style
    if (_colorStyle == ColorStyle::Outlined) {
        // Outline rendering with different line style would go here
        // Color outlineColor = getOutlineColorForStyle(_colorStyle, true,
        // false);
    }
}

void Button::hoveredRender(ecs::ComponentRegistry &registry,
                           const ecs::Entity::Identifier &id,
                           Renderer &renderer) {
    auto &transform = registry.getComponent<components::Transform>(id);
    auto &bound = registry.getComponent<components::Bound>(id);

    // Get button dimensions
    auto [width, height] = getButtonDimensions(_size);
    bound.setSize({width, height});

    // Calculate corner radius (remain default shape on hover)
    float cornerRadius = getCornerRadius(_size, _shape, MorphState::Default);

    // Get container color with hover state layer
    Color containerColor = getColorForStyle(_colorStyle, _size, true, true,
                                            false, false, _toggleState);

    // Draw container
    drawables::Rectangle rectangle(
        transform.getPosition(), transform.getRotation(), transform.getScale(),
        containerColor, bound.getSize(), cornerRadius);
    auto vertices = rectangle.getVertices();
    renderer.drawVertices(vertices);

    // Draw outline if outlined style
    if (_colorStyle == ColorStyle::Outlined) {
        // Outline rendering with different line style would go here
        // Color outlineColor = getOutlineColorForStyle(_colorStyle, true,
        // false);
    }
}

void Button::clickedRender(ecs::ComponentRegistry &registry,
                           const ecs::Entity::Identifier &id,
                           Renderer &renderer) {
    auto &transform = registry.getComponent<components::Transform>(id);
    auto &bound = registry.getComponent<components::Bound>(id);

    // Get button dimensions
    auto [width, height] = getButtonDimensions(_size);
    bound.setSize({width, height});

    // Calculate corner radius for pressed state (morphs to square)
    float cornerRadius = getCornerRadius(_size, _shape, MorphState::Pressed);

    // Get container color with pressed state layer
    Color containerColor = getColorForStyle(_colorStyle, _size, true, false,
                                            false, true, _toggleState);

    // Draw container with morphed shape
    drawables::Rectangle rectangle(
        transform.getPosition(), transform.getRotation(), transform.getScale(),
        containerColor, bound.getSize(), cornerRadius);
    auto vertices = rectangle.getVertices();
    renderer.drawVertices(vertices);

    // Draw outline if outlined style
    if (_colorStyle == ColorStyle::Outlined) {
        // Outline rendering with different line style would go here
        // Color outlineColor = getOutlineColorForStyle(_colorStyle, true,
        // false);
    }
}

void Button::activeRender(ecs::ComponentRegistry &registry,
                          const ecs::Entity::Identifier &id,
                          Renderer &renderer) {
    // Active state is used for selected/toggled state
    auto &transform = registry.getComponent<components::Transform>(id);
    auto &bound = registry.getComponent<components::Bound>(id);

    // Get button dimensions
    auto [width, height] = getButtonDimensions(_size);
    bound.setSize({width, height});

    // Calculate corner radius for selected state (morphs to square)
    float cornerRadius = getCornerRadius(_size, _shape, MorphState::Selected);

    // Get container color with selected state
    Color containerColor = getColorForStyle(
        _colorStyle, _size, true, false, false, false, ToggleState::Selected);

    // Draw container with morphed shape
    drawables::Rectangle rectangle(
        transform.getPosition(), transform.getRotation(), transform.getScale(),
        containerColor, bound.getSize(), cornerRadius);
    auto vertices = rectangle.getVertices();
    renderer.drawVertices(vertices);

    // Draw outline if outlined style
    if (_colorStyle == ColorStyle::Outlined) {
        // Outline rendering with different line style would go here
        // Color outlineColor = getOutlineColorForStyle(_colorStyle, true,
        // false);
    }
}

Button::Button(ecs::ComponentRegistry &registry, ToggleState toggleState,
               ColorStyle colorStyle, Shape shape, Size size,
               MorphState morphState, std::unique_ptr<ecs::Entity> icon,
               std::unique_ptr<ecs::Entity> label,
               std::function<void(void)> onClick)
    : ecs::NodeEntityFiller<components::Transform, components::Bound,
                            components::Hover, components::Click,
                            components::Render>(registry),
      _toggleState(toggleState), _colorStyle(colorStyle), _shape(shape),
      _size(size), _morphState(morphState), _icon(std::move(icon)),
      _label(std::move(label)), _onClick(std::move(onClick)) {
    // Initialize button position, rotation, and scale
    registry.getComponent<components::Transform>(getIdentifier())
        .setPosition({100.0f, 100.0f, 100.0f})
        .setRotation({0.0f, 0.0f, 0.0f})
        .setScale({1.0f, 1.0f, 1.0f});

    // Initialize button size based on Material Design 3 specs
    auto [width, height] = getButtonDimensions(_size);
    registry.getComponent<components::Bound>(getIdentifier())
        .setSize({width, height});

    // Set up hover event handlers
    registry.getComponent<components::Hover>(getIdentifier())
        .setOnHoverHandler([this]() { this->hoverHandler(); })
        .setOnUnhoverHandler([this]() { this->unHoverHandler(); });

    // Set up click event handlers
    registry.getComponent<components::Click>(getIdentifier())
        .setOnClickHandler(
            utility::event::MouseButtonEvent::MouseButton::LEFT,
            [this](
                utility::event::MouseMotionEvent::MousePosition mousePosition) {
                this->leftClickHandler(mousePosition);
            });

    // Set up render handlers for different button states
    registry.getComponent<components::Render>(getIdentifier())
        .setNormalHandler([this](ecs::ComponentRegistry &registry,
                                 const ecs::Entity::Identifier &id,
                                 Renderer &renderer) {
            this->normalRender(registry, id, renderer);
        })
        .setHoveredHandler([this](ecs::ComponentRegistry &registry,
                                  const ecs::Entity::Identifier &id,
                                  Renderer &renderer) {
            this->hoveredRender(registry, id, renderer);
        })
        .setClickedHandler([this](ecs::ComponentRegistry &registry,
                                  const ecs::Entity::Identifier &id,
                                  Renderer &renderer) {
            this->clickedRender(registry, id, renderer);
        })
        .setActiveHandler([this](ecs::ComponentRegistry &registry,
                                 const ecs::Entity::Identifier &id,
                                 Renderer &renderer) {
            this->activeRender(registry, id, renderer);
        });
}

Button::~Button() {}

} // namespace guillaume::entities