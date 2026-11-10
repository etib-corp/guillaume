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

#include <algorithm>
#include <memory>

#include "guillaume/entities/menu.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief M3 menu item height.
		 */
		constexpr float MenuItemHeight = 48.0f;

		/**
		 * @brief Paper dense menu item height.
		 */
		constexpr float DenseMenuItemHeight = 32.0f;

		/**
		 * @brief M3 menu item horizontal padding.
		 */
		constexpr float MenuItemPadding = theme::SpacingScale::get(3);

		/**
		 * @brief M3 gap between the icons and the label.
		 */
		constexpr float MenuItemSpacing = theme::SpacingScale::get(3);

		/**
		 * @brief M3 menu item icon size.
		 */
		constexpr float MenuIconSize = 24.0f;

		/**
		 * @brief Gap between the anchor and the menu.
		 */
		constexpr float AnchorGap = theme::SpacingScale::get(1);
	}	 // namespace

	Menu::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<Menu>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<Menu> Menu::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<Menu>(this->getComponentRegistry(),
											 _config, _variant, _items);
		return entity;
	}

	void Menu::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::Dropdown;
		_items.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding		 = 0.0f;
		_config.spacing		 = 0.0f;
		_config.borderRadius = getShapeRadius(theme::ShapeRole::ExtraSmall);
		_config.setFixedWidth(MinWidth);
	}

	Menu::Builder &Menu::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	Menu::Builder &Menu::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	Menu::Builder &Menu::Builder::addItem(const std::string &label,
										  const std::string &leadingIcon)
	{
		Item item;
		item.label		 = label;
		item.leadingIcon = leadingIcon;
		_items.push_back(item);
		return *this;
	}

	Menu::Builder &Menu::Builder::addItem(const Item &item)
	{
		_items.push_back(item);
		return *this;
	}

	std::shared_ptr<Menu> Menu::Director::makeMenu(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::vector<Item> &items)
	{
		builder.withVariant(variant);

		for (const auto &item: items) {
			builder.addItem(item);
		}

		return builder.registerEntity(parent);
	}

	Menu::Menu(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::vector<Item> &items)
		: SurfaceBase<components::Overlay, components::Elevation>(registry,
																  config)
		, _variant(variant)
		, _items(items)
		, _rows()
		, _rowsDirty(true)
	{
		if (!getComponentRegistry().hasComponent<components::Accessibility>(
				getIdentifier())) {
			getComponentRegistry().addComponent<components::Accessibility>(
				getIdentifier());
		}
		getComponentRegistry()
			.getComponent<components::Accessibility>(getIdentifier())
			.setRole(components::AccessibilityRole::Menu)
			.setFocusable(false);

		applyVariant();
		applyOverlay();
	}

	Menu::~Menu(void)
	{
	}

	void Menu::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Vertical;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		config.padding		= 0.0f;
		config.spacing		= 0.0f;
		config.borderRadius = getShapeRadius(theme::ShapeRole::ExtraSmall);

		setSurfaceConfig(config);

		const auto level = _mode == Mode::Elevated
			? theme::ElevationLevel::Level2
			: theme::ElevationLevel::Level0;
		getComponentRegistry()
			.getComponent<components::Elevation>(getIdentifier())
			.setElevationLevel(level);
		setColor(_mode == Mode::Elevated
					 ? getElevatedSurfaceColor(theme::ElevationLevel::Level2)
					 : schemeColor(SchemeColorRole::SurfaceContainer));
	}

	void Menu::applyOverlay(void)
	{
		auto &overlay =
			this->getComponentRegistry().getComponent<components::Overlay>(
				this->getIdentifier());

		overlay.setModal(false);
		overlay.setDismissOnOutsideClick(true);
		overlay.setDismissOnEscape(true);
	}

	Menu::Row Menu::buildRow(std::size_t index)
	{
		const Item &item = _items[index];
		Row row;

		View::Builder builder(this->getComponentRegistry(), *this);
		View::Director director;

		row.view = director.makeDefaultView(builder, this->shared_from_this(),
											utility::graphic::PoseF(), {});

		if (row.view == nullptr) {
			return row;
		}

		row.view->setColor(transparentColor());
		row.view->setAxis(components::Layout::Axis::Horizontal);
		row.view->setCrossAxisAlignment(
			components::Layout::CrossAxisAlignment::Center);
		row.view->setSpacing(MenuItemSpacing);
		row.view->setPadding(MenuItemPadding);
		row.view->setFixedHeight(item.dense ? DenseMenuItemHeight
											: MenuItemHeight);

		if (!item.leadingIcon.empty()) {
			auto icon = buildIcon(
				this->getComponentRegistry(), *this, row.view, item.leadingIcon,
				MenuIconSize, schemeColor(SchemeColorRole::OnSurfaceVariant));

			if (icon != nullptr) {
				row.content.push_back(icon);
				row.icons.push_back(icon);
			}
		}

		if (!item.label.empty()) {
			row.label =
				buildText(this->getComponentRegistry(), *this, row.view,
						  item.label, theme::TypeRole::BodyLarge,
						  schemeColor(SchemeColorRole::OnSurface));

			if (row.label != nullptr) {
				row.content.push_back(row.label);
			}
		}

		if (!item.trailingIcon.empty()) {
			auto icon =
				buildIcon(this->getComponentRegistry(), *this, row.view,
						  item.trailingIcon, MenuIconSize,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));

			if (icon != nullptr) {
				row.content.push_back(icon);
				row.icons.push_back(icon);
			}
		}

		row.view->setEntities(row.content);

		const auto rowIdentifier = row.view->getIdentifier();
		makeInteractive(
			this->getComponentRegistry(), rowIdentifier,
			components::AccessibilityRole::MenuItem,
			[this, index]() {
				if (index < _items.size() && _items[index].onPress) {
					_items[index].onPress();
				}
			},
			[this, index]() {
				styleRow(index);
			});
		this->getComponentRegistry()
			.getComponent<components::Accessibility>(rowIdentifier)
			.setLabel(item.label);
		setEnabled(this->getComponentRegistry(), rowIdentifier, !item.disabled);

		return row;
	}

	void Menu::styleRow(std::size_t index)
	{
		if (index >= _rows.size() || _rows[index].view == nullptr) {
			return;
		}

		auto &row			= _rows[index];
		const auto identity = row.view->getIdentifier();
		const bool enabled	= isEnabled(this->getComponentRegistry(), identity);

		const auto labelColor = enabled
			? schemeColor(SchemeColorRole::OnSurface)
			: disabledContentColor();
		const auto iconColor = enabled
			? schemeColor(SchemeColorRole::OnSurfaceVariant)
			: disabledContentColor();

		row.view->setColor(stateLayerColor(this->getComponentRegistry(),
										   identity, transparentColor(),
										   schemeColor(SchemeColorRole::OnSurface)));
		if (row.label != nullptr) {
			row.label->setColor(labelColor);
		}
		for (const auto &icon: row.icons) {
			if (auto glyph = std::dynamic_pointer_cast<Icon>(icon)) {
				glyph->setColor(iconColor);
			}
		}
	}

	void Menu::buildItems(void)
	{
		if (_rowsDirty) {
			_rows.clear();

			for (std::size_t i = 0; i < _items.size(); ++i) {
				auto row = buildRow(i);

				if (row.view != nullptr) {
					_rows.push_back(row);
				}
			}

			_rowsDirty = false;
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;
		children.reserve(_rows.size());

		for (std::size_t i = 0; i < _rows.size(); ++i) {
			children.push_back(_rows[i].view);
			styleRow(i);
		}

		setChildren(children);
	}

	void Menu::applyWidth(void)
	{
		float contentWidth = 0.0f;
		for (const auto &row: _rows) {
			float width = 2.0f * MenuItemPadding;
			std::size_t count = 0;
			for (const auto &child: row.content) {
				if (child == nullptr
					|| !this->getComponentRegistry()
							.hasComponent<components::Bound>(
								child->getIdentifier())) {
					continue;
				}
				width += this->getComponentRegistry()
							 .getComponent<components::Bound>(
								 child->getIdentifier())
							 .getWidth();
				++count;
			}
			if (count > 1) {
				width += MenuItemSpacing * static_cast<float>(count - 1);
			}
			contentWidth = std::max(contentWidth, width);
		}

		const float menuWidth = std::clamp(contentWidth, MinWidth, MaxWidth);

		auto config = getSurfaceConfig();
		if (!config.hasFixedWidth || config.fixedWidth != menuWidth) {
			config.hasFixedWidth = true;
			config.fixedWidth	 = menuWidth;
			setSurfaceConfig(config);
		}

		for (const auto &row: _rows) {
			if (row.view != nullptr) {
				row.view->setFixedWidth(menuWidth);
			}
		}
	}

	void Menu::applyAnchor(void)
	{
		auto &registry = this->getComponentRegistry();
		if (_anchor == ecs::Entity::InvalidIdentifier
			|| !registry.hasComponent<components::Transform>(_anchor)
			|| !registry.hasComponent<components::Bound>(_anchor)) {
			return;
		}

		const auto anchorPose =
			registry.getComponent<components::Transform>(_anchor).getPose();
		const auto &anchorBound = registry.getComponent<components::Bound>(_anchor);
		const auto &bound =
			registry.getComponent<components::Bound>(this->getIdentifier());

		const float offsetY = _anchorPosition == AnchorPosition::Bottom
			? anchorBound.getHeight() + AnchorGap
			: -(bound.getHeight() + AnchorGap);

		const utility::graphic::PositionF position(
			anchorPose.getPosition().getX(),
			anchorPose.getPosition().getY() + offsetY,
			anchorPose.getPosition().getZ());
		setPose(utility::graphic::PoseF(position, anchorPose.getOrientation()));
	}

	Menu &Menu::setVariant(Variant variant)
	{
		_variant   = variant;
		_rowsDirty = true;
		applyVariant();
		return *this;
	}

	Menu::Variant Menu::getVariant(void) const
	{
		return _variant;
	}

	Menu &Menu::setMode(Mode mode)
	{
		_mode = mode;
		applyVariant();
		return *this;
	}

	Menu::Mode Menu::getMode(void) const
	{
		return _mode;
	}

	Menu &Menu::setItems(const std::vector<Item> &items)
	{
		_items	   = items;
		_rowsDirty = true;
		return *this;
	}

	std::size_t Menu::getItemCount(void) const
	{
		return _items.size();
	}

	ecs::Entity::Identifier Menu::getItemIdentifier(std::size_t index) const
	{
		if (index >= _rows.size() || _rows[index].view == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _rows[index].view->getIdentifier();
	}

	Menu &Menu::setItemDisabled(std::size_t index, bool disabled)
	{
		if (index >= _items.size()) {
			return *this;
		}
		_items[index].disabled = disabled;
		if (index < _rows.size() && _rows[index].view != nullptr) {
			setEnabled(this->getComponentRegistry(),
					   _rows[index].view->getIdentifier(), !disabled);
			styleRow(index);
		}
		return *this;
	}

	Menu &Menu::setAnchor(ecs::Entity::Identifier anchor)
	{
		_anchor = anchor;
		applyAnchor();
		return *this;
	}

	ecs::Entity::Identifier Menu::getAnchor(void) const
	{
		return _anchor;
	}

	Menu &Menu::setAnchorPosition(AnchorPosition position)
	{
		_anchorPosition = position;
		applyAnchor();
		return *this;
	}

	Menu::AnchorPosition Menu::getAnchorPosition(void) const
	{
		return _anchorPosition;
	}

	Menu &Menu::setOnDismiss(std::function<void(void)> onDismiss)
	{
		this->getComponentRegistry()
			.getComponent<components::Overlay>(this->getIdentifier())
			.setOnDismissHandler(std::move(onDismiss));
		return *this;
	}

	Menu &Menu::dismiss(void)
	{
		if (!isVisible()) {
			return *this;
		}
		hide();
		if (const auto handler =
				this->getComponentRegistry()
					.getComponent<components::Overlay>(this->getIdentifier())
					.getOnDismissHandler()) {
			handler();
		}
		return *this;
	}

	Menu &Menu::open(void)
	{
		return show();
	}

	Menu &Menu::close(void)
	{
		return hide();
	}

	bool Menu::isOpen(void) const
	{
		return isVisible();
	}

	Menu &Menu::show(void)
	{
		applyAnchor();
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  true);
		return *this;
	}

	Menu &Menu::hide(void)
	{
		setOverlayVisible(this->getComponentRegistry(), this->getIdentifier(),
						  false);
		return *this;
	}

	bool Menu::isVisible(void) const
	{
		return isOverlayVisible(this->getComponentRegistry(),
								this->getIdentifier());
	}

	void Menu::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		applyOverlay();
		buildItems();
		applyWidth();
		applyAnchor();
	}

	void Menu::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildItems();
		applyWidth();
		applyAnchor();
	}

}	 // namespace guillaume::entities
