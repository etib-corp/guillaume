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

#include "guillaume/entities/navigation_drawer.hpp"
#include "guillaume/entities/overlay_helpers.hpp"

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
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/systems/selection.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float DrawerDestinationHeight = 56.0f;

		/**
		 * @brief Internal drawer destination: a selectable layout holding an
		 * icon and a label.
		 */
		class DrawerDestination:
			public std::enable_shared_from_this<DrawerDestination>,
			public ecs::ParentEntityFiller<
				components::Layout, components::Transform, components::Bound,
				components::Color, components::Borders, components::Selectable>
		{
			private:
			std::string _iconGlyph {};				  ///< Icon glyph name.
			std::string _label {};					  ///< Label text.
			std::shared_ptr<Icon> _icon {};			  ///< Icon child.
			std::shared_ptr<Text> _labelEntity {};	  ///< Label child.

			public:
			/**
			 * @brief Construct a drawer destination.
			 * @param registry The component registry.
			 * @param iconGlyph The icon glyph name.
			 * @param label The label text.
			 */
			DrawerDestination(ecs::ComponentRegistry &registry,
							  const std::string &iconGlyph,
							  const std::string &label)
				: ecs::ParentEntityFiller<
					  components::Layout, components::Transform,
					  components::Bound, components::Color, components::Borders,
					  components::Selectable>(registry)
				, _iconGlyph(iconGlyph)
				, _label(label)
				, _icon()
				, _labelEntity()
			{
				getComponentRegistry()
					.getComponent<components::Layout>(getIdentifier())
					.setAxis(components::Layout::Axis::Horizontal)
					.setMainAxisAlignment(
						components::Layout::MainAxisAlignment::Start)
					.setCrossAxisAlignment(
						components::Layout::CrossAxisAlignment::Center)
					.setSpacing(12.0f)
					.setPadding(16.0f)
					.setFixedHeight(DrawerDestinationHeight);

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(transparentColor());

				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setBorderRadius(DrawerDestinationHeight / 2.0f)
					.setColor(transparentColor());
			}

			/**
			 * @brief Default destructor.
			 */
			~DrawerDestination(void) override = default;

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
						_label, 14.0f,
						schemeColor(SchemeColorRole::OnSurfaceVariant));
				}

				applyGeometry();
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
					_labelEntity->setColor(color);
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
			this->getComponentRegistry(), _config, _variant, _destinations);
		return entity;
	}

	void NavigationDrawer::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Standard;
		_destinations.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding = 12.0f;
		_config.spacing = 4.0f;
		_config.setFixedWidth(320.0f);
		_config.setFixedHeight(640.0f);
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

	NavigationDrawer::Builder &
		NavigationDrawer::Builder::addDestination(const std::string &iconGlyph,
												  const std::string &label)
	{
		_destinations.emplace_back(iconGlyph, label);
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
		: SurfaceBase<components::Overlay, components::SelectionGroup>(registry,
																	   config)
		, _variant(variant)
		, _destinations(destinations)
		, _destinationEntities()
		, _updatingSelection(false)
	{
		getComponentRegistry()
			.getComponent<components::SelectionGroup>(getIdentifier())
			.setAllowEmpty(false);
	}

	NavigationDrawer::~NavigationDrawer(void)
	{
	}

	void NavigationDrawer::buildDestinations(void)
	{
		_destinationEntities.clear();

		for (const auto &destination: _destinations) {
			auto entity = std::make_shared<DrawerDestination>(
				getComponentRegistry(), destination.first, destination.second);
			entity->setParent(shared_from_this());
			this->addEntity(entity);
			_destinationEntities.push_back(entity);
		}

		for (std::size_t i = 0; i < _destinationEntities.size(); ++i) {
			auto entity = std::dynamic_pointer_cast<DrawerDestination>(
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
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		config.padding		  = 12.0f;
		config.spacing		  = 4.0f;
		config.hasFixedWidth  = true;
		config.fixedWidth	  = 320.0f;
		config.hasFixedHeight = true;
		config.fixedHeight	  = 640.0f;

		setSurfaceConfig(config);

		auto &borders =
			this->getComponentRegistry().getComponent<components::Borders>(
				this->getIdentifier());
		borders.setTopLeftRadius(0.0f)
			.setBottomLeftRadius(0.0f)
			.setTopRightRadius(16.0f)
			.setBottomRightRadius(16.0f);

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

		if (_variant == Variant::Modal) {
			overlay.setModal(true);
			overlay.setDismissOnOutsideClick(true);
		} else {
			overlay.setModal(false);
			overlay.setDismissOnOutsideClick(false);
		}

		overlay.setDismissOnEscape(true);
	}

	void NavigationDrawer::applySelection(void)
	{
		if (_destinationEntities.empty()) {
			return;
		}

		_updatingSelection = true;
		selectEntity(_destinationEntities[0]->getIdentifier());
		_updatingSelection = false;

		applyVariant();
	}

	NavigationDrawer &NavigationDrawer::setVariant(Variant variant)
	{
		_variant = variant;
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

	NavigationDrawer &NavigationDrawer::show(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		return *this;
	}

	NavigationDrawer &NavigationDrawer::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
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
		return _destinations.size();
	}

	ecs::Entity::Identifier
		NavigationDrawer::getDestinationIdentifier(std::size_t index) const
	{
		if (index >= _destinationEntities.size()) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _destinationEntities[index]->getIdentifier();
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
