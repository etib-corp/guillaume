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

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/selection.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/entities/badge.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/placement_helpers.hpp"
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
		/**
		 * @brief M3 navigation bar height.
		 */
		constexpr float NavigationBarHeight = 80.0f;

		/**
		 * @brief M3 active indicator width.
		 */
		constexpr float IndicatorWidth = 64.0f;

		/**
		 * @brief M3 active indicator height.
		 */
		constexpr float IndicatorHeight = 32.0f;

		/**
		 * @brief Horizontal badge offset from the indicator origin, so the
		 * badge sits on the icon's top-right corner.
		 */
		constexpr float BadgeOffsetX = 36.0f;

		/**
		 * @brief Vertical badge offset from the indicator origin.
		 */
		constexpr float BadgeOffsetY = 2.0f;

		/**
		 * @brief Internal navigation destination: a pressable vertical layout
		 * holding the active-indicator pill (wrapping the icon) and a label.
		 */
		class BarDestination:
			public std::enable_shared_from_this<BarDestination>,
			public ecs::ParentEntityFiller<
				components::Layout, components::Transform, components::Bound,
				components::Color, components::Borders, components::Selectable>
		{
			private:
			NavigationBar::Destination _spec {};	///< Destination data.
			bool _showLabel { true };	   ///< Whether the label is shown.
			std::shared_ptr<View> _pill {};		   ///< Active indicator.
			std::shared_ptr<Icon> _icon {};		   ///< Icon child.
			std::shared_ptr<Text> _labelEntity {};	  ///< Label child.
			std::shared_ptr<Badge> _badge {};		  ///< Badge child.
			std::function<void(void)> _onPress {};	  ///< Press handler.

			public:
			/**
			 * @brief Construct a navigation destination.
			 * @param registry The component registry.
			 * @param spec The destination data.
			 */
			BarDestination(ecs::ComponentRegistry &registry,
						   const NavigationBar::Destination &spec)
				: ecs::ParentEntityFiller<
					  components::Layout, components::Transform,
					  components::Bound, components::Color, components::Borders,
					  components::Selectable>(registry)
				, _spec(spec)
			{
				getComponentRegistry()
					.getComponent<components::Layout>(getIdentifier())
					.setAxis(components::Layout::Axis::Vertical)
					.setMainAxisAlignment(
						components::Layout::MainAxisAlignment::Center)
					.setCrossAxisAlignment(
						components::Layout::CrossAxisAlignment::Center)
					.setSpacing(theme::SpacingScale::get(1))
					.setPadding(0.0f);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(transparentColor());

				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setBorderRadius(getShapeRadius(theme::ShapeRole::None))
					.setColor(transparentColor());

				makeInteractive(
					getComponentRegistry(), getIdentifier(),
					components::AccessibilityRole::Tab,
					[this]() {
						if (_onPress) {
							_onPress();
						}
					},
					[this]() {
						applySelectionColor(isSelected());
					});
				getComponentRegistry()
					.getComponent<components::Accessibility>(getIdentifier())
					.setLabel(_spec.label);
			}

			/**
			 * @brief Default destructor.
			 */
			~BarDestination(void) override = default;

			/**
			 * @brief Build the indicator, icon, label and badge children.
			 */
			void initialize(void) override
			{
				View::Builder pillBuilder(getComponentRegistry(), *this);
				View::Director pillDirector;
				_pill = pillDirector.makeDefaultView(
					pillBuilder, shared_from_this(), utility::graphic::PoseF(),
					{});
				_pill->setColor(transparentColor());
				_pill->setBorderRadius(getShapeRadius(theme::ShapeRole::Full));
				_pill->setAxis(components::Layout::Axis::Horizontal);
				_pill->setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Center);
				_pill->setCrossAxisAlignment(
					components::Layout::CrossAxisAlignment::Center);
				_pill->setPadding(0.0f);
				_pill->setFixedWidth(IndicatorWidth);
				_pill->setFixedHeight(IndicatorHeight);

				_icon = buildIcon(getComponentRegistry(), *_pill, _pill,
								  _spec.icon, 24.0f,
								  schemeColor(SchemeColorRole::OnSurfaceVariant));
				if (_icon != nullptr) {
					_pill->setEntities({ _icon });
				}

				if (!_spec.label.empty()) {
					_labelEntity = buildText(
						getComponentRegistry(), *this, shared_from_this(),
						_spec.label, theme::TypeRole::LabelMedium,
						schemeColor(SchemeColorRole::OnSurfaceVariant));
				}

				setBadge(_spec.badge);
				applyGeometry();
			}

			/**
			 * @brief Set the press handler.
			 * @param onPress Press handler.
			 */
			void setOnPress(std::function<void(void)> onPress)
			{
				_onPress = std::move(onPress);
			}

			/**
			 * @brief Set or update the badge label.
			 * @param badge Badge label ("" keeps no badge, or shows a dot on
			 * an existing badge).
			 */
			void setBadge(const std::string &badge)
			{
				_spec.badge = badge;
				if (_badge == nullptr && !badge.empty() && _pill != nullptr) {
					Badge::Builder builder(getComponentRegistry(), *_pill);
					_badge = builder.withVariant(Badge::Variant::Large)
								 .withLabel(badge)
								 .registerEntity(_pill);
				} else if (_badge != nullptr) {
					_badge->setVariant(badge.empty() ? Badge::Variant::Dot
													 : Badge::Variant::Large);
					_badge->setLabel(badge);
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
			 * @brief Apply the selection colors, glyph and semantics.
			 * @param selected Whether the destination is selected.
			 */
			void applySelectionColor(bool selected)
			{
				const auto pillColor = selected
					? schemeColor(SchemeColorRole::SecondaryContainer)
					: transparentColor();
				const auto iconColor = selected
					? schemeColor(SchemeColorRole::OnSecondaryContainer)
					: schemeColor(SchemeColorRole::OnSurfaceVariant);
				const auto labelColor = selected
					? schemeColor(SchemeColorRole::OnSurface)
					: schemeColor(SchemeColorRole::OnSurfaceVariant);

				if (_pill != nullptr) {
					_pill->setColor(stateLayerColor(getComponentRegistry(),
													getIdentifier(), pillColor,
													iconColor));
				}

				if (_icon != nullptr) {
					_icon->setGlyphName(selected && !_spec.focusedIcon.empty()
											? _spec.focusedIcon
											: _spec.icon);
					_icon->setColor(iconColor);
				}

				if (_labelEntity != nullptr) {
					_labelEntity->setColor(_showLabel ? labelColor
													  : transparentColor());
				}

				getComponentRegistry()
					.getComponent<components::Accessibility>(getIdentifier())
					.setSelected(selected);
			}

			/**
			 * @brief Arrange the destination content.
			 */
			void applyGeometry(void)
			{
				std::vector<ecs::Entity::Identifier> children;

				if (_pill != nullptr) {
					children.push_back(_pill->getIdentifier());
				}

				if (_labelEntity != nullptr) {
					children.push_back(_labelEntity->getIdentifier());
				}

				systems::Layout::apply(getComponentRegistry(), getIdentifier(),
									   children,
									   static_cast<std::uint32_t>(getLayer()));

				if (_badge != nullptr && _pill != nullptr) {
					placeChild(getComponentRegistry(), _pill->getIdentifier(),
							   _badge->getIdentifier(),
							   utility::graphic::PositionF(BadgeOffsetX,
														   BadgeOffsetY, 0.0f),
							   static_cast<std::uint32_t>(_pill->getLayer()));
				}
			}

			/**
			 * @brief Get the indicator identifier.
			 * @return The identifier, or InvalidIdentifier.
			 */
			ecs::Entity::Identifier getIndicatorIdentifier(void) const
			{
				return _pill != nullptr ? _pill->getIdentifier()
										: ecs::Entity::InvalidIdentifier;
			}

			/**
			 * @brief Get the icon identifier.
			 * @return The identifier, or InvalidIdentifier.
			 */
			ecs::Entity::Identifier getIconIdentifier(void) const
			{
				return _icon != nullptr ? _icon->getIdentifier()
										: ecs::Entity::InvalidIdentifier;
			}

			/**
			 * @brief Get the label identifier.
			 * @return The identifier, or InvalidIdentifier.
			 */
			ecs::Entity::Identifier getLabelIdentifier(void) const
			{
				return _labelEntity != nullptr ? _labelEntity->getIdentifier()
											   : ecs::Entity::InvalidIdentifier;
			}

			/**
			 * @brief Get the badge identifier.
			 * @return The identifier, or InvalidIdentifier.
			 */
			ecs::Entity::Identifier getBadgeIdentifier(void) const
			{
				return _badge != nullptr ? _badge->getIdentifier()
										 : ecs::Entity::InvalidIdentifier;
			}
		};

		std::vector<NavigationBar::Destination> toDestinations(
			const std::vector<std::pair<std::string, std::string>> &pairs)
		{
			std::vector<NavigationBar::Destination> destinations;
			destinations.reserve(pairs.size());
			for (const auto &pair: pairs) {
				destinations.push_back(
					NavigationBar::Destination { pair.first, pair.second, "",
												 "" });
			}
			return destinations;
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
		auto entity = std::make_shared<NavigationBar>(
			this->getComponentRegistry(), _config, _variant, _specs,
			_onIndexChange);
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
		: NavigationBar(registry, config, variant, toDestinations(destinations),
						{})
	{
	}

	NavigationBar::NavigationBar(ecs::ComponentRegistry &registry,
								 const SurfaceConfig &config, Variant variant,
								 const std::vector<Destination> &destinations,
								 std::function<void(std::size_t)> onIndexChange)
		: SurfaceBase<components::SelectionGroup>(registry, config)
		, _variant(variant)
		, _destinations(destinations)
		, _destinationEntities()
		, _onIndexChange(std::move(onIndexChange))
		, _onTabPress()
		, _updatingSelection(false)
	{
		getComponentRegistry()
			.getComponent<components::SelectionGroup>(getIdentifier())
			.setAllowEmpty(false);

		if (!getComponentRegistry().hasComponent<components::Accessibility>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Accessibility>(
				getIdentifier());
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::TabList)
			.setFocusable(false);

		setColor(schemeColor(SchemeColorRole::SurfaceContainer));
	}

	NavigationBar::~NavigationBar(void)
	{
	}

	void NavigationBar::buildDestinations(void)
	{
		_destinationEntities.clear();

		const auto &bound =
			getComponentRegistry().getComponent<components::Bound>(
				getIdentifier());
		const float padding = getSurfaceConfig().padding;
		const float width	= getSurfaceConfig().hasFixedWidth
			  ? getSurfaceConfig().fixedWidth
			  : bound.getWidth();
		const float destinationWidth = _destinations.empty()
			? 0.0f
			: (width - 2.0f * padding)
				/ static_cast<float>(_destinations.size());

		for (std::size_t i = 0; i < _destinations.size(); ++i) {
			auto entity = std::make_shared<BarDestination>(getComponentRegistry(),
														   _destinations[i]);
			entity->setParent(shared_from_this());
			this->addEntity(entity);
			getComponentRegistry()
				.getComponent<components::Layout>(entity->getIdentifier())
				.setFixedWidth(destinationWidth)
				.setFixedHeight(NavigationBarHeight - 2.0f * padding);
			entity->setOnPress([this, i]() {
				this->pressDestination(i);
			});
			_destinationEntities.push_back(entity);
		}

		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity = std::dynamic_pointer_cast<BarDestination>(
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
		setColor(schemeColor(SchemeColorRole::SurfaceContainer));

		const bool showAllLabels = (_variant == Variant::WithLabels);

		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity = std::dynamic_pointer_cast<BarDestination>(
				_destinationEntities[i]);
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

	void NavigationBar::pressDestination(std::size_t index)
	{
		if (index >= _destinationEntities.size()) {
			return;
		}

		if (_onTabPress) {
			_onTabPress(index);
		}

		const bool changed = getSelectedIndex() != index;
		select(index);

		if (changed && _onIndexChange) {
			_onIndexChange(index);
		}
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
		return addDestination(Destination { iconGlyph, label, "", "" });
	}

	NavigationBar &NavigationBar::addDestination(const Destination &destination)
	{
		_destinations.push_back(destination);
		buildDestinations();
		applySelection();
		return *this;
	}

	NavigationBar &NavigationBar::setBadge(std::size_t index,
										   const std::string &badge)
	{
		if (index >= _destinations.size()) {
			return *this;
		}
		_destinations[index].badge = badge;
		if (index < _destinationEntities.size()) {
			auto entity = std::dynamic_pointer_cast<BarDestination>(
				_destinationEntities[index]);
			if (entity != nullptr) {
				entity->setBadge(badge);
			}
		}
		return *this;
	}

	NavigationBar &NavigationBar::setOnIndexChange(
		std::function<void(std::size_t)> onIndexChange)
	{
		_onIndexChange = std::move(onIndexChange);
		return *this;
	}

	NavigationBar &
		NavigationBar::setOnTabPress(std::function<void(std::size_t)> onTabPress)
	{
		_onTabPress = std::move(onTabPress);
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
			auto entity = std::dynamic_pointer_cast<BarDestination>(
				_destinationEntities[i]);
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

	namespace
	{
		template<typename Getter>
		ecs::Entity::Identifier destinationPart(
			const std::vector<std::shared_ptr<ecs::Entity>> &destinations,
			std::size_t index, Getter getter)
		{
			if (index >= destinations.size()) {
				return ecs::Entity::InvalidIdentifier;
			}
			const auto entity =
				std::dynamic_pointer_cast<BarDestination>(destinations[index]);
			return entity != nullptr ? getter(*entity)
									 : ecs::Entity::InvalidIdentifier;
		}
	}	 // namespace

	ecs::Entity::Identifier
		NavigationBar::getIndicatorIdentifier(std::size_t index) const
	{
		return destinationPart(_destinationEntities, index,
							   [](const BarDestination &destination) {
								   return destination.getIndicatorIdentifier();
							   });
	}

	ecs::Entity::Identifier
		NavigationBar::getIconIdentifier(std::size_t index) const
	{
		return destinationPart(_destinationEntities, index,
							   [](const BarDestination &destination) {
								   return destination.getIconIdentifier();
							   });
	}

	ecs::Entity::Identifier
		NavigationBar::getLabelIdentifier(std::size_t index) const
	{
		return destinationPart(_destinationEntities, index,
							   [](const BarDestination &destination) {
								   return destination.getLabelIdentifier();
							   });
	}

	ecs::Entity::Identifier
		NavigationBar::getBadgeIdentifier(std::size_t index) const
	{
		return destinationPart(_destinationEntities, index,
							   [](const BarDestination &destination) {
								   return destination.getBadgeIdentifier();
							   });
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
			auto destination = std::dynamic_pointer_cast<BarDestination>(entity);
			if (destination != nullptr) {
				destination->applyGeometry();
			}
		}

		applyVariant();
	}

}	 // namespace guillaume::entities
