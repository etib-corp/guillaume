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

#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a list: a column of rows, each with a headline and
	 * optional supporting text, icons or image, plus subheaders, dividers
	 * and accordions.
	 *
	 * The entities::List widget writes it: the entries, and one Row of
	 * entity identifiers per entry it builds. systems::List reads it every
	 * frame in the Layout phase and turns it into the list's Layout, Color
	 * and Borders, and into the Layout, Color, enabled state and
	 * accessibility state of every row and of its content.
	 *
	 * Entries and rows are two trees that must stay in step: the Row at a
	 * given Path holds the entities built for the Item at the same Path.
	 * Replacing the entries does not rebuild the rows; the widget raises
	 * the rows-dirty flag and rebuilds them.
	 *
	 * An entry with children is an accordion: pressing it shows or hides
	 * its children. Only top-level entries can be accordions.
	 *
	 * @see systems::List, entities::List
	 */
	class List: public ecs::Component
	{
		public:
		/**
		 * @brief Number of text lines a row is meant for.
		 *
		 * SingleLine hides the supporting text; TwoLine and ThreeLine both
		 * show it.
		 */
		enum class Variant { SingleLine, TwoLine, ThreeLine };

		/**
		 * @brief What a list entry is.
		 */
		enum class ItemKind {
			Item,		  ///< Pressable row (or accordion when it has
						  ///< children).
			Subheader,	  ///< Section header (`titleSmall` text).
			Divider		  ///< Horizontal divider line.
		};

		/**
		 * @brief Description of one list entry, as given by the user.
		 *
		 * A subheader only uses `headline`; a divider uses no field but
		 * `kind`.
		 */
		struct Item {
			std::string headline;		 ///< Main text of the row.
			std::string supporting;		 ///< Second text, under the headline;
										 ///< empty for none.
			std::string leadingIcon;	 ///< Glyph name of the icon before the
										 ///< texts; empty for none.
			std::string trailingIcon;	 ///< Glyph name of the icon at the
										 ///< far edge; empty for none.
			std::string leadingImage {};	///< Path of the image before the
											///< texts; empty for none.
			std::function<void(void)> onPress {};	 ///< Called on press.
			bool disabled { false };			 ///< True when the row ignores
												 ///< presses.
			ItemKind kind { ItemKind::Item };	 ///< What the entry is.
			std::vector<Item> children {};		 ///< Rows shown under this one
												 ///< while it is expanded.
			bool expanded { false };	///< True while an accordion shows
										///< its children.
		};

		/**
		 * @brief Identifiers of the entities built for one entry.
		 *
		 * A subheader or a divider only has `entity`. A pressable row is a
		 * container (`view`) holding a leading group (`lead`: nesting
		 * spacer, leading image or icon, text column) and an optional
		 * trailing icon. A field is ecs::Entity::InvalidIdentifier when the
		 * entry has no such part.
		 */
		struct Row {
			ecs::Entity::Identifier entity {
				ecs::Entity::InvalidIdentifier
			};	  ///< Row root (container, subheader text or divider).
			ecs::Entity::Identifier view {
				ecs::Entity::InvalidIdentifier
			};	  ///< Pressable row container.
			ecs::Entity::Identifier lead {
				ecs::Entity::InvalidIdentifier
			};	  ///< Leading group.
			ecs::Entity::Identifier spacer {
				ecs::Entity::InvalidIdentifier
			};	  ///< Nesting inset of an accordion child.
			ecs::Entity::Identifier leadingImage {
				ecs::Entity::InvalidIdentifier
			};	  ///< Leading image child.
			ecs::Entity::Identifier leadingIcon {
				ecs::Entity::InvalidIdentifier
			};	  ///< Leading icon child.
			ecs::Entity::Identifier content {
				ecs::Entity::InvalidIdentifier
			};	  ///< Column holding the headline and supporting text.
			ecs::Entity::Identifier headline {
				ecs::Entity::InvalidIdentifier
			};	  ///< Headline text child.
			ecs::Entity::Identifier supporting {
				ecs::Entity::InvalidIdentifier
			};	  ///< Supporting text child.
			ecs::Entity::Identifier trailingIcon {
				ecs::Entity::InvalidIdentifier
			};	  ///< Trailing icon child (the chevron of an accordion).
			std::vector<Row> children {};	 ///< Rows of the accordion
											 ///< children.

			/**
			 * @brief Compare two rows.
			 * @param other The row to compare with.
			 * @return True when both name the same entities, children
			 * included.
			 */
			bool operator==(const Row &other) const noexcept;
		};

		/// Position of an entry in the tree: the top-level index, then one
		/// child index per nesting level. `{2}` is the third entry; `{2, 0}`
		/// is its first child.
		using Path = std::vector<std::size_t>;

		/// Default width of a list, in dp.
		static constexpr float DefaultWidth = 360.0f;

		/// Size of the leading and trailing icons, in dp.
		static constexpr float IconSize = 24.0f;

		/// Glyph name of the trailing icon of a collapsed accordion.
		static constexpr const char *CollapsedGlyph = "expand_more";

		/// Glyph name of the trailing icon of an expanded accordion.
		static constexpr const char *ExpandedGlyph = "expand_less";

		private:
		Variant _variant { Variant::SingleLine };	 ///< Lines per row
		std::vector<Item> _items {};				 ///< List entries
		std::vector<Row> _rows {};					 ///< Entry rows
		bool _rowsDirty { true };					 ///< Rows must be rebuilt

		public:
		/**
		 * @brief Create the configuration of an empty single-line list.
		 */
		List(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~List(void) = default;

		/**
		 * @brief Get the number of text lines a row is meant for.
		 * @return The variant, Variant::SingleLine by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the number of text lines a row is meant for.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		List &setVariant(Variant variant);

		/**
		 * @brief Get the description of every top-level entry.
		 * @return The entries, from top to bottom; accordion children are
		 * in Item::children.
		 */
		const std::vector<Item> &getItems(void) const noexcept;

		/**
		 * @brief Replace the entries.
		 *
		 * The press handlers are always replaced. The component is marked
		 * changed only when the texts, icons, kinds or flags differ
		 * (children included), because handlers cannot be compared. The
		 * rows are left as they are: whoever owns the row entities rebuilds
		 * them.
		 * @param items The new entries, from top to bottom.
		 * @return This component, for chaining.
		 */
		List &setItems(std::vector<Item> items);

		/**
		 * @brief Find an entry from its position in the tree.
		 * @param path The index path of the entry.
		 * @return The entry, or nullptr when the path is empty or names no
		 * entry. The pointer is valid until the entries are replaced.
		 */
		const Item *findItem(const Path &path) const noexcept;

		/**
		 * @brief Find the entities built for an entry from its position in
		 * the tree.
		 * @param path The index path of the entry.
		 * @return The row, or nullptr when the path is empty or names no
		 * built row. The pointer is valid until the rows are replaced.
		 */
		const Row *findRow(const Path &path) const noexcept;

		/**
		 * @brief Change the disabled flag recorded for one top-level entry.
		 *
		 * To update the row on screen too, call
		 * systems::List::setItemDisabled. Marks the component changed only
		 * when the flag differs.
		 * @param index The top-level entry index, starting at 0. An index
		 * out of range is ignored.
		 * @param disabled True to disable the entry.
		 * @return This component, for chaining.
		 */
		List &setItemDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Record whether one top-level accordion shows its children.
		 *
		 * To update the rows on screen too, call
		 * systems::List::setExpanded. Marks the component changed only when
		 * the flag differs.
		 * @param index The top-level entry index, starting at 0. An index
		 * out of range, or an entry without children, is ignored.
		 * @param expanded True to show the children.
		 * @return This component, for chaining.
		 */
		List &setItemExpanded(std::size_t index, bool expanded);

		/**
		 * @brief Get the entities built for the entries.
		 * @return One row per built top-level entry, in entry order. Empty
		 * until the widget has built them.
		 */
		const std::vector<Row> &getRows(void) const noexcept;

		/**
		 * @brief Record the entities built for the entries.
		 *
		 * Marks the component changed only when the rows differ.
		 * @param rows One row per top-level entry, in entry order.
		 * @return This component, for chaining.
		 */
		List &setRows(const std::vector<Row> &rows);

		/**
		 * @brief Tell whether the rows must be rebuilt from the entries.
		 * @return True when the entries changed since the rows were built.
		 * True for a new component.
		 */
		bool isRowsDirty(void) const noexcept;

		/**
		 * @brief Mark the rows as out of date or as up to date.
		 *
		 * The widget raises the flag when it replaces the entries and
		 * lowers it once it has rebuilt the row entities. Marks the
		 * component changed only when the flag differs.
		 * @param dirty True when the rows must be rebuilt.
		 * @return This component, for chaining.
		 */
		List &setRowsDirty(bool dirty);
	};

}	 // namespace guillaume::components
