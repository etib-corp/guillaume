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
	 * @brief Configuration of a menu.
	 *
	 * This component is the whole state of a menu, apart from its visibility
	 * and dismiss callback, which live in components::Overlay. systems::Menu
	 * turns it into the menu's Layout, Color, Borders, Elevation, Overlay and
	 * Transform, and into the Layout, Color and enabled state of its rows and
	 * the color of their labels and icons.
	 */
	class Menu: public ecs::Component
	{
		public:
		/**
		 * @brief Menu variant.
		 */
		enum class Variant { Dropdown, Context, Cascading };

		/**
		 * @brief Surface mode (Paper `mode`).
		 */
		enum class Mode {
			Elevated,	 ///< Level 2 shadow and tonal tint.
			Flat		 ///< No shadow.
		};

		/**
		 * @brief Where the menu opens relative to its anchor.
		 */
		enum class AnchorPosition {
			Bottom,	   ///< Below the anchor (Paper default).
			Top		   ///< Above the anchor.
		};

		/**
		 * @brief Menu item.
		 */
		struct Item {
			std::string label;				///< Item label text.
			std::string leadingIcon;		///< Optional leading icon glyph.
			std::string trailingIcon {};	///< Optional trailing icon glyph.
			std::function<void(void)> onPress {};	 ///< Press handler.
			bool disabled { false };	///< Whether the item is disabled.
			bool dense { false };		///< Whether the item is 32 dp high.
		};

		/**
		 * @brief Entities built for one item.
		 */
		struct Row {
			ecs::Entity::Identifier view {
				ecs::Entity::InvalidIdentifier
			};	  ///< Row container.
			std::vector<ecs::Entity::Identifier>
				content {};	   ///< Row children, in layout order.
			ecs::Entity::Identifier label {
				ecs::Entity::InvalidIdentifier
			};	  ///< Label child.
			std::vector<ecs::Entity::Identifier>
				icons {};	 ///< Leading/trailing icon children.

			/**
			 * @brief Compare two rows.
			 * @param other The row to compare with.
			 * @return True when both name the same entities.
			 */
			bool operator==(const Row &other) const noexcept;
		};

		/// Smallest M3 menu width, in dp.
		static constexpr float MinWidth = 112.0f;

		/// Largest M3 menu width, in dp.
		static constexpr float MaxWidth = 280.0f;

		private:
		Variant _variant { Variant::Dropdown };	   ///< Menu variant
		Mode _mode { Mode::Elevated };			   ///< Surface mode
		std::vector<Item> _items {};			   ///< Menu items
		std::vector<Row> _rows {};				   ///< Item rows
		bool _rowsDirty { true };				   ///< Rows must be rebuilt
		ecs::Entity::Identifier _anchor {
			ecs::Entity::InvalidIdentifier
		};	  ///< Anchor entity
		AnchorPosition _anchorPosition {
			AnchorPosition::Bottom
		};	  ///< Anchor position

		public:
		/**
		 * @brief Default constructor.
		 */
		Menu(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Menu(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		Menu &setVariant(Variant variant);

		/** @brief Get the surface mode. @return The mode. */
		Mode getMode(void) const noexcept;

		/** @brief Set the surface mode. @param mode It. @return It. */
		Menu &setMode(Mode mode);

		/** @brief Get the items. @return The items. */
		const std::vector<Item> &getItems(void) const noexcept;

		/**
		 * @brief Replace the items.
		 *
		 * The component is marked changed only when the labels, icons or
		 * flags differ; the press handlers are always replaced. The rows are
		 * not touched: whoever owns the row entities rebuilds them.
		 * @param items The new items.
		 * @return Reference to this component for chaining.
		 */
		Menu &setItems(std::vector<Item> items);

		/**
		 * @brief Enable or disable one item.
		 * @param index Item index (ignored when out of range).
		 * @param disabled Whether the item is disabled.
		 * @return Reference to this component for chaining.
		 */
		Menu &setItemDisabled(std::size_t index, bool disabled);

		/** @brief Get the rows. @return One row per built item. */
		const std::vector<Row> &getRows(void) const noexcept;

		/** @brief Set the rows. @param rows They. @return It. */
		Menu &setRows(const std::vector<Row> &rows);

		/** @brief Check whether the rows are stale. @return True if so. */
		bool isRowsDirty(void) const noexcept;

		/** @brief Flag the rows as stale or not. @param dirty It. @return It.
		 */
		Menu &setRowsDirty(bool dirty);

		/** @brief Get the anchor entity. @return Its identifier, or Invalid. */
		ecs::Entity::Identifier getAnchor(void) const noexcept;

		/** @brief Set the anchor entity. @param anchor It. @return It. */
		Menu &setAnchor(ecs::Entity::Identifier anchor);

		/** @brief Get the anchor position. @return The position. */
		AnchorPosition getAnchorPosition(void) const noexcept;

		/** @brief Set the anchor position. @param position It. @return It. */
		Menu &setAnchorPosition(AnchorPosition position);
	};

}	 // namespace guillaume::components
