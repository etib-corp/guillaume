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

#include "guillaume/entities/navigation_bar.hpp"

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
#include "guillaume/entities/icon.hpp"
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
		constexpr float NavigationBarHeight = 80.0f;

		/**
		 * @brief Internal navigation destination: a selectable layout holding
		 * an icon and a label.
		 */
		class Destination:
			public std::enable_shared_from_this<Destination>,
			public ecs::ParentEntityFiller<
				components::Layout, components::Transform, components::Bound,
				components::Color, components::Borders, components::Selectable>
		{
			private:
			std::string _iconGlyph {};		   ///< Icon glyph name.
			std::string _label {};			   ///< Label text.
			bool _showLabel { true };		   ///< Whether the label is shown.
			std::shared_ptr<Icon> _icon {};	   ///< Icon child.
			std::shared_ptr<Text> _labelEntity {};	  ///< Label child.

			public:
			/**
			 * @brief Construct a navigation destination.
			 * @param registry The component registry.
			 * @param iconGlyph The icon glyph name.
			 * @param label The label text.
			 */
			Destination(ecs::ComponentRegistry &registry,
						const std::string &iconGlyph, const std::string &label)
				: ecs::ParentEntityFiller<
					  components::Layout, components::Transform,
					  components::Bound, components::Color, components::Borders,
					  components::Selectable>(registry)
				, _iconGlyph(iconGlyph)
				, _label(label)
				, _showLabel(true)
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
					.setPadding(4.0f);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(transparentColor());

				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setBorderRadius(0.0f)
					.setColor(transparentColor());
			}

			/**
			 * @brief Default destructor.
			 */
			~Destination(void) override = default;

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
				const auto iconColor = selected
					? schemeColor(SchemeColorRole::OnSecondaryContainer)
					: schemeColor(SchemeColorRole::OnSurfaceVariant);

				if (_icon != nullptr) {
					_icon->setColor(iconColor);
				}

				if (_labelEntity != nullptr) {
					_labelEntity->setColor(_showLabel ? iconColor
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

	NavigationBar::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
									ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<NavigationBar>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<NavigationBar> NavigationBar::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<NavigationBar>(
			this->getComponentRegistry(), _config, _variant, _destinations);
		return entity;
	}

	void NavigationBar::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::WithLabels;
		_destinations.clear();

		_config.axis = components::Layout::Axis::Horizontal;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::SpaceBetween;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Center;
		_config.padding = 8.0f;
		_config.spacing = 0.0f;
		_config.setFixedWidth(360.0f);
		_config.setFixedHeight(NavigationBarHeight);
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

	NavigationBar::Builder &
		NavigationBar::Builder::addDestination(const std::string &iconGlyph,
											   const std::string &label)
	{
		_destinations.emplace_back(iconGlyph, label);
		return *this;
	}

	std::shared_ptr<NavigationBar> NavigationBar::Director::makeNavigationBar(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::vector<std::pair<std::string, std::string>> &destinations)
	{
		builder.withVariant(variant);

		for (const auto &destination: destinations) {
			builder.addDestination(destination.first, destination.second);
		}

		return builder.registerEntity(parent);
	}

	NavigationBar::NavigationBar(
		ecs::ComponentRegistry &registry, const SurfaceConfig &config,
		Variant variant,
		const std::vector<std::pair<std::string, std::string>> &destinations)
		: SurfaceBase<components::SelectionGroup>(registry, config)
		, _variant(variant)
		, _destinations(destinations)
		, _destinationEntities()
		, _updatingSelection(false)
	{
		getComponentRegistry()
			.getComponent<components::SelectionGroup>(getIdentifier())
			.setAllowEmpty(false);
	}

	NavigationBar::~NavigationBar(void)
	{
	}

	void NavigationBar::buildDestinations(void)
	{
		_destinationEntities.clear();

		for (const auto &destination: _destinations) {
			auto entity = std::make_shared<Destination>(
				getComponentRegistry(), destination.first, destination.second);
			entity->setParent(shared_from_this());
			this->addEntity(entity);
			_destinationEntities.push_back(entity);
		}

		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity =
				std::dynamic_pointer_cast<Destination>(_destinationEntities[i]);
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

		setChildren(_destinationEntities);
	}

	void NavigationBar::selectEntity(ecs::Entity::Identifier identifier)
	{
		std::vector<ecs::Entity::Identifier> childIdentifiers;
		childIdentifiers.reserve(_destinationEntities.size());

		for (const auto &entity: _destinationEntities) {
			childIdentifiers.push_back(entity->getIdentifier());
		}

		systems::Selection::select(getComponentRegistry(), getIdentifier(),
								   childIdentifiers, identifier);
	}

	void NavigationBar::applyVariant(void)
	{
		const bool showAllLabels = (_variant == Variant::WithLabels);

		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity =
				std::dynamic_pointer_cast<Destination>(_destinationEntities[i]);
			if (entity == nullptr) {
				continue;
			}

			const bool selected = entity->isSelected();
			entity->setShowLabel(showAllLabels || selected);
			entity->applySelectionColor(selected);
		}
	}

	void NavigationBar::applySelection(void)
	{
		if (_destinationEntities.empty()) {
			return;
		}

		_updatingSelection = true;
		selectEntity(_destinationEntities[0]->getIdentifier());
		_updatingSelection = false;

		applyVariant();
	}

	NavigationBar &NavigationBar::setVariant(Variant variant)
	{
		_variant = variant;
		applyVariant();
		return *this;
	}

	NavigationBar::Variant NavigationBar::getVariant(void) const
	{
		return _variant;
	}

	NavigationBar &NavigationBar::addDestination(const std::string &iconGlyph,
												 const std::string &label)
	{
		_destinations.emplace_back(iconGlyph, label);
		buildDestinations();
		applySelection();
		return *this;
	}

	NavigationBar &NavigationBar::select(std::size_t index)
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

	std::size_t NavigationBar::getSelectedIndex(void) const
	{
		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity =
				std::dynamic_pointer_cast<Destination>(_destinationEntities[i]);
			if (entity != nullptr && entity->isSelected()) {
				return i;
			}
		}

		return 0;
	}

	std::size_t NavigationBar::getDestinationCount(void) const
	{
		return _destinations.size();
	}

	ecs::Entity::Identifier
		NavigationBar::getDestinationIdentifier(std::size_t index) const
	{
		if (index >= _destinationEntities.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _destinationEntities[index]->getIdentifier();
	}

	void NavigationBar::initialize(void)
	{
		SurfaceBase::initialize();

		if (_destinationEntities.empty()) {
			buildDestinations();
		}

		applySelection();
	}

	void NavigationBar::update(void)
	{
		SurfaceBase::update();

		for (auto &entity: _destinationEntities) {
			auto destination = std::dynamic_pointer_cast<Destination>(entity);
			if (destination != nullptr) {
				destination->applyGeometry();
			}
		}

		applyVariant();
	}

}	 // namespace guillaume::entities
