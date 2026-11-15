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

#include "guillaume/systems/search.hpp"

#include <functional>
#include <utility>
#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/text_field.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/text_field.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::Search::Variant;
		using style::schemeColor;
		using style::transparentColor;

		/**
		 * @brief Check whether an identifier names an entity.
		 * @param identifier The identifier.
		 * @return True unless it is the invalid identifier.
		 */
		bool isValid(ecs::Entity::Identifier identifier)
		{
			return identifier != ecs::Entity::InvalidIdentifier;
		}

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
			if (isValid(child)
				&& registry.hasComponent<components::Color>(child)) {
				registry.getComponent<components::Color>(child).setColor(color);
			}
		}

		/**
		 * @brief Set the glyph of an icon child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param glyph The glyph name to write.
		 */
		void glyphChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child, const std::string &glyph)
		{
			if (isValid(child)
				&& registry.hasComponent<components::Glyph>(child)) {
				registry.getComponent<components::Glyph>(child).setName(glyph);
			}
		}

		/**
		 * @brief Make an icon child a labelled button.
		 * @param registry The registry holding the components.
		 * @param icon The icon identifier (ignored when invalid).
		 * @param label Its accessibility label.
		 * @param onPress Its press handler.
		 */
		void makeIconButton(ecs::ComponentRegistry &registry,
							ecs::Entity::Identifier icon,
							const std::string &label,
							std::function<void(void)> onPress)
		{
			if (!isValid(icon)) {
				return;
			}
			interaction::makeInteractive(registry, icon,
										 components::AccessibilityRole::Button,
										 std::move(onPress));
			registry.getComponent<components::Accessibility>(icon).setLabel(
				label);
		}

		/**
		 * @brief Style the field itself: size, shape, divider and color.
		 * @param registry The registry holding the components.
		 * @param identifier The search entity.
		 * @param search Its configuration.
		 */
		void applyVariant(ecs::ComponentRegistry &registry,
						  const ecs::Entity::Identifier &identifier,
						  const components::Search &search)
		{
			const bool fullScreen = search.getVariant() == Variant::FullScreen;

			auto &layout =
				registry.getComponent<components::Layout>(identifier);
			layout.setAxis(components::Layout::Axis::Horizontal)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Start)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center)
				.setSpacing(theme::SpacingScale::get(4))
				.setPadding(theme::SpacingScale::get(4))
				.setFixedHeight(fullScreen ? components::Search::ViewHeight
										   : components::Search::BarHeight);
			if (!layout.hasFixedWidth() || layout.getFixedWidth() <= 0.0f) {
				layout.setFixedWidth(components::Search::DefaultWidth);
			}

			auto &line = registry.getComponent<components::Line>(identifier);
			if (fullScreen) {
				line.setThickness(1.0f)
					.setPlacement(components::Line::Placement::Bottom)
					.setColor(schemeColor(SchemeColorRole::OutlineVariant));
			} else {
				line.setThickness(0.0f).clearColor();
			}

			registry.getComponent<components::Borders>(identifier)
				.setBorderRadius(
					getShapeRadius(fullScreen ? theme::ShapeRole::None
											  : theme::ShapeRole::ExtraLarge));

			const auto level =
				registry.getComponent<components::Elevation>(identifier)
					.getElevationLevel();
			registry.getComponent<components::Color>(identifier)
				.setColor(
					level == theme::ElevationLevel::Level0
						? schemeColor(SchemeColorRole::SurfaceContainerHigh)
						: getElevatedSurfaceColor(level));
		}
	}	 // namespace

	Search::Search(void)
		: ecs::SystemFiller<components::Search>(ecs::Phase::Layout)
	{
	}

	void Search::install(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setRole(components::AccessibilityRole::Search);

		// Paper's Searchbar defaults to elevation 0.
		componentRegistry.getComponent<components::Elevation>(entityIdentifier)
			.setElevationLevel(theme::ElevationLevel::Level0);

		installIcon(componentRegistry, entityIdentifier);
		installClearIcon(componentRegistry, entityIdentifier);
		installTrailingIcon(componentRegistry, entityIdentifier);
	}

	void Search::installIcon(ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		makeIconButton(
			componentRegistry,
			componentRegistry.getComponent<components::Search>(identifier)
				.getSearchIcon(),
			"search", [registry, identifier]() {
				pressIcon(*registry, identifier);
			});
	}

	void Search::installClearIcon(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		makeIconButton(
			componentRegistry,
			componentRegistry.getComponent<components::Search>(identifier)
				.getClearIcon(),
			"clear search", [registry, identifier]() {
				clear(*registry, identifier);
			});
	}

	void Search::installTrailingIcon(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		const auto &search =
			componentRegistry.getComponent<components::Search>(identifier);
		// Copy: making the icon interactive adds components.
		const std::string label = search.getTrailingGlyph();

		makeIconButton(componentRegistry, search.getTrailingIcon(), label,
					   [registry, identifier]() {
						   pressTrailingIcon(*registry, identifier);
					   });
	}

	std::string Search::text(const ecs::ComponentRegistry &componentRegistry,
							 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &search = componentRegistry.getComponent<components::Search>(
			entityIdentifier);
		const auto textEntity = search.getTextEntity();

		if (isValid(textEntity)
			&& componentRegistry.hasComponent<components::Text>(textEntity)) {
			return componentRegistry.getComponent<components::Text>(textEntity)
				.getContent();
		}
		return search.getText();
	}

	void Search::setText(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 const std::string &text)
	{
		auto &search = componentRegistry.getComponent<components::Search>(
			entityIdentifier);
		const auto textEntity = search.getTextEntity();

		search.setText(text);
		if (isValid(textEntity)
			&& componentRegistry.hasComponent<components::Text>(textEntity)) {
			componentRegistry.getComponent<components::Text>(textEntity)
				.setContent(text);
		}
		apply(componentRegistry, entityIdentifier);
	}

	void Search::clear(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		setText(componentRegistry, entityIdentifier, "");

		// Copy: the handler may replace itself.
		const auto onClear =
			componentRegistry.getComponent<components::Search>(entityIdentifier)
				.getOnClearIconPress();
		if (onClear) {
			onClear();
		}
	}

	void Search::pressIcon(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		// Copy: the handler may replace itself.
		const auto onPress =
			componentRegistry.getComponent<components::Search>(entityIdentifier)
				.getOnIconPress();
		if (onPress) {
			onPress();
		}
	}

	void Search::pressTrailingIcon(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		// Copy: the handler may replace itself.
		const auto onPress =
			componentRegistry.getComponent<components::Search>(entityIdentifier)
				.getOnTrailingIconPress();
		if (onPress) {
			onPress();
		}
	}

	void Search::show(ecs::ComponentRegistry &componentRegistry,
					  const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(true);
	}

	void Search::hide(ecs::ComponentRegistry &componentRegistry,
					  const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(false);
	}

	bool Search::isVisible(const ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
			.getComponent<components::Overlay>(entityIdentifier)
			.isVisible();
	}

	void Search::apply(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		auto &search = componentRegistry.getComponent<components::Search>(
			entityIdentifier);

		// The query child is what the user types in: it is the source of
		// the query as soon as it exists.
		search.setText(text(componentRegistry, entityIdentifier));

		const bool hasQuery		= !search.getText().empty();
		const bool loading		= search.isLoading();
		const bool showClear	= hasQuery && !loading;
		const bool showTrailing = !search.getTrailingGlyph().empty();
		const auto iconColor = schemeColor(SchemeColorRole::OnSurfaceVariant);

		applyVariant(componentRegistry, entityIdentifier, search);

		// Both Paper modes are inline fields; neither is modal.
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setModal(false)
			.setDismissOnOutsideClick(false)
			.setDismissOnEscape(true);

		auto &textField = componentRegistry.getComponent<components::TextField>(
			entityIdentifier);
		textField.setPlaceholder(search.getPlaceholder())
			.setLabel(search.getPlaceholder());
		TextField::reconcile(textField, hasQuery);

		componentRegistry
			.getComponent<components::Accessibility>(entityIdentifier)
			.setLabel(search.getPlaceholder());

		const auto searchIcon  = search.getSearchIcon();
		const auto textEntity  = search.getTextEntity();
		const auto placeholder = search.getPlaceholderEntity();
		const auto clearIcon   = search.getClearIcon();
		const auto loadingIcon = search.getLoadingIcon();
		const auto trailing	   = search.getTrailingIcon();

		glyphChild(componentRegistry, searchIcon, search.getIconGlyph());
		colorChild(componentRegistry, searchIcon, iconColor);

		if (isValid(placeholder)
			&& componentRegistry.hasComponent<components::Text>(placeholder)) {
			componentRegistry.getComponent<components::Text>(placeholder)
				.setContent(search.getPlaceholder());
		}
		colorChild(componentRegistry, placeholder,
				   hasQuery ? transparentColor() : iconColor);

		colorChild(componentRegistry, clearIcon,
				   showClear ? iconColor : transparentColor());
		if (isValid(clearIcon)) {
			interaction::setEnabled(componentRegistry, clearIcon, showClear);
		}

		colorChild(componentRegistry, loadingIcon,
				   loading ? schemeColor(SchemeColorRole::Primary)
						   : transparentColor());

		if (showTrailing) {
			glyphChild(componentRegistry, trailing, search.getTrailingGlyph());
		}
		colorChild(componentRegistry, trailing,
				   showTrailing ? iconColor : transparentColor());
		if (isValid(trailing)) {
			interaction::setEnabled(componentRegistry, trailing, showTrailing);
		}

		// The query replaces the placeholder, the loading glyph replaces
		// the clear button, and the trailing icon shows while it has a
		// glyph.
		std::vector<ecs::Entity::Identifier> children;
		const auto push = [&children](ecs::Entity::Identifier child) {
			if (isValid(child)) {
				children.push_back(child);
			}
		};

		push(searchIcon);
		push(hasQuery || !isValid(placeholder) ? textEntity : placeholder);
		push(loading ? loadingIcon : clearIcon);
		if (showTrailing) {
			push(trailing);
		}

		componentRegistry.getComponent<components::Layout>(entityIdentifier)
			.setChildren(children);
		Layout::apply(componentRegistry, entityIdentifier, children,
					  Layout::layerOf(componentRegistry, entityIdentifier));
	}

	void Search::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::Search>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
