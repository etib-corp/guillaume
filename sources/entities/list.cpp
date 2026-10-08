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

#include <memory>

#include "guillaume/entities/list.hpp"

#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	List::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
						   ecs::EntityRegistry &entityRegistry)
		: EntityBuilderBase<List>(componentRegistry, entityRegistry)
	{
		reset();
	}

	std::shared_ptr<List> List::Builder::buildEntity(void)
	{
		auto entity = std::make_shared<List>(this->getComponentRegistry(),
											 _config, _variant, _items);
		return entity;
	}

	void List::Builder::reset(void)
	{
		_config	 = SurfaceConfig();
		_variant = Variant::SingleLine;
		_items.clear();

		_config.axis = components::Layout::Axis::Vertical;
		_config.mainAxisAlignment =
			components::Layout::MainAxisAlignment::Start;
		_config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		_config.padding = 0.0f;
		_config.spacing = 0.0f;
		_config.setFixedWidth(360.0f);
	}

	List::Builder &List::Builder::withPose(const utility::graphic::PoseF &pose)
	{
		_config.pose = pose;
		return *this;
	}

	List::Builder &List::Builder::withVariant(Variant variant)
	{
		_variant = variant;
		return *this;
	}

	List::Builder &List::Builder::withItems(const std::vector<Item> &items)
	{
		_items = items;
		return *this;
	}

	List::Builder &List::Builder::withFixedWidth(float width)
	{
		_config.hasFixedWidth = true;
		_config.fixedWidth	  = width;
		return *this;
	}

	std::shared_ptr<List> List::Director::makeList(
		Builder &builder, std::shared_ptr<ecs::Entity> parent, Variant variant,
		const std::vector<Item> &items)
	{
		return builder.withVariant(variant).withItems(items).registerEntity(
			parent);
	}

	List::List(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::vector<Item> &items)
		: SurfaceBase<>(registry, config)
		, _variant(variant)
		, _items(items)
		, _rows()
		, _rowsDirty(true)
	{
		applyVariant();
	}

	List::~List(void)
	{
	}

	float List::getVariantHeight(void) const
	{
		switch (_variant) {
			case Variant::TwoLine:
				return 64.0f;
			case Variant::ThreeLine:
				return 88.0f;
			case Variant::SingleLine:
			default:
				return 48.0f;
		}
	}

	void List::applyVariant(void)
	{
		auto config = getSurfaceConfig();

		config.axis				 = components::Layout::Axis::Vertical;
		config.mainAxisAlignment = components::Layout::MainAxisAlignment::Start;
		config.crossAxisAlignment =
			components::Layout::CrossAxisAlignment::Start;
		config.padding		= 0.0f;
		config.spacing		= 0.0f;
		config.borderRadius = 0.0f;

		if (!config.hasFixedWidth) {
			config.hasFixedWidth = true;
			config.fixedWidth	 = 360.0f;
		}

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::Surface));
	}

	std::shared_ptr<Layout> List::buildRow(const Item &item)
	{
		const float rowHeight = getVariantHeight();

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
		row->setSpacing(16.0f);
		row->setPadding(16.0f);
		row->setFixedHeight(rowHeight);

		std::vector<std::shared_ptr<ecs::Entity>> children;

		if (!item.leadingIcon.empty()) {
			auto icon = buildIcon(
				this->getComponentRegistry(), *this, row, item.leadingIcon,
				24.0f, schemeColor(SchemeColorRole::OnSurfaceVariant));

			if (icon != nullptr) {
				children.push_back(icon);
			}
		}

		if (!item.headline.empty()) {
			auto headline = buildText(this->getComponentRegistry(), *this, row,
									  item.headline, 16.0f,
									  schemeColor(SchemeColorRole::OnSurface));

			if (headline != nullptr) {
				children.push_back(headline);
			}
		}

		if (!item.supporting.empty()) {
			auto supporting = buildText(
				this->getComponentRegistry(), *this, row, item.supporting,
				14.0f, schemeColor(SchemeColorRole::OnSurfaceVariant));

			if (supporting != nullptr) {
				children.push_back(supporting);
			}
		}

		if (!item.trailingIcon.empty()) {
			auto icon = buildIcon(
				this->getComponentRegistry(), *this, row, item.trailingIcon,
				24.0f, schemeColor(SchemeColorRole::OnSurfaceVariant));

			if (icon != nullptr) {
				children.push_back(icon);
			}
		}

		row->setEntities(children);

		return row;
	}

	void List::buildItems(void)
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

	List &List::setVariant(Variant variant)
	{
		_variant   = variant;
		_rowsDirty = true;
		applyVariant();
		return *this;
	}

	List::Variant List::getVariant(void) const
	{
		return _variant;
	}

	List &List::setItems(const std::vector<Item> &items)
	{
		_items	   = items;
		_rowsDirty = true;
		return *this;
	}

	std::size_t List::getItemCount(void) const
	{
		return _items.size();
	}

	ecs::Entity::Identifier List::getItemIdentifier(std::size_t index) const
	{
		if (index >= _rows.size() || _rows[index] == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}
		return _rows[index]->getIdentifier();
	}

	void List::initialize(void)
	{
		SurfaceBase::initialize();
		applyVariant();
		buildItems();
	}

	void List::update(void)
	{
		SurfaceBase::update();
		applyVariant();
		buildItems();
	}

}	 // namespace guillaume::entities
