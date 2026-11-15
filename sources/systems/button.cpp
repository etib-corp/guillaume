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
#include "guillaume/systems/button.hpp"

#include <cmath>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/hand_hover_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/mouse_hover_interaction.hpp"
#include "guillaume/components/style_state.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/typography.hpp"
#include "guillaume/systems/typography.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	Button::Child::Child(ecs::ComponentRegistry &registry,
						 ecs::Entity::Identifier identifier)
		: _registry(registry)
		, _identifier(identifier)
	{
	}

	Button::Child::operator bool(void) const
	{
		return _identifier != ecs::Entity::InvalidIdentifier
			&& _registry.hasComponent<components::Transform>(_identifier);
	}

	ecs::Entity::Identifier Button::Child::getIdentifier(void) const noexcept
	{
		return _identifier;
	}

	const Button::Child &
		Button::Child::setColor(const utility::graphic::Color32Bit &color) const
	{
		if (_registry.hasComponent<components::Color>(_identifier)) {
			_registry.getComponent<components::Color>(_identifier)
				.setColor(color);
		}
		return *this;
	}

	const Button::Child &Button::Child::setFontSize(float size) const
	{
		if (_registry.hasComponent<components::Glyph>(_identifier)) {
			_registry.getComponent<components::Glyph>(_identifier)
				.setFontSize(size);
		}
		return *this;
	}

	const Button::Child &Button::Child::setTypeRole(theme::TypeRole role) const
	{
		if (_registry.hasComponent<components::Typography>(_identifier)
			&& _registry.hasComponent<components::Text>(_identifier)) {
			_registry.getComponent<components::Typography>(_identifier)
				.setRole(role)
				.setFontSize(std::nullopt);
			Typography::apply(_registry, _identifier);
		}
		return *this;
	}

	const Button::Child &
		Button::Child::setContent(const std::string &content) const
	{
		if (_registry.hasComponent<components::Text>(_identifier)) {
			_registry.getComponent<components::Text>(_identifier)
				.setContent(content);
		}
		return *this;
	}

	Button::Button(void)
		: ecs::SystemFiller<components::Button>(ecs::Phase::Event)
	{
	}

	void Button::install(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier)
	{
		ecs::ComponentRegistry *storage	 = &registry;
		const ecs::Entity::Identifier id = identifier;
		const auto pressed				 = [storage, id]() {
			  press(*storage, id);
		};
		const auto released = [storage, id]() {
			release(*storage, id);
		};
		const auto hovered = [storage, id]() {
			hoverChanged(*storage, id);
		};

		registry.getComponent<components::HandButtonInteraction>(identifier)
			.setOnButtonPressHandler(utility::event::HandButtonEvent::Button::A,
									 pressed)
			.setOnButtonReleaseHandler(
				utility::event::HandButtonEvent::Button::A, released);
		registry.getComponent<components::MouseButtonInteraction>(identifier)
			.setOnButtonPressHandler(
				utility::event::MouseButtonEvent::Button::Left, pressed)
			.setOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left, released);
		registry.getComponent<components::HandHoverInteraction>(identifier)
			.setOnHoverHandler(hovered)
			.setOnUnhoverHandler(hovered);
		registry.getComponent<components::MouseHoverInteraction>(identifier)
			.setOnHoverHandler(hovered)
			.setOnUnhoverHandler(hovered);
	}

	bool Button::isDisabled(const ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier)
	{
		return !registry.getComponent<components::StyleState>(identifier)
					.isEnabled();
	}

	bool Button::isHovered(const ecs::ComponentRegistry &registry,
						   const ecs::Entity::Identifier &identifier)
	{
		return registry
				   .getComponent<components::HandHoverInteraction>(identifier)
				   .isHovered()
			|| registry
				   .getComponent<components::MouseHoverInteraction>(identifier)
				   .isHovered();
	}

	bool Button::isPressed(const ecs::ComponentRegistry &registry,
						   const ecs::Entity::Identifier &identifier)
	{
		return registry
				   .getComponent<components::HandButtonInteraction>(identifier)
				   .isButtonPressed(utility::event::HandButtonEvent::Button::A)
			|| registry
				   .getComponent<components::MouseButtonInteraction>(identifier)
				   .isButtonPressed(
					   utility::event::MouseButtonEvent::Button::Left);
	}

	const std::string &
		Button::activeIconGlyphName(const components::Button &button)
	{
		static const std::string loadingGlyph(LoadingGlyph);

		if (button.isLoading()) {
			return loadingGlyph;
		}
		if (button.isSelected() && !button.getSelectedIconGlyphName().empty()) {
			return button.getSelectedIconGlyphName();
		}
		return button.getIconGlyphName();
	}

	utility::graphic::Color32Bit
		Button::resolveContainerColor(const components::Button &button,
									  const utility::graphic::Color32Bit &color)
	{
		return button.getContainerColorOverride().value_or(color);
	}

	utility::graphic::Color32Bit
		Button::resolveContentColor(const components::Button &button,
									const utility::graphic::Color32Bit &color)
	{
		return button.getContentColorOverride().value_or(color);
	}

	void Button::applyTouchTarget(ecs::ComponentRegistry &registry,
								  const ecs::Entity::Identifier &identifier,
								  float width, float height)
	{
		const utility::graphic::SizeUI margin(
			static_cast<unsigned int>(
				std::ceil(theme::SpacingScale::touchMargin(width))),
			static_cast<unsigned int>(
				std::ceil(theme::SpacingScale::touchMargin(height))));

		registry.getComponent<components::MouseHoverInteraction>(identifier)
			.setAccessibilityMargin(margin);
		registry.getComponent<components::HandHoverInteraction>(identifier)
			.setAccessibilityMargin(margin);
	}

	void Button::syncContent(ecs::ComponentRegistry &registry,
							 const ecs::Entity::Identifier &identifier)
	{
		const auto &button =
			registry.getComponent<components::Button>(identifier);

		if (registry.hasComponent<components::Glyph>(button.getIcon())) {
			registry.getComponent<components::Glyph>(button.getIcon())
				.setName(activeIconGlyphName(button))
				.setStyle(button.getIconStyle());
		}
		registry.getComponent<components::Accessibility>(identifier)
			.setSelected(button.isToggle() && button.isSelected());
	}

	void Button::restyle(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier)
	{
		const auto function =
			registry.getComponent<components::Button>(identifier).getRestyle();

		if (function != nullptr) {
			function(registry, identifier);
		}
	}

	void Button::setSelected(ecs::ComponentRegistry &registry,
							 const ecs::Entity::Identifier &identifier,
							 bool selected)
	{
		auto &button = registry.getComponent<components::Button>(identifier);

		if (!button.isToggle()) {
			return;
		}
		button.setSelected(selected);
		syncContent(registry, identifier);
		restyle(registry, identifier);
	}

	void Button::hoverChanged(ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier)
	{
		if (isDisabled(registry, identifier)) {
			return;
		}
		restyle(registry, identifier);
	}

	void Button::press(ecs::ComponentRegistry &registry,
					   const ecs::Entity::Identifier &identifier)
	{
		auto &button = registry.getComponent<components::Button>(identifier);

		if (isDisabled(registry, identifier) || !button.isVisible()) {
			return;
		}
		button.setPressStart(std::chrono::steady_clock::now());
		restyle(registry, identifier);
	}

	void Button::release(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier)
	{
		auto &button = registry.getComponent<components::Button>(identifier);

		if (isDisabled(registry, identifier) || !button.isVisible()) {
			return;
		}
		if (button.isToggle()) {
			button.setSelected(!button.isSelected());
		}
		syncContent(registry, identifier);
		restyle(registry, identifier);

		// Copies: a callback may replace itself or the other one.
		const auto onLongPress = button.getOnLongPress();
		const auto onClick	   = button.getOnClick();
		const bool longPress   = onLongPress
			&& button.getPressStart() != components::Button::TimePoint {}
			&& std::chrono::steady_clock::now() - button.getPressStart()
				>= LongPressDuration;
		button.setPressStart({});
		if (longPress) {
			onLongPress();
		} else if (onClick) {
			onClick();
		}
	}

	void Button::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Button>(entityIdentifier)) {
			return;
		}

		syncContent(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
