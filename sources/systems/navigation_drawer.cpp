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

#include "guillaume/systems/navigation_drawer.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/navigation_drawer_destination.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/systems/style_helpers.hpp"
#include "guillaume/systems/backdrop.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/navigation_drawer_destination.hpp"
#include "guillaume/systems/selection.hpp"

namespace guillaume::systems
{
	namespace
	{
		using EntryKind = components::NavigationDrawer::EntryKind;
		using Variant	= components::NavigationDrawer::Variant;
		using style::schemeColor;

		/**
		 * @brief Whether a child is a destination this system can drive.
		 * @param registry The registry holding the components.
		 * @param destination The child identifier (may be invalid).
		 * @return True when it has a
		 * components::NavigationDrawerDestination.
		 */
		bool isDestination(const ecs::ComponentRegistry &registry,
						   ecs::Entity::Identifier destination)
		{
			return registry
				.hasComponent<components::NavigationDrawerDestination>(
					destination);
		}
	}	 // namespace

	NavigationDrawer::NavigationDrawer(void)
		: ecs::SystemFiller<components::NavigationDrawer>(ecs::Phase::Layout)
	{
	}

	float
		NavigationDrawer::widthOf(components::NavigationDrawer::Variant variant)
	{
		return variant == Variant::Collapsed
			? components::NavigationDrawer::CollapsedWidth
			: components::NavigationDrawer::Width;
	}

