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

#include "guillaume/entities/navigation_rail.hpp"

#include <cstdint>
#include <memory>
#include <vector>

#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/selection.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float RailCollapsedWidth = 80.0f;
		constexpr float RailExpandedWidth  = 256.0f;

		/**
		 * @brief Internal rail destination: a selectable layout holding an icon
		 * and a label.
		 */
		class RailDestination:
			public std::enable_shared_from_this<RailDestination>,
			public ecs::ParentEntityFiller<
				components::Layout, components::Transform, components::Bound,
				components::Color, components::Borders, components::Selectable>
		{
			private:
			std::string _iconGlyph {};		   ///< Icon glyph name.
			std::string _label {};			   ///< Label text.
			bool _showLabel { false };		   ///< Whether label is shown.
			std::shared_ptr<Icon> _icon {};	   ///< Icon child.
			std::shared_ptr<Text> _labelEntity {};	  ///< Label child.

			public:
			/**
			 * @brief Construct a rail destination.
			 * @param registry The component registry.
			 * @param iconGlyph The icon glyph name.
			 * @param label The label text.
			 */
			RailDestination(ecs::ComponentRegistry &registry,
							const std::string &iconGlyph,
							const std::string &label)
				: ecs::ParentEntityFiller<
					  components::Layout, components::Transform,
					  components::Bound, components::Color, components::Borders,
					  components::Selectable>(registry)
				, _iconGlyph(iconGlyph)
				, _label(label)
				, _showLabel(false)
				, _icon()
				, _labelEntity()
			{
				getComponentRegistry()
					.getComponent<components::Layout>(getIdentifier())
					.setAxis(components::Layout::Axis::Vertical)
					.setMainAxisAlignment(
						components::Layout::MainAxisAlignment::Center)
					.setCrossAxisAlignment(
						components::Layout::CrossAxisAlignment::Center)
					.setSpacing(4.0f)
					.setPadding(8.0f);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(transparentColor());

				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setBorderRadius(16.0f)
					.setColor(transparentColor());
			}

			/**
			 * @brief Default destructor.
			 */
			~RailDestination(void) override = default;

			/**
			 * @brief Build the icon and label children.
			 */
			void initialize(void) override
			{
				_icon =
					buildIcon(getComponentRegistry(), *this, shared_from_this(),
							  _iconGlyph, 24.0f,
							  schemeColor(SchemeColorRole::OnSurfaceVariant));

				if (!_label.empty()) {
					_labelEntity = buildText(
						getComponentRegistry(), *this, shared_from_this(),
						_label, 12.0f,
						schemeColor(SchemeColorRole::OnSurfaceVariant));
				}

				applyGeometry();
			}

			/**
			 * @brief Set whether the label is shown.
			 * @param show Whether to show the label.
			 */
			void setShowLabel(bool show)
			{
				_showLabel = show;
			}

			/**
			 * @brief Set the selection change handler.
			 * @param handler The handler to invoke on selection change.
			 */
			void setOnSelectionChanged(
				const components::Selectable::Handler &handler)
			{
				getComponentRegistry()
					.getComponent<components::Selectable>(getIdentifier())
					.setOnSelectionChangedHandler(handler);
			}

			/**
			 * @brief Set the destination selected state.
			 * @param selected The new selected state.
			 */
			void setSelected(bool selected)
			{
				getComponentRegistry()
					.getComponent<components::Selectable>(getIdentifier())
					.setSelected(selected);
			}

			/**
			 * @brief Whether the destination is selected.
			 * @return True when selected.
			 */
			bool isSelected(void)
			{
				return getComponentRegistry()
					.getComponent<components::Selectable>(getIdentifier())
					.isSelected();
			}

			/**
			 * @brief Apply the selection colors to the icon and label.
			 * @param selected Whether the destination is selected.
			 */
			void applySelectionColor(bool selected)
			{
				const auto color = selected
					? schemeColor(SchemeColorRole::OnSecondaryContainer)
					: schemeColor(SchemeColorRole::OnSurfaceVariant);

				if (_icon != nullptr) {
					_icon->setColor(color);
				}

				if (_labelEntity != nullptr) {
					_labelEntity->setColor(_showLabel ? color
													  : transparentColor());
				}
			}

			/**
			 * @brief Arrange the destination content.
			 */
			void applyGeometry(void)
			{
				std::vector<ecs::Entity::Identifier> children;

				if (_icon != nullptr) {
					children.push_back(_icon->getIdentifier());
				}

				if (_labelEntity != nullptr) {
					children.push_back(_labelEntity->getIdentifier());
				}

				systems::Layout::apply(getComponentRegistry(), getIdentifier(),
									   children,
									   static_cast<std::uint32_t>(getLayer()));
			}
		};
	}	 // namespace

	NavigationRail::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
									 ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<NavigationRail>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<NavigationRail> NavigationRail::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<NavigationRail>(
			this->getComponentRegistry(), _config, _variant, _destinations,
			_fabGlyph);
		return entity;
	}

	void NavigationRail::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Collapsed;
		_destinations.clear();
		_fabGlyph.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = 8.0f;
		_config.spacing = 8.0f;
		_config.setFixedWidth(RailCollapsedWidth);
		_config.setFixedHeight(640.0f);
	}

	NavigationRail::Builder &
		NavigationRail::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	NavigationRail::Builder &
		NavigationRail::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	NavigationRail::Builder &
		NavigationRail::Builder::addDestination(const std::string &iconGlyph,
												const std::string &label)
	{
		_destinations.emplace_back(iconGlyph, label);
		return *this;
	}

	NavigationRail::Builder &
		NavigationRail::Builder::withFab(const std::string &iconGlyph)
	{
		_fabGlyph = iconGlyph;
		return *this;
	}

	std::shared_ptr<NavigationRail>
		NavigationRail::Director::makeNavigationRail(
			Builder &builder, std::shared_ptr<ecs::Entity> parent,
			Variant variant,
			const std::vector<std::pair<std::string, std::string>>
				&destinations,
			const std::string &fabGlyph)
	{
		builder.withVariant(variant).withFab(fabGlyph);

		for (const auto &destination: destinations) {
			builder.addDestination(destination.first, destination.second);
		}

		return builder.registerEntity(parent);
	}

	NavigationRail::NavigationRail(
		ecs::ComponentRegistry &registry, const SurfaceConfig &config,
		Variant variant,
		const std::vector<std::pair<std::string, std::string>> &destinations,
		const std::string &fabGlyph)
		: SurfaceBase<components::SelectionGroup>(registry, config)
		, _variant(variant)
		, _destinations(destinations)
		, _fabGlyph(fabGlyph)
		, _destinationEntities()
		, _fabEntity()
		, _updatingSelection(false)
	{
		getComponentRegistry()
			.getComponent<components::SelectionGroup>(getIdentifier())
			.setAllowEmpty(false);
	}

	NavigationRail::~NavigationRail(void)
	{
	}

	void NavigationRail::buildDestinations(void)
	{
		_destinationEntities.clear();

		for (const auto &destination: _destinations) {
			auto entity = std::make_shared<RailDestination>(
				getComponentRegistry(), destination.first, destination.second);
			entity->setParent(shared_from_this());
			this->addEntity(entity);
			_destinationEntities.push_back(entity);
		}

		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity = std::dynamic_pointer_cast<RailDestination>(
				_destinationEntities[i]);
			if (entity == nullptr) {
				continue;
			}

			entity->setOnSelectionChanged([this, i](bool selected) {
				if (this->_updatingSelection || !selected) {
					return;
				}

				this->_updatingSelection = true;
				this->selectEntity(
					this->_destinationEntities[i]->getIdentifier());
				this->_updatingSelection = false;

				this->applyVariant();
			});
		}

		std::vector<std::shared_ptr<ecs::Entity>> children(
			_destinationEntities.begin(), _destinationEntities.end());

		if (!_fabGlyph.empty() && _fabEntity == nullptr) {
			_fabEntity = buildIcon(
				getComponentRegistry(), *this, shared_from_this(), _fabGlyph,
				24.0f, schemeColor(SchemeColorRole::OnPrimaryContainer));
		}

		if (_fabEntity != nullptr) {
			children.push_back(_fabEntity);
		}

		setChildren(children);
	}

	void NavigationRail::applyVariant(void)
	{
		const bool expanded = (_variant == Variant::Expanded);

		auto config			 = getSurfaceConfig();
		config.hasFixedWidth = true;
		config.fixedWidth = expanded ? RailExpandedWidth : RailCollapsedWidth;
		config.crossAxisAlignment = expanded
			? components::Layout::CrossAxisAlignment::Start
			: components::Layout::CrossAxisAlignment::Center;
		setSurfaceConfig(config);

		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity = std::dynamic_pointer_cast<RailDestination>(
				_destinationEntities[i]);
			if (entity == nullptr) {
				continue;
			}

			const bool selected = entity->isSelected();
			entity->setShowLabel(expanded);
			entity->applySelectionColor(selected);
		}
	}

	void NavigationRail::selectEntity(ecs::Entity::Identifier identifier)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_destinationEntities.size());

		for (const auto &entity: _destinationEntities) {
			childIdentifiers.push_back(entity->getIdentifier());
		}

		systems::Selection::select(getComponentRegistry(), getIdentifier(),
								   childIdentifiers, identifier);
	}

	void NavigationRail::applySelection(void)
	{
		if (_destinationEntities.empty()) {
			return;
		}

		_updatingSelection = true;
		selectEntity(_destinationEntities[0]->getIdentifier());
		_updatingSelection = false;

		applyVariant();
	}

	NavigationRail &NavigationRail::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	NavigationRail::Variant NavigationRail::getVariant(void) const
	{
		return _variant;
	}

	NavigationRail &NavigationRail::select(std::size_t index)
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

	std::size_t NavigationRail::getSelectedIndex(void) const
	{
		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity = std::dynamic_pointer_cast<RailDestination>(
				_destinationEntities[i]);
			if (entity != nullptr && entity->isSelected()) {
				return i;
			}
		}

		return 0;
	}

	std::size_t NavigationRail::getDestinationCount(void) const
	{
		return _destinations.size();
	}

	ecs::Entity::Identifier
		NavigationRail::getDestinationIdentifier(std::size_t index) const
	{
		if (index >= _destinationEntities.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _destinationEntities[index]->getIdentifier();
	}

	void NavigationRail::initialize(void)
	{
		SurfaceBase::initialize();

		if (_destinationEntities.empty()) {
			buildDestinations();
		}

		applySelection();
	}

	void NavigationRail::update(void)
	{
		SurfaceBase::update();

		for (auto &entity: _destinationEntities) {
			auto destination =
				std::dynamic_pointer_cast<RailDestination>(entity);
			if (destination != nullptr) {
				destination->applyGeometry();
			}
		}

		applyVariant();
	}

}	 // namespace guillaume::entities
