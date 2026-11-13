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

#include "guillaume/systems/navigation_drawer_destination.hpp"

#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/style_state.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/systems/interaction_helpers.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using style::disabledContentColor;
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
	}	 // namespace

	NavigationDrawerDestination::NavigationDrawerDestination(void)
		: ecs::SystemFiller<components::NavigationDrawerDestination>(
			  ecs::Phase::Layout)
	{
	}

	void NavigationDrawerDestination::install(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		interaction::makeInteractive(
			componentRegistry, identifier,
			components::AccessibilityRole::Button,
			[registry, identifier]() {
				press(*registry, identifier);
			},
			[registry, identifier]() {
				apply(*registry, identifier);
			});
	}

	bool NavigationDrawerDestination::isSelected(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry.hasComponent<components::Selectable>(
				   entityIdentifier)
			&& componentRegistry
				   .getComponent<components::Selectable>(entityIdentifier)
				   .isSelected();
	}

	bool NavigationDrawerDestination::isDisabled(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		return componentRegistry
				   .getComponent<components::NavigationDrawerDestination>(
					   entityIdentifier)
				   .isDisabled()
			|| !interaction::isEnabled(componentRegistry, entityIdentifier);
	}

	void NavigationDrawerDestination::setDisabled(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, bool disabled)
	{
		componentRegistry
			.getComponent<components::NavigationDrawerDestination>(
				entityIdentifier)
			.setDisabled(disabled);
		apply(componentRegistry, entityIdentifier);
	}

	void NavigationDrawerDestination::press(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (isDisabled(componentRegistry, entityIdentifier)) {
			return;
		}

		// Copy: the callback may replace itself.
		const auto onPress =
			componentRegistry
				.getComponent<components::NavigationDrawerDestination>(
					entityIdentifier)
				.getOnPress();

		if (onPress) {
			onPress();
		}
	}

	void NavigationDrawerDestination::apply(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &destination =
			componentRegistry
				.getComponent<components::NavigationDrawerDestination>(
					entityIdentifier);
		const bool collapsed = destination.isCollapsed();

		// A collapsed item stacks its indicator above its label; a regular
		// item is a row with the trailing text pushed to the far edge.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		if (collapsed) {
			layout.setAxis(components::Layout::Axis::Vertical)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Center)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center)
				.setSpacing(theme::SpacingScale::get(1))
				.setPadding(0.0f)
				.setFixedHeight(components::NavigationDrawerDestination::Height
								+ theme::SpacingScale::get(2));
		} else {
			layout.setAxis(components::Layout::Axis::Horizontal)
				.setMainAxisAlignment(
					destination.getRight().empty()
						? components::Layout::MainAxisAlignment::Start
						: components::Layout::MainAxisAlignment::SpaceBetween)
				.setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center)
				.setSpacing(theme::SpacingScale::get(3))
				.setPadding(theme::SpacingScale::get(4))
				.setFixedHeight(
					components::NavigationDrawerDestination::Height);
		}

		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(getShapeRadius(theme::ShapeRole::Full))
			.setColor(transparentColor());

		if (componentRegistry.hasComponent<components::StyleState>(
				entityIdentifier)) {
			componentRegistry
				.getComponent<components::StyleState>(entityIdentifier)
				.setEnabled(!destination.isDisabled());
		}

		const bool selected	 = isSelected(componentRegistry, entityIdentifier);
		const bool disabled	 = isDisabled(componentRegistry, entityIdentifier);
		const auto pill		 = selected && !disabled
				 ? schemeColor(SchemeColorRole::SecondaryContainer)
				 : transparentColor();
		const auto content	 = disabled
			  ? disabledContentColor()
			  : (selected ? schemeColor(SchemeColorRole::OnSecondaryContainer)
						  : schemeColor(SchemeColorRole::OnSurfaceVariant));
		const auto pillColor = interaction::stateLayerColor(
			componentRegistry, entityIdentifier, pill, content);

		// The active pill is the indicator of a collapsed item, and the
		// whole row of a regular one.
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(collapsed ? transparentColor() : pillColor);
		colorChild(componentRegistry, destination.getLead(),
				   collapsed ? pillColor : transparentColor());

		colorChild(componentRegistry, destination.getIconEntity(), content);
		colorChild(componentRegistry, destination.getLabelEntity(), content);
		colorChild(componentRegistry, destination.getRightEntity(), content);

		if (componentRegistry.hasComponent<components::Accessibility>(
				entityIdentifier)) {
			componentRegistry
				.getComponent<components::Accessibility>(entityIdentifier)
				.setLabel(destination.getLabel())
				.setSelected(selected);
		}

		arrange(componentRegistry, entityIdentifier);
	}

	void NavigationDrawerDestination::arrange(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &destination =
			componentRegistry
				.getComponent<components::NavigationDrawerDestination>(
					entityIdentifier);
		const auto lead = destination.getLead();
		std::vector<ecs::Entity::Identifier> children;

		// The label of a regular item is inside the lead group.
		if (lead != ecs::Entity::InvalidIdentifier) {
			children.push_back(lead);
		}
		if (destination.isCollapsed()
			&& destination.getLabelEntity() != ecs::Entity::InvalidIdentifier) {
			children.push_back(destination.getLabelEntity());
		}
		if (destination.getRightEntity() != ecs::Entity::InvalidIdentifier) {
			children.push_back(destination.getRightEntity());
		}

		// Size the lead group, place the children, then place the content
		// of the moved lead group.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setChildren(children);
		arrangeGroup(componentRegistry, lead);
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));
		arrangeGroup(componentRegistry, lead);
	}

	void NavigationDrawerDestination::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::NavigationDrawerDestination>(
				entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
