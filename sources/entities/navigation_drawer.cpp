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
		/**
		 * @brief M3 drawer destination height.
		 */
		constexpr float DrawerDestinationHeight = 56.0f;

		/**
		 * @brief M3 standard/modal drawer width.
		 */
		constexpr float DrawerWidth = 320.0f;

		/**
		 * @brief Collapsed drawer (rail) width.
		 */
		constexpr float CollapsedWidth = 80.0f;

		/**
		 * @brief Collapsed item indicator width.
		 */
		constexpr float CollapsedIndicatorWidth = 56.0f;

		/**
		 * @brief Collapsed item indicator height.
		 */
		constexpr float CollapsedIndicatorHeight = 32.0f;

		/**
		 * @brief Drawer padding.
		 */
		constexpr float DrawerPadding = theme::SpacingScale::get(3);

		/**
		 * @brief Drawer height.
		 */
		constexpr float DrawerHeight = 640.0f;

		/**
		 * @brief Make a transparent nested layout view.
		 */
		std::shared_ptr<View> makeGroup(ecs::ComponentRegistry &registry,
										ecs::EntityRegistry &entityRegistry,
										std::shared_ptr<ecs::Entity> parent,
										components::Layout::Axis axis)
		{
			View::Builder builder(registry, entityRegistry);
			auto view =
				builder.makeDefaultView(parent, utility::graphic::PoseF(), {});
			view->setColor(transparentColor());
			view->setAxis(axis);
			view->setMainAxisAlignment(
				components::Layout::MainAxisAlignment::Center);
			view->setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Center);
			view->setPadding(0.0f);
			view->setSpacing(theme::SpacingScale::get(3));
			return view;
		}

		/**
		 * @brief Internal drawer destination (Paper `Drawer.Item` or
		 * `Drawer.CollapsedItem`).
		 */
		class DrawerDestination:
			public std::enable_shared_from_this<DrawerDestination>,
			public ecs::ParentEntityFiller<
				components::Layout, components::Transform, components::Bound,
				components::Color, components::Borders, components::Selectable>
		{
			private:
			NavigationDrawer::Entry _entry {};	  ///< Destination data.
			bool _collapsed { false };		   ///< Collapsed (rail) rendering.
			std::shared_ptr<View> _lead {};	   ///< Icon+label group / pill.
			std::shared_ptr<Icon> _icon {};	   ///< Icon child.
			std::shared_ptr<Text> _labelEntity {};	  ///< Label child.
			std::shared_ptr<Text> _rightEntity {};	  ///< Trailing text.
			std::function<void(void)> _onPress {};	  ///< Press handler.

			public:
			/**
			 * @brief Construct a drawer destination.
			 * @param registry The component registry.
			 * @param entry The destination data.
			 * @param collapsed Whether to render a collapsed item.
			 * @param width Destination width.
			 */
			DrawerDestination(ecs::ComponentRegistry &registry,
							  const NavigationDrawer::Entry &entry,
							  bool collapsed, float width)
				: ecs::ParentEntityFiller<
					  components::Layout, components::Transform,
					  components::Bound, components::Color, components::Borders,
					  components::Selectable>(registry)
				, _entry(entry)
				, _collapsed(collapsed)
			{
				auto &layout =
					getComponentRegistry().getComponent<components::Layout>(
						getIdentifier());
				if (_collapsed) {
					layout.setAxis(components::Layout::Axis::Vertical)
						.setMainAxisAlignment(
							components::Layout::MainAxisAlignment::Center)
						.setCrossAxisAlignment(
							components::Layout::CrossAxisAlignment::Center)
						.setSpacing(theme::SpacingScale::get(1))
						.setPadding(0.0f)
						.setFixedWidth(width)
						.setFixedHeight(DrawerDestinationHeight
										+ theme::SpacingScale::get(2));
				} else {
					layout.setAxis(components::Layout::Axis::Horizontal)
						.setMainAxisAlignment(
							entry.right.empty()
								? components::Layout::MainAxisAlignment::Start
								: components::Layout::MainAxisAlignment::
									  SpaceBetween)
						.setCrossAxisAlignment(
							components::Layout::CrossAxisAlignment::Center)
						.setSpacing(theme::SpacingScale::get(3))
						.setPadding(theme::SpacingScale::get(4))
						.setFixedWidth(width)
						.setFixedHeight(DrawerDestinationHeight);
				}

				getComponentRegistry()
					.getComponent<components::Color>(getIdentifier())
					.setColor(transparentColor());

				getComponentRegistry()
					.getComponent<components::Borders>(getIdentifier())
					.setBorderRadius(getShapeRadius(theme::ShapeRole::Full))
					.setColor(transparentColor());

				makeInteractive(
					getComponentRegistry(), getIdentifier(),
					components::AccessibilityRole::Button,
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
					.setLabel(_entry.label);
				setEnabled(getComponentRegistry(), getIdentifier(),
						   !_entry.disabled);
			}

			/**
			 * @brief Default destructor.
			 */
			~DrawerDestination(void) override = default;

			/**
			 * @brief Build the icon, label and trailing children.
			 */
			void initialize(void) override
			{
				if (_collapsed) {
					_lead = makeGroup(getComponentRegistry(), *this,
									  shared_from_this(),
									  components::Layout::Axis::Horizontal);
					_lead->setBorderRadius(
						getShapeRadius(theme::ShapeRole::Full));
					_lead->setFixedWidth(CollapsedIndicatorWidth);
					_lead->setFixedHeight(CollapsedIndicatorHeight);
					_icon = buildIcon(
						getComponentRegistry(), *_lead, _lead, _entry.icon,
						24.0f, schemeColor(SchemeColorRole::OnSurfaceVariant));
					if (_icon != nullptr) {
						_lead->setEntities({ _icon });
					}
					if (!_entry.label.empty()) {
						_labelEntity = buildText(
							getComponentRegistry(), *this, shared_from_this(),
							_entry.label, theme::TypeRole::LabelMedium,
							schemeColor(SchemeColorRole::OnSurfaceVariant));
					}
				} else {
					_lead = makeGroup(getComponentRegistry(), *this,
									  shared_from_this(),
									  components::Layout::Axis::Horizontal);
					std::vector<std::shared_ptr<ecs::Entity>> lead;
					_icon = buildIcon(
						getComponentRegistry(), *_lead, _lead, _entry.icon,
						24.0f, schemeColor(SchemeColorRole::OnSurfaceVariant));
					if (_icon != nullptr) {
						lead.push_back(_icon);
					}
					if (!_entry.label.empty()) {
						_labelEntity = buildText(
							getComponentRegistry(), *_lead, _lead, _entry.label,
							theme::TypeRole::LabelLarge,
							schemeColor(SchemeColorRole::OnSurfaceVariant));
						if (_labelEntity != nullptr) {
							lead.push_back(_labelEntity);
						}
					}
					_lead->setEntities(lead);

					if (!_entry.right.empty()) {
						_rightEntity = buildText(
							getComponentRegistry(), *this, shared_from_this(),
							_entry.right, theme::TypeRole::LabelLarge,
							schemeColor(SchemeColorRole::OnSurfaceVariant));
					}
				}

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
			 * @brief Enable or disable the destination.
			 * @param disabled Whether the destination is disabled.
			 */
			void setDisabled(bool disabled)
			{
				_entry.disabled = disabled;
				setEnabled(getComponentRegistry(), getIdentifier(), !disabled);
				applySelectionColor(isSelected());
			}

			/**
			 * @brief Check whether the destination is disabled.
			 * @return True when disabled.
			 */
			bool isDisabled(void) const
			{
				return !isEnabled(getComponentRegistry(), getIdentifier());
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
			 * @brief Apply the active pill, content colors and semantics.
			 * @param selected Whether the destination is selected.
			 */
			void applySelectionColor(bool selected)
			{
				const bool disabled = isDisabled();
				const auto pill		= selected && !disabled
						? schemeColor(SchemeColorRole::SecondaryContainer)
						: transparentColor();
				auto content		= selected
						   ? schemeColor(SchemeColorRole::OnSecondaryContainer)
						   : schemeColor(SchemeColorRole::OnSurfaceVariant);
				if (disabled) {
					content = disabledContentColor();
				}

				const auto pillColor = stateLayerColor(
					getComponentRegistry(), getIdentifier(), pill, content);
				if (_collapsed) {
					if (_lead != nullptr) {
						_lead->setColor(pillColor);
					}
				} else {
					getComponentRegistry()
						.getComponent<components::Color>(getIdentifier())
						.setColor(pillColor);
				}

				if (_icon != nullptr) {
					_icon->setColor(content);
				}
				if (_labelEntity != nullptr) {
					_labelEntity->setColor(content);
				}
				if (_rightEntity != nullptr) {
					_rightEntity->setColor(content);
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

				if (_lead != nullptr) {
					children.push_back(_lead->getIdentifier());
				}

				if (_collapsed && _labelEntity != nullptr) {
					children.push_back(_labelEntity->getIdentifier());
				}

				if (_rightEntity != nullptr) {
					children.push_back(_rightEntity->getIdentifier());
				}

				systems::Layout::apply(getComponentRegistry(), getIdentifier(),
									   children,
									   static_cast<std::uint32_t>(getLayer()));
			}

			/**
			 * @brief Get the trailing text identifier.
			 * @return The identifier, or InvalidIdentifier.
			 */
			ecs::Entity::Identifier getRightIdentifier(void) const
			{
				return _rightEntity != nullptr ? _rightEntity->getIdentifier()
											   : ecs::Entity::InvalidIdentifier;
			}
		};

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
					auto divider =
						builder.makeDivider(shared_from_this(), itemWidth,
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
