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

#include "guillaume/entities/navigation_bar_destination.hpp"
#include "guillaume/entities/navigation_bar.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "guillaume/components/navigation_bar_destination.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/systems/navigation_bar.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		using detail::BarDestination;

		/**
		 * @brief Turn (icon, label) pairs into destinations without a
		 * focused icon or a badge.
		 * @param pairs The (icon, label) pairs.
		 * @return One destination per pair.
		 */
		std::vector<NavigationBar::Destination> toDestinations(
			const std::vector<std::pair<std::string, std::string>> &pairs)
		{
			std::vector<NavigationBar::Destination> destinations;
			destinations.reserve(pairs.size());
			for (const auto &pair: pairs) {
				destinations.push_back(NavigationBar::Destination {
					pair.first, pair.second, "", "" });
			}
			return destinations;
		}

		/**
		 * @brief Get one child of a destination of a navigation bar.
		 * @param registry The registry holding the components.
		 * @param identifier The navigation bar entity.
		 * @param index The destination index.
		 * @param getter The child getter of the destination component.
		 * @return The child identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier destinationPart(
			const ecs::ComponentRegistry &registry,
			ecs::Entity::Identifier identifier, std::size_t index,
			ecs::Entity::Identifier (
				components::NavigationBarDestination::*getter)(void)
				const noexcept)
		{
			const auto &destinations =
				registry.getComponent<components::NavigationBar>(identifier)
					.getDestinationEntities();

			if (index >= destinations.size()
				|| !registry.hasComponent<components::NavigationBarDestination>(
					destinations[index])) {
				return ecs::Entity::InvalidIdentifier;
			}
			return (registry.getComponent<components::NavigationBarDestination>(
						destinations[index])
					.*getter)();
		}
	}	 // namespace

	NavigationBar::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
									ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<NavigationBar>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<NavigationBar> NavigationBar::Builder::buildEntity(void)
	{
		auto entity =
			NavigationBar::create(this->getComponentRegistry(), _config,
								  _variant, _specs, _onIndexChange);
		return entity;
	}

	void NavigationBar::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::WithLabels;
		_destinations.clear();
		_specs.clear();
		_onIndexChange = {};

		_config.axis = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::SpaceBetween;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = theme::SpacingScale::get(2);
		_config.spacing = 0.0f;
		_config.setFixedWidth(360.0f);
		_config.setFixedHeight(components::NavigationBar::Height);
	}

	NavigationBar::Builder &
		NavigationBar::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	NavigationBar::Builder &NavigationBar::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	NavigationBar::Builder &NavigationBar::Builder::addDestination(
		const std::string &iconGlyph, const std::string &label,
		const std::string &focusedIconGlyph, const std::string &badge)
	{
		_destinations.emplace_back(iconGlyph, label);
		_specs.push_back(
			Destination { iconGlyph, label, focusedIconGlyph, badge });
		return *this;
	}

	NavigationBar::Builder &NavigationBar::Builder::withOnIndexChange(
		std::function<void(std::size_t)> onIndexChange)
	{
		_onIndexChange = std::move(onIndexChange);
		return *this;
	}

	std::shared_ptr<NavigationBar> NavigationBar::Builder::makeNavigationBar(
		std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::vector<std::pair<std::string, std::string>> &destinations)
	{
		withVariant(variant);

		for (const auto &destination: destinations) {
			addDestination(destination.first, destination.second);
		}

		return registerEntity(parent);
	}

	NavigationBar::NavigationBar(
		ecs::ComponentRegistry &registry, const SurfaceConfig &config,
		Variant variant,
		const std::vector<std::pair<std::string, std::string>> &destinations)
		: NavigationBar(registry, config, variant, toDestinations(destinations),
						{})
	{
	}

	NavigationBar::NavigationBar(ecs::ComponentRegistry &registry,
								 const SurfaceConfig &config, Variant variant,
								 const std::vector<Destination> &destinations,
								 std::function<void(std::size_t)> onIndexChange)
		: SurfaceBase<components::SelectionGroup, components::NavigationBar>(
			  registry, config)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBar>(getIdentifier())
			.setVariant(variant)
			.setDestinations(destinations)
			.setOnIndexChange(std::move(onIndexChange));
		systems::NavigationBar::install(getComponentRegistry(),
										getIdentifier());
		systems::NavigationBar::apply(getComponentRegistry(), getIdentifier());
	}

	NavigationBar::~NavigationBar(void)
	{
	}

	void NavigationBar::buildDestinations(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		// Copy: building children may move the component storage.
		const std::vector<Destination> destinations =
			registry.getComponent<components::NavigationBar>(getIdentifier())
				.getDestinations();
		std::vector<ecs::Entity::Identifier> identifiers =
			registry.getComponent<components::NavigationBar>(getIdentifier())
				.getDestinationEntities();

		// systems::NavigationBar sizes the destinations and wires their
		// press and selection change.
		for (std::size_t index = identifiers.size();
			 index < destinations.size(); ++index) {
			const auto destination =
				BarDestination::create(registry, destinations[index]);
			destination->setParent(shared_from_this());
			addEntity(destination);
			identifiers.push_back(destination->getIdentifier());
		}

		registry.getComponent<components::NavigationBar>(getIdentifier())
			.setDestinationEntities(identifiers);

		// Hand the layout children to the surface.
		Children children;
		for (const auto identifier: identifiers) {
			if (auto destination = getEntity<BarDestination>(identifier)) {
				children.push_back(std::move(destination));
			}
		}
		setChildren(children);

		systems::NavigationBar::install(registry, getIdentifier());
	}

	NavigationBar &NavigationBar::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBar>(getIdentifier())
			.setVariant(variant);
		systems::NavigationBar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	NavigationBar::Variant NavigationBar::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationBar>(getIdentifier())
			.getVariant();
	}

	NavigationBar &NavigationBar::addDestination(const std::string &iconGlyph,
												 const std::string &label)
	{
		return addDestination(Destination { iconGlyph, label, "", "" });
	}

	NavigationBar &NavigationBar::addDestination(const Destination &destination)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBar>(getIdentifier())
			.addDestination(destination);
		// A new destination needs its child entity.
		buildDestinations();
		systems::NavigationBar::select(getComponentRegistry(), getIdentifier(),
									   0);
		systems::NavigationBar::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	NavigationBar &NavigationBar::setBadge(std::size_t index,
										   const std::string &badge)
	{
		auto &registry = getComponentRegistry();
		const auto &bar =
			registry.getComponent<components::NavigationBar>(getIdentifier());

		if (index >= bar.getDestinations().size()) {
			return *this;
		}

		// A first badge label needs the badge child, which only the
		// destination entity can build.
		const auto &destinations = bar.getDestinationEntities();
		const auto destination	 = index < destinations.size()
			  ? getEntity<BarDestination>(destinations[index])
			  : nullptr;
		if (destination != nullptr) {
			destination->setBadge(badge);
		}
		systems::NavigationBar::setBadge(registry, getIdentifier(), index,
										 badge);
		return *this;
	}

	NavigationBar &NavigationBar::setOnIndexChange(
		std::function<void(std::size_t)> onIndexChange)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBar>(getIdentifier())
			.setOnIndexChange(std::move(onIndexChange));
		return *this;
	}

	NavigationBar &NavigationBar::setOnTabPress(
		std::function<void(std::size_t)> onTabPress)
	{
		getComponentRegistry()
			.getComponent<components::NavigationBar>(getIdentifier())
			.setOnTabPress(std::move(onTabPress));
		return *this;
	}

	NavigationBar &NavigationBar::select(std::size_t index)
	{
		systems::NavigationBar::select(getComponentRegistry(), getIdentifier(),
									   index);
		return *this;
	}

	std::size_t NavigationBar::getSelectedIndex(void) const
	{
		return systems::NavigationBar::selectedIndex(getComponentRegistry(),
													 getIdentifier());
	}

	std::size_t NavigationBar::getDestinationCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationBar>(getIdentifier())
			.getDestinations()
			.size();
	}

	ecs::Entity::Identifier
		NavigationBar::getDestinationIdentifier(std::size_t index) const
	{
		const auto &destinations =
			getComponentRegistry()
				.getComponent<components::NavigationBar>(getIdentifier())
				.getDestinationEntities();

		return index < destinations.size() ? destinations[index]
										   : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier
		NavigationBar::getIndicatorIdentifier(std::size_t index) const
	{
		return destinationPart(
			getComponentRegistry(), getIdentifier(), index,
			&components::NavigationBarDestination::getIndicator);
	}

	ecs::Entity::Identifier
		NavigationBar::getIconIdentifier(std::size_t index) const
	{
		return destinationPart(
			getComponentRegistry(), getIdentifier(), index,
			&components::NavigationBarDestination::getIconEntity);
	}

	ecs::Entity::Identifier
		NavigationBar::getLabelIdentifier(std::size_t index) const
	{
		return destinationPart(
			getComponentRegistry(), getIdentifier(), index,
			&components::NavigationBarDestination::getLabelEntity);
	}

	ecs::Entity::Identifier
		NavigationBar::getBadgeIdentifier(std::size_t index) const
	{
		return destinationPart(
			getComponentRegistry(), getIdentifier(), index,
			&components::NavigationBarDestination::getBadgeEntity);
	}

	void NavigationBar::build(void)
	{
		buildDestinations();
		systems::NavigationBar::select(getComponentRegistry(), getIdentifier(),
									   0);
		systems::NavigationBar::apply(getComponentRegistry(), getIdentifier());
	}

}	 // namespace guillaume::entities