	void NavigationDrawer::install(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;

		componentRegistry.getComponent<components::SelectionGroup>(identifier)
			.setAllowEmpty(false);

		const auto &drawer =
			componentRegistry.getComponent<components::NavigationDrawer>(
				identifier);
		const auto &destinations = drawer.getDestinationEntities();
		const auto &sections	 = drawer.getSectionEntities();

		for (std::size_t index = 0; index < destinations.size(); ++index) {
			const auto destination = destinations[index];
			if (!isDestination(componentRegistry, destination)) {
				continue;
			}

			componentRegistry
				.getComponent<components::NavigationDrawerDestination>(
					destination)
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

		// The section headers are the titled section entries, in order.
		std::size_t section = 0;
		for (const auto &entry: drawer.getEntries()) {
			if (section >= sections.size()) {
				break;
			}
			if (entry.kind != EntryKind::Section || entry.label.empty()) {
				continue;
			}

			const auto header = sections[section++];
			if (!componentRegistry.hasComponent<components::Accessibility>(
					header)) {
				componentRegistry.addComponent<components::Accessibility>(
					header);
			}
			componentRegistry.getComponent<components::Accessibility>(header)
				.setRole(components::AccessibilityRole::Header)
				.setLabel(entry.label)
				.setFocusable(false);
		}
	}

	std::size_t NavigationDrawer::selectedIndex(
		const ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &destinations =
			componentRegistry
				.getComponent<components::NavigationDrawer>(entityIdentifier)
				.getDestinationEntities();

		for (std::size_t index = 0; index < destinations.size(); ++index) {
			if (NavigationDrawerDestination::isSelected(componentRegistry,
														destinations[index])) {
				return index;
			}
		}
		return 0;
	}

	void NavigationDrawer::select(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index)
	{
		auto &drawer =
			componentRegistry.getComponent<components::NavigationDrawer>(
				entityIdentifier);
		const auto &destinations = drawer.getDestinationEntities();

		if (index >= destinations.size()) {
			return;
		}

		// The destinations report their selection change back to this
		// system: the guard keeps that from selecting again.
		drawer.setUpdatingSelection(true);
		Selection::select(componentRegistry, entityIdentifier, destinations,
						  destinations[index]);
		drawer.setUpdatingSelection(false);

		apply(componentRegistry, entityIdentifier);
	}

	void NavigationDrawer::selectFirstEnabled(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &destinations =
			componentRegistry
				.getComponent<components::NavigationDrawer>(entityIdentifier)
				.getDestinationEntities();

		for (std::size_t index = 0; index < destinations.size(); ++index) {
			if (isDestination(componentRegistry, destinations[index])
				&& !NavigationDrawerDestination::isDisabled(
					componentRegistry, destinations[index])) {
				select(componentRegistry, entityIdentifier, index);
				return;
			}
		}

		apply(componentRegistry, entityIdentifier);
	}

	void NavigationDrawer::destinationSelected(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index)
	{
		if (componentRegistry
				.getComponent<components::NavigationDrawer>(entityIdentifier)
				.isUpdatingSelection()) {
			return;
		}

		select(componentRegistry, entityIdentifier, index);
	}

	void
		NavigationDrawer::press(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier,
								std::size_t index)
	{
		const auto &destinations =
			componentRegistry
				.getComponent<components::NavigationDrawer>(entityIdentifier)
				.getDestinationEntities();

		if (index >= destinations.size()
			|| !isDestination(componentRegistry, destinations[index])
			|| NavigationDrawerDestination::isDisabled(componentRegistry,
													   destinations[index])) {
			return;
		}

		const bool changed =
			selectedIndex(componentRegistry, entityIdentifier) != index;
		select(componentRegistry, entityIdentifier, index);

		// Copy: the callback may replace itself.
		const auto onIndexChange =
			componentRegistry
				.getComponent<components::NavigationDrawer>(entityIdentifier)
				.getOnIndexChange();
		if (changed && onIndexChange) {
			onIndexChange(index);
		}
	}

	void NavigationDrawer::setDestinationDisabled(
		ecs::ComponentRegistry &componentRegistry,
		const ecs::Entity::Identifier &entityIdentifier, std::size_t index,
		bool disabled)
	{
		auto &drawer =
			componentRegistry.getComponent<components::NavigationDrawer>(
				entityIdentifier);
		const auto &destinations = drawer.getDestinationEntities();

		drawer.setDestinationDisabled(index, disabled);
		if (index < destinations.size()
			&& isDestination(componentRegistry, destinations[index])) {
			NavigationDrawerDestination::setDisabled(
				componentRegistry, destinations[index], disabled);
		}
	}

	void NavigationDrawer::show(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(true);
		apply(componentRegistry, entityIdentifier);
	}

	void NavigationDrawer::hide(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		componentRegistry.getComponent<components::Overlay>(entityIdentifier)
			.setVisible(false);
		apply(componentRegistry, entityIdentifier);
	}

	void
		NavigationDrawer::apply(ecs::ComponentRegistry &componentRegistry,
								const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &drawer =
			componentRegistry.getComponent<components::NavigationDrawer>(
				entityIdentifier);
		const Variant variant	 = drawer.getVariant();
		const bool collapsed	 = variant == Variant::Collapsed;
		const bool modal		 = variant == Variant::Modal;
		const float width		 = widthOf(variant);
		const auto &destinations = drawer.getDestinationEntities();

		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				collapsed ? components::Layout::CrossAxisAlignment::Center
						  : components::Layout::CrossAxisAlignment::Start)
			.setPadding(Padding)
			.setSpacing(Spacing)
			.setFixedWidth(width)
			.setFixedHeight(components::NavigationDrawer::Height);

		// The drawer is attached to the leading edge: only its trailing
		// corners are rounded.
		const float radius = getShapeRadius(
			collapsed ? theme::ShapeRole::None : theme::ShapeRole::Large);
		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setTopLeftRadius(0.0f)
			.setBottomLeftRadius(0.0f)
			.setTopRightRadius(radius)
			.setBottomRightRadius(radius);

		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(schemeColor(modal ? SchemeColorRole::SurfaceContainerLow
										: SchemeColorRole::Surface));

		for (const auto destination: destinations) {
			if (!isDestination(componentRegistry, destination)) {
				continue;
			}

			componentRegistry.getComponent<components::Layout>(destination)
				.setFixedWidth(width - 2.0f * Padding);
			NavigationDrawerDestination::apply(componentRegistry, destination);
		}

		// The entries are the children the Layout system places in a
		// column; the content of the destinations follows them.
		layout.setChildren(drawer.getEntryEntities());
		Layout::apply(componentRegistry, entityIdentifier, layout.getChildren(),
					  Layout::layerOf(componentRegistry, entityIdentifier));

		for (const auto destination: destinations) {
			if (isDestination(componentRegistry, destination)) {
				NavigationDrawerDestination::arrange(componentRegistry,
													 destination);
			}
		}

		auto &overlay = componentRegistry.getComponent<components::Overlay>(
			entityIdentifier);
		overlay.setModal(modal)
			.setDismissOnOutsideClick(modal)
			.setDismissOnEscape(true);

		const auto backdrop = drawer.getBackdrop();
		if (backdrop != ecs::Entity::InvalidIdentifier) {
			// Only a modal drawer dims what is behind it.
			Backdrop::setVisible(componentRegistry, backdrop,
								 modal && overlay.isVisible());
			const auto &bound =
				componentRegistry.getComponent<components::Bound>(
					entityIdentifier);
			Backdrop::follow(
				componentRegistry, backdrop,
				componentRegistry
					.getComponent<components::Transform>(entityIdentifier)
					.getPose(),
				bound.getWidth(), bound.getHeight());
		}
	}

	void NavigationDrawer::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::NavigationDrawer>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
