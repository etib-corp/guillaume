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
	 * @brief Configuration of a list.
	 *
	 * This component is the whole state of a list: its variant, its entries
	 * and the entities built for them. systems::List turns it into the list's
	 * Layout, Color and Borders, and into the Layout, Color, enabled state
	 * and accessibility state of every row and of its content.
	 */
	class List: public ecs::Component
	{
		public:
		/**
		 * @brief List variant.
		 */
		enum class Variant { SingleLine, TwoLine, ThreeLine };

		/**
		 * @brief Kind of a list entry.
		 */
		enum class ItemKind {
			Item,		  ///< Pressable row (or accordion when it has
						  ///< children).
			Subheader,	  ///< Section header (`titleSmall`).
			Divider		  ///< Horizontal divider.
		};

		/**
		 * @brief List entry.
		 */
		struct Item {
			std::string headline;		 ///< Primary headline text.
			std::string supporting;		 ///< Optional supporting text.
			std::string leadingIcon;	 ///< Optional leading icon glyph name.
			std::string trailingIcon;	 ///< Optional trailing icon glyph name.
			std::string leadingImage {};	///< Optional leading image path.
			std::function<void(void)> onPress {};	 ///< Press handler.
			bool disabled { false };	///< Whether the row is disabled.
			ItemKind kind { ItemKind::Item };	 ///< Entry kind.
			std::vector<Item> children {};		 ///< Accordion children.
			bool expanded { false };			 ///< Accordion expanded state.
		};

		/**
		 * @brief Entities built for one entry.
		 *
		 * A subheader or a divider only has its root entity. A pressable row
		 * is a container holding a leading group (nesting spacer, leading
		 * image or icon, content column) and an optional trailing icon.
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
			};	  ///< Title/description column.
			ecs::Entity::Identifier headline {
				ecs::Entity::InvalidIdentifier
			};	  ///< Title child.
			ecs::Entity::Identifier supporting {
				ecs::Entity::InvalidIdentifier
			};	  ///< Description child.
			ecs::Entity::Identifier trailingIcon {
				ecs::Entity::InvalidIdentifier
			};	  ///< Trailing icon child (the chevron of an accordion).
			std::vector<Row> children {};	 ///< Accordion child rows.

			/**
			 * @brief Compare two rows.
			 * @param other The row to compare with.
			 * @return True when both name the same entities.
			 */
			bool operator==(const Row &other) const noexcept;
		};

		/// Index path of an entry: top-level index, then child indices.
		using Path = std::vector<std::size_t>;

		/// Default list width, in dp.
		static constexpr float DefaultWidth = 360.0f;

		/// M3 list icon size, in dp.
		static constexpr float IconSize = 24.0f;

		/// Trailing glyph of a collapsed accordion.
		static constexpr const char *CollapsedGlyph = "expand_more";

		/// Trailing glyph of an expanded accordion.
		static constexpr const char *ExpandedGlyph = "expand_less";

		private:
		Variant _variant { Variant::SingleLine };	 ///< List variant
		std::vector<Item> _items {};				 ///< List entries
		std::vector<Row> _rows {};					 ///< Entry rows
		bool _rowsDirty { true };					 ///< Rows must be rebuilt

		public:
		/**
		 * @brief Default constructor.
		 */
		List(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~List(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		List &setVariant(Variant variant);

		/** @brief Get the entries. @return The entries. */
		const std::vector<Item> &getItems(void) const noexcept;

		/**
		 * @brief Replace the entries.
		 *
		 * The component is marked changed only when the texts, icons, kinds
		 * or flags differ; the press handlers are always replaced. The rows
		 * are not touched: whoever owns the row entities rebuilds them.
		 * @param items The new entries.
		 * @return Reference to this component for chaining.
		 */
		List &setItems(std::vector<Item> items);

		/**
		 * @brief Find an entry by index path.
		 * @param path Index path.
		 * @return The entry, or nullptr when the path names none.
		 */
		const Item *findItem(const Path &path) const noexcept;

		/**
		 * @brief Find a row by index path.
		 * @param path Index path.
		 * @return The row, or nullptr when the path names none.
		 */
		const Row *findRow(const Path &path) const noexcept;

		/**
		 * @brief Enable or disable one top-level entry.
		 * @param index Entry index (ignored when out of range).
		 * @param disabled Whether the entry is disabled.
		 * @return Reference to this component for chaining.
		 */
		List &setItemDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Expand or collapse one top-level accordion entry.
		 * @param index Entry index (ignored when out of range or when the
		 * entry has no children).
		 * @param expanded Whether the accordion is expanded.
		 * @return Reference to this component for chaining.
		 */
		List &setItemExpanded(std::size_t index, bool expanded);

		/** @brief Get the rows. @return One row per built entry. */
		const std::vector<Row> &getRows(void) const noexcept;

		/** @brief Set the rows. @param rows They. @return It. */
		List &setRows(const std::vector<Row> &rows);

		/** @brief Check whether the rows are stale. @return True if so. */
		bool isRowsDirty(void) const noexcept;

		/** @brief Flag the rows as stale or not. @param dirty It. @return It.
		 */
		List &setRowsDirty(bool dirty);
	};

}	 // namespace guillaume::components
