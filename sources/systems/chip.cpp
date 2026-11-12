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

#include "guillaume/systems/chip.hpp"

#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Mode	  = components::Chip::Mode;
		using Variant = components::Chip::Variant;
		using entities::disabledContainerColor;
		using entities::disabledContentColor;
		using entities::schemeColor;
		using entities::transparentColor;

		/**
		 * @brief Set the color of a child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param color The color to write.
		 */
		void colorChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child,
						const utility::graphic::Color32Bit &color)
		{
			if (registry.hasComponent<components::Color>(child)) {
				registry.getComponent<components::Color>(child).setColor(color);
			}
		}

		/**
		 * @brief Record the children the Layout system arranges: the check
		 * replaces the leading icon while it is shown.
		 * @param registry The registry holding the components.
		 * @param identifier The chip entity.
		 * @param showCheck Whether the selected check is shown.
		 */
		void attachChildren(ecs::ComponentRegistry &registry,
							ecs::Entity::Identifier identifier, bool showCheck)
		{
			const auto &chip =
				registry.getComponent<components::Chip>(identifier);
			const auto invalid	= ecs::Entity::InvalidIdentifier;
			const auto leading	= chip.getLeadingIcon();
			const auto check	= chip.getCheckIcon();
			const auto label	= chip.getLabelEntity();
			const auto trailing = chip.getTrailingIcon();

			// A chip without content of its own keeps whatever children it
			// was given.
			if (leading == invalid && check == invalid && label == invalid
				&& trailing == invalid) {
				return;
			}

			std::vector<ecs::Entity::Identifier> children;
			if (check != invalid && showCheck) {
				children.push_back(check);
			} else if (leading != invalid) {
				children.push_back(leading);
			}
			if (label != invalid) {
				children.push_back(label);
			}
			if (trailing != invalid) {
				children.push_back(trailing);
			}
			registry.getComponent<components::Layout>(identifier)
				.setChildren(children);
		}
	}	 // namespace

	Chip::Chip(void)
		: ecs::SystemFiller<components::Chip>(ecs::Phase::Layout)
	{
	}

	void Chip::install(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		entities::makeInteractive(
			componentRegistry, identifier,
			components::AccessibilityRole::Button,
			[registry, identifier]() {
				press(*registry, identifier);
			},
			[registry, identifier]() {
				apply(*registry, identifier);
			});

		componentRegistry.getComponent<components::Selectable>(identifier)
			.setOnSelectionChangedHandler([registry, identifier](bool) {
				apply(*registry, identifier);
			});
	}

	void Chip::installCloseIcon(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		const ecs::Entity::Identifier closeIcon =
			componentRegistry.getComponent<components::Chip>(identifier)
				.getTrailingIcon();

		if (closeIcon == ecs::Entity::InvalidIdentifier) {
			return;
		}

		entities::makeInteractive(componentRegistry, closeIcon,
								  components::AccessibilityRole::Button,
								  [registry, identifier]() {
									  close(*registry, identifier);
								  });
		componentRegistry.getComponent<components::Accessibility>(closeIcon)
			.setLabel("Close");
	}

	bool Chip::isSelected(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
			.getComponent<components::Selectable>(entityIdentifier)
			.isSelected();
	}

	bool Chip::isDisabled(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier)
	{
		return !entities::isEnabled(componentRegistry, entityIdentifier);
	}

	std::string
		Chip::leadingGlyph(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &chip =
			componentRegistry.getComponent<components::Chip>(entityIdentifier);

		if (chip.getVariant() == Variant::Assist
			&& chip.getIconGlyph().empty()) {
			return components::Chip::DefaultAssistGlyph;
		}
		return chip.getIconGlyph();
	}

	bool Chip::needsCheckIcon(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
				   .getComponent<components::Chip>(entityIdentifier)
				   .getVariant()
			== Variant::Filter;
	}

	bool Chip::needsCloseIcon(const ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &chip =
			componentRegistry.getComponent<components::Chip>(entityIdentifier);

		return chip.getVariant() == Variant::Input
			|| static_cast<bool>(chip.getOnClose());
	}

	void Chip::setSelected(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   bool selected)
	{
		componentRegistry.getComponent<components::Selectable>(entityIdentifier)
			.setSelected(selected);
		apply(componentRegistry, entityIdentifier);
	}

	void Chip::setDisabled(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   bool disabled)
	{
		entities::setEnabled(componentRegistry, entityIdentifier, !disabled);
		apply(componentRegistry, entityIdentifier);
	}

	void Chip::press(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isDisabled(componentRegistry, entityIdentifier)) {
			return;
		}

		const auto &chip =
			componentRegistry.getComponent<components::Chip>(entityIdentifier);
		if (chip.getVariant() == Variant::Filter) {
			setSelected(componentRegistry, entityIdentifier,
						!isSelected(componentRegistry, entityIdentifier));
		}
		// Copy: the callback may replace itself.
		const auto onPress = chip.getOnPress();
		if (onPress) {
			onPress();
		}
	}

	void Chip::close(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isDisabled(componentRegistry, entityIdentifier)) {
			return;
		}

		// Copy: the callback may replace itself.
		const auto onClose =
			componentRegistry.getComponent<components::Chip>(entityIdentifier)
				.getOnClose();
		if (onClose) {
			onClose();
		}
	}

	void Chip::apply(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &chip =
			componentRegistry.getComponent<components::Chip>(entityIdentifier);
		auto &borders = componentRegistry.getComponent<components::Borders>(
			entityIdentifier);
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);

		const bool selected	 = isSelected(componentRegistry, entityIdentifier);
		const bool disabled	 = isDisabled(componentRegistry, entityIdentifier);
		const bool outlined	 = (chip.getMode() == Mode::Outlined);
		const bool showCheck = selected && chip.isShowSelectedCheck();

		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setLabel(chip.getLabel())
			.setSelected(selected);

		// Paper v5 Chip/helpers.tsx: flat chips sit on secondaryContainer;
		// outlined chips are transparent with an outline until selected.
		utility::graphic::Color32Bit container = transparentColor();
		if (!outlined || selected) {
			container = schemeColor(SchemeColorRole::SecondaryContainer);
		}
		auto content   = (outlined && !selected)
			  ? schemeColor(SchemeColorRole::OnSurfaceVariant)
			  : schemeColor(SchemeColorRole::OnSecondaryContainer);
		auto iconColor = selected
			? schemeColor(SchemeColorRole::OnSecondaryContainer)
			: schemeColor(SchemeColorRole::Primary);

		if (disabled) {
			container =
				outlined ? transparentColor() : disabledContainerColor();
			content	  = disabledContentColor();
			iconColor = disabledContentColor();
		} else {
			if (selected && chip.isShowSelectedOverlay()) {
				container = entities::stateLayer(container, content,
												 theme::StateLayer::Pressed);
			}
			container = entities::stateLayerColor(
				componentRegistry, entityIdentifier, container, content);
		}

		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(container);

		if (outlined && !selected) {
			borders
				.setColor(disabled ? disabledContainerColor()
								   : schemeColor(SchemeColorRole::Outline))
				.setWidth(1.0f);
		} else {
			borders.setColor(transparentColor()).setWidth(0.0f);
		}

		componentRegistry.getComponent<components::Elevation>(entityIdentifier)
			.setElevationLevel((chip.isElevated() && !disabled)
								   ? theme::ElevationLevel::Level1
								   : theme::ElevationLevel::Level0);

		layout.setPadding(chip.isCompact() ? components::Chip::CompactPadding
										   : components::Chip::Padding);

		colorChild(componentRegistry, chip.getLeadingIcon(), iconColor);
		colorChild(componentRegistry, chip.getCheckIcon(),
				   showCheck ? iconColor : transparentColor());
		colorChild(componentRegistry, chip.getTrailingIcon(), content);
		colorChild(componentRegistry, chip.getLabelEntity(), content);
		if (componentRegistry.hasComponent<components::Text>(
				chip.getLabelEntity())) {
			componentRegistry
				.getComponent<components::Text>(chip.getLabelEntity())
				.setContent(chip.getLabel());
		}

		attachChildren(componentRegistry, entityIdentifier, showCheck);
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
	}

	void Chip::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Chip>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
