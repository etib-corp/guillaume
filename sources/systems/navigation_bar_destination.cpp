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

#include "guillaume/systems/navigation_bar_destination.hpp"

#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/badge.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/placement_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/badge.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using style::schemeColor;
		using style::transparentColor;

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
		 * @brief Arrange the layout children of a nested group, when the
		 * child is one.
		 * @param registry The registry holding the components.
		 * @param group The group identifier (may be invalid).
		 */
		void arrangeGroup(ecs::ComponentRegistry &registry,
						  ecs::Entity::Identifier group)
		{
			if (!registry.hasComponent<components::Layout>(group)
				|| !registry.hasComponent<components::Transform>(group)
				|| !registry.hasComponent<components::Bound>(group)) {
				return;
			}

			Layout::apply(
				registry, group,
				registry.getComponent<components::Layout>(group).getChildren(),
				Layout::layerOf(registry, group));
		}

		/**
		 * @brief Make the badge child show the badge label, or a dot when
		 * the label is empty.
		 * @param registry The registry holding the components.
		 * @param badge The badge identifier (may be invalid).
		 * @param label The badge label.
		 */
		void styleBadge(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier badge, const std::string &label)
		{
			if (!registry.hasComponent<components::Badge>(badge)) {
				return;
			}

			auto &config = registry.getComponent<components::Badge>(badge);
			const auto variant = label.empty()
				? components::Badge::Variant::Dot
				: components::Badge::Variant::Large;

			if (config.getVariant() == variant && config.getLabel() == label) {
				return;
			}

			// The badge's own system sizes it; apply it now so the result is
			// visible at once.
			config.setVariant(variant).setLabel(label);
			Badge::apply(registry, badge);
		}
	}	 // namespace

	NavigationBarDestination::NavigationBarDestination(void)
		: ecs::SystemFiller<components::NavigationBarDestination>(
			  ecs::Phase::Layout)
	{
	}

	void NavigationBarDestination::install(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		interaction::makeInteractive(
			componentRegistry, identifier, components::AccessibilityRole::Tab,
			[registry, identifier]() {
				press(*registry, identifier);
			},
			[registry, identifier]() {
				apply(*registry, identifier);
			});
	}

	bool NavigationBarDestination::isSelected(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry.hasComponent<components::Selectable>(
				   entityIdentifier)
			&& componentRegistry
				   .getComponent<components::Selectable>(entityIdentifier)
				   .isSelected();
	}

	void NavigationBarDestination::press(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		// Copy: the callback may replace itself.
		const auto onPress =
			componentRegistry
				.getComponent<components::NavigationBarDestination>(
					entityIdentifier)
				.getOnPress();

		if (onPress) {
			onPress();
		}
	}

	void NavigationBarDestination::setBadge(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier,
		const std::string &badge)
	{
		componentRegistry
			.getComponent<components::NavigationBarDestination>(
				entityIdentifier)
			.setBadge(badge);
		apply(componentRegistry, entityIdentifier);
	}

	void NavigationBarDestination::apply(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &destination =
			componentRegistry
				.getComponent<components::NavigationBarDestination>(
					entityIdentifier);
		const bool selected = isSelected(componentRegistry, entityIdentifier);

		componentRegistry.getComponent<components::Layout>(entityIdentifier)
			.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Center)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center)
			.setSpacing(theme::SpacingScale::get(1))
			.setPadding(0.0f);
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(transparentColor());
		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(getShapeRadius(theme::ShapeRole::None))
			.setColor(transparentColor());

		const auto pillColor  = selected
			 ? schemeColor(SchemeColorRole::SecondaryContainer)
			 : transparentColor();
		const auto iconColor  = selected
			 ? schemeColor(SchemeColorRole::OnSecondaryContainer)
			 : schemeColor(SchemeColorRole::OnSurfaceVariant);
		const auto labelColor = selected
			? schemeColor(SchemeColorRole::OnSurface)
			: schemeColor(SchemeColorRole::OnSurfaceVariant);

		// The state layer of the destination is drawn on its indicator.
		colorChild(componentRegistry, destination.getIndicator(),
				   interaction::stateLayerColor(componentRegistry,
												entityIdentifier, pillColor,
												iconColor));

		const auto icon = destination.getIconEntity();
		if (componentRegistry.hasComponent<components::Glyph>(icon)) {
			componentRegistry.getComponent<components::Glyph>(icon).setName(
				selected && !destination.getFocusedIcon().empty()
					? destination.getFocusedIcon()
					: destination.getIcon());
		}
		colorChild(componentRegistry, icon, iconColor);

		// A hidden label keeps its place in the column.
		colorChild(componentRegistry, destination.getLabelEntity(),
				   destination.isLabelShown() ? labelColor
											  : transparentColor());

		styleBadge(componentRegistry, destination.getBadgeEntity(),
				   destination.getBadge());

		if (componentRegistry.hasComponent<components::Accessibility>(
				entityIdentifier)) {
			componentRegistry
				.getComponent<components::Accessibility>(entityIdentifier)
				.setLabel(destination.getLabel())
				.setSelected(selected);
		}

		arrange(componentRegistry, entityIdentifier);
	}

	void NavigationBarDestination::arrange(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &destination =
			componentRegistry
				.getComponent<components::NavigationBarDestination>(
					entityIdentifier);
		const auto indicator = destination.getIndicator();
		const auto badge	 = destination.getBadgeEntity();
		std::vector<ecs::Entity::Identifier> children;

		for (const auto child: { indicator, destination.getLabelEntity() }) {
			if (child != ecs::Entity::InvalidIdentifier) {
				children.push_back(child);
			}
		}

		// Size the indicator, place it and the label in the column, then
		// place the icon inside the moved indicator.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setChildren(children);
		arrangeGroup(componentRegistry, indicator);
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
		arrangeGroup(componentRegistry, indicator);

		if (badge != ecs::Entity::InvalidIdentifier
			&& componentRegistry.hasComponent<components::Transform>(
				indicator)) {
			placement::placeChild(
				componentRegistry, indicator, badge,
				utility::graphic::PositionF(
					components::NavigationBarDestination::BadgeOffsetX,
					components::NavigationBarDestination::BadgeOffsetY, 0.0f),
				Layout::layerOf(componentRegistry, indicator));
			// The label of the badge follows it.
			arrangeGroup(componentRegistry, badge);
		}
	}

	void NavigationBarDestination::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::NavigationBarDestination>(
				entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
