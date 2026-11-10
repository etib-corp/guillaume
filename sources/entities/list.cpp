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
#include <utility>

#include "guillaume/entities/list.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/divider.hpp"
#include "guillaume/entities/image.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		/**
		 * @brief Default list width.
		 */
		constexpr float ListWidth = 360.0f;

		/**
		 * @brief M3 list item padding (16 dp on every side).
		 */
		constexpr float RowPadding = theme::SpacingScale::get(4);

		/**
		 * @brief M3 gap between the leading slot, content and trailing slot.
		 */
		constexpr float RowSpacing = theme::SpacingScale::get(4);

		/**
		 * @brief Inset of accordion children (Paper aligns them with the
		 * parent title).
		 */
		constexpr float AccordionInset = 40.0f;

		/**
		 * @brief M3 list icon size.
		 */
		constexpr float ListIconSize = 24.0f;

		std::shared_ptr<View> makeView(ecs::ComponentRegistry &registry,
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
				components::Layout::MainAxisAlignment::Start);
			view->setCrossAxisAlignment(
				axis == components::Layout::Axis::Vertical
					? components::Layout::CrossAxisAlignment::Start
					: components::Layout::CrossAxisAlignment::Center);
			view->setPadding(0.0f);
			view->setSpacing(0.0f);
			return view;
		}
	}	 // namespace

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
		_config.setFixedWidth(ListWidth);
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

	std::shared_ptr<List>
		List::Builder::makeList(std::shared_ptr<ecs::Entity> parent,
								Variant variant, const std::vector<Item> &items)
	{
		return withVariant(variant).withItems(items).registerEntity(parent);
	}

	List::Item List::subheader(const std::string &title)
	{
		Item item;
		item.kind	  = ItemKind::Subheader;
		item.headline = title;
		return item;
	}

	List::Item List::divider(void)
	{
		Item item;
		item.kind = ItemKind::Divider;
		return item;
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

	float List::getListWidth(void) const
	{
		const auto &config = getSurfaceConfig();
		return config.hasFixedWidth ? config.fixedWidth : ListWidth;
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
		config.borderRadius = getShapeRadius(theme::ShapeRole::None);

		if (!config.hasFixedWidth) {
			config.hasFixedWidth = true;
			config.fixedWidth	 = ListWidth;
		}

		setSurfaceConfig(config);
		setColor(schemeColor(SchemeColorRole::Surface));
	}

	List::Row List::buildRow(const Item &item, std::vector<std::size_t> path,
							 float inset)
	{
		auto &registry = this->getComponentRegistry();
		Row row;

		if (item.kind == ItemKind::Subheader) {
			auto header =
				buildText(registry, *this, this->shared_from_this(),
						  item.headline, theme::TypeRole::TitleSmall,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (header != nullptr) {
				registry.addComponent<components::Accessibility>(
					header->getIdentifier());
				registry
					.getComponent<components::Accessibility>(
						header->getIdentifier())
					.setRole(components::AccessibilityRole::Header)
					.setLabel(item.headline)
					.setFocusable(false);
			}
			row.entity = header;
			return row;
		}

		if (item.kind == ItemKind::Divider) {
			Divider::Builder builder(registry, *this);
			row.entity =
				builder.makeDivider(this->shared_from_this(), getListWidth(),
									Divider::Variant::FullWidth);
			return row;
		}

		row.view   = makeView(registry, *this, this->shared_from_this(),
							  components::Layout::Axis::Horizontal);
		row.entity = row.view;
		row.view->setPadding(RowPadding);
		row.view->setSpacing(RowSpacing);
		row.view->setFixedWidth(getListWidth());

		auto lead = makeView(registry, *row.view, row.view,
							 components::Layout::Axis::Horizontal);
		lead->setSpacing(RowSpacing);

		std::vector<std::shared_ptr<ecs::Entity>> leadChildren;

		if (inset > 0.0f) {
			auto spacer = makeView(registry, *lead, lead,
								   components::Layout::Axis::Horizontal);
			spacer->setFixedWidth(inset);
			spacer->setFixedHeight(1.0f);
			leadChildren.push_back(spacer);
		}

		if (!item.leadingImage.empty()) {
			Image::Builder builder(registry, *lead);
			auto image = builder.makeImage(lead, item.leadingImage);
			if (image != nullptr) {
				leadChildren.push_back(image);
			}
		} else if (!item.leadingIcon.empty()) {
			auto icon =
				buildIcon(registry, *lead, lead, item.leadingIcon, ListIconSize,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (icon != nullptr) {
				leadChildren.push_back(icon);
				row.icons.push_back(icon);
			}
		}

		row.content =
			makeView(registry, *lead, lead, components::Layout::Axis::Vertical);
		std::vector<std::shared_ptr<ecs::Entity>> contentChildren;

		if (!item.headline.empty()) {
			row.headline = buildText(registry, *row.content, row.content,
									 item.headline, theme::TypeRole::BodyLarge,
									 schemeColor(SchemeColorRole::OnSurface));
			if (row.headline != nullptr) {
				contentChildren.push_back(row.headline);
			}
		}

		if (!item.supporting.empty() && _variant != Variant::SingleLine) {
			row.supporting =
				buildText(registry, *row.content, row.content, item.supporting,
						  theme::TypeRole::BodyMedium,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (row.supporting != nullptr) {
				contentChildren.push_back(row.supporting);
			}
		}

		row.content->setEntities(contentChildren);
		leadChildren.push_back(row.content);
		lead->setEntities(leadChildren);

		std::vector<std::shared_ptr<ecs::Entity>> rowChildren { lead };

		const bool accordion	   = !item.children.empty();
		const std::string trailing = accordion
			? std::string(item.expanded ? "expand_less" : "expand_more")
			: item.trailingIcon;

		if (!trailing.empty()) {
			auto icon =
				buildIcon(registry, *row.view, row.view, trailing, ListIconSize,
						  schemeColor(SchemeColorRole::OnSurfaceVariant));
			if (icon != nullptr) {
				rowChildren.push_back(icon);
				row.icons.push_back(icon);
				if (accordion) {
					row.chevron = icon;
				}
			}
			row.view->setMainAxisAlignment(
				components::Layout::MainAxisAlignment::SpaceBetween);
		}

		row.view->setEntities(rowChildren);

		const auto identifier = row.view->getIdentifier();
		makeInteractive(
			registry, identifier, components::AccessibilityRole::ListItem,
			[this, path]() {
				pressRow(path);
			},
			[this, path]() {
				if (Row *target = findRow(path)) {
					styleRow(*target);
				}
			});
		auto &accessibility =
			registry.getComponent<components::Accessibility>(identifier);
		accessibility.setLabel(item.headline).setHint(item.supporting);
		if (accordion) {
			accessibility.setExpanded(item.expanded);
		}
		setEnabled(registry, identifier, !item.disabled);

		for (std::size_t i = 0; i < item.children.size(); ++i) {
			auto childPath = path;
			childPath.push_back(i);
			row.children.push_back(
				buildRow(item.children[i], childPath, inset + AccordionInset));
		}

		return row;
	}

	void List::styleRow(Row &row)
	{
		if (row.view == nullptr) {
			return;
		}

		auto &registry		  = this->getComponentRegistry();
		const auto identifier = row.view->getIdentifier();
		const bool enabled	  = isEnabled(registry, identifier);

		auto headlineColor	 = enabled ? schemeColor(SchemeColorRole::OnSurface)
									   : disabledContentColor();
		auto supportingColor = enabled
			? schemeColor(SchemeColorRole::OnSurfaceVariant)
			: disabledContentColor();
		auto iconColor		 = supportingColor;
		auto rowColor =
			stateLayerColor(registry, identifier, transparentColor(),
							schemeColor(SchemeColorRole::OnSurface));

		if (row.hidden) {
			headlineColor	= transparentColor();
			supportingColor = transparentColor();
			iconColor		= transparentColor();
			rowColor		= transparentColor();
		}

		row.view->setColor(rowColor);
		if (row.headline != nullptr) {
			row.headline->setColor(headlineColor);
		}
		if (row.supporting != nullptr) {
			row.supporting->setColor(supportingColor);
		}
		for (const auto &icon: row.icons) {
			icon->setColor(iconColor);
		}
	}

	List::Row *List::findRow(const std::vector<std::size_t> &path)
	{
		if (path.empty() || path[0] >= _rows.size()) {
			return nullptr;
		}
		Row *row = &_rows[path[0]];
		for (std::size_t i = 1; i < path.size(); ++i) {
			if (path[i] >= row->children.size()) {
				return nullptr;
			}
			row = &row->children[path[i]];
		}
		return row;
	}

	List::Item *List::findItem(const std::vector<std::size_t> &path)
	{
		if (path.empty() || path[0] >= _items.size()) {
			return nullptr;
		}
		Item *item = &_items[path[0]];
		for (std::size_t i = 1; i < path.size(); ++i) {
			if (path[i] >= item->children.size()) {
				return nullptr;
			}
			item = &item->children[path[i]];
		}
		return item;
	}

	void List::pressRow(const std::vector<std::size_t> &path)
	{
		Item *item = findItem(path);
		Row *row   = findRow(path);
		if (item == nullptr || row == nullptr || row->hidden) {
			return;
		}

		if (!item->children.empty() && path.size() == 1) {
			setExpanded(path[0], !item->expanded);
		}

		if (item->onPress) {
			item->onPress();
		}
	}

	void List::buildItems(void)
	{
		if (_rowsDirty) {
			_rows.clear();

			for (std::size_t i = 0; i < _items.size(); ++i) {
				_rows.push_back(buildRow(_items[i], { i }, 0.0f));
			}

			_rowsDirty = false;
		}

		std::vector<std::shared_ptr<ecs::Entity>> children;
		children.reserve(_rows.size());

		for (std::size_t i = 0; i < _rows.size(); ++i) {
			auto &row = _rows[i];
			if (row.entity != nullptr) {
				children.push_back(row.entity);
			}
			styleRow(row);

			const bool expanded = _items[i].expanded;
			for (auto &child: row.children) {
				child.hidden = !expanded;
				if (child.view != nullptr) {
					setEnabled(this->getComponentRegistry(),
							   child.view->getIdentifier(),
							   expanded && !_items[i].children.empty());
				}
				styleRow(child);
				if (expanded && child.entity != nullptr) {
					children.push_back(child.entity);
				}
			}
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
		if (index >= _rows.size() || _rows[index].entity == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}
		return _rows[index].entity->getIdentifier();
	}

	ecs::Entity::Identifier
		List::getChildIdentifier(std::size_t index,
								 std::size_t childIndex) const
	{
		if (index >= _rows.size() || childIndex >= _rows[index].children.size()
			|| _rows[index].children[childIndex].entity == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}
		return _rows[index].children[childIndex].entity->getIdentifier();
	}

	ecs::Entity::Identifier List::getContentIdentifier(std::size_t index) const
	{
		if (index >= _rows.size() || _rows[index].content == nullptr) {
			return ecs::Entity::InvalidIdentifier;
		}
		return _rows[index].content->getIdentifier();
	}

	List &List::setItemDisabled(std::size_t index, bool disabled)
	{
		if (index >= _items.size()) {
			return *this;
		}
		_items[index].disabled = disabled;
		if (index < _rows.size() && _rows[index].view != nullptr) {
			setEnabled(this->getComponentRegistry(),
					   _rows[index].view->getIdentifier(), !disabled);
			styleRow(_rows[index]);
		}
		return *this;
	}

	List &List::setExpanded(std::size_t index, bool expanded)
	{
		if (index >= _items.size() || _items[index].children.empty()) {
			return *this;
		}
		_items[index].expanded = expanded;

		if (index < _rows.size()) {
			auto &row = _rows[index];
			if (row.chevron != nullptr) {
				row.chevron->setGlyphName(expanded ? "expand_less"
												   : "expand_more");
			}
			if (row.view != nullptr) {
				this->getComponentRegistry()
					.getComponent<components::Accessibility>(
						row.view->getIdentifier())
					.setExpanded(expanded);
			}
		}

		if (!_rowsDirty) {
			buildItems();
		}
		return *this;
	}

	bool List::isExpanded(std::size_t index) const
	{
		return index < _items.size() && _items[index].expanded;
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
