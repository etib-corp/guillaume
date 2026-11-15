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

#include "guillaume/systems/icon_button.hpp"

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
#include "guillaume/systems/button_style.hpp"
#include "guillaume/systems/fab_style.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/button.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::IconButton::Variant;
		using Role	  = SchemeColorRole;

		/// Metrics of one icon button size.
		struct SizeMetrics {
			float containerSize;	///< Container extent.
			float iconFontSize;		///< Icon font size.
		};

		/// Indexed by components::IconButton::Size.
		constexpr std::array<SizeMetrics, 3> sizeTable { {
			{ 32.0f, 18.0f },	 // Small
			{ 40.0f, 24.0f },	 // Medium
			{ 48.0f, 30.0f },	 // Large
		} };

		/// Paper v5 IconButton/utils.ts colors of one variant. No container
		/// role means transparent.
		struct Palette {
			std::optional<Role> activeContainer;	///< Container when active.
			std::optional<Role> idleContainer;		///< Container otherwise.
			Role activeContent;						///< Content when active.
			Role idleContent;						///< Content otherwise.
			/// True when a plain (non-toggle) button uses the active palette;
			/// otherwise only a selected button does.
			bool plainIsActive;
		};

		/// Indexed by components::IconButton::Variant.
		constexpr std::array<Palette, 4> paletteTable { {
			// Standard
			{ std::nullopt, std::nullopt, Role::Primary, Role::OnSurfaceVariant,
			  false },
			// Filled
			{ Role::Primary, Role::SurfaceVariant, Role::OnPrimary,
			  Role::Primary, true },
			// Tonal
			{ Role::SecondaryContainer, Role::SurfaceVariant,
			  Role::OnSecondaryContainer, Role::OnSurfaceVariant, true },
			// Outlined
			{ Role::InverseSurface, std::nullopt, Role::InverseOnSurface,
			  Role::OnSurfaceVariant, false },
		} };

		const SizeMetrics &metrics(components::IconButton::Size size)
		{
			const auto index = static_cast<std::size_t>(size);
			if (index >= sizeTable.size()) {
				throw std::runtime_error("Invalid icon button size");
			}
			return sizeTable[index];
		}

		const Palette &palette(components::IconButton::Variant variant)
		{
			const auto index = static_cast<std::size_t>(variant);
			if (index >= paletteTable.size()) {
				throw std::runtime_error("Invalid icon button variant");
			}
			return paletteTable[index];
		}

		bool isActive(const Palette &entry, bool isToggle, bool isSelected)
		{
			return isSelected || (entry.plainIsActive && !isToggle);
		}

		static float getContainerSize(components::IconButton::Size size)
		{
			return metrics(size).containerSize;
		}

		static float getIconFontSize(components::IconButton::Size size)
		{
			return metrics(size).iconFontSize;
		}

		static float getBorderRadius(components::IconButton::Size size)
		{
			// Paper/M3 icon buttons are circular at every size.
			static_cast<void>(metrics(size));
			return getShapeRadius(theme::ShapeRole::Full);
		}

		static utility::graphic::Color32Bit
			getContainerColorFor(components::IconButton::Variant variant,
								 bool isToggle, bool isSelected)
		{
			const Palette &entry = palette(variant);
			const auto role		 = isActive(entry, isToggle, isSelected)
					 ? entry.activeContainer
					 : entry.idleContainer;

			return role ? getActiveColor(*role)
						: utility::graphic::Color32Bit(0, 0, 0, 0);
		}

		static utility::graphic::Color32Bit
			getContentColor(components::IconButton::Variant variant,
							bool isToggle, bool isSelected)
		{
			const Palette &entry = palette(variant);

			return getActiveColor(isActive(entry, isToggle, isSelected)
									  ? entry.activeContent
									  : entry.idleContent);
		}

		/**
		 * Blend a state layer; over a transparent container the layer is the
		 * content color at the layer opacity.
		 */
		static utility::graphic::Color32Bit
			stateLayerOver(const utility::graphic::Color32Bit &base,
						   const utility::graphic::Color32Bit &content,
						   theme::StateLayer layer)
		{
			if (base.getAlpha() == 0) {
				return style::withAlpha(content, getStateLayerAlpha(layer));
			}
			return style::stateLayer(base, content, layer);
		}

		static utility::graphic::Color32Bit
			getBorderColor(components::IconButton::Variant variant,
						   bool isSelected)
		{
			const auto &scheme = guillaume::getActiveScheme();

			if (variant != components::IconButton::Variant::Outlined) {
				return utility::graphic::Color32Bit(0, 0, 0, 0);
			}

			if (isSelected) {
				return utility::graphic::Color32Bit(0, 0, 0, 0);
			}

			return scheme.getColor(SchemeColorRole::Outline).getColor();
		}

	}	 // namespace

	IconButton::IconButton(void)
		: ecs::SystemFiller<components::IconButton, components::Button>(
			  ecs::Phase::Layout)
	{
	}

	void IconButton::install(ecs::ComponentRegistry &registry,
							 const ecs::Entity::Identifier &identifier)
	{
		if (!registry.hasComponent<components::Elevation>(identifier)) {
			registry.addComponent<components::Elevation>(identifier)
				.setElevationLevel(theme::ElevationLevel::Level0);
		}
		Button::install(registry, identifier);
		registry.getComponent<components::Button>(identifier)
			.setRestyle(&IconButton::apply);
	}

	float IconButton::iconFontSize(const ecs::ComponentRegistry &registry,
								   const ecs::Entity::Identifier &identifier)
	{
		const auto &config =
			registry.getComponent<components::IconButton>(identifier);
		return getIconFontSize(config.getSize());
	}

	utility::graphic::Color32Bit
		IconButton::contentColor(const ecs::ComponentRegistry &registry,
								 const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::IconButton>(identifier);
		return Button::resolveContentColor(
			button,
			getContentColor(config.getVariant(), button.isToggle(),
							button.isSelected()));
	}

	void IconButton::styleState(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::IconButton>(identifier);
		const Button::Child icon(registry, button.getIcon());
		const bool disabled = Button::isDisabled(registry, identifier);
		const bool hovered	= Button::isHovered(registry, identifier);
		const bool pressed	= Button::isPressed(registry, identifier);
		const bool toggle	= button.isToggle();
		const bool selected = button.isSelected();
		const auto variant	= config.getVariant();
		if (disabled) {
			registry.getComponent<components::Color>(identifier)
				.setColor(
					(variant == Variant::Filled || variant == Variant::Tonal)
						? style::disabledContainerColor()
						: style::transparentColor());

			registry.getComponent<components::Borders>(identifier)
				.setColor(variant == Variant::Outlined
							  ? style::disabledContainerColor()
							  : style::transparentColor())
				.setWidth(1.0f);

			if (icon) {
				icon.setColor(style::disabledContentColor());
			}

			return;
		}

		const auto baseContainerColor = Button::resolveContainerColor(
			button, getContainerColorFor(variant, toggle, selected));
		const auto contentColor = Button::resolveContentColor(
			button, getContentColor(variant, toggle, selected));

		utility::graphic::Color32Bit containerColor = baseContainerColor;

		if (pressed) {
			containerColor = stateLayerOver(baseContainerColor, contentColor,
											theme::StateLayer::Pressed);
		} else if (hovered) {
			containerColor = stateLayerOver(baseContainerColor, contentColor,
											theme::StateLayer::Hovered);
		}

		registry.getComponent<components::Color>(identifier)
			.setColor(containerColor);

		registry.getComponent<components::Borders>(identifier)
			.setColor(getBorderColor(variant, selected))
			.setWidth(1.0f);

		if (icon) {
			icon.setColor(contentColor);
		}
	}

	void IconButton::geometry(ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::IconButton>(identifier);
		const Button::Child icon(registry, button.getIcon());
		const auto layer		  = Layout::layerOf(registry, identifier);
		const auto size			  = config.getSize();
		const float iconSize	  = iconFontSize(registry, identifier);
		const float containerSize = getContainerSize(size);

		registry.getComponent<components::Bound>(identifier)
			.setWidth(containerSize)
			.setHeight(containerSize);
		Button::applyTouchTarget(registry, identifier, containerSize,
								 containerSize);

		registry.getComponent<components::Borders>(identifier)
			.setBorderRadius(getBorderRadius(size));

		if (icon) {
			icon.setFontSize(iconSize);

			const auto &buttonPose =
				registry.getComponent<components::Transform>(identifier)
					.getPose();
			const auto &iconBound =
				registry.getComponent<components::Bound>(icon.getIdentifier());

			const float iconX = buttonPose.getPosition().x
				+ (containerSize - iconBound.getWidth()) / 2.0f;
			const float iconY = buttonPose.getPosition().y
				- (containerSize - iconBound.getHeight()) / 2.0f;

			const auto iconPose = Layout::contentLayerPose(
				utility::graphic::PositionF(iconX, iconY,
											buttonPose.getPosition().z),
				buttonPose.getOrientation(), layer);

			registry.getComponent<components::Transform>(icon.getIdentifier())
				.setPose(iconPose);
		}
	}

	void IconButton::apply(ecs::ComponentRegistry &registry,
						   const ecs::Entity::Identifier &identifier)
	{
		styleState(registry, identifier);
		geometry(registry, identifier);
	}

	void IconButton::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::IconButton>(entityIdentifier)
			|| !requireComponent<components::Button>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
