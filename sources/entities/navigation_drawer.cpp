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

#include <cstdint>
#include <memory>
#include <vector>

#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/divider.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/selection.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		using detail::DrawerDestination;

		/**
		 * @brief M3 standard/modal drawer width.
		 */
		constexpr float DrawerWidth = 320.0f;

		/**
		 * @brief Collapsed drawer (rail) width.
		 */
		constexpr float CollapsedWidth = 80.0f;

		/**
		 * @brief Drawer padding.
		 */
		constexpr float DrawerPadding = theme::SpacingScale::get(3);

		/**
		 * @brief Drawer height.
		 */
		constexpr float DrawerHeight = 640.0f;

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
		auto entity = std::make_shared<NavigationDrawer>(
			this->getComponentRegistry(), _config, _variant, _entries,
			_onIndexChange);
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
		_config.padding = DrawerPadding;
		_config.spacing = theme::SpacingScale::get(1);
		_config.setFixedWidth(DrawerWidth);
		_config.setFixedHeight(DrawerHeight);
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
		NavigationDrawer::Director::makeNavigationDrawer(
			Builder &builder, std::shared_ptr<ecs::Entity> parent,
			Variant variant,
			const std::vector<std::pair<std::string, std::string>>
				&destinations)
	{
		builder.withVariant(variant);

		for (const auto &destination: destinations) {
			builder.addDestination(destination.first, destination.second);
		}

		return builder.registerEntity(parent);
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
		: SurfaceBase<components::Overlay, components::SelectionGroup>(registry,
																	   config)
		, _variant(variant)
		, _entries(entries)
		, _destinationEntities()
		, _sectionEntities()
		, _dividerEntities()
		, _backdrop()
		, _onIndexChange(std::move(onIndexChange))
		, _updatingSelection(false)
	{
		getComponentRegistry()
			.getComponent<components::SelectionGroup>(getIdentifier())
			.setAllowEmpty(false);
	}

	NavigationDrawer::~NavigationDrawer(void)
	{
	}

	float NavigationDrawer::getDrawerWidth(void) const
	{
		return _variant == Variant::Collapsed ? CollapsedWidth : DrawerWidth;
	}

	void NavigationDrawer::buildDestinations(void)
	{
		_destinationEntities.clear();
		_sectionEntities.clear();
		_dividerEntities.clear();

		if (_backdrop == nullptr) {
			_backdrop = std::make_shared<Backdrop>(getComponentRegistry());
			_backdrop->setParent(shared_from_this());
			this->addEntity(_backdrop);
		}

		const bool collapsed  = (_variant == Variant::Collapsed);
		const float itemWidth = getDrawerWidth() - 2.0f * DrawerPadding;

		std::vector<std::shared_ptr<ecs::Entity>> children;

		for (const auto &entry: _entries) {
			switch (entry.kind) {
				case EntryKind::Section: {
					if (collapsed || entry.label.empty()) {
						break;
					}
					auto header = buildText(
						getComponentRegistry(), *this, shared_from_this(),
						entry.label, theme::TypeRole::TitleSmall,
						schemeColor(SchemeColorRole::OnSurfaceVariant));
					if (header != nullptr) {
						getComponentRegistry()
							.addComponent<components::Accessibility>(
								header->getIdentifier());
						getComponentRegistry()
							.getComponent<components::Accessibility>(
								header->getIdentifier())
							.setRole(components::AccessibilityRole::Header)
							.setLabel(entry.label)
							.setFocusable(false);
						_sectionEntities.push_back(header);
						children.push_back(header);
					}
					break;
				}
				case EntryKind::Divider: {
					Divider::Builder builder(getComponentRegistry(), *this);
					Divider::Director director;
					auto divider = director.makeDivider(
						builder, shared_from_this(), itemWidth,
						Divider::Variant::FullWidth);
					if (divider != nullptr) {
						_dividerEntities.push_back(divider);
						children.push_back(divider);
					}
					break;
				}
				case EntryKind::Destination:
				default: {
					const std::size_t index = _destinationEntities.size();
					auto destination = std::make_shared<DrawerDestination>(
						getComponentRegistry(), entry, collapsed, itemWidth);
					destination->setParent(shared_from_this());
					this->addEntity(destination);
					destination->setOnPress([this, index]() {
						this->pressDestination(index);
					});
					destination->setOnSelectionChanged([this,
														index](bool selected) {
						if (this->_updatingSelection || !selected) {
							return;
						}
						this->_updatingSelection = true;
						this->selectEntity(
							this->_destinationEntities[index]->getIdentifier());
						this->_updatingSelection = false;
						this->applyVariant();
					});
					_destinationEntities.push_back(destination);
					children.push_back(destination);
					break;
				}
			}
		}

		setChildren(children);
	}

	void NavigationDrawer::selectEntity(ecs::Entity::Identifier identifier)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_destinationEntities.size());

		for (const auto &entity: _destinationEntities) {
			childIdentifiers.push_back(entity->getIdentifier());
		}

		systems::Selection::select(getComponentRegistry(), getIdentifier(),
								   childIdentifiers, identifier);
	}

	void NavigationDrawer::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Vertical;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment = _variant == Variant::Collapsed
			? components::Layout::CrossAxisAlignment::Center
			: components::Layout::CrossAxisAlignment::Start;
		config.padding			  = DrawerPadding;
		config.spacing			  = theme::SpacingScale::get(1);
		config.hasFixedWidth	  = true;
		config.fixedWidth		  = getDrawerWidth();
		config.hasFixedHeight	  = true;
		config.fixedHeight		  = DrawerHeight;

		setSurfaceConfig(config);

		const float radius = _variant == Variant::Collapsed
			? getShapeRadius(theme::ShapeRole::None)
			: getShapeRadius(theme::ShapeRole::Large);
		auto &borders =
			this->getComponentRegistry().getComponent<components::Borders>(
				this->getIdentifier());
		borders.setTopLeftRadius(0.0f)
			.setBottomLeftRadius(0.0f)
			.setTopRightRadius(radius)
			.setBottomRightRadius(radius);

		if (_variant == Variant::Modal) {
			setColor(schemeColor(SchemeColorRole::SurfaceContainerLow));
		} else {
			setColor(schemeColor(SchemeColorRole::Surface));
		}

		for (auto &entity: _destinationEntities) {
			auto destination =
				std::dynamic_pointer_cast<DrawerDestination>(entity);
			if (destination != nullptr) {
				destination->applySelectionColor(destination->isSelected());
			}
		}
	}

	void NavigationDrawer::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		const bool modal = (_variant == Variant::Modal);
		overlay.setModal(modal);
		overlay.setDismissOnOutsideClick(modal);
		overlay.setDismissOnEscape(true);

		if (_backdrop != nullptr) {
			_backdrop->setVisible(modal && overlay.isVisible());
			const auto &bound =
				getComponentRegistry().getComponent<components::Bound>(
					getIdentifier());
			_backdrop->follow(
				getComponentRegistry()
					.getComponent<components::Transform>(getIdentifier())
					.getPose(),
				bound.getWidth(), bound.getHeight());
		}
	}

	void NavigationDrawer::applySelection(void)
	{
		for (const auto &entity: _destinationEntities) {
			auto destination =
				std::dynamic_pointer_cast<DrawerDestination>(entity);
			if (destination != nullptr && !destination->isDisabled()) {
				_updatingSelection = true;
				selectEntity(destination->getIdentifier());
				_updatingSelection = false;
				break;
			}
		}

		applyVariant();
	}

	void NavigationDrawer::pressDestination(std::size_t index)
	{
		if (index >= _destinationEntities.size()) {
			return;
		}
		auto destination = std::dynamic_pointer_cast<DrawerDestination>(
			_destinationEntities[index]);
		if (destination == nullptr || destination->isDisabled()) {
			return;
		}

		const bool changed = getSelectedIndex() != index;
		select(index);
		if (changed && _onIndexChange) {
			_onIndexChange(index);
		}
	}

	NavigationDrawer &NavigationDrawer::setVariant(Variant variant)
	{
		const bool rebuild =
			(variant == Variant::Collapsed) != (_variant == Variant::Collapsed);
		const std::size_t selected = getSelectedIndex();
		_variant				   = variant;
		if (rebuild && !_destinationEntities.empty()) {
			buildDestinations();
			select(selected);
		}
		applyVariant();
		applyOverlay();
		return *this;
	}

	NavigationDrawer::Variant NavigationDrawer::getVariant(void) const
	{
		return _variant;
	}

	NavigationDrawer &NavigationDrawer::select(std::size_t index)
	{
		if (index >= _destinationEntities.size()) {
			return *this;
		}

		_updatingSelection = true;
		selectEntity(_destinationEntities[index]->getIdentifier());
		_updatingSelection = false;

		applyVariant();

		return *this;
	}

	std::size_t NavigationDrawer::getSelectedIndex(void) const
	{
		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity = std::dynamic_pointer_cast<DrawerDestination>(
				_destinationEntities[i]);
			if (entity != nullptr && entity->isSelected()) {
				return i;
			}
		}

		return 0;
	}

	NavigationDrawer &
		NavigationDrawer::setDestinationDisabled(std::size_t index,
												 bool disabled)
	{
		std::size_t destinationIndex = 0;
		for (auto &entry: _entries) {
			if (entry.kind != EntryKind::Destination) {
				continue;
			}
			if (destinationIndex++ == index) {
				entry.disabled = disabled;
				break;
			}
		}

		if (index < _destinationEntities.size()) {
			auto destination = std::dynamic_pointer_cast<DrawerDestination>(
				_destinationEntities[index]);
			if (destination != nullptr) {
				destination->setDisabled(disabled);
			}
		}
		return *this;
	}

	NavigationDrawer &NavigationDrawer::setOnIndexChange(
		std::function<void(std::size_t)> onIndexChange)
	{
		_onIndexChange = std::move(onIndexChange);
		return *this;
	}

	NavigationDrawer &NavigationDrawer::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		applyOverlay();
		return *this;
	}

	NavigationDrawer &NavigationDrawer::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		applyOverlay();
		return *this;
	}

	bool NavigationDrawer::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
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
		std::size_t count = 0;
		for (const auto &entry: _entries) {
			if (entry.kind == EntryKind::Destination) {
				++count;
			}
		}
		return count;
	}

	ecs::Entity::Identifier
		NavigationDrawer::getDestinationIdentifier(std::size_t index) const
	{
		if (index >= _destinationEntities.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _destinationEntities[index]->getIdentifier();
	}

	ecs::Entity::Identifier
		NavigationDrawer::getRightIdentifier(std::size_t index) const
	{
		if (index >= _destinationEntities.size()) {
			return ecs::Entity::InvalidIdentifier;
		}
		const auto destination = std::dynamic_pointer_cast<DrawerDestination>(
			_destinationEntities[index]);
		return destination != nullptr ? destination->getRightIdentifier()
									  : ecs::Entity::InvalidIdentifier;
	}

	std::size_t NavigationDrawer::getSectionCount(void) const
	{
		return _sectionEntities.size();
	}

	ecs::Entity::Identifier
		NavigationDrawer::getSectionIdentifier(std::size_t index) const
	{
		if (index >= _sectionEntities.size()) {
			return ecs::Entity::InvalidIdentifier;
		}
		return _sectionEntities[index]->getIdentifier();
	}

	std::size_t NavigationDrawer::getDividerCount(void) const
	{
		return _dividerEntities.size();
	}

	ecs::Entity::Identifier NavigationDrawer::getBackdropIdentifier(void) const
	{
		return _backdrop != nullptr ? _backdrop->getIdentifier()
									: ecs::Entity::InvalidIdentifier;
	}

	void NavigationDrawer::initialize(void)
	{
		SurfaceBase::initialize();

		if (_destinationEntities.empty()) {
			buildDestinations();
		}

		applyOverlay();
		applySelection();
	}

	void NavigationDrawer::update(void)
	{
		SurfaceBase::update();

		for (auto &entity: _destinationEntities) {
			auto destination =
				std::dynamic_pointer_cast<DrawerDestination>(entity);
			if (destination != nullptr) {
				destination->applyGeometry();
			}
		}

		applyOverlay();
		applyVariant();
	}

}	 // namespace guillaume::entities
