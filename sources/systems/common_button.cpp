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

#include "guillaume/systems/common_button.hpp"

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
		using Color = components::CommonButton::Color;
		using Size	= components::CommonButton::Size;
	}	 // namespace

	CommonButton::CommonButton(void)
		: ecs::SystemFiller<components::CommonButton, components::Button>(
			  ecs::Phase::Layout)
	{
	}

	void CommonButton::install(ecs::ComponentRegistry &registry,
							   const ecs::Entity::Identifier &identifier)
	{
		if (!registry.hasComponent<components::Elevation>(identifier)) {
			registry.addComponent<components::Elevation>(identifier)
				.setElevationLevel(theme::ElevationLevel::Level0);
		}
		Button::install(registry, identifier);
		registry.getComponent<components::Button>(identifier)
			.setRestyle(&CommonButton::apply);
	}

	float CommonButton::iconFontSize(const ecs::ComponentRegistry &registry,
									 const ecs::Entity::Identifier &identifier)
	{
		const auto &config =
			registry.getComponent<components::CommonButton>(identifier);
		return entities::button_style::getIconFontSize(config.getSize());
	}

	utility::graphic::Color32Bit
		CommonButton::contentColor(const ecs::ComponentRegistry &registry,
								   const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::CommonButton>(identifier);
		return entities::button_style::getContentColor(
			config.getColorStyle(), button.isToggle(), button.isSelected());
	}

	void CommonButton::styleState(ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::CommonButton>(identifier);
		const Button::Child icon(registry, button.getIcon());
		const Button::Child label(registry, button.getLabel());
		const bool disabled	  = Button::isDisabled(registry, identifier);
		const bool hovered	  = Button::isHovered(registry, identifier);
		const bool pressed	  = Button::isPressed(registry, identifier);
		const bool toggle	  = button.isToggle();
		const bool selected	  = button.isSelected();
		const auto colorStyle = config.getColorStyle();
		// M3 elevated buttons rest at level 1 and lift to level 2 on hover;
		// every other style, and any disabled button, is flat.
		auto level = theme::ElevationLevel::Level0;
		if (colorStyle == Color::Elevated && !disabled) {
			level = (hovered && !pressed) ? theme::ElevationLevel::Level2
										  : theme::ElevationLevel::Level1;
		}
		registry.getComponent<components::Elevation>(identifier)
			.setElevationLevel(level);

		if (disabled) {
			registry.getComponent<components::Color>(identifier)
				.setColor(entities::disabledContainerColor());

			registry.getComponent<components::Borders>(identifier)
				.setColor(entities::disabledContainerColor())
				.setWidth(1.0f);

			if (icon) {
				icon.setColor(entities::disabledContentColor());
			}

			if (label) {
				label.setColor(entities::disabledContentColor());
			}

			return;
		}

		registry.getComponent<components::Color>(identifier)
			.setColor(Button::resolveContainerColor(
				button,
				entities::button_style::getContainerColor(
					colorStyle, toggle, selected, hovered, pressed)));

		registry.getComponent<components::Borders>(identifier)
			.setColor(entities::button_style::getBorderColor(colorStyle, toggle,
															 selected))
			.setWidth(1.0f);

		if (icon) {
			icon.setColor(Button::resolveContentColor(
				button,
				entities::button_style::getContentColor(colorStyle, toggle,
														selected)));
		}

		if (label) {
			label.setColor(Button::resolveContentColor(
				button,
				entities::button_style::getContentColor(colorStyle, toggle,
														selected)));
		}
	}

	void CommonButton::geometry(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);
		const auto &config =
			registry.getComponent<components::CommonButton>(identifier);
		const Button::Child icon(registry, button.getIcon());
		const Button::Child label(registry, button.getLabel());
		const bool pressed		= Button::isPressed(registry, identifier);
		const bool toggle		= button.isToggle();
		const bool selected		= button.isSelected();
		const bool morph		= button.isMorph();
		const bool trailingIcon = button.isTrailingIcon();
		const auto layer		= Layout::layerOf(registry, identifier);
		const auto shape		= config.getShape();
		const auto size			= config.getSize();
		const bool compact		= config.isCompact();
		const float minWidth	= config.getMinWidth();
		const bool uppercase	= config.isUppercase();
		const float iconSize	= iconFontSize(registry, identifier);
		const float radius		= entities::button_style::getBorderRadius(
			 size,
			 entities::button_style::getRestingShape(shape, toggle, selected,
														 morph),
			 pressed);

		// A button joined to its neighbours keeps the corners its group gave
		// it; writing the uniform radius first would flip Borders each frame.
		auto &borders = registry.getComponent<components::Borders>(identifier);
		if (const auto &corners = config.getCorners()) {
			borders.setTopLeftRadius(corners->topLeft)
				.setTopRightRadius(corners->topRight)
				.setBottomRightRadius(corners->bottomRight)
				.setBottomLeftRadius(corners->bottomLeft);
		} else {
			borders.setBorderRadius(radius);
		}

		if (!icon && !label) {
			return;
		}

		const float fontHeight = iconSize;

		if (icon) {
			icon.setFontSize(fontHeight);
		}

		if (label) {
			label.setTypeRole(theme::TypeRole::LabelLarge);
			std::string content = button.getLabelContent();
			if (uppercase) {
				std::transform(content.begin(), content.end(), content.begin(),
							   [](unsigned char character) {
								   return static_cast<char>(
									   std::toupper(character));
							   });
			}
			label.setContent(content);
		}

		const auto &buttonPose =
			registry.getComponent<components::Transform>(identifier).getPose();

		const bool hasIcon	= static_cast<bool>(icon);
		const bool hasLabel = static_cast<bool>(label);

		const float iconWidth	= hasIcon
			  ? registry.getComponent<components::Bound>(icon.getIdentifier())
					.getWidth()
			  : 0.0f;
		const float iconHeight	= hasIcon
			 ? registry.getComponent<components::Bound>(icon.getIdentifier())
				   .getHeight()
			 : 0.0f;
		const float labelWidth	= hasLabel
			 ? registry.getComponent<components::Bound>(label.getIdentifier())
				   .getWidth()
			 : 0.0f;
		const float labelHeight = hasLabel
			? registry.getComponent<components::Bound>(label.getIdentifier())
				  .getHeight()
			: 0.0f;

		const float gap			  = (!button.getIconGlyphName().empty()
							 && !button.getLabelContent().empty())
					  ? entities::button_style::getSpaceBetweenIconAndLabel(size)
					  : 0.0f;
		const float contentWidth  = iconWidth + gap + labelWidth;
		const float contentHeight = std::max(iconHeight, labelHeight);

		// The label typography (labelLarge) is independent of the icon size;
		// the container grows to the larger child and to the M3 minimums.
		const float minHeight	= (size == Size::ExtraSmall)
			  ? 0.0f
			  : components::CommonButton::MinHeight;
		const float buttonWidth = std::max(
			{ contentWidth
				  + entities::button_style::getWidthPadding(size, compact)
					  * 2.0f,
			  components::CommonButton::MinWidth, minWidth });
		const float buttonHeight = std::max(
			contentHeight
				+ entities::button_style::getHeightPadding(size) * 2.0f,
			minHeight);

		const auto place = [&](const Button::Child &child, float x,
							   float childHeight) {
			const float y = (buttonHeight - childHeight) / 2.0f;
			registry.getComponent<components::Transform>(child.getIdentifier())
				.setPose(Layout::contentLayerPose(
					utility::graphic::PositionF(buttonPose.getPosition().x + x,
												buttonPose.getPosition().y + y,
												buttonPose.getPosition().z),
					buttonPose.getOrientation(), layer));
		};

		float cursor = (buttonWidth - contentWidth) / 2.0f;
		if (trailingIcon) {
			if (hasLabel) {
				place(label, cursor, labelHeight);
				cursor += labelWidth + gap;
			}
			if (hasIcon) {
				place(icon, cursor, iconHeight);
			}
		} else {
			if (hasIcon) {
				place(icon, cursor, iconHeight);
				cursor += iconWidth + gap;
			}
			if (hasLabel) {
				place(label, cursor, labelHeight);
			}
		}

		registry.getComponent<components::Bound>(identifier)
			.setWidth(buttonWidth)
			.setHeight(buttonHeight);
		Button::applyTouchTarget(registry, identifier, buttonWidth,
								 buttonHeight);
	}

	void CommonButton::apply(ecs::ComponentRegistry &registry,
							 const ecs::Entity::Identifier &identifier)
	{
		styleState(registry, identifier);
		geometry(registry, identifier);
	}

	void CommonButton::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::CommonButton>(entityIdentifier)
			|| !requireComponent<components::Button>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
