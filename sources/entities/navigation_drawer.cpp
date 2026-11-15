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

#include "guillaume/entities/navigation_drawer_destination.hpp"
#include "guillaume/entities/navigation_drawer.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "guillaume/components/navigation_drawer_destination.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/divider.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/systems/navigation_drawer.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		using detail::DrawerDestination;

		/**
		 * @brief Turn (icon, label) pairs into destination entries.
		 * @param pairs The (icon, label) pairs.
		 * @return One enabled destination entry per pair.
		 */
		std::vector<NavigationDrawer::Entry> toEntries(
			const std::vector<std::pair<std::string, std::string>> &pairs)
		{
			std::vector<NavigationDrawer::Entry> entries;
			entries.reserve(pairs.size());
			for (const auto &pair: pairs) {
				NavigationDrawer::Entry entry;
				entry.icon	= pair.first;
				entry.label = pair.second;
				entries.push_back(entry);
			}
			return entries;
		}
	}	 // namespace

	NavigationDrawer::Builder::Builder(
		ecs::ComponentRegistry &componentRegistry,
		ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<NavigationDrawer>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<NavigationDrawer>
		NavigationDrawer::Builder::buildEntity(void)
	{
		auto entity =
			NavigationDrawer::create(this->getComponentRegistry(), _config,
									 _variant, _entries, _onIndexChange);
		return entity;
	}

	void NavigationDrawer::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Standard;
		_entries.clear();
		_onIndexChange = {};

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding = systems::NavigationDrawer::Padding;
		_config.spacing = systems::NavigationDrawer::Spacing;
		_config.setFixedWidth(components::NavigationDrawer::Width);
		_config.setFixedHeight(components::NavigationDrawer::Height);
	}

	NavigationDrawer::Builder &
		NavigationDrawer::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	NavigationDrawer::Builder &
		NavigationDrawer::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	NavigationDrawer::Builder &NavigationDrawer::Builder::addDestination(
		const std::string &iconGlyph, const std::string &label,
		const std::string &right, bool disabled)
	{
		Entry entry;
		entry.kind	   = EntryKind::Destination;
		entry.icon	   = iconGlyph;
		entry.label	   = label;
		entry.right	   = right;
		entry.disabled = disabled;
		_entries.push_back(entry);
		return *this;
	}

	NavigationDrawer::Builder &
		NavigationDrawer::Builder::addSection(const std::string &title)
	{
		Entry entry;
		entry.kind	= EntryKind::Section;
		entry.label = title;
		_entries.push_back(entry);
		return *this;
	}

	NavigationDrawer::Builder &NavigationDrawer::Builder::addDivider(void)
	{
		Entry entry;
		entry.kind = EntryKind::Divider;
		_entries.push_back(entry);
		return *this;
	}

	NavigationDrawer::Builder &NavigationDrawer::Builder::withOnIndexChange(
		std::function<void(std::size_t)> onIndexChange)
	{
		_onIndexChange = std::move(onIndexChange);
		return *this;
	}

	std::shared_ptr<NavigationDrawer>
		NavigationDrawer::Builder::makeNavigationDrawer(
			std::shared_ptr<ecs::Entity> parent, Variant variant,
			const std::vector<std::pair<std::string, std::string>>
				&destinations)
	{
		withVariant(variant);

		for (const auto &destination: destinations) {
			addDestination(destination.first, destination.second);
		}

		return registerEntity(parent);
	}

	NavigationDrawer::NavigationDrawer(
		ecs::ComponentRegistry &registry, const SurfaceConfig &config,
		Variant variant,
		const std::vector<std::pair<std::string, std::string>> &destinations)
		: NavigationDrawer(registry, config, variant, toEntries(destinations),
						   {})
	{
	}

	NavigationDrawer::NavigationDrawer(
		ecs::ComponentRegistry &registry, const SurfaceConfig &config,
		Variant variant, const std::vector<Entry> &entries,
		std::function<void(std::size_t)> onIndexChange)
		: SurfaceBase<components::Overlay, components::SelectionGroup,
					  components::NavigationDrawer>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::NavigationDrawer>(getIdentifier())
			.setVariant(variant)
			.setEntries(entries)
			.setOnIndexChange(std::move(onIndexChange));
		systems::NavigationDrawer::install(getComponentRegistry(),
										   getIdentifier());
		systems::NavigationDrawer::apply(getComponentRegistry(),
										 getIdentifier());
	}

	NavigationDrawer::~NavigationDrawer(void)
	{
	}

	void NavigationDrawer::buildDestinations(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();
		// Copy: building children may move the component storage.
		const std::vector<Entry> entries =
			registry.getComponent<components::NavigationDrawer>(getIdentifier())
				.getEntries();
		const bool collapsed =
			registry.getComponent<components::NavigationDrawer>(getIdentifier())
				.getVariant()
			== Variant::Collapsed;
		const float itemWidth =
			systems::NavigationDrawer::widthOf(collapsed ? Variant::Collapsed
														 : Variant::Standard)
			- 2.0f * systems::NavigationDrawer::Padding;

		if (registry.getComponent<components::NavigationDrawer>(getIdentifier())
				.getBackdrop()
			== ecs::Entity::InvalidIdentifier) {
			const auto backdrop = Backdrop::create(registry);
			backdrop->setParent(shared_from_this());
			addEntity(backdrop);
			registry.getComponent<components::NavigationDrawer>(getIdentifier())
				.setBackdrop(backdrop->getIdentifier());
		}

		// Remove the entries of a previous build (a rail and a sheet use
		// different children); the backdrop is kept.
		const auto previousEntries =
			registry.getComponent<components::NavigationDrawer>(getIdentifier())
				.getEntryEntities();
		for (const auto previous: previousEntries) {
			removeEntity(previous);
		}

		Children children;
		std::vector<ecs::Entity::Identifier> entryIdentifiers;
		std::vector<ecs::Entity::Identifier> destinationIdentifiers;
		std::vector<ecs::Entity::Identifier> sectionIdentifiers;
		std::vector<ecs::Entity::Identifier> dividerIdentifiers;

		for (const auto &entry: entries) {
			std::shared_ptr<ecs::Entity> child;

			switch (entry.kind) {
				case EntryKind::Section: {
					// A rail has no room for section titles.
					// systems::NavigationDrawer::install gives the header
					// its semantics.
					if (collapsed) {
						break;
					}
					child = buildText(
						registry, *this, shared_from_this(), entry.label,
						theme::TypeRole::TitleSmall,
						schemeColor(SchemeColorRole::OnSurfaceVariant));
					if (child != nullptr) {
						sectionIdentifiers.push_back(child->getIdentifier());
					}
					break;
				}
				case EntryKind::Divider: {
					Divider::Builder builder(registry, *this);
					child = builder.makeDivider(shared_from_this(), itemWidth,
												Divider::Variant::FullWidth);
					if (child != nullptr) {
						dividerIdentifiers.push_back(child->getIdentifier());
					}
					break;
				}
				case EntryKind::Destination:
				default: {
					// systems::NavigationDrawer::install wires its press
					// and selection change.
					child = DrawerDestination::create(registry, entry,
													  collapsed, itemWidth);
					child->setParent(shared_from_this());
					addEntity(child);
					destinationIdentifiers.push_back(child->getIdentifier());
					break;
				}
			}

			if (child != nullptr) {
				entryIdentifiers.push_back(child->getIdentifier());
				children.push_back(std::move(child));
			}
		}

		registry.getComponent<components::NavigationDrawer>(getIdentifier())
			.setEntryEntities(entryIdentifiers)
			.setDestinationEntities(destinationIdentifiers)
			.setSectionEntities(sectionIdentifiers)
			.setDividerEntities(dividerIdentifiers);

		// Hand the layout children to the surface.
		setChildren(children);

		systems::NavigationDrawer::install(registry, getIdentifier());
	}

	NavigationDrawer &NavigationDrawer::setVariant(Variant variant)
	{
		auto &registry = getComponentRegistry();
		const Variant previous =
			registry.getComponent<components::NavigationDrawer>(getIdentifier())
				.getVariant();
		const bool rebuild =
			(variant == Variant::Collapsed) != (previous == Variant::Collapsed);
		const std::size_t selected =
			systems::NavigationDrawer::selectedIndex(registry, getIdentifier());

		registry.getComponent<components::NavigationDrawer>(getIdentifier())
			.setVariant(variant);
		// A rail and a sheet are made of different children.
		if (rebuild
			&& !registry
					.getComponent<components::NavigationDrawer>(getIdentifier())
					.getDestinationEntities()
					.empty()) {
			buildDestinations();
			systems::NavigationDrawer::select(registry, getIdentifier(),
											  selected);
		}
		systems::NavigationDrawer::apply(registry, getIdentifier());
		return *this;
	}

	NavigationDrawer::Variant NavigationDrawer::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationDrawer>(getIdentifier())
			.getVariant();
	}

	NavigationDrawer &NavigationDrawer::select(std::size_t index)
	{
		systems::NavigationDrawer::select(getComponentRegistry(),
										  getIdentifier(), index);
		return *this;
	}

	std::size_t NavigationDrawer::getSelectedIndex(void) const
	{
		return systems::NavigationDrawer::selectedIndex(getComponentRegistry(),
														getIdentifier());
	}

	NavigationDrawer &
		NavigationDrawer::setDestinationDisabled(std::size_t index,
												 bool disabled)
	{
		systems::NavigationDrawer::setDestinationDisabled(
			getComponentRegistry(), getIdentifier(), index, disabled);
		return *this;
	}

	NavigationDrawer &NavigationDrawer::setOnIndexChange(
		std::function<void(std::size_t)> onIndexChange)
	{
		getComponentRegistry()
			.getComponent<components::NavigationDrawer>(getIdentifier())
			.setOnIndexChange(std::move(onIndexChange));
		return *this;
	}

	NavigationDrawer &NavigationDrawer::show(void)
	{
		systems::NavigationDrawer::show(getComponentRegistry(),
										getIdentifier());
		return *this;
	}

	NavigationDrawer &NavigationDrawer::hide(void)
	{
		systems::NavigationDrawer::hide(getComponentRegistry(),
										getIdentifier());
		return *this;
	}

	bool NavigationDrawer::isVisible(void) const
	{
		return getComponentRegistry()
			.getComponent<components::Overlay>(getIdentifier())
			.isVisible();
	}

	NavigationDrawer &NavigationDrawer::open(void)
	{
		return show();
	}

	NavigationDrawer &NavigationDrawer::close(void)
	{
		return hide();
	}

	bool NavigationDrawer::isOpen(void) const
	{
		return isVisible();
	}

	std::size_t NavigationDrawer::getDestinationCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationDrawer>(getIdentifier())
			.getDestinationCount();
	}

	ecs::Entity::Identifier
		NavigationDrawer::getDestinationIdentifier(std::size_t index) const
	{
		const auto &destinations =
			getComponentRegistry()
				.getComponent<components::NavigationDrawer>(getIdentifier())
				.getDestinationEntities();

		return index < destinations.size() ? destinations[index]
										   : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier
		NavigationDrawer::getRightIdentifier(std::size_t index) const
	{
		const auto &registry   = getComponentRegistry();
		const auto destination = getDestinationIdentifier(index);

		if (!registry.hasComponent<components::NavigationDrawerDestination>(
				destination)) {
			return ecs::Entity::InvalidIdentifier;
		}
		return registry
			.getComponent<components::NavigationDrawerDestination>(destination)
			.getRightEntity();
	}

	std::size_t NavigationDrawer::getSectionCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationDrawer>(getIdentifier())
			.getSectionEntities()
			.size();
	}

	ecs::Entity::Identifier
		NavigationDrawer::getSectionIdentifier(std::size_t index) const
	{
		const auto &sections =
			getComponentRegistry()
				.getComponent<components::NavigationDrawer>(getIdentifier())
				.getSectionEntities();

		return index < sections.size() ? sections[index]
									   : ecs::Entity::InvalidIdentifier;
	}

	std::size_t NavigationDrawer::getDividerCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationDrawer>(getIdentifier())
			.getDividerEntities()
			.size();
	}

	ecs::Entity::Identifier NavigationDrawer::getBackdropIdentifier(void) const
	{
		return getComponentRegistry()
			.getComponent<components::NavigationDrawer>(getIdentifier())
			.getBackdrop();
	}

	void NavigationDrawer::build(void)
	{
		auto &registry = getComponentRegistry();

		if (registry.getComponent<components::NavigationDrawer>(getIdentifier())
				.getDestinationEntities()
				.empty()) {
			buildDestinations();
		}
		systems::NavigationDrawer::selectFirstEnabled(registry,
													  getIdentifier());
	}

}	 // namespace guillaume::entities
