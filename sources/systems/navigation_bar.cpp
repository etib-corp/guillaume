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

#include "guillaume/systems/navigation_bar.hpp"

#include <algorithm>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/navigation_bar_destination.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/navigation_bar_destination.hpp"
#include "guillaume/systems/selection.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Variant = components::NavigationBar::Variant;

		/**
		 * @brief Whether a child is a destination this system can drive.
		 * @param registry The registry holding the components.
		 * @param destination The child identifier (may be invalid).
		 * @return True when it has a components::NavigationBarDestination.
		 */
		bool isDestination(const ecs::ComponentRegistry &registry,
						   ecs::Entity::Identifier destination)
		{
			return registry.hasComponent<components::NavigationBarDestination>(
				destination);
		}
	}	 // namespace

	NavigationBar::NavigationBar(void)
		: ecs::SystemFiller<components::NavigationBar>(ecs::Phase::Layout)
	{
	}

	void NavigationBar::install(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::SelectionGroup>(identifier)
			.setAllowEmpty(false);

		if (!componentRegistry.hasComponent<components::Accessibility>(
				identifier)) {
			componentRegistry.addComponent<components::Accessibility>(
				identifier);
		}
		componentRegistry.getComponent<components::Accessibility>(identifier)
			.setRole(components::AccessibilityRole::TabList)
			.setFocusable(false);

		const auto &destinations =
			componentRegistry
				.getComponent<components::NavigationBar>(identifier)
				.getDestinationEntities();

		for (std::size_t index = 0; index < destinations.size(); ++index) {
			const auto destination = destinations[index];
			if (!isDestination(componentRegistry, destination)) {
				continue;
			}

			componentRegistry
				.getComponent<components::NavigationBarDestination>(destination)
				.setOnPress([registry, identifier, index]() {
					press(*registry, identifier, index);
				});

			if (componentRegistry.hasComponent<components::Selectable>(
					destination)) {
				componentRegistry
					.getComponent<components::Selectable>(destination)
					.setOnSelectionChangedHandler([registry, identifier,
												   index](bool selected) {
						if (selected) {
							destinationSelected(*registry, identifier, index);
						}
					});
			}
		}
	}

	std::size_t NavigationBar::selectedIndex(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &destinations =
			componentRegistry
				.getComponent<components::NavigationBar>(entityIdentifier)
				.getDestinationEntities();

		for (std::size_t index = 0; index < destinations.size(); ++index) {
			if (NavigationBarDestination::isSelected(componentRegistry,
													 destinations[index])) {
				return index;
			}
		}
		return 0;
	}

	void NavigationBar::select(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index)
	{
		auto &bar = componentRegistry.getComponent<components::NavigationBar>(
			entityIdentifier);
		const auto &destinations = bar.getDestinationEntities();

		if (index >= destinations.size()) {
			return;
		}

		// The destinations report their selection change back to this
		// system: the guard keeps that from selecting again.
		bar.setUpdatingSelection(true);
		Selection::select(componentRegistry, entityIdentifier, destinations,
						  destinations[index]);
		bar.setUpdatingSelection(false);

		apply(componentRegistry, entityIdentifier);
	}

	void NavigationBar::destinationSelected(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index)
	{
		if (componentRegistry
				.getComponent<components::NavigationBar>(entityIdentifier)
				.isUpdatingSelection()) {
			return;
		}

		select(componentRegistry, entityIdentifier, index);
	}

	void NavigationBar::press(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier,
							  std::size_t index)
	{
		if (index
			>= componentRegistry
				   .getComponent<components::NavigationBar>(entityIdentifier)
				   .getDestinationEntities()
				   .size()) {
			return;
		}

		// Copy: a callback may replace itself.
		const auto onTabPress =
			componentRegistry
				.getComponent<components::NavigationBar>(entityIdentifier)
				.getOnTabPress();
		if (onTabPress) {
			onTabPress(index);
		}

		const bool changed =
			selectedIndex(componentRegistry, entityIdentifier) != index;
		select(componentRegistry, entityIdentifier, index);

		const auto onIndexChange =
			componentRegistry
				.getComponent<components::NavigationBar>(entityIdentifier)
				.getOnIndexChange();
		if (changed && onIndexChange) {
			onIndexChange(index);
		}
	}

	void
		NavigationBar::setBadge(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index, const std::string &badge)
	{
		auto &bar = componentRegistry.getComponent<components::NavigationBar>(
			entityIdentifier);
		const auto &destinations = bar.getDestinationEntities();

		bar.setDestinationBadge(index, badge);
		if (index < bar.getDestinations().size() && index < destinations.size()
			&& isDestination(componentRegistry, destinations[index])) {
			NavigationBarDestination::setBadge(componentRegistry,
											   destinations[index], badge);
		}
	}

	void NavigationBar::apply(ecs::ComponentRegistry &componentRegistry,
							  const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &bar =
			componentRegistry.getComponent<components::NavigationBar>(
				entityIdentifier);
		const auto &destinations = bar.getDestinationEntities();
		const bool showAllLabels = bar.getVariant() == Variant::WithLabels;

		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(entities::schemeColor(SchemeColorRole::SurfaceContainer));

		// The destinations share the width of the bar.
		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		const float padding			 = layout.getPadding();
		const float width			 = layout.hasFixedWidth()
					   ? layout.getFixedWidth()
					   : componentRegistry
					 .getComponent<components::Bound>(entityIdentifier)
					 .getWidth();
		const float destinationWidth = destinations.empty()
			? 0.0f
			: std::max(0.0f,
					   (width - 2.0f * padding)
						   / static_cast<float>(destinations.size()));
		const float destinationHeight =
			components::NavigationBar::Height - 2.0f * padding;

		for (const auto destination: destinations) {
			if (!isDestination(componentRegistry, destination)) {
				continue;
			}

			componentRegistry.getComponent<components::Layout>(destination)
				.setFixedWidth(destinationWidth)
				.setFixedHeight(destinationHeight);
			// Without labels, only the selected destination shows its own.
			componentRegistry
				.getComponent<components::NavigationBarDestination>(destination)
				.setShowLabel(showAllLabels
							  || NavigationBarDestination::isSelected(
								  componentRegistry, destination));
			NavigationBarDestination::apply(componentRegistry, destination);
		}

		// The destinations are the children the Layout system places in a
		// row; their content follows them.
		layout.setChildren(destinations);
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));

		for (const auto destination: destinations) {
			if (isDestination(componentRegistry, destination)) {
				NavigationBarDestination::arrange(componentRegistry,
												  destination);
			}
		}
	}

	void NavigationBar::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::NavigationBar>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
