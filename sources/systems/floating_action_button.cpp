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

#include "guillaume/systems/floating_action_button.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/entities/button_style.hpp"
#include "guillaume/entities/fab_style.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/button.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Mode = components::FloatingActionButton::Mode;
	}	 // namespace

	FloatingActionButton::FloatingActionButton(void)
		: ecs::SystemFiller<components::FloatingActionButton,
							components::Button>(ecs::Phase::Layout)
	{
	}

	void
		FloatingActionButton::install(ecs::ComponentRegistry &registry,
									  const ecs::Entity::Identifier &identifier)
	{
		if (!registry.hasComponent<components::Elevation>(identifier)) {
			registry.addComponent<components::Elevation>(identifier)
				.setElevationLevel(theme::ElevationLevel::Level0);
		}
		Button::install(registry, identifier);
		registry.getComponent<components::Button>(identifier)
			.setRestyle(&FloatingActionButton::apply);
	}

	float FloatingActionButton::iconFontSize(
		const ecs::ComponentRegistry &registry,
		const ecs::Entity::Identifier &identifier)
	{
		const auto &config =
			registry.getComponent<components::FloatingActionButton>(identifier);
		return entities::fab_style::getIconFontSize(config.getSize());
	}

	utility::graphic::Color32Bit FloatingActionButton::contentColor(
		const ecs::ComponentRegistry &registry,
		const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::FloatingActionButton>(identifier);
		return Button::resolveContentColor(
			button, entities::fab_style::getContentColor(config.getVariant()));
	}

	void FloatingActionButton::elevation(
		ecs::ComponentRegistry &registry,
		const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::FloatingActionButton>(identifier);
		const bool disabled = Button::isDisabled(registry, identifier);
		const bool hovered	= Button::isHovered(registry, identifier);
		const bool pressed	= Button::isPressed(registry, identifier);
		const bool visible	= button.isVisible();
		const auto mode		= config.getMode();
		const bool lowered	= config.isLowered();
		// M3 FAB: level 3 at rest (1 when lowered), one level up on hover,
		// and flat when disabled.
		auto level = lowered ? theme::ElevationLevel::Level1
							 : theme::ElevationLevel::Level3;
		if (disabled || !visible || mode == Mode::Flat) {
			level = theme::ElevationLevel::Level0;
		} else if (hovered && !pressed) {
			level = static_cast<theme::ElevationLevel>(
				static_cast<std::size_t>(level) + 1);
		}
		registry.getComponent<components::Elevation>(identifier)
			.setElevationLevel(level);
	}

	void FloatingActionButton::styleState(
		ecs::ComponentRegistry &registry,
		const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::FloatingActionButton>(identifier);
		const Button::Child icon(registry, button.getIcon());
		const Button::Child label(registry, button.getLabel());
		const bool disabled = Button::isDisabled(registry, identifier);
		const bool hovered	= Button::isHovered(registry, identifier);
		const bool pressed	= Button::isPressed(registry, identifier);
		const bool visible	= button.isVisible();
		const auto variant	= config.getVariant();
		const bool lowered	= config.isLowered();
		elevation(registry, identifier);

		if (!visible) {
			registry.getComponent<components::Color>(identifier)
				.setColor(entities::transparentColor());
			if (icon) {
				icon.setColor(entities::transparentColor());
			}
			if (label) {
				label.setColor(entities::transparentColor());
			}
			return;
		}

		if (disabled) {
			registry.getComponent<components::Color>(identifier)
				.setColor(entities::disabledContainerColor());

			if (icon) {
				icon.setColor(entities::disabledContentColor());
			}

			if (label) {
				label.setColor(entities::disabledContentColor());
			}

			return;
		}

		const auto baseContainerColor = Button::resolveContainerColor(
			button, entities::fab_style::getContainerColor(variant, lowered));
		const auto contentColor = Button::resolveContentColor(
			button, entities::fab_style::getContentColor(variant));

		utility::graphic::Color32Bit containerColor = baseContainerColor;

		if (pressed) {
			containerColor = entities::stateLayer(
				baseContainerColor, contentColor, theme::StateLayer::Pressed);
		} else if (hovered) {
			containerColor = entities::stateLayer(
				baseContainerColor, contentColor, theme::StateLayer::Hovered);
		}

		registry.getComponent<components::Color>(identifier)
			.setColor(containerColor);

		if (icon) {
			icon.setColor(contentColor);
		}

		if (label) {
			label.setColor(contentColor);
		}
	}

	void FloatingActionButton::geometry(
		ecs::ComponentRegistry &registry,
		const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::FloatingActionButton>(identifier);
		const Button::Child icon(registry, button.getIcon());
		const Button::Child label(registry, button.getLabel());
		const auto layer	   = Layout::layerOf(registry, identifier);
		const auto size		   = config.getSize();
		const auto &customSize = config.getCustomSize();
		const float iconSize   = iconFontSize(registry, identifier);
		const float containerSize =
			customSize.value_or(entities::fab_style::getContainerSize(size));
		const float fontHeight = iconSize;

		registry.getComponent<components::Bound>(identifier)
			.setWidth(containerSize)
			.setHeight(containerSize);
		Button::applyTouchTarget(registry, identifier, containerSize,
								 containerSize);

		registry.getComponent<components::Borders>(identifier)
			.setBorderRadius(customSize.has_value()
								 // Paper: customSize / 3.5 (16 dp at 56 dp).
								 ? *customSize / 3.5f
								 : entities::fab_style::getBorderRadius(size));

		if (icon) {
			icon.setFontSize(fontHeight);
		}

		if (label) {
			label.setTypeRole(theme::TypeRole::LabelLarge);
			label.setContent(button.getLabelContent());
		}

		if (!icon && !label) {
			return;
		}

		const auto &buttonPose =
			registry.getComponent<components::Transform>(identifier).getPose();

		const float iconWidth	= icon
			  ? registry.getComponent<components::Bound>(icon.getIdentifier())
					.getWidth()
			  : 0.0f;
		const float iconHeight	= icon
			 ? registry.getComponent<components::Bound>(icon.getIdentifier())
				   .getHeight()
			 : 0.0f;
		const float labelWidth	= label
			 ? registry.getComponent<components::Bound>(label.getIdentifier())
				   .getWidth()
			 : 0.0f;
		const float labelHeight = label
			? registry.getComponent<components::Bound>(label.getIdentifier())
				  .getHeight()
			: 0.0f;

		const float gap			 = (icon && label) ? 8.0f : 0.0f;
		const float contentWidth = iconWidth + gap + labelWidth;

		// Content is centered in the container; when the row is wider than the
		// container the bound grows so nothing is clipped.
		const float width = std::max(containerSize, contentWidth);
		const float startX =
			buttonPose.getPosition().x + (width - contentWidth) / 2.0f;

		if (icon) {
			const float iconY = buttonPose.getPosition().y
				+ (containerSize - iconHeight) / 2.0f;

			const auto iconPose = Layout::contentLayerPose(
				utility::graphic::PositionF(startX, iconY,
											buttonPose.getPosition().z),
				buttonPose.getOrientation(), layer);

			registry.getComponent<components::Transform>(icon.getIdentifier())
				.setPose(iconPose);
		}

		if (label) {
			const float labelX = startX + iconWidth + gap;
			const float labelY = buttonPose.getPosition().y
				+ (containerSize - labelHeight) / 2.0f;

			const auto labelPose = Layout::contentLayerPose(
				utility::graphic::PositionF(labelX, labelY,
											buttonPose.getPosition().z),
				buttonPose.getOrientation(), layer);

			registry.getComponent<components::Transform>(label.getIdentifier())
				.setPose(labelPose);
		}

		if (width > containerSize) {
			registry.getComponent<components::Bound>(identifier)
				.setWidth(width);
		}
	}

	void FloatingActionButton::apply(ecs::ComponentRegistry &registry,
									 const ecs::Entity::Identifier &identifier)
	{
		styleState(registry, identifier);
		geometry(registry, identifier);
	}

	void FloatingActionButton::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::FloatingActionButton>(
				entityIdentifier)
			|| !requireComponent<components::Button>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
