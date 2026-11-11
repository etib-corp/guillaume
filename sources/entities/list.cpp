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
#include <string>
#include <utility>
#include <vector>

#include "guillaume/entities/list.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/content_helpers.hpp"
#include "guillaume/entities/divider.hpp"
#include "guillaume/entities/image.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/list.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::entities
{
	namespace
	{
		using Row	   = components::List::Row;
		using Children = std::vector<std::shared_ptr<ecs::Entity>>;

		/**
		 * @brief Build a transparent container.
		 *
		 * A bare layout surface: systems::List configures and arranges it
		 * through its components.
		 * @param registry The component registry.
		 * @param entityRegistry The entity registry receiving the container.
		 * @param parent The parent entity.
		 * @return The container.
		 */
		std::shared_ptr<View> makeView(ecs::ComponentRegistry &registry,
									   ecs::EntityRegistry &entityRegistry,
									   std::shared_ptr<ecs::Entity> parent)
		{
			View::Builder builder(registry, entityRegistry);
			auto view =
				builder.makeDefaultView(parent, utility::graphic::PoseF(), {});
			view->setColor(transparentColor());
			view->setPadding(0.0f);
			view->setSpacing(0.0f);
			return view;
		}

		/**
		 * @brief Build a subheader row: a title text with the Header role.
		 * @param registry The component registry.
		 * @param list The list, as registry and parent of the row.
		 * @param item Entry.
		 * @return The subheader entity, or nullptr when it has no text.
		 */
		std::shared_ptr<ecs::Entity>
			buildSubheader(ecs::ComponentRegistry &registry,
						   const std::shared_ptr<List> &list,
						   const List::Item &item)
		{
			auto header =
				buildText(registry, *list, list, item.headline,
						  theme::TypeRole::TitleSmall,
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
			return header;
		}

		/**
		 * @brief Build the leading group of a row: the nesting spacer, the
		 * leading image or icon, and the headline/supporting column.
		 * @param registry The component registry.
		 * @param view The row container.
		 * @param row Row receiving the identifiers of the created entities.
		 * @param item Entry.
		 * @param nested Whether the row is an accordion child.
		 */
		void buildRowLead(ecs::ComponentRegistry &registry,
						  const std::shared_ptr<View> &view, Row &row,
						  const List::Item &item, bool nested)
		{
			const auto lead = makeView(registry, *view, view);
			row.lead		= lead->getIdentifier();

			if (nested) {
				row.spacer = makeView(registry, *lead, lead)->getIdentifier();
			}

			if (!item.leadingImage.empty()) {
				Image::Builder builder(registry, *lead);
				if (const auto image =
						builder.makeImage(lead, item.leadingImage)) {
					row.leadingImage = image->getIdentifier();
				}
			} else if (const auto icon = buildIcon(
						   registry, *lead, lead, item.leadingIcon,
						   components::List::IconSize,
						   schemeColor(SchemeColorRole::OnSurfaceVariant))) {
				row.leadingIcon = icon->getIdentifier();
			}

			const auto content = makeView(registry, *lead, lead);
			row.content		   = content->getIdentifier();

			if (const auto headline =
					buildText(registry, *content, content, item.headline,
							  theme::TypeRole::BodyLarge,
							  schemeColor(SchemeColorRole::OnSurface))) {
				row.headline = headline->getIdentifier();
			}
			// Built whatever the variant: systems::List decides whether the
			// description shows.
			if (const auto supporting =
					buildText(registry, *content, content, item.supporting,
							  theme::TypeRole::BodyMedium,
							  schemeColor(SchemeColorRole::OnSurfaceVariant))) {
				row.supporting = supporting->getIdentifier();
			}
		}

		/**
		 * @brief Build one row and the rows nested under it.
		 * @param registry The component registry.
		 * @param list The list, as registry and parent of the rows.
		 * @param item Entry.
		 * @param width The list width.
		 * @param nested Whether the row is an accordion child.
		 * @param roots Receives the root entity of every built row.
		 * @return The identifiers of the row.
		 */
		Row buildRow(ecs::ComponentRegistry &registry,
					 const std::shared_ptr<List> &list, const List::Item &item,
					 float width, bool nested, Children &roots)
		{
			Row row;

			if (item.kind == List::ItemKind::Subheader) {
				if (const auto header = buildSubheader(registry, list, item)) {
					row.entity = header->getIdentifier();
					roots.push_back(header);
				}
				return row;
			}

			if (item.kind == List::ItemKind::Divider) {
				Divider::Builder builder(registry, *list);
				if (const auto divider = builder.makeDivider(
						list, width, Divider::Variant::FullWidth)) {
					row.entity = divider->getIdentifier();
					roots.push_back(divider);
				}
				return row;
			}

			const auto view = makeView(registry, *list, list);
			row.view		= view->getIdentifier();
			row.entity		= row.view;
			roots.push_back(view);

			buildRowLead(registry, view, row, item, nested);

			// An accordion shows a chevron; systems::List keeps its glyph in
			// step with the expanded state.
			const std::string trailing = item.children.empty()
				? item.trailingIcon
				: std::string(item.expanded ? components::List::ExpandedGlyph
											: components::List::CollapsedGlyph);
			if (const auto icon = buildIcon(
					registry, *view, view, trailing, components::List::IconSize,
					schemeColor(SchemeColorRole::OnSurfaceVariant))) {
				row.trailingIcon = icon->getIdentifier();
			}

			for (const auto &child: item.children) {
				row.children.push_back(
					buildRow(registry, list, child, width, true, roots));
			}

			return row;
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
		auto entity = List::create(this->getComponentRegistry(), _config,
								   _variant, _items);
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
		_config.setFixedWidth(components::List::DefaultWidth);
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
		: SurfaceBase<components::List>(registry, config)
	{
		getComponentRegistry()
			.getComponent<components::List>(getIdentifier())
			.setVariant(variant)
			.setItems(items);
		systems::List::apply(getComponentRegistry(), getIdentifier());
	}

	List::~List(void)
	{
	}

	List &List::setVariant(Variant variant)
	{
		getComponentRegistry()
			.getComponent<components::List>(getIdentifier())
			.setVariant(variant);
		systems::List::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	List::Variant List::getVariant(void) const
	{
		return getComponentRegistry()
			.getComponent<components::List>(getIdentifier())
			.getVariant();
	}

	List &List::setItems(const std::vector<Item> &items)
	{
		getComponentRegistry()
			.getComponent<components::List>(getIdentifier())
			.setItems(items)
			.setRowsDirty(true);
		buildItems();
		systems::List::apply(getComponentRegistry(), getIdentifier());
		return *this;
	}

	std::size_t List::getItemCount(void) const
	{
		return getComponentRegistry()
			.getComponent<components::List>(getIdentifier())
			.getItems()
			.size();
	}

	ecs::Entity::Identifier List::getItemIdentifier(std::size_t index) const
	{
		const auto *row = getComponentRegistry()
							  .getComponent<components::List>(getIdentifier())
							  .findRow({ index });

		return row != nullptr ? row->entity : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier
		List::getChildIdentifier(std::size_t index,
								 std::size_t childIndex) const
	{
		const auto *row = getComponentRegistry()
							  .getComponent<components::List>(getIdentifier())
							  .findRow({ index, childIndex });

		return row != nullptr ? row->entity : ecs::Entity::InvalidIdentifier;
	}

	ecs::Entity::Identifier List::getContentIdentifier(std::size_t index) const
	{
		const auto *row = getComponentRegistry()
							  .getComponent<components::List>(getIdentifier())
							  .findRow({ index });

		return row != nullptr ? row->content : ecs::Entity::InvalidIdentifier;
	}

	List &List::setItemDisabled(std::size_t index, bool disabled)
	{
		systems::List::setItemDisabled(getComponentRegistry(), getIdentifier(),
									   index, disabled);
		return *this;
	}

	List &List::setExpanded(std::size_t index, bool expanded)
	{
		systems::List::setExpanded(getComponentRegistry(), getIdentifier(),
								   index, expanded);
		return *this;
	}

	bool List::isExpanded(std::size_t index) const
	{
		return systems::List::isExpanded(getComponentRegistry(),
										 getIdentifier(), index);
	}

	void List::build(void)
	{
		buildItems();
		systems::List::apply(getComponentRegistry(), getIdentifier());
	}

	void List::buildItems(void)
	{
		// Children need the owning handle as their parent.
		if (weak_from_this().expired()) {
			return;
		}

		auto &registry = getComponentRegistry();

		if (!registry.getComponent<components::List>(getIdentifier())
				 .isRowsDirty()) {
			return;
		}

		const auto self = std::static_pointer_cast<List>(shared_from_this());
		const auto &layout =
			registry.getComponent<components::Layout>(getIdentifier());
		const float width = layout.hasFixedWidth()
			? layout.getFixedWidth()
			: components::List::DefaultWidth;

		// Copy: the entries are read while the row entities are created.
		const auto items =
			registry.getComponent<components::List>(getIdentifier()).getItems();
		// The rows are rebuilt from scratch: remove the previous generation
		// with its components. Everything the list owns is a row.
		clearEntities();

		std::vector<Row> rows;
		Children roots;

		for (const auto &item: items) {
			rows.push_back(buildRow(registry, self, item, width, false, roots));
		}

		registry.getComponent<components::List>(getIdentifier())
			.setRows(rows)
			.setRowsDirty(false);
		systems::List::install(registry, getIdentifier());

		// Hand every row to the surface; systems::List then narrows the
		// layout children to the rows currently shown.
		setChildren(roots);
	}

}	 // namespace guillaume::entities
