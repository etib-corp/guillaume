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
	 * @brief State of a menu: a floating list of choices that opens next to
	 * an anchor.
	 *
	 * The entities::Menu widget writes it: the items, the anchor, and one
	 * Row of entity identifiers per item it builds. systems::Menu reads it
	 * every frame in the Layout phase and turns it into the menu's Layout,
	 * Color, Borders, Elevation, Overlay flags and Transform (placed next
	 * to the anchor), and into the Layout, Color and enabled state of each
	 * row.
	 *
	 * Whether the menu is shown, and the callback run when it is dismissed,
	 * are not stored here: they are in the components::Overlay of the same
	 * entity (see systems::Menu::show and systems::Menu::hide).
	 *
	 * Items and rows are two lists that must stay in step: `getRows()[i]`
	 * holds the entities built for `getItems()[i]`. Replacing the items
	 * does not rebuild the rows; the widget raises the rows-dirty flag and
	 * rebuilds them.
	 *
	 * @see systems::Menu, components::Overlay, entities::Menu
	 */
	class Menu: public ecs::Component
	{
		public:
		/**
		 * @brief Kind of menu. It is stored for the application; the menu
		 * system styles and places every kind alike.
		 */
		enum class Variant { Dropdown, Context, Cascading };

		/**
		 * @brief How the menu surface stands out from what is behind it.
		 */
		enum class Mode {
			Elevated,	 ///< Level 2 shadow and tonal tint.
			Flat		 ///< No shadow.
		};

		/**
		 * @brief Side of the anchor the menu opens on.
		 */
		enum class AnchorPosition {
			Bottom,	   ///< Below the anchor.
			Top		   ///< Above the anchor.
		};

		/**
		 * @brief Description of one menu item, as given by the user.
		 */
		struct Item {
			std::string label;			///< Text of the item.
			std::string leadingIcon;	///< Glyph name of the icon before the
										///< label; empty for none.
			std::string trailingIcon {};	///< Glyph name of the icon after
											///< the label; empty for none.
			std::function<void(void)> onPress {};	 ///< Called on press.
			bool disabled { false };	///< True when the item ignores
										///< presses.
			bool dense { false };		///< True for a 32 dp high row instead
										///< of the regular height.
		};

		/**
		 * @brief Identifiers of the entities built for one item.
		 */
		struct Row {
			ecs::Entity::Identifier view {
				ecs::Entity::InvalidIdentifier
			};	  ///< Pressable container of the row.
			std::vector<ecs::Entity::Identifier>
				content {};	   ///< Children of the row, in layout order.
			ecs::Entity::Identifier label {
				ecs::Entity::InvalidIdentifier
			};	  ///< Text child that draws the label.
			std::vector<ecs::Entity::Identifier>
				icons {};	 ///< Leading and trailing icon children.

			/**
			 * @brief Compare two rows.
			 * @param other The row to compare with.
			 * @return True when both name the same entities.
			 */
			bool operator==(const Row &other) const noexcept;
		};

		/// Smallest width of a menu, in dp.
		static constexpr float MinWidth = 112.0f;

		/// Largest width of a menu, in dp.
		static constexpr float MaxWidth = 280.0f;

		private:
		Variant _variant { Variant::Dropdown };	   ///< Kind of menu
		Mode _mode { Mode::Elevated };			   ///< Surface mode
		std::vector<Item> _items {};			   ///< Menu items
		std::vector<Row> _rows {};				   ///< Item rows
		bool _rowsDirty { true };				   ///< Rows must be rebuilt
		ecs::Entity::Identifier _anchor {
			ecs::Entity::InvalidIdentifier
		};	  ///< Anchor entity

		AnchorPosition _anchorPosition {
			AnchorPosition::Bottom
		};	  ///< Side of the anchor

		public:
		/**
		 * @brief Create the configuration of an empty, elevated dropdown
		 * menu with no anchor.
		 */
		Menu(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Menu(void) = default;

		/**
		 * @brief Get the kind of menu.
		 * @return The variant, Variant::Dropdown by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the kind of menu.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		Menu &setVariant(Variant variant);

		/**
		 * @brief Get how the menu surface stands out.
		 * @return The mode, Mode::Elevated by default.
		 */
		Mode getMode(void) const noexcept;

		/**
		 * @brief Choose between an elevated and a flat menu surface.
		 *
		 * Marks the component changed only when the mode differs.
		 * @param mode The new mode.
		 * @return This component, for chaining.
		 */
		Menu &setMode(Mode mode);

		/**
		 * @brief Get the description of every item.
		 * @return The items, from top to bottom.
		 */
		const std::vector<Item> &getItems(void) const noexcept;

		/**
		 * @brief Replace the items.
		 *
		 * The press handlers are always replaced. The component is marked
		 * changed only when the labels, icons or flags differ, because
		 * handlers cannot be compared. The rows are left as they are:
		 * whoever owns the row entities rebuilds them.
		 * @param items The new items, from top to bottom.
		 * @return This component, for chaining.
		 */
		Menu &setItems(std::vector<Item> items);

		/**
		 * @brief Change the disabled flag recorded for one item.
		 *
		 * To update the row on screen too, call
		 * systems::Menu::setItemDisabled. Marks the component changed only
		 * when the flag differs.
		 * @param index The item index, starting at 0. An index out of range
		 * is ignored.
		 * @param disabled True to disable the item.
		 * @return This component, for chaining.
		 */
		Menu &setItemDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Get the entities built for the items.
		 * @return One row per built item, in item order. Empty until the
		 * widget has built them.
		 */
		const std::vector<Row> &getRows(void) const noexcept;

		/**
		 * @brief Record the entities built for the items.
		 *
		 * Marks the component changed only when the rows differ.
		 * @param rows One row per item, in item order.
		 * @return This component, for chaining.
		 */
		Menu &setRows(const std::vector<Row> &rows);

		/**
		 * @brief Tell whether the rows must be rebuilt from the items.
		 * @return True when the items changed since the rows were built.
		 * True for a new component.
		 */
		bool isRowsDirty(void) const noexcept;

		/**
		 * @brief Mark the rows as out of date or as up to date.
		 *
		 * The widget raises the flag when it replaces the items and lowers
		 * it once it has rebuilt the row entities. Marks the component
		 * changed only when the flag differs.
		 * @param dirty True when the rows must be rebuilt.
		 * @return This component, for chaining.
		 */
		Menu &setRowsDirty(bool dirty);

		/**
		 * @brief Get the entity the menu opens next to.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when
		 * the menu has no anchor.
		 */
		ecs::Entity::Identifier getAnchor(void) const noexcept;

		/**
		 * @brief Set the entity the menu opens next to.
		 *
		 * Every frame, systems::Menu aligns the menu with the leading edge
		 * of the anchor, below or above it. The anchor needs a Transform
		 * and a Bound; without an anchor the menu keeps its own pose. Marks
		 * the component changed only when the identifier differs.
		 * @param anchor The anchor's identifier, or
		 * ecs::Entity::InvalidIdentifier for none.
		 * @return This component, for chaining.
		 */
		Menu &setAnchor(ecs::Entity::Identifier anchor);

		/**
		 * @brief Get the side of the anchor the menu opens on.
		 * @return The position, AnchorPosition::Bottom by default.
		 */
		AnchorPosition getAnchorPosition(void) const noexcept;

		/**
		 * @brief Choose the side of the anchor the menu opens on.
		 *
		 * Marks the component changed only when the position differs.
		 * @param position Below or above the anchor.
		 * @return This component, for chaining.
		 */
		Menu &setAnchorPosition(AnchorPosition position);
	};

}	 // namespace guillaume::components
