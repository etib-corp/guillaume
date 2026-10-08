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

#include "guillaume/entities/menu.hpp"
#include "guillaume/entities/overlay_helpers.hpp"

#include <memory>
#include <vector>

#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/entities/overlay_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		constexpr float MenuItemHeight = 48.0f;
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
		_config.borderRadius = 4.0f;
		_config.setFixedWidth(200.0f);
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

	std::shared_ptr<Menu> Menu::Director::makeMenu(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::vector<Item> &items)
	{
		builder.withVariant(variant);

		for (const auto &item: items) {
			builder.addItem(item.label, item.leadingIcon);
		}

		return builder.registerEntity(parent);
	}

	Menu::Menu(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::vector<Item> &items)
		: SurfaceBase<components::Overlay>(registry, config)
		, _variant(variant)
		, _items(items)
		, _rows()
		, _rowsDirty(true)
	{
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
		config.padding		 = 0.0f;
		config.spacing		 = 0.0f;
		config.borderRadius	 = 4.0f;
		config.hasFixedWidth = true;
		config.fixedWidth	 = 200.0f;

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::SurfaceContainer));
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

	std::shared_ptr<Layout> Menu::buildRow(const Item &item)
	{
		Layout::Builder builder(this->getComponentRegistry(), *this);
		Layout::Director director;

		auto row = director.makeDefaultLayout(builder, this->shared_from_this(),
											  utility::graphic::PoseF(), {});

		if (row == nullptr) {
			return nullptr;
		}

		row->setColor(transparentColor());
		row->setAxis(components::Layout::Axis::Horizontal);
		row->setCrossAxisAlignment(
			components::Layout::CrossAxisAlignment::Center);
		row->setSpacing(12.0f);
		row->setPadding(12.0f);
		row->setFixedHeight(MenuItemHeight);

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (!item.leadingIcon.empty()) {
			auto icon = buildIcon(
				this->getComponentRegistry(), *this, row, item.leadingIcon,
				20.0f, schemeColor(SchemeColorRole::OnSurfaceVariant));

			if (icon != nullptr) {
				children.push_back(icon);
			}
		}

		if (!item.label.empty()) {
			auto label =
				buildText(this->getComponentRegistry(), *this, row, item.label,
						  14.0f, schemeColor(SchemeColorRole::OnSurface));

			if (label != nullptr) {
				children.push_back(label);
			}
		}

		row->setEntities(children);

		return row;
	}

	void Menu::buildItems(void)
	{
		if (_rowsDirty) {
			_rows.clear();

			for (const auto &item: _items) {
				auto row = buildRow(item);

				if (row != nullptr) {
					_rows.push_back(row);
				}
			}

			_rowsDirty = false;
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;
		children.reserve(_rows.size());

		for (const auto &row: _rows) {
			children.push_back(row);
		}

		setChildren(children);
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
		if (index >= _rows.size() || _rows[index] == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}

		return _rows[index]->getIdentifier();
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
	}

	void Menu::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		applyOverlay();
		buildItems();
	}

}	 // namespace guillaume::entities
