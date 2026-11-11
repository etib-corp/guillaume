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

#include "guillaume/systems/list.hpp"

#include <vector>

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/borders.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/layout.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/entities/interaction_helpers.hpp"
#include "guillaume/entities/style_helpers.hpp"
#include "guillaume/systems/layout.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems
{
	namespace
	{
		using Item	  = components::List::Item;
		using Row	  = components::List::Row;
		using Variant = components::List::Variant;
		using Ids	  = std::vector<ecs::Entity::Identifier>;
		using entities::disabledContentColor;
		using entities::schemeColor;
		using entities::transparentColor;

		/// M3 list item padding (16 dp on every side).
		constexpr float RowPadding = theme::SpacingScale::get(4);

		/// M3 gap between the leading slot, content and trailing slot.
		constexpr float RowSpacing = theme::SpacingScale::get(4);

		/// Inset of accordion children (Paper aligns them with the parent
		/// title), in dp per nesting level.
		constexpr float AccordionInset = 40.0f;

		/**
		 * @brief Append an identifier to a child list, when it names an
		 * entity.
		 * @param children The child list.
		 * @param identifier The identifier (may be invalid).
		 */
		void pushValid(Ids &children, ecs::Entity::Identifier identifier)
		{
			if (identifier != ecs::Entity::InvalidIdentifier) {
				children.push_back(identifier);
			}
		}

		/**
		 * @brief Set the color of a child, when the child exists.
		 * @param registry The registry holding the components.
		 * @param child The child identifier (may be invalid).
		 * @param color The color to write.
		 */
		void colorChild(ecs::ComponentRegistry &registry,
						ecs::Entity::Identifier child,
						const utility::graphic::Color32Bit &color)
		{
			if (child != ecs::Entity::InvalidIdentifier
				&& registry.hasComponent<components::Color>(child)) {
				registry.getComponent<components::Color>(child).setColor(color);
			}
		}

		/**
		 * @brief Check whether an entity is a layout container.
		 * @param registry The registry holding the components.
		 * @param identifier The entity (may be invalid).
		 * @return True when it has a Layout, a Transform and a Bound.
		 */
		bool isContainer(const ecs::ComponentRegistry &registry,
						 ecs::Entity::Identifier identifier)
		{
			return identifier != ecs::Entity::InvalidIdentifier
				&& registry.hasComponent<components::Layout>(identifier)
				&& registry.hasComponent<components::Transform>(identifier)
				&& registry.hasComponent<components::Bound>(identifier);
		}

		/**
		 * @brief Arrange the layout children of a container, when it is one.
		 * @param registry The registry holding the components.
		 * @param identifier The container (may be invalid).
		 */
		void arrange(ecs::ComponentRegistry &registry,
					 ecs::Entity::Identifier identifier)
		{
			if (!isContainer(registry, identifier)) {
				return;
			}
			Layout::apply(registry, identifier,
						  registry.getComponent<components::Layout>(identifier)
							  .getChildren(),
						  Layout::layerOf(registry, identifier));
		}

		/**
		 * @brief Configure a container without padding.
		 * @param registry The registry holding the components.
		 * @param identifier The container (may be invalid).
		 * @param axis Its arrangement axis.
		 * @param spacing The gap between its children.
		 * @param children Its layout children.
		 */
		void configureGroup(ecs::ComponentRegistry &registry,
							ecs::Entity::Identifier identifier,
							components::Layout::Axis axis, float spacing,
							const Ids &children)
		{
			if (!isContainer(registry, identifier)) {
				return;
			}
			registry.getComponent<components::Layout>(identifier)
				.setAxis(axis)
				.setMainAxisAlignment(
					components::Layout::MainAxisAlignment::Start)
				.setCrossAxisAlignment(
					axis == components::Layout::Axis::Vertical
						? components::Layout::CrossAxisAlignment::Start
						: components::Layout::CrossAxisAlignment::Center)
				.setPadding(0.0f)
				.setSpacing(spacing)
				.setChildren(children);
		}

		/**
		 * @brief Check whether the row of a path is folded away.
		 *
		 * Only the direct children of an expanded top-level accordion are
		 * shown; deeper rows are always hidden.
		 * @param list The list configuration.
		 * @param path Index path of the row.
		 * @return True when the row is not shown.
		 */
		bool isHidden(const components::List &list,
					  const components::List::Path &path)
		{
			if (path.size() <= 1) {
				return false;
			}
			return path.size() > 2 || path[0] >= list.getItems().size()
				|| !list.getItems()[path[0]].expanded;
		}

		/**
		 * @brief Color a row, its texts and its icons.
		 * @param registry The registry holding the components.
		 * @param row The row.
		 * @param hidden Whether the row is folded away.
		 * @param showSupporting Whether the variant shows the description.
		 */
		void paintRow(ecs::ComponentRegistry &registry, const Row &row,
					  bool hidden, bool showSupporting)
		{
			if (row.view == ecs::Entity::InvalidIdentifier) {
				return;
			}

			const bool enabled	 = entities::isEnabled(registry, row.view);
			auto headlineColor	 = enabled
				  ? schemeColor(SchemeColorRole::OnSurface)
				  : disabledContentColor();
			auto supportingColor = enabled
				? schemeColor(SchemeColorRole::OnSurfaceVariant)
				: disabledContentColor();
			auto iconColor		 = supportingColor;
			auto rowColor		 = entities::stateLayerColor(
				   registry, row.view, transparentColor(),
				   schemeColor(SchemeColorRole::OnSurface));

			if (hidden) {
				headlineColor	= transparentColor();
				supportingColor = transparentColor();
				iconColor		= transparentColor();
				rowColor		= transparentColor();
			}
			if (!showSupporting) {
				supportingColor = transparentColor();
			}

			colorChild(registry, row.view, rowColor);
			colorChild(registry, row.headline, headlineColor);
			colorChild(registry, row.supporting, supportingColor);
			colorChild(registry, row.leadingIcon, iconColor);
			colorChild(registry, row.trailingIcon, iconColor);
		}

		/**
		 * @brief Configure a row and the rows nested under it: layout of its
		 * containers, enabled and accessibility state, chevron and colors.
		 * @param registry The registry holding the components.
		 * @param row The row.
		 * @param item Its entry, or nullptr when the rows are stale.
		 * @param width The list width.
		 * @param depth Nesting depth of the row (0 for a top-level row).
		 * @param hidden Whether the row is folded away.
		 * @param showSupporting Whether the variant shows the description.
		 */
		void configureRow(ecs::ComponentRegistry &registry, const Row &row,
						  const Item *item, float width, std::size_t depth,
						  bool hidden, bool showSupporting)
		{
			const bool accordion = item != nullptr && !item->children.empty();

			if (isContainer(registry, row.view)) {
				Ids rowChildren;
				pushValid(rowChildren, row.lead);
				pushValid(rowChildren, row.trailingIcon);

				Ids leadChildren;
				pushValid(leadChildren, row.spacer);
				pushValid(leadChildren, row.leadingImage);
				pushValid(leadChildren, row.leadingIcon);
				pushValid(leadChildren, row.content);

				Ids contentChildren;
				pushValid(contentChildren, row.headline);
				if (showSupporting) {
					pushValid(contentChildren, row.supporting);
				}

				registry.getComponent<components::Layout>(row.view)
					.setAxis(components::Layout::Axis::Horizontal)
					.setMainAxisAlignment(
						row.trailingIcon != ecs::Entity::InvalidIdentifier
							? components::Layout::MainAxisAlignment::
								  SpaceBetween
							: components::Layout::MainAxisAlignment::Start)
					.setCrossAxisAlignment(
						components::Layout::CrossAxisAlignment::Center)
					.setPadding(RowPadding)
					.setSpacing(RowSpacing)
					.setFixedWidth(width)
					.setChildren(rowChildren);
				configureGroup(registry, row.lead,
							   components::Layout::Axis::Horizontal, RowSpacing,
							   leadChildren);
				configureGroup(registry, row.content,
							   components::Layout::Axis::Vertical, 0.0f,
							   contentChildren);
				if (isContainer(registry, row.spacer)) {
					registry.getComponent<components::Layout>(row.spacer)
						.setFixedWidth(static_cast<float>(depth)
									   * AccordionInset)
						.setFixedHeight(1.0f);
				}
			}

			if (row.view != ecs::Entity::InvalidIdentifier && item != nullptr) {
				if (registry.hasComponent<components::Accessibility>(
						row.view)) {
					auto &accessibility =
						registry.getComponent<components::Accessibility>(
							row.view);
					accessibility.setLabel(item->headline)
						.setHint(item->supporting);
					if (accordion) {
						accessibility.setExpanded(item->expanded);
					}
				}
				entities::setEnabled(registry, row.view,
									 !hidden && !item->disabled);
				if (accordion
					&& row.trailingIcon != ecs::Entity::InvalidIdentifier
					&& registry.hasComponent<components::Glyph>(
						row.trailingIcon)) {
					registry.getComponent<components::Glyph>(row.trailingIcon)
						.setName(item->expanded
									 ? components::List::ExpandedGlyph
									 : components::List::CollapsedGlyph);
				}
			}

			paintRow(registry, row, hidden, showSupporting);

			// Only the children of an expanded top-level accordion show.
			const bool childrenHidden =
				hidden || depth > 0 || item == nullptr || !item->expanded;
			for (std::size_t index = 0; index < row.children.size(); ++index) {
				const Item *child =
					item != nullptr && index < item->children.size()
					? &item->children[index]
					: nullptr;
				configureRow(registry, row.children[index], child, width,
							 depth + 1, childrenHidden, showSupporting);
			}
		}

		/**
		 * @brief Size a row from its content, innermost container first.
		 * @param registry The registry holding the components.
		 * @param row The row.
		 */
		void measureRow(ecs::ComponentRegistry &registry, const Row &row)
		{
			arrange(registry, row.spacer);
			arrange(registry, row.content);
			arrange(registry, row.lead);
			arrange(registry, row.view);
		}

		/**
		 * @brief Place the content of a row from the pose of the row,
		 * outermost container first.
		 * @param registry The registry holding the components.
		 * @param row The row.
		 */
		void placeRow(ecs::ComponentRegistry &registry, const Row &row)
		{
			arrange(registry, row.view);
			arrange(registry, row.lead);
			arrange(registry, row.content);
		}

		/**
		 * @brief Make a row and the rows nested under it pressable.
		 * @param registry The registry holding the components.
		 * @param identifier The list entity.
		 * @param row The row.
		 * @param path Index path of the row.
		 */
		void installRows(ecs::ComponentRegistry &registry,
						 const ecs::Entity::Identifier &identifier,
						 const Row &row, const components::List::Path &path)
		{
			List::installRow(registry, identifier, path);
			for (std::size_t index = 0; index < row.children.size(); ++index) {
				auto childPath = path;
				childPath.push_back(index);
				installRows(registry, identifier, row.children[index],
							childPath);
			}
		}
	}	 // namespace

	List::List(void)
		: ecs::SystemFiller<components::List>(ecs::Phase::Layout)
	{
	}

	void List::install(ecs::ComponentRegistry &componentRegistry,
					   const ecs::Entity::Identifier &entityIdentifier)
	{
		// Copy: installing a row adds components, never rows, but the
		// component storage may move while it grows.
		const auto rows =
			componentRegistry.getComponent<components::List>(entityIdentifier)
				.getRows();

		for (std::size_t index = 0; index < rows.size(); ++index) {
			installRows(componentRegistry, entityIdentifier, rows[index],
						{ index });
		}
	}

	void List::installRow(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  const Path &path)
	{
		ecs::ComponentRegistry *registry		 = &componentRegistry;
		const ecs::Entity::Identifier identifier = entityIdentifier;
		const auto *row =
			componentRegistry.getComponent<components::List>(identifier)
				.findRow(path);

		if (row == nullptr || row->view == ecs::Entity::InvalidIdentifier) {
			return;
		}

		entities::makeInteractive(
			componentRegistry, row->view,
			components::AccessibilityRole::ListItem,
			[registry, identifier, path]() {
				pressItem(*registry, identifier, path);
			},
			[registry, identifier, path]() {
				styleRow(*registry, identifier, path);
			});
	}

	void List::styleRow(ecs::ComponentRegistry &componentRegistry,
						const ecs::Entity::Identifier &entityIdentifier,
						const Path &path)
	{
		const auto &list =
			componentRegistry.getComponent<components::List>(entityIdentifier);
		const auto *row = list.findRow(path);

		if (row == nullptr) {
			return;
		}
		paintRow(componentRegistry, *row, isHidden(list, path),
				 list.getVariant() != Variant::SingleLine);
	}

	void List::pressItem(ecs::ComponentRegistry &componentRegistry,
						 const ecs::Entity::Identifier &entityIdentifier,
						 const Path &path)
	{
		const auto &list =
			componentRegistry.getComponent<components::List>(entityIdentifier);
		const auto *item = list.findItem(path);

		if (item == nullptr || item->disabled || isHidden(list, path)) {
			return;
		}

		// Copy: the handler may replace the entries.
		const auto onPress = item->onPress;

		if (!item->children.empty() && path.size() == 1) {
			setExpanded(componentRegistry, entityIdentifier, path[0],
						!item->expanded);
		}
		if (onPress) {
			onPress();
		}
	}

	void List::setItemDisabled(ecs::ComponentRegistry &componentRegistry,
							   const ecs::Entity::Identifier &entityIdentifier,
							   std::size_t index, bool disabled)
	{
		componentRegistry.getComponent<components::List>(entityIdentifier)
			.setItemDisabled(index, disabled);
		apply(componentRegistry, entityIdentifier);
	}

	void List::setExpanded(ecs::ComponentRegistry &componentRegistry,
						   const ecs::Entity::Identifier &entityIdentifier,
						   std::size_t index, bool expanded)
	{
		componentRegistry.getComponent<components::List>(entityIdentifier)
			.setItemExpanded(index, expanded);
		apply(componentRegistry, entityIdentifier);
	}

	bool List::isExpanded(const ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier,
						  std::size_t index)
	{
		const auto &items =
			componentRegistry.getComponent<components::List>(entityIdentifier)
				.getItems();

		return index < items.size() && items[index].expanded;
	}

	void List::apply(ecs::ComponentRegistry &componentRegistry,
					 const ecs::Entity::Identifier &entityIdentifier)
	{
		const auto &list =
			componentRegistry.getComponent<components::List>(entityIdentifier);
		const auto &rows		  = list.getRows();
		const auto &items		  = list.getItems();
		const bool showSupporting = list.getVariant() != Variant::SingleLine;

		auto &layout = componentRegistry.getComponent<components::Layout>(
			entityIdentifier);
		layout.setAxis(components::Layout::Axis::Vertical)
			.setMainAxisAlignment(components::Layout::MainAxisAlignment::Start)
			.setCrossAxisAlignment(
				components::Layout::CrossAxisAlignment::Start)
			.setPadding(0.0f)
			.setSpacing(0.0f);
		if (!layout.hasFixedWidth()) {
			layout.setFixedWidth(components::List::DefaultWidth);
		}
		const float width = layout.getFixedWidth();

		componentRegistry.getComponent<components::Borders>(entityIdentifier)
			.setBorderRadius(getShapeRadius(theme::ShapeRole::None));
		componentRegistry.getComponent<components::Color>(entityIdentifier)
			.setColor(schemeColor(SchemeColorRole::Surface));

		// The rows shown: every top-level row, each followed by its
		// children while it is an expanded accordion.
		std::vector<const Row *> shown;
		Ids children;

		for (std::size_t index = 0; index < rows.size(); ++index) {
			const auto &row	 = rows[index];
			const Item *item = index < items.size() ? &items[index] : nullptr;

			configureRow(componentRegistry, row, item, width, 0, false,
						 showSupporting);

			if (row.entity != ecs::Entity::InvalidIdentifier) {
				shown.push_back(&row);
				children.push_back(row.entity);
			}
			if (item == nullptr || !item->expanded) {
				continue;
			}
			for (const auto &child: row.children) {
				if (child.entity != ecs::Entity::InvalidIdentifier) {
					shown.push_back(&child);
					children.push_back(child.entity);
				}
			}
		}

		layout.setChildren(children);

		// The rows are sized first, so the list can stack them; their
		// content is then placed at the pose the list gave them.
		for (const auto *row: shown) {
			measureRow(componentRegistry, *row);
		}
		Layout::apply(componentRegistry, entityIdentifier, children,
					  Layout::layerOf(componentRegistry, entityIdentifier));
		for (const auto *row: shown) {
			placeRow(componentRegistry, *row);
		}
	}

	void List::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		if (!requireComponent<components::List>(entityIdentifier)) {
			return;
		}

		apply(getComponentRegistry(), entityIdentifier);
	}
}	 // namespace guillaume::systems
